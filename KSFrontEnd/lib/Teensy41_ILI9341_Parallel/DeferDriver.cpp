// ============================================================================
// Include
// ============================================================================
#include "DeferDriver.h"
#include "ILI9341_Constants.h"

#include "Arduino.h"

namespace ILI9341
{

// ============================================================================
// DeferDriver - Public
// ============================================================================

DeferDriver::DeferDriver(T4_ILI9341 device) : DisplayDriver(device)
{
	// Initialise grid.
	for(uint16_t bx = 0; bx < DB_GRID_WIDTH; ++bx)
	{
		for(uint16_t by = 0; by < DB_GRID_HEIGHT; ++by)
		{
			DrawBlock& newBlock = GetBlock(bx, by);
			newBlock.mDirty = false;

			// Note: every pixel starts out as black.
			memset(newBlock.mColorBuff, 0, sizeof(newBlock.mColorBuff));
		}
	}

	mFirstDirtyBlock = -1;
	mLastDirtyBlock = -1;
}



void DeferDriver::DrawPixel(uint16_t x, uint16_t y, ILIColor col)
{
	if(x >= T4_ILI9341::WIDTH || y >= T4_ILI9341::HEIGHT)
	{
		return;
	}

	// Block index
	uint16_t bx = x / DRAW_BLOCK_SIZE;
	uint16_t by = y / DRAW_BLOCK_SIZE;

	// Root position(pixels)
	uint16_t rx = x - (bx * DRAW_BLOCK_SIZE);
	uint16_t ry = y - (by * DRAW_BLOCK_SIZE);

	int blockIndex = GetBlockIdx(bx, by);
	DrawBlock& block = mDBGrid[blockIndex];
	block.mColorBuff[rx + ry * DRAW_BLOCK_SIZE] = col;

	// Mark as dirty and update min/max indices
	block.mDirty = true;
	if(mFirstDirtyBlock == -1 || mLastDirtyBlock == -1)
	{
		mFirstDirtyBlock = blockIndex;
		mLastDirtyBlock = blockIndex;
	}
	else if(blockIndex < mFirstDirtyBlock)
	{
		mFirstDirtyBlock = blockIndex;
	}
	else if(blockIndex > mLastDirtyBlock)
	{
		mLastDirtyBlock = blockIndex;
	}
}



void DeferDriver::ForceClear(ILIColor col)
{
	for(uint16_t bx = 0; bx < DB_GRID_WIDTH; ++bx)
	{
		for(uint16_t by = 0; by < DB_GRID_HEIGHT; ++by)
		{
			DrawBlock& newBlock = GetBlock(bx, by);
			newBlock.mDirty = false;

			FillBlock(newBlock, col);
		}
	}

	DisplayDriver::ForceClear(col);
}



void DeferDriver::FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ILIColor col)
{
	if(x > T4_ILI9341::WIDTH || y > T4_ILI9341::HEIGHT)
	{
		// oob, skip.
		return;
	}

	uint16_t bStartx = x / DRAW_BLOCK_SIZE;
	uint16_t bStarty = y / DRAW_BLOCK_SIZE;
	uint16_t bEndx = min((x+w-1) / DRAW_BLOCK_SIZE, DB_GRID_WIDTH-1);
	uint16_t bEndy = min((y+h-1) / DRAW_BLOCK_SIZE, DB_GRID_HEIGHT-1);

	mFirstDirtyBlock = GetBlockIdx(bStartx, bStarty);
	mLastDirtyBlock = GetBlockIdx(bEndx, bEndy);
	for(uint16_t bx = bStartx; bx <= bEndx; ++bx)
	{
		for(uint16_t by = bStarty; by <= bEndy; ++by)
		{
			//printf("%u %u---\n", bx, by);
			int idx = GetBlockIdx(bx, by);
			DrawBlock& block = mDBGrid[idx];

			// printf("%u %u %u %u\n", bStartx, bEndx, bStarty, bEndy);
			if(bStartx < bx && bx < bEndx &&
			   bStarty < by && by < bEndy)
			{
				FillBlock(block, col);
				block.mDirty = false;
				continue;
			}

			uint16_t sx = max(bx * DRAW_BLOCK_SIZE, x) - bx * DRAW_BLOCK_SIZE;
			uint16_t sy = max(by * DRAW_BLOCK_SIZE, y) - by * DRAW_BLOCK_SIZE;
			uint16_t ex = min((bx+1) * DRAW_BLOCK_SIZE, x+w) - bx * DRAW_BLOCK_SIZE;
			uint16_t ey = min((by+1) * DRAW_BLOCK_SIZE, y+h) - by * DRAW_BLOCK_SIZE;

			//printf("    X %u-%u Y %u-%u\n", sx, ex, sy, ey);
			PartialFillBlock(block, sx, sy, ex, ey, col);
			block.mDirty = true;
		}
	}
}

void DeferDriver::RenderPixels(uint16_t numBlocks)
{
	for(; mFirstDirtyBlock <= mLastDirtyBlock && numBlocks > 0; mFirstDirtyBlock++)
	{
		DrawBlock& block = mDBGrid[mFirstDirtyBlock];
		
		if(block.mDirty)
		{
			// Block root in pixels
			uint16_t ry = (mFirstDirtyBlock / DB_GRID_WIDTH) * DRAW_BLOCK_SIZE;
			uint16_t rx = (mFirstDirtyBlock % DB_GRID_WIDTH) * DRAW_BLOCK_SIZE;

			mDevice.ColumnAddrSet(rx, rx + DRAW_BLOCK_SIZE + 1);
			mDevice.PageAddrSet(ry, ry + DRAW_BLOCK_SIZE + 1);

			mDevice.MemoryWrite(block.mColorBuff, sizeof(block.mColorBuff) / sizeof(ILIColor));
			block.mDirty = false;
			numBlocks--;
		}

		if(mFirstDirtyBlock == mLastDirtyBlock) // Every block is drawn!
		{
			mFirstDirtyBlock = -1;
			mLastDirtyBlock = -1;
			break;
		}
	}
}

/// @brief Actually render the pixels out the the screen.
void DeferDriver::RenderAllPixels()
{
	if(mFirstDirtyBlock == -1)
	{
		return;
	}

	for(int i = 0; i < DB_GRID_WIDTH * DB_GRID_HEIGHT; ++i)
	{
		DrawBlock& block = mDBGrid[i];
		
		if(block.mDirty)
		{
			// Block root in pixels
			uint16_t ry = (i / DB_GRID_WIDTH) * DRAW_BLOCK_SIZE;
			uint16_t rx = (i % DB_GRID_WIDTH) * DRAW_BLOCK_SIZE;

			mDevice.ColumnAddrSet(rx, rx + DRAW_BLOCK_SIZE-1);
			mDevice.PageAddrSet(ry, ry + DRAW_BLOCK_SIZE-1);

			mDevice.MemoryWrite(block.mColorBuff, sizeof(block.mColorBuff) / sizeof(ILIColor));
			block.mDirty = false;
		}
	}

	mFirstDirtyBlock = -1;
	mLastDirtyBlock = -1;
}

// ============================================================================
// DeferDriver - Private
// ============================================================================

DeferDriver::DrawBlock& DeferDriver::GetBlock(uint16_t bx, uint16_t by)
{
	return mDBGrid[GetBlockIdx(bx, by)];
}

int DeferDriver::GetBlockIdx(uint16_t bx, uint16_t by)
{
	return bx + by * DB_GRID_WIDTH;
}


DeferDriver::DrawBlock& DeferDriver::GetBlockAtPix(uint16_t x, uint16_t y)
{
	return mDBGrid[GetBlockIdxAtPix(x, y)];
}

int DeferDriver::GetBlockIdxAtPix(uint16_t x, uint16_t y)
{
	uint16_t bx = x / DRAW_BLOCK_SIZE;
	uint16_t by = y / DRAW_BLOCK_SIZE;

	return GetBlockIdx(bx, by);
}

void DeferDriver::FillBlock(DrawBlock& block, ILIColor col)
{
	constexpr size_t SIZE_OF_BUFF_AS_U32 = 
			sizeof(DrawBlock::mColorBuff) / (sizeof(uint32_t));
	static_assert(sizeof(DrawBlock::mColorBuff) % sizeof(uint32_t) == 0);

	const uint32_t colCol = (col) | (col << 16);

	uint32_t* colBuffasU32 = reinterpret_cast<uint32_t*>(block.mColorBuff);
	for(size_t i = 0; i < SIZE_OF_BUFF_AS_U32; i+=4ull)
	{
		// Do in groups of 4 to encourage compiler to make this SIMD
		colBuffasU32[i]   = colCol;
		colBuffasU32[i+1] = colCol;
		colBuffasU32[i+2] = colCol;
		colBuffasU32[i+3] = colCol;
	}
}

/// @brief Fill a block. Important: these are coords relative to the block!!
void DeferDriver::PartialFillBlock(DrawBlock& block, uint16_t sx, uint16_t sy, 
										uint16_t ex, uint16_t ey, ILIColor col)
{
	for(uint16_t py = sy; py < ey; ++py)
	{
		//uint16_t yoffset = py * DB_GRID_WIDTH;
		for(uint16_t px = sx; px < ex; ++px)
		{
			block.mColorBuff[px + py * DRAW_BLOCK_SIZE] = col;
		}	
	}
}

}
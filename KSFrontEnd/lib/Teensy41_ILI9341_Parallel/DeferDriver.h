// ============================================================================
// ILI9341 deferred display driver. Writes to buffer then later renders out to
// the screen.
// ----------------------------------------------------------------------------
// ============================================================================
// Include
// ============================================================================
#include <stdint.h>
#include "DisplayDriver.h"
#include "T4_ILI9341.h"

#ifndef ILI9341_DEFER_DRIVER_H
#define ILI9341_DEFER_DRIVER_H

namespace ILI9341
{

// ============================================================================
// DeferDriver. ILI9341 driver that writes to buffer then later renders out to
// the screen later (i.e. it is deferred).
// ============================================================================
class DeferDriver final : public DisplayDriver
{
public:
	/// @brief Create ILI9341 driver that writes to buffer then later renders out to
	/// the screen later (i.e. it is deferred).
	/// @param device TFT screen device.
	DeferDriver(T4_ILI9341 device);



	/// @brief Draw a single pixel
	/// @param x x-coordinate
	/// @param y y-coordinate
	/// @param col packed 16-bit color
	void DrawPixel(uint16_t x, uint16_t y, ILIColor col) final;



	/// @brief Flood entire screen with color
	/// @param col Color to flood
	void ForceClear(ILIColor col) final; 



	/// @brief Draw rectangle
	/// @param x top-left x-coordinate
	/// @param y top-left y-coordinate
	/// @param w width
	/// @param h height
	/// @param col packed 16-bit color
	void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ILIColor col) final;



	/// @brief Actually render the pixels out the the screen.
	/// @param numBlocks Number of blocks to render before returning.
	void RenderPixels(uint16_t numBlocks = 4);



	/// @brief Actually render the pixels out the the screen.
	void RenderAllPixels();

private:
	// Constants

	static constexpr uint16_t DRAW_BLOCK_SIZE = 16; // 16x16 pixels
	static constexpr uint16_t DB_GRID_WIDTH = T4_ILI9341::WIDTH / DRAW_BLOCK_SIZE;
	static constexpr uint16_t DB_GRID_HEIGHT = T4_ILI9341::HEIGHT / DRAW_BLOCK_SIZE;
	static_assert(T4_ILI9341::WIDTH % DRAW_BLOCK_SIZE == 0);
	static_assert(T4_ILI9341::HEIGHT % DRAW_BLOCK_SIZE == 0);

	// Types

	struct DrawBlock
	{
		// Flags
		bool mDirty : 1;

		// Pixel data
		alignas(uint32_t) ILIColor mColorBuff[DRAW_BLOCK_SIZE * DRAW_BLOCK_SIZE];
	};

	// Methods
	
	// Get block at grid index
	DrawBlock& GetBlock(uint16_t bx, uint16_t by);
	int GetBlockIdx(uint16_t bx, uint16_t by);
	
	// Get block at pixel coordinate
	DrawBlock& GetBlockAtPix(uint16_t x, uint16_t y);
	int GetBlockIdxAtPix(uint16_t x, uint16_t y);

	void FillBlock(DrawBlock& block, ILIColor col);
	void PartialFillBlock(DrawBlock& block, uint16_t sx, uint16_t sy, 
		uint16_t ex, uint16_t ey, ILIColor col);

	// Members

	DrawBlock mDBGrid[DB_GRID_WIDTH * DB_GRID_HEIGHT];
	int mFirstDirtyBlock;
	int mLastDirtyBlock;
};

}

#endif // ILI9341_DEFER_DRIVER_H
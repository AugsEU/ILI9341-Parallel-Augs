// ============================================================================
// Include
// ============================================================================
#include "DisplayDriver.h"
#include "ILI9341_Constants.h"
#include "ILI9341_Macros.h"
#include "ILI9341_Fonts.h"

#include "Arduino.h"





namespace ILI9341
{

// ============================================================================
// Public
// ============================================================================

DisplayDriver::DisplayDriver(T4_ILI9341 device) :
	mDevice(device),
	mStatus(Status::Offline),
	mCursorX(0),
	mCursorY(0),
	mFontSize(1),
	mFontColor(0xFFFF),
	mFontData(gFuturisticFont)
{

}





int DisplayDriver::Begin()
{
	int err = EstablishConnection();
	ILI9341_CHECK_ERR_ABORT(err);


	mDevice.SoftwareReset();
	delay(50); // Wait for reset.

	mDevice.DisplayOff();

	// Setup power
	mDevice.PowerCtrl1(0x23);
	mDevice.PowerCtrl2(0x00); //DDVDH=VCIx2, VGH=VCIx7, VGL=VCIx4
	mDevice.VcomCtrl1(0x2B, 0x2B); // VCOMH = 3.775V , VCOML = -1.425V
	mDevice.VcomCtrl2(true, 0x40);

	// Pixel formats
	mDevice.MemoryAccessCtrl(RowOrder::TopToBottom, ColOrder::RightToLeft,
							 RowColExchange::Normal, RowOrder::BottomToTop,
							 RgbOrder::BGR,          ColOrder::RightToLeft);
	mDevice.PixelFormatSet(PixelFormat::Form16Bit);
	mDevice.FrameRateCtrl(0x00, 0x1B);
	
	mDevice.EntryModeSet(false, GateOuputLevel::Nominal);

	mDevice.SleepOut();
	delay(150);
	mDevice.DisplayOn();

	// Set screen to black.
	ForceClear(0x0000);

	return ILI9341_OK;
}





void DisplayDriver::ForceClear(ILIColor col)
{
	mDevice.ColumnAddrSet(0, mDevice.WIDTH - 1);
	mDevice.PageAddrSet(0, mDevice.HEIGHT - 1);

	uint16_t buff[mDevice.HEIGHT * mDevice.WIDTH];
	memset(buff, col, sizeof(buff));
	mDevice.MemoryWrite(buff, mDevice.HEIGHT * mDevice.WIDTH);
}





void DisplayDriver::DrawLine(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, ILIColor col)
{
	// Bresenham's line algorithm 
	int dx = (int)ex - (int)sx;
	int dy = (int)ey - (int)sy;

	int adx = dx < 0 ? -dx : dx;
	int ady = dy < 0 ? -dy : dy;

	int stepX = dx < 0 ? -1 : 1;
	int stepY = dy < 0 ? -1 : 1;

	int x = sx, y = sy;

	if (adx >= ady)
	{
		// More horizontal, step along X
		int err = 2 * ady - adx;
		for (int i = 0; i <= adx; ++i)
		{
			DrawPixel((uint16_t)x, (uint16_t)y, col);
			if (err >= 0)
			{
				y += stepY;
				err -= 2 * adx;
			}
			err += 2 * ady;
			x += stepX;
		}
	}
	else
	{
		// More vertical, step along Y
		int err = 2 * adx - ady;
		for (int i = 0; i <= ady; ++i)
		{
			DrawPixel((uint16_t)x, (uint16_t)y, col);
			if (err >= 0)
			{
				x += stepX;
				err -= 2 * ady;
			}
			err += 2 * adx;
			y += stepY;
		}
	}
}





void DisplayDriver::DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ILIColor col)
{
	FillRect(x, y, w, 1, col);
	FillRect(x, y, 1, h, col);
	FillRect(x, y+h, w, 1, col);
	FillRect(x+w, y, 1, h+1, col);
}





void DisplayDriver::FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ILIColor col)
{
	// Default impl, very bad perf. Should be overriden in all drivers
	for(uint16_t px = x; px < x + w; ++px)
	{
		for(uint16_t py = y; py < y + h; ++py)
		{
			DrawPixel(px, py, col);
		}
	}
}





void DisplayDriver::DrawCircle(uint16_t x, uint16_t y, uint16_t r, ILIColor color)
{
	DrawPixel(x,     y + r, color);
	DrawPixel(x,     y - r, color);
	DrawPixel(x + r, y,     color);
	DrawPixel(x - r, y,     color);

	int32_t dx = 0;
	int32_t dy = r;
	int32_t s = r * r;
	int32_t r2 = s;

	while (dx < dy)
	{
		// Make diagonal step
		dx++;
		dy--;
		s += 2 * (dx - dy) + 2;

		if (s < r2) // Check if we went too far
		{
			s += 2 * dy + 1;
			dy++;
		}

		DrawPixel(x + dx, y + dy, color);
		DrawPixel(x - dx, y + dy, color);
		DrawPixel(x + dx, y - dy, color);
		DrawPixel(x - dx, y - dy, color);
		DrawPixel(x + dy, y + dx, color);
		DrawPixel(x - dy, y + dx, color);
		DrawPixel(x + dy, y - dx, color);
		DrawPixel(x - dy, y - dx, color);
	}
}





void DisplayDriver::FillCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
	// Centre column always drawn alone
	FillRect(x, y - r, 1, 2 * r + 1, color);

	int32_t dx = 0;
	int32_t dy = r;
	int32_t s  = r * r;
	int32_t r2 = r * r;

	int32_t colFlushStart = 1;
	int32_t last   = r;

	auto FlushColumns = [&](int32_t end_dx)
	{
		if (end_dx < colFlushStart) return;  // nothing to flush
		int32_t width = end_dx - colFlushStart + 1;
		FillRect(x + colFlushStart, y - last, width, 2 * last + 1, color);
		FillRect(x - end_dx,    y - last, width, 2 * last + 1, color);
	};

	while (dx < dy)
	{
		dx++;
		dy--;
		s += 2 * (dx - dy) + 2;

		if (s < r2)
		{
			s += 2 * dy + 1;
			dy++;
		}

		if (dy != last)
		{
			FlushColumns(dx - 1);
			colFlushStart = dx;
			last   = dy;
		}
	}

	FlushColumns(dx);

	dy = 0;
	dx = r;
	s = r2;

	while (dy < dx)
	{
		dy++;
		dx--;
		s += 2 * (dy - dx) + 2;

		if (s < r2)
		{
			s += 2 * dx + 1;
			dx++;
		}

		FillRect(x + dx, y - dy, 1, 2 * dy + 1, color);
		FillRect(x - dx, y - dy, 1, 2 * dy + 1, color);
	}
}





void DisplayDriver::DrawRoundedRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
					 				uint16_t radius, ILIColor color)
{
	if(w <= radius*2 || h <= radius*2)
	{
		// Invalid rectangle
		return;
	}

	uint16_t xEdgeLen = w - radius*2;
	uint16_t yEdgeLen = h - radius*2;
	
	// Draw straight edges
	FillRect(x+radius, y, xEdgeLen, 1, color);
	FillRect(x, y+radius, 1, yEdgeLen, color);
	FillRect(x+radius, y+h, xEdgeLen, 1, color);
	FillRect(x+w, y+radius, 1, yEdgeLen+1, color);

	// Draw corners
	DrawCircleQuadrant<CircleSection::TopLeft>(x+radius, y+radius, radius, color);
	DrawCircleQuadrant<CircleSection::TopRight>(x+w-radius, y+radius, radius, color);
	DrawCircleQuadrant<CircleSection::BottomLeft>(x+radius, y+h-radius, radius, color);
	DrawCircleQuadrant<CircleSection::BottomRight>(x+w-radius, y+h-radius, radius, color);
}





void DisplayDriver::FillRoundedRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                                    uint16_t radius, ILIColor color)
{
    uint16_t cx     = x + radius;
    uint16_t cy     = y + radius;
    uint16_t inner  = w - 2 * radius;
    uint16_t extend = h - 2 * radius;

    int32_t dx = 0;
    int32_t dy = radius;
    int32_t s  = (int32_t)radius * radius;
    int32_t r2 = s;
    int32_t colFlushStart = 1;
    int32_t last = radius;

    auto FlushColumns = [&](int32_t end_dx)
    {
        if (end_dx < colFlushStart) return;
        int32_t width  = end_dx - colFlushStart + 1;
        int32_t height = 2 * last + 1 + extend;

        // Left cap: columns going left only
        FillRect(cx - end_dx, cy - last, width, height, color);

        // Right cap: columns going right only
        FillRect(cx + inner + colFlushStart, cy - last, width, height, color);
    };

    while (dx < dy)
    {
        dx++;
        dy--;
        s += 2 * (dx - dy) + 2;
        if (s < r2)
        {
            s += 2 * dy + 1;
            dy++;
        }
        if (dy != last)
        {
            FlushColumns(dx - 1);
            colFlushStart = dx;
            last = dy;
        }
    }
    FlushColumns(dx);

    // Second half of caps (past 45°)
    dy = 0;
    dx = radius;
    s  = r2;
    while (dy < dx)
    {
        dy++;
        dx--;
        s += 2 * (dy - dx) + 2;
        if (s < r2)
        {
            s += 2 * dx + 1;
            dx++;
        }
        int32_t height = 2 * dy + 1 + extend;

        // Left cap: left side only
        FillRect(cx - dx, cy - dy, 1, height, color);

        // Right cap: right side only
        FillRect(cx + inner + dx, cy - dy, 1, height, color);
    }

    // Middle rectangle covers everything between the two caps
    FillRect(cx, y, inner, h, color);
}


void DisplayDriver::DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
								 uint16_t x2, int16_t y2, ILIColor color)
{
	DrawLine(x0, y0, x1, y1, color);
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x0, y0, color);
}





void DisplayDriver::FillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                                 uint16_t x2, uint16_t y2, ILIColor color)
{
    if (y1 < y0) { std::swap(y1, y0); std::swap(x1, x0); }
    if (y2 < y0) { std::swap(y2, y0); std::swap(x2, x0); }
    if (y2 < y1) { std::swap(y2, y1); std::swap(x2, x1); }

    if (y0 == y2)
    {
        uint16_t left  = min(min(x0, x1), x2);
        uint16_t right = max(max(x0, x1), x2);
        FillRect(left, y0, right - left + 1, 1, color);
        return;
    }

    int32_t dxLong  = (int32_t)x2 - (int32_t)x0;
    int32_t dyLong  = (int32_t)y2 - (int32_t)y0;
    int32_t dxShort = (int32_t)x1 - (int32_t)x0;
    int32_t dyShort = (int32_t)y1 - (int32_t)y0;

    if (dyShort > 0)
    {
        for (int32_t y = y0; y <= y1; y++)
        {
            int32_t dy = y - y0;
            int32_t xa = (int32_t)x0 + dxLong  * dy / dyLong;
            int32_t xb = (int32_t)x0 + dxShort * dy / dyShort;
            if (xa > xb) std::swap(xa, xb);
            FillRect(xa, y, xb - xa + 1, 1, color);
        }
    }

    dxShort = (int32_t)x2 - (int32_t)x1;
    dyShort = (int32_t)y2 - (int32_t)y1;

    if (dyShort > 0)
    {
        for (int32_t y = y1 + 1; y <= y2; y++)
        {
            int32_t dy  = y - y0;
            int32_t dy2 = y - y1;
            int32_t xa  = (int32_t)x0 + dxLong  * dy  / dyLong;
            int32_t xb  = (int32_t)x1 + dxShort * dy2 / dyShort;
            if (xa > xb) std::swap(xa, xb);
            FillRect(xa, y, xb - xa + 1, 1, color);
        }
    }
}





void DisplayDriver::DrawText(const char* string)
{
	uint16_t lineBegin = mCursorX;
	uint16_t charWidth = mFontSize * (FONT_CHAR_WIDTH+1);
	uint16_t charHeight = mFontSize * (FONT_CHAR_HEIGHT+1);

	for(const char* currChar = string; *currChar != '\0'; ++currChar)
	{
		char c = *currChar;
		if(c >= 0x21 && c < 0x7F)
		{
			DrawChar(mCursorX, mCursorY, c);
			//DrawRect(mCursorX, mCursorY, charWidth, charHeight, 0xFFFF);
		}

		mCursorX += charWidth;
		if(c == '\n' || mCursorX + charWidth > T4_ILI9341::WIDTH)
		{
			mCursorY += charHeight;
			mCursorX = lineBegin; // Todo: setup proper text bounds
		}
	}

	mCursorY += charHeight;
	mCursorX = lineBegin;
}





void DisplayDriver::DrawChar(uint16_t x, uint16_t y, char c)
{
	int idx = c - 0x21;
	const uint8_t* currCol = &mFontData[idx*FONT_CHAR_WIDTH];

	for(uint8_t col = 0; col < FONT_CHAR_WIDTH; col++)
	{
		uint8_t colData = *currCol;

		uint16_t yc = y;
		for(uint8_t row = 0; row < FONT_CHAR_HEIGHT; row++)
		{
			if(colData & (1 << row))
			{
				FillRect(x, yc, mFontSize, mFontSize, mFontColor);
			}
			yc+=mFontSize;
		}

		x += mFontSize;
		currCol++;
	}
}

// ============================================================================
// Private
// ============================================================================

int DisplayDriver::EstablishConnection()
{
	constexpr int NUM_RETRY = 5;
	for(int i = 0; i < NUM_RETRY; i++)
	{
		Id4 id = mDevice.ReadId4();
		if(id.IsValid())
		{
			return ILI9341_OK;
		}
		delay(10);
	}

	return ILI9341_OK;
}

}
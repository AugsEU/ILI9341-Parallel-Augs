// ============================================================================
// Include
// ============================================================================
#include "DisplayDriver.h"
#include "ILI9341_Constants.h"
#include "ILI9341Macros.h"

#include "Arduino.h"





namespace ILI9341
{

// ============================================================================
// Public
// ============================================================================

DisplayDriver::DisplayDriver(T4_ILI9341 device) :
	mDevice(device),
	mStatus(Status::Offline)
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
	mDevice.MemoryAccessCtrl(RowOrder::BottomToTop, ColOrder::LeftToRight,
							 RowColExchange::Normal, RowOrder::TopToBottom,
							 RgbOrder::BGR,          ColOrder::LeftToRight);
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
	// Default impl, very bad perf. Should be overriden in all drivers
	for(uint16_t px = x; px < x + w; ++px)
	{
		for(uint16_t py = y; py < y + h; ++py)
		{
			DrawPixel(px, py, col);
		}
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
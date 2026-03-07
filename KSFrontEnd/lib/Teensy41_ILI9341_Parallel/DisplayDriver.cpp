// ============================================================================
// Include
// ============================================================================
#include "DisplayDriver.h"
#include "ILI9341_Constants.h"

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
	mDevice.PageAddrSet(0, mDevice.HEIGHT - 1);
	mDevice.ColumnAddrSet(0, mDevice.WIDTH - 1);

	return ILI9341_OK;
}

int DisplayDriver::TestWritePixel(uint16_t x, uint16_t y, uint16_t col)
{
	mDevice.ColumnAddrSet(x, mDevice.HEIGHT - 1);
	delay(10);
	mDevice.PageAddrSet(y, mDevice.WIDTH - 1);
	delay(10);
	mDevice.MemoryWrite(&col, 1);
	delay(10);
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
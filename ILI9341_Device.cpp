// ============================================================================
// Include
// ============================================================================
#include "ILI9341_Device.h"
#include "ILI9341_Constants.h"
#include "ILI9341_Macros.h"

#include "Arduino.h"





// ============================================================================
// Private macros
// ============================================================================
#define BEGIN_CMD(x) \
	ILI9341_WRITE_PIN_FAST(mPinCS, LOW); \
	ILI9341_WRITE_PIN_FAST(mPinCD, LOW); \
	ILI9341_WRITE_DATA_BUS(x);	         \
	ILI9341_STROBE_PIN(mPinWrite)

#define BEGIN_DATA \
	ILI9341_WRITE_PIN_FAST(mPinCD, HIGH)

#define WRITE_DATA(x) \
	ILI9341_WRITE_DATA_BUS(x);	         \
	ILI9341_STROBE_PIN(mPinWrite)

#define END_CMD \
	ILI9341_WRITE_PIN_FAST(mPinCS, HIGH)


namespace ILI9341
{

// ============================================================================
// T4_ILI9341
// ============================================================================

T4_ILI9341::T4_ILI9341(uint8_t cs, uint8_t cd, uint8_t wr, uint8_t rd) :
    mPinCS(cs),
    mPinCD(cd),
    mPinWrite(wr),
    mPinRead(rd)
{
	pinMode(cs, OUTPUT);
	pinMode(cd, OUTPUT);
	pinMode(wr, OUTPUT);
	pinMode(rd, OUTPUT);

	ILI9341_END_READ();

	ILI9341_WRITE_PIN_FAST(cs, HIGH);
	ILI9341_WRITE_PIN_FAST(cd, HIGH);
	ILI9341_WRITE_PIN_FAST(wr, HIGH);
	ILI9341_WRITE_PIN_FAST(rd, HIGH);
}

// ============================================================================
// Level 1 commands
// ============================================================================

void T4_ILI9341::SoftwareReset()
{
    Cmd8Data0(ILI9341_CMD_SOFTWARE_RESET);
}



DisplayIdentification T4_ILI9341::ReadDisplayIdentification()
{
	DisplayIdentification ret;
	BeginReadMode(ILI9341_CMD_READ_DISPLAY_ID_INFO);
	
	ret.mManufacturerId = ReadNext8();
	ret.mDriverVersion = ReadNext8();
	ret.mDriverId = ReadNext8();

	EndReadMode();

	return ret;
}


DisplayStatus T4_ILI9341::ReadDisplayStatus()
{
	DisplayStatus ret;
	BeginReadMode(ILI9341_CMD_READ_DISPLAY_STATUS);
	
	uint32_t d31_25 = ReadNext8() & 0b11111110;
	uint32_t d22_16 = ReadNext8() & 0b01111111;
	uint32_t d10_8  = ReadNext8() & 0b00000111;
	uint32_t d7_5   = ReadNext8() & 0b11100000;

	ret.mPackedBits = (d31_25 << 24) | (d22_16 << 16) | (d10_8 << 8) | d7_5; 

	EndReadMode();

	return ret;
}


DisplayPowerMode T4_ILI9341::ReadDisplayPowerMode()
{
	DisplayPowerMode ret;
	BeginReadMode(ILI9341_CMD_READ_DISPLAY_POWER_MODE);
	
	ret.mPackedBits = ReadNext8();

	EndReadMode();

	return ret;
}


DisplayMADCTL T4_ILI9341::ReadDisplayMADCTL()
{
	DisplayMADCTL ret;
	BeginReadMode(ILI9341_CMD_READ_DISPLAY_MADCTL);
	
	ret.mPackedBits = ReadNext8();

	EndReadMode();

	return ret;
}


PixelFormat T4_ILI9341::ReadDisplayPixelFormat()
{
	PixelFormat ret;
	BeginReadMode(ILI9341_CMD_READ_DISPLAY_PIXEL_FORMAT);
	
	uint8_t packedBits = ReadNext8();
	if(packedBits & 0b00000010)
	{
		ret = PixelFormat::Form18Bit;
	}
	else
	{
		ret = PixelFormat::Form16Bit;
	}

	EndReadMode();

	return ret;
}


uint8_t T4_ILI9341::ReadDisplayImageFormat()
{
	BeginReadMode(ILI9341_CMD_READ_DISPLAY_IMAGE_FORMAT);
	
	uint8_t ret = ReadNext8();

	EndReadMode();

	return ret;
}


DisplaySignalMode T4_ILI9341::ReadDisplaySignalMode()
{	
	DisplaySignalMode ret;
	BeginReadMode(ILI9341_CMD_READ_DISPLAY_SIGNAL_MODE);
	
	ret.mPackedBits = ReadNext8();

	EndReadMode();

	return ret;
}


DisplaySelfDiagnostic T4_ILI9341::ReadDisplaySelfDiagnostic()
{
	DisplaySelfDiagnostic ret;
	BeginReadMode(ILI9341_CMD_READ_DISPLAY_SELF_DIAG_RESULT);
	
	ret.mPackedBits = ReadNext8();

	EndReadMode();

	return ret;
}



void T4_ILI9341::EnterSleepMode()
{
	Cmd8Data0(ILI9341_CMD_ENTER_SLEEP_MODE);
}


void T4_ILI9341::SleepOut()
{
	Cmd8Data0(ILI9341_CMD_SLEEP_OUT);
}


void T4_ILI9341::PartialModeOn()
{
	Cmd8Data0(ILI9341_CMD_PARTIAL_ON);
}


void T4_ILI9341::NormalDisplayModeOn()
{
	Cmd8Data0(ILI9341_CMD_NORMAL_ON);
}


void T4_ILI9341::DisplayInversionOff()
{
	Cmd8Data0(ILI9341_CMD_DISPLAY_INVERT_OFF);
}


void T4_ILI9341::DisplayInversionOn()
{
	Cmd8Data0(ILI9341_CMD_DISPLAY_INVERT_ON);
}



void T4_ILI9341::GammaSet(uint8_t gammaIndex)
{
	gammaIndex = (1 << gammaIndex);
	Cmd8Data1(ILI9341_CMD_GAMMA_SET, gammaIndex);
}



void T4_ILI9341::DisplayOff()
{
	Cmd8Data0(ILI9341_CMD_DISPLAY_OFF);
}


void T4_ILI9341::DisplayOn()
{
    Cmd8Data0(ILI9341_CMD_DISPLAY_ON);
}



void T4_ILI9341::ColumnAddrSet(uint16_t startCol, uint16_t endCol)
{
	uint8_t sc15_8 = startCol >> 8;
	uint8_t sc7_0 = startCol & 0xFF;
	uint8_t ec15_8 = endCol >> 8;
	uint8_t ec7_0 = endCol & 0xFF;
	Cmd8Data4(ILI9341_CMD_COL_ADDR_SET, sc15_8, sc7_0, ec15_8, ec7_0);
}


void T4_ILI9341::PageAddrSet(uint16_t startPage, uint16_t endPage)
{
	uint8_t sp15_8 = startPage >> 8;
	uint8_t sp7_0 = startPage & 0xFF;
	uint8_t ep15_8 = endPage >> 8;
	uint8_t ep7_0 = endPage & 0xFF;
	Cmd8Data4(ILI9341_CMD_PAGE_ADDR_SET, sp15_8, sp7_0, ep15_8, ep7_0);
}


void T4_ILI9341::MemoryWrite(uint16_t* buff, uint32_t size)
{
	BEGIN_CMD(ILI9341_CMD_MEMORY_WRITE);
	BEGIN_DATA;
	for(uint32_t i = 0; i < size; ++i)
	{
		uint16_t color = buff[i];
		WRITE_DATA((uint8_t)(color >> 8));
		WRITE_DATA((uint8_t)color);
	}
	END_CMD;
}



void T4_ILI9341::ColorSet(ColorLUT table)
{
	BEGIN_CMD(ILI9341_CMD_COLOR_SET);
	BEGIN_DATA;
	for(uint8_t r : table.mReds)
	{
		WRITE_DATA(r);
	}
	for(uint8_t g : table.mGreens)
	{
		WRITE_DATA(g);
	}
	for(uint8_t b : table.mBlues)
	{
		WRITE_DATA(b);
	}
	END_CMD;
}



uint16_t* T4_ILI9341::MemoryRead(uint32_t size)
{
	BeginReadMode(ILI9341_CMD_MEMORY_READ);
	uint16_t* buf = (uint16_t*)malloc(size * sizeof(uint16_t));
	for(uint32_t i = 0; i < size; i++)
	{
		uint8_t high = ReadNext8();
		uint8_t low = ReadNext8();
		buf[i] = (high << 8) | low;
	}
	EndReadMode();

	return buf;
}



void T4_ILI9341::PartialArea(uint16_t startRow, uint16_t endRow)
{
	uint8_t sr15_8 = startRow >> 8;
	uint8_t sr7_0 = startRow & 0xFF;
	uint8_t er15_8 = endRow >> 8;
	uint8_t er7_0 = endRow & 0xFF;
	Cmd8Data4(ILI9341_CMD_PARTIAL_AREA_SET, sr15_8, sr7_0, er15_8, er7_0);
}



void T4_ILI9341::VerticalScrollingDefinition(uint16_t topFixedArea, 
											uint16_t vertialScrollArea,
											uint16_t bottomFixedArea)
{
	uint8_t tfa15_8 = topFixedArea >> 8;
	uint8_t tfa7_0 = topFixedArea & 0xFF;
	uint8_t vsa15_8 = vertialScrollArea >> 8;
	uint8_t vsa7_0 = vertialScrollArea & 0xFF;
	uint8_t bfa15_8 = bottomFixedArea >> 8;
	uint8_t bga7_0 = bottomFixedArea & 0xFF;

	Cmd8Data6(ILI9341_CMD_COL_ADDR_SET, tfa15_8, tfa7_0, 
										vsa15_8, vsa7_0,
										bfa15_8, bga7_0);
}



void T4_ILI9341::TearingEffectLineOff()
{
	Cmd8Data0(ILI9341_CMD_TEARING_EFX_OFF);
}


void T4_ILI9341::TearingEffectLineOn(TearingEffectMode mode)
{
	Cmd8Data0(ILI9341_CMD_TEARING_EFX_ON);
}



void T4_ILI9341::MemoryAccessCtrl(RowOrder rowOrder, 
					ColOrder colOrder, 
					RowColExchange exchange, 
					RowOrder verticalRefresh,
					RgbOrder rgbOrder,
					ColOrder horizontalRefresh)
{
	uint8_t data = 0;
	if(rowOrder == RowOrder::BottomToTop)          data |= (1 << 7);
	if(colOrder == ColOrder::RightToLeft)          data |= (1 << 6);
	if(exchange == RowColExchange::Reverse)        data |= (1 << 5);
	if(verticalRefresh == RowOrder::BottomToTop)   data |= (1 << 4);
	if(rgbOrder == RgbOrder::BGR)                  data |= (1 << 3);
	if(horizontalRefresh == ColOrder::RightToLeft) data |= (1 << 2);

	Cmd8Data1(ILI9341_CMD_MEM_ACCESS_CTL, data);
}



void T4_ILI9341::VerticalScrollStartAddr(uint16_t start)
{
	uint8_t vsp15_8 = start >> 8;
	uint8_t vsp7_0 = start & 0xFF;
	Cmd8Data2(ILI9341_CMD_VERT_SCROLL_START_ADDR, vsp15_8, vsp7_0);
}



void T4_ILI9341::IdleModeOff()
{
	Cmd8Data0(ILI9341_CMD_IDLE_OFF);
}


void T4_ILI9341::IdleModeOn()
{
	Cmd8Data0(ILI9341_CMD_IDLE_ON);
}



void T4_ILI9341::PixelFormatSet(PixelFormat format)
{
	uint8_t data = 0;
	if(format == PixelFormat::Form16Bit)
	{
		data = 0b01010101;
	}
	else if(format == PixelFormat::Form18Bit)
	{
		data = 0b01100110;
	}

	Cmd8Data1(ILI9341_CMD_PIXEL_FMT_SET, data);
}



void T4_ILI9341::WriteMemoryContinue(uint16_t* buff, uint32_t size)
{
	BEGIN_CMD(ILI9341_CMD_MEMORY_WRITE_CONT);
	BEGIN_DATA;
	for(uint32_t i = 0; i < size; ++i)
	{
		uint16_t color = buff[i];
		WRITE_DATA((uint8_t)(color >> 8));
		WRITE_DATA((uint8_t)color);
	}
	END_CMD;
}


uint16_t* T4_ILI9341::ReadMemoryContinue(uint32_t size)
{
	BeginReadMode(ILI9341_CMD_MEMORY_READ_CONT);
	uint16_t* buf = (uint16_t*)malloc(size * sizeof(uint16_t));
	for(uint32_t i = 0; i < size; i++)
	{
		uint8_t high = ReadNext8();
		uint8_t low = ReadNext8();
		buf[i] = (high << 8) | low;
	}
	EndReadMode();

	return buf;
}

void T4_ILI9341::SetTearScanline(uint16_t scanLine)
{
	uint8_t sts8 = (scanLine >> 8) & 0b00000001;
	uint8_t sts7_0 = scanLine & 0xFF;
	Cmd8Data2(ILI9341_CMD_SET_TEAR_SCANLINE, sts8, sts7_0);
}


uint16_t T4_ILI9341::GetScanline()
{
	BeginReadMode(ILI9341_CMD_GET_SCANLINE);
	uint8_t gts9_8 = ReadNext8() & 0b00000011;
	uint8_t gts7_0 = ReadNext8();
	EndReadMode();

	return (gts9_8 << 8) | gts7_0;
}



void T4_ILI9341::WriteDisplayBrightness(uint8_t brightness)
{
	Cmd8Data1(ILI9341_CMD_WRITE_DISPLAY_BRIGHT, brightness);
}


uint8_t T4_ILI9341::ReadDisplayBrightness()
{
	BeginReadMode(ILI9341_CMD_READ_DISPLAY_BRIGHT);
	uint8_t dbv = ReadNext8();
	EndReadMode();

	return dbv;
}



void T4_ILI9341::WriteCtrlDisplay(bool brightnessBlock, bool displayDimming, bool backlightOnOff)
{
	uint8_t data = 0;
	if(brightnessBlock) data |= (1 << 5);
	if(displayDimming) data |= (1 << 3);
	if(backlightOnOff) data |= (1 << 2);
	Cmd8Data1(ILI9341_CMD_WRITE_CTRL_DISPLAY, data);
}


CtrlDisplay T4_ILI9341::ReadCtrlDisplay()
{
	CtrlDisplay ret;
	BeginReadMode(ILI9341_CMD_READ_CTRL_DISPLAY);
	ret.mPackedBits = ReadNext8();
	EndReadMode();

	return ret;
}



void T4_ILI9341::WriteAdaptiveBrightness(AdaptiveBrightnessCtrl mode)
{
	uint8_t modeInt = (uint8_t)mode;
	Cmd8Data1(ILI9341_CMD_WRITE_CONTENT_ADAPT_BRIGHT, modeInt);
}


AdaptiveBrightnessCtrl T4_ILI9341::ReadAdaptiveBrightness()
{
	BeginReadMode(ILI9341_CMD_READ_CONTENT_ADAPT_BRIGHT);
	AdaptiveBrightnessCtrl ret = (AdaptiveBrightnessCtrl)ReadNext8();
	EndReadMode();

	return ret;
}



void T4_ILI9341::WriteCabcMinBrightness(uint8_t brightness)
{
	Cmd8Data1(ILI9341_CMD_WRITE_CABC_MIN_BRIGHT, brightness);
}


uint8_t T4_ILI9341::ReadCabcMinBrightness()
{
	BeginReadMode(ILI9341_CMD_READ_CABC_MIN_BRIGHT);
	uint8_t bright = ReadNext8();
	EndReadMode();

	return bright;
}



uint8_t T4_ILI9341::ReadId1()
{
	BeginReadMode(ILI9341_CMD_READ_ID1);
	uint8_t id1 = ReadNext8();
	EndReadMode();

	return id1;
}


uint8_t T4_ILI9341::ReadId2()
{
	BeginReadMode(ILI9341_CMD_READ_ID2);
	uint8_t id2 = ReadNext8();
	EndReadMode();

	return id2;
}


uint8_t T4_ILI9341::ReadId3()
{
	BeginReadMode(ILI9341_CMD_READ_ID3);
	uint8_t id3 = ReadNext8();
	EndReadMode();

	return id3;
}


void T4_ILI9341::RgbInterfaceSignalCtrl(RgbBypassMode bypass,
							RgbInterfaceRCM rcm,
							SyncPolarity vspl, 
							SyncPolarity hspl, 
							TransitionPolarity dotclk, 
							bool eplLow)
{
	uint8_t data = 0;
	if(bypass == RgbBypassMode::Memory) data |= (1 << 7);
	data |= (uint8_t)rcm << 5;
	if(vspl == SyncPolarity::HighLevelSyncClock) data |= (1 << 3);
	if(hspl == SyncPolarity::HighLevelSyncClock) data |= (1 << 2);
	if(dotclk == TransitionPolarity::FallingEdge) data |= (1 << 1);
	if(eplLow) data |= 1;

	Cmd8Data1(ILI9341_CMD2_RGB_INTERFACE_SIG_CTL, data);
}



void T4_ILI9341::FrameRateCtrl(uint8_t divisionRatio, uint8_t rtn)
{
	Cmd8Data2(ILI9341_CMD2_FRAME_RATE_CTL_NORM, divisionRatio & 0b00000011,
												rtn & 0b00011111);
}


void T4_ILI9341::FrameRateCtrlIdle(uint8_t divisionRatio, uint8_t rtn)
{	
	Cmd8Data2(ILI9341_CMD2_FRAME_RATE_CTL_IDLE, divisionRatio & 0b00000011,
												rtn & 0b00011111);
}


void T4_ILI9341::FrameRateCtrlPartial(uint8_t divisionRatio, uint8_t rtn)
{
	Cmd8Data2(ILI9341_CMD2_FRAME_RATE_CTL_PARTIAL, divisionRatio & 0b00000011,
												rtn & 0b00011111);
}



void T4_ILI9341::DisplayInversionCtrl(bool fullColor, bool idle, bool partial)
{
	uint8_t data = 0;
	if(fullColor) data |= (1 << 2);
	if(idle) data |= (1 << 1);
	if(partial) data |= (1 << 0);

	Cmd8Data1(ILI9341_CMD2_DISPLAY_INVERT_CTL, data);
}


void T4_ILI9341::BlankPorchCtrl(uint8_t vfp, uint8_t vbp, uint8_t hfp, uint8_t hbp)
{
	Cmd8Data4(ILI9341_CMD2_BLANK_PORCH_CTL, vfp & 0b01111111, vbp & 0b01111111,
										 	hfp & 0b00011111, hbp & 0b00011111);
}



void T4_ILI9341::DisplayFunctionCtrl(ScanMode scanMode, 
						SourceOutput pt,
						ShiftDirectionOfSourceDriver ss,
						LiquidCrystalType rev,
						uint8_t isc,
						GateOutputScanDiraction gs,
						bool sm,
						uint8_t nl,
						uint8_t pcdiv)
{
	uint8_t data0 = 0;
	data0 |= ((uint8_t)scanMode << 2);
	data0 |= (uint8_t)pt;

	uint8_t data1 = 0;
	data1 |= ((uint8_t)rev << 7);
	data1 |= ((uint8_t)gs << 6);
	data1 |= ((uint8_t)ss << 5);
	data1 |= ((uint8_t)sm << 4);
	data1 |= (isc);

	uint8_t data2 = nl & 0b00111111;
	uint8_t data3 = pcdiv & 0b0011111;

	Cmd8Data4(ILI9341_CMD2_DISPLAY_FUNCT_CTL, data0, data1, data2, data3);
}

 // No idea what these params stand for...
void T4_ILI9341::EntryModeSet(bool lowVoltageDetection, GateOuputLevel output)
{
	uint8_t data = ((uint8_t)output << 1) | (uint8_t)(!lowVoltageDetection);
	Cmd8Data1(ILI9341_CMD2_ENTRY_MODE_SET, data);
}



// T4_ILI9341::You must read datasheet to understand these...
void T4_ILI9341::BacklightControl1(uint8_t levelUi)
{
	Cmd8Data1(ILI9341_CMD2_BACKLIGHT_CTL_1, levelUi & 0b00001111);
}


void T4_ILI9341::BacklightControl2(uint8_t levelStillPicture, uint8_t levelMovingImage)
{
	uint8_t thmv = levelMovingImage & 0b00001111;
	uint8_t thst = levelStillPicture & 0b00001111;
	Cmd8Data1(ILI9341_CMD2_BACKLIGHT_CTL_2, (thmv << 4) | thst);
}


void T4_ILI9341::BacklightControl3(uint8_t dthUi)
{
	Cmd8Data1(ILI9341_CMD2_BACKLIGHT_CTL_3, dthUi & 0b00001111);
}


void T4_ILI9341::BacklightControl4(uint8_t dth_st, uint8_t dth_mv)
{
	dth_st &= 0b00001111;
	dth_mv &= 0b00001111;
	Cmd8Data1(ILI9341_CMD2_BACKLIGHT_CTL_4, (dth_mv << 4) | dth_st);
}


void T4_ILI9341::BacklightControl5(uint8_t dim1, uint8_t dim2)
{
	dim1 &= 0b00000111;
	dim2 &= 0b00001111;
	Cmd8Data1(ILI9341_CMD2_BACKLIGHT_CTL_5, (dim2 << 4) | dim1);
}


void T4_ILI9341::BacklightControl7(uint8_t pwm_div)
{
	Cmd8Data1(ILI9341_CMD2_BACKLIGHT_CTL_7, pwm_div);
}


void T4_ILI9341::BacklightControl8(bool ledpwmpol, bool ledonpol, bool ledonr)
{
	uint8_t data = 0;
	
	data |= ((uint8_t)ledonr << 2);
	data |= ((uint8_t)ledonpol << 1);
	data |= ((uint8_t)ledpwmpol);

	Cmd8Data1(ILI9341_CMD2_BACKLIGHT_CTL_8, data);
}



void T4_ILI9341::PowerCtrl1(uint8_t gvddLevel)
{
	Cmd8Data1(ILI9341_CMD2_POWER_CTL_1, gvddLevel & 0b00111111);
}


void T4_ILI9341::PowerCtrl2(uint8_t stepUpFactor)
{
	Cmd8Data1(ILI9341_CMD2_POWER_CTL_2, stepUpFactor & 0b00000111);
}



void T4_ILI9341::VcomCtrl1(uint8_t vcomhVoltage, uint8_t vcomlVoltage)
{
	Cmd8Data2(ILI9341_CMD2_VCOM_CTL_1,  vcomhVoltage & 0b01111111, 
										vcomlVoltage & 0b01111111);
}


void T4_ILI9341::VcomCtrl2(bool nVM, uint8_t vcomOffset)
{
	uint8_t data = vcomOffset & 0b01111111;
	if(nVM)
	{
		data |= 0b10000000;
	}
	Cmd8Data1(ILI9341_CMD2_VCOM_CTL_2, data);
}



void T4_ILI9341::NvMemoryWrite(PGMAddr pgmAddress, uint8_t pgmData)
{
	Cmd8Data2(ILI9341_CMD2_NV_MEM_WRITE, (uint8_t)pgmAddress & 0b00000111, pgmData);
}


void T4_ILI9341::NvMemoryProtectionKey(uint32_t key)
{
	uint8_t key32_16 = key >> 16;
	uint8_t key15_8 = key >> 8;
	uint8_t key7_0 = key;
	Cmd8Data3(ILI9341_CMD2_NV_MEM_PROTECT_KEY, key32_16, key15_8, key7_0);
}


NvMemoryStatus T4_ILI9341::NvStatusRead()
{
	NvMemoryStatus ret;
	BeginReadMode(ILI9341_CMD2_NV_MEM_STATUS_READ);

	uint8_t data0 = ReadNext8();
	ret.mId1Count = data0 & 0b00000111;
	ret.mId2Count = (data0 >> 4) & 0b00000111;

	uint8_t data1 = ReadNext8();
	ret.mBusy = data1 & 0b10000000;
	ret.mId3Count = data1 & 0b00000111;
	ret.mVmfCount = (data1 >> 4) & 0b00000111;

	EndReadMode();

	return ret;
}


Id4 T4_ILI9341::ReadId4()
{
	Id4 ret;
	BeginReadMode(ILI9341_CMD2_READ_ID4);
	ret.mIcVersion = ReadNext8();
	uint16_t modelHigh = ReadNext8();
	uint16_t modelLow = ReadNext8();
	ret.mIcModelName = (modelHigh << 8) | modelLow;
	Serial.printf("%x %x\n", ret.mIcVersion, ret.mIcModelName);

	EndReadMode();

	return ret;
}



void T4_ILI9341::PositiveGammaCorrection(GammaCorrection table)
{
	uint8_t* buf = (uint8_t*)&table;
	Cmd8DataN(ILI9341_CMD2_POS_GAMMA_CORRECTION, buf, sizeof(GammaCorrection));
}



void T4_ILI9341::NegativeGammaCorrection(GammaCorrection table)
{
	uint8_t* buf = (uint8_t*)&table;
	Cmd8DataN(ILI9341_CMD2_NEG_GAMMA_CORRECTION, buf, sizeof(GammaCorrection));
}



void T4_ILI9341::DigitalGammaControl1(GammaColorCorrection1 table)
{
	uint8_t buf[GammaColorCorrection1::NUM_ENTRIES];
	for(size_t i = 0; i < GammaColorCorrection1::NUM_ENTRIES; ++i)
	{
		buf[i] = ((table.mRedCurve[i] & 0b11110000) << 4) 
				| (table.mBlueCurve[i] & 0b00001111);
	}
	Cmd8DataN(ILI9341_CMD2_DIGITAL_GAMMA_CTL_1, buf, GammaColorCorrection1::NUM_ENTRIES);
}


void T4_ILI9341::DigitalGammaControl2(GammaColorCorrection2 table)
{
	uint8_t buf[GammaColorCorrection2::NUM_ENTRIES];
	for(size_t i = 0; i < GammaColorCorrection2::NUM_ENTRIES; ++i)
	{
		buf[i] = ((table.mRedCurve[i] & 0b11110000) << 4) 
				| (table.mBlueCurve[i] & 0b00001111);
	}
	Cmd8DataN(ILI9341_CMD2_DIGITAL_GAMMA_CTL_2, buf, GammaColorCorrection1::NUM_ENTRIES);
}



void T4_ILI9341::InterfaceControl(bool myEor, bool mxEor, bool mvEor, bool bgrEor, uint8_t mdt, bool weMode,
						Endianess endian, DisplayOperationMode dm, GramInterfaceMode rm,
						bool rim, ColorFormat65K epf)
{
	uint8_t data0 = 0;
	data0 |= ((uint8_t)myEor << 7);
	data0 |= ((uint8_t)mxEor << 6);
	data0 |= ((uint8_t)mvEor << 5);
	data0 |= ((uint8_t)bgrEor << 3);
	data0 |= ((uint8_t)weMode << 7);

	uint8_t data1 = 0;
	data1 |= ((uint8_t)epf & 0b11) << 4;
	data1 |= (mdt & 0b11);

	uint8_t data2 = 0;
	data2 |= ((uint8_t)endian << 5);
	data2 |= ((uint8_t)dm << 2);
	data2 |= ((uint8_t)rm << 1);
	data2 |= ((uint8_t)rim);

	Cmd8Data3(ILI9341_CMD2_INTERFACE_CTL, data0, data1, data2);
}



// T4_ILI9341::Extend register commands
void T4_ILI9341::PowerCtrlA(uint8_t vcore, uint8_t ddvdh)
{
	uint8_t data0 = 0x39;
	uint8_t data1 = 0x2C;
	uint8_t data2 = 0x00;
	uint8_t data3 = 0b01100000;
	data3 |= vcore & 0b111;
	uint8_t data4 = 0;
	data4 |= ddvdh & 0b111;

	Cmd8Data5(ILI9341_EXT_REG_CMD_POWER_CTL_A, data0, data1, data2, data3, data4);
}


void T4_ILI9341::PowerCtrlB(uint8_t powerCtrl, bool pceq, bool drvEna, 
			uint8_t drvVml, uint8_t drvVmh, bool esdProtection)
{
	uint8_t data0 = 0x00;
	uint8_t data1 = 0x80;
	data1 |= (uint8_t)pceq << 6;
	data1 |= (uint8_t)drvEna << 5;
	data1 |= powerCtrl << 3;
	
	uint8_t data2 = 0x20;
	data2 |= (drvVml & 0b110) << 5;
	data2 |= (uint8_t)esdProtection << 4;
	data2 |= (drvVml & 0b001) << 3;
	data2 |= (drvVmh & 0b11);

	Cmd8Data3(ILI9341_EXT_REG_CMD_POWER_CTL_B, data0, data1, data2);
}


void T4_ILI9341::DriverTimingCtrlA1(bool eq, bool now, bool cr, PreChargeTimingControl pc)
{
	uint8_t data0 = 0x84;
	data0 |= (uint8_t)now;

	uint8_t data1 = 0;
	data1 |= (uint8_t)eq << 4;
	data1 |= (uint8_t)cr;

	uint8_t data2 = 0x78;
	data2 |= (uint8_t)pc;

	Cmd8Data3(ILI9341_EXT_REG_CMD_DRIVER_TIME_CTL_A1, data0, data1, data2);
}


void T4_ILI9341::DriverTimingCtrlA2(bool eqe, bool nowe, bool cre, PreChargeTimingControl pc)
{
	uint8_t data0 = 0x84;
	data0 |= (uint8_t)nowe;

	uint8_t data1 = 0;
	data1 |= (uint8_t)eqe << 4;
	data1 |= (uint8_t)cre;

	uint8_t data2 = 0x78;
	data2 |= (uint8_t)pc;

	Cmd8Data3(ILI9341_EXT_REG_CMD_DRIVER_TIME_CTL_A1, data0, data1, data2);
}

void T4_ILI9341::DriverTimingCtrlB(uint8_t vgSwT1, uint8_t vgSwT2, uint8_t vgSwT3, uint8_t vgSwT4)
{
	uint8_t data0 = 0;
	data0 |= (vgSwT4 && 0b11) << 6;
	data0 |= (vgSwT3 && 0b11) << 4;
	data0 |= (vgSwT2 && 0b11) << 2;
	data0 |= (vgSwT1 && 0b11);

	Cmd8Data2(ILI9341_EXT_REG_CMD_DRIVER_TIME_CTL_B, data0, 0x00);
}


void T4_ILI9341::PowerSequenceCtrl(uint8_t cp1SoftStart, uint8_t cp23SoftStart, uint8_t enVcl,
						uint8_t enDdvdh, uint8_t enVgh, uint8_t enVgl, bool ddvdhEnable)
{
	uint8_t data0 = 0x44;
	data0 |= (cp1SoftStart & 0b11) << 4;
	data0 |= (cp23SoftStart & 0b11);

	uint8_t data1 = 0;
	data1 |= (enVcl & 0b11) << 4;
	data1 |= (enDdvdh & 0b11);

	uint8_t data2 = 0;
	data2 |= (enVgh & 0b11) << 4;
	data2 |= (enVgl & 0b11);

	uint8_t data3 = 0x01;
	data3 |= (uint8_t)ddvdhEnable;

	Cmd8Data4(ILI9341_EXT_REG_CMD_POWER_ON_SEQ_CTL, data0, data1, data2, data3);
}


void T4_ILI9341::Enable3Gamma(bool enable)
{
	Cmd8Data1(ILI9341_EXT_REG_CMD_ENABLE_3G, 0x02 | (enable));
}


void T4_ILI9341::PumpRatioCtrl(PumpRatio ratio)
{
	Cmd8Data1(ILI9341_EXT_REG_CMD_PUMP_RATIO_CTL, (uint8_t)ratio << 4);
}

// ============================================================================
// Data bus
// ============================================================================

void T4_ILI9341::Cmd8Data0(uint8_t cmd)
{
	BEGIN_CMD(cmd);
	END_CMD;
}

void T4_ILI9341::Cmd8Data1(uint8_t cmd, uint8_t d0)
{
	BEGIN_CMD(cmd);
	BEGIN_DATA;
	WRITE_DATA(d0);
	END_CMD;
}

void T4_ILI9341::Cmd8Data2(uint8_t cmd, uint8_t d0, uint8_t d1)
{
	BEGIN_CMD(cmd);
	BEGIN_DATA;
	WRITE_DATA(d0);
	WRITE_DATA(d1);
	END_CMD;
}

void T4_ILI9341::Cmd8Data3(uint8_t cmd, uint8_t d0, uint8_t d1, uint8_t d2)
{
	BEGIN_CMD(cmd);
	BEGIN_DATA;
	WRITE_DATA(d0);
	WRITE_DATA(d1);
	WRITE_DATA(d2);
	END_CMD;
}

void T4_ILI9341::Cmd8Data4(uint8_t cmd, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	BEGIN_CMD(cmd);
	BEGIN_DATA;
	WRITE_DATA(d0);
	WRITE_DATA(d1);
	WRITE_DATA(d2);
	WRITE_DATA(d3);
	END_CMD;
}

void T4_ILI9341::Cmd8Data5(uint8_t cmd, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
							uint8_t d4)
{
	BEGIN_CMD(cmd);
	BEGIN_DATA;
	WRITE_DATA(d0);
	WRITE_DATA(d1);
	WRITE_DATA(d2);
	WRITE_DATA(d3);
	WRITE_DATA(d4);
	END_CMD;
}

void T4_ILI9341::Cmd8Data6(uint8_t cmd, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
							uint8_t d4, uint8_t d5)
{
	BEGIN_CMD(cmd);
	BEGIN_DATA;
	WRITE_DATA(d0);
	WRITE_DATA(d1);
	WRITE_DATA(d2);
	WRITE_DATA(d3);
	WRITE_DATA(d4);
	WRITE_DATA(d5);
	END_CMD;
}

void T4_ILI9341::Cmd8DataN(uint8_t cmd, uint8_t* d, uint32_t n)
{
	BEGIN_CMD(cmd);
	BEGIN_DATA;
	for(uint32_t i = 0; i < n; ++i)
	{
		WRITE_DATA(d[i]);
	}
	END_CMD;
}

void T4_ILI9341::BeginReadMode(uint8_t cmd)
{
	BEGIN_CMD(cmd);
	BEGIN_DATA;
	ILI9341_START_READ();
	uint8_t discard = ReadNext8();
	(void)discard;
}

uint8_t T4_ILI9341::ReadNext8()
{
	ILI9341_STROBE_PIN(mPinRead);
	return ILI9341_READ_DATA_BUS();
}

void T4_ILI9341::EndReadMode()
{
	ILI9341_END_READ();
	END_CMD;
}

}
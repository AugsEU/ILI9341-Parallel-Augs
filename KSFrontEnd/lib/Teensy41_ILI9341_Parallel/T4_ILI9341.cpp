// ============================================================================
// Include
// ============================================================================
#include "T4_ILI9341.h"
#include "ILI9341_Constants.h"

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
	ILI9341_WRITE_PIN_FAST(mPinCS, HIGH); \
	delay(10)


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
}


PixelFormat T4_ILI9341::ReadDisplayPixelFormat()
{
}


uint8_t T4_ILI9341::ReadDisplayImageFormat()
{
}


DisplaySignalMode T4_ILI9341::ReadDisplaySignalMode()
{
}


DisplaySelfDiagnostic T4_ILI9341::ReadDisplaySelfDiagnostic()
{
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
	uint16_t sc15_8 = startCol >> 8;
	uint16_t sc7_0 = startCol & 0xFF;
	uint16_t ec15_8 = endCol >> 8;
	uint16_t ec7_0 = endCol & 0xFF;
	Cmd8Data4(ILI9341_CMD_COL_ADDR_SET, sc15_8, sc7_0, ec15_8, ec7_0);
}


void T4_ILI9341::PageAddrSet(uint16_t startPage, uint16_t endPage)
{
	uint16_t sp15_8 = startPage >> 8;
	uint16_t sp7_0 = startPage & 0xFF;
	uint16_t ep15_8 = endPage >> 8;
	uint16_t ep7_0 = endPage & 0xFF;
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



uint8_t* T4_ILI9341::MemoryRead(uint32_t size)
{
}



void T4_ILI9341::PartialArea(uint16_t startRow, uint16_t endRow)
{
}



void T4_ILI9341::VerticalScrollingDefinition(uint16_t topFixedArea, 
								uint16_t vertialScrollArea,
								uint16_t bottomFixedArea)
{
}



void T4_ILI9341::TearingEffectLineOff()
{
}


void T4_ILI9341::TearingEffectLineOn(TearingEffectMode mode)
{
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
}



void T4_ILI9341::IdleModeOff()
{
}


void T4_ILI9341::IdleModeOn()
{
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
}


uint16_t* T4_ILI9341::ReadMemoryContinue(uint32_t size)
{
}

 // you take ownership of this pointer

void T4_ILI9341::SetTearScanline(uint16_t scanLine)
{
}


uint16_t T4_ILI9341::GetScanline()
{
}



void T4_ILI9341::WriteDisplayBrightness(uint8_t brightness)
{
}


uint8_t T4_ILI9341::ReadDisplayBrightness()
{
}



void T4_ILI9341::WriteCtrlDisplay(bool brightnessBlock, bool displayDimming, bool backlightOnOff)
{
}


CtrlDisplay T4_ILI9341::ReadCtrlDisplay()
{
}



void T4_ILI9341::WriteAdaptiveBrightness(AdaptiveBrightnessCtrl mode)
{
}


AdaptiveBrightnessCtrl T4_ILI9341::ReadAdaptiveBrightness()
{
}



void T4_ILI9341::WriteCabcMinBrightness(uint8_t brightness)
{
}


uint8_t T4_ILI9341::ReadCabcMinBrightness()
{
}



uint8_t T4_ILI9341::ReadId1()
{
}


uint8_t T4_ILI9341::ReadId2()
{
}


uint8_t T4_ILI9341::ReadId3()
{
}



// T4_ILI9341::Level 2 commands
void T4_ILI9341::RgbInterfaceSignalCtrl(RgbBypassMode bypass, 
							SyncPolarity vspl, 
							SyncPolarity hspl, 
							TransitionPolarity dotclk, 
							bool eplLow)
{
}



void T4_ILI9341::FrameRateCtrl(uint8_t divisionRatio, uint8_t rtn)
{
	Cmd8Data2(ILI9341_CMD2_FRAME_RATE_CTL_NORM, divisionRatio & 0b00000011,
												rtn & 0b00011111);
}


void T4_ILI9341::FrameRateCtrlIdle(uint8_t divisionRatio, uint8_t rtn)
{
}


void T4_ILI9341::FrameRateCtrlPartial(uint8_t divisionRatio, uint8_t rtn)
{
}



void T4_ILI9341::DisplayInversionCtrl(bool fullColor, bool idle, bool partial)
{
}


void T4_ILI9341::BlankPorchCtrl(uint8_t vfp, uint8_t vbp, uint8_t hfp, uint8_t hbp)
{
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
}


void T4_ILI9341::BacklightControl2(uint8_t levelStillPicture, uint8_t levelMovingImage)
{
}


void T4_ILI9341::BacklightControl3(uint8_t dthUi)
{
}


void T4_ILI9341::BacklightControl4(uint8_t dth_st, uint8_t dth_mv)
{
}


void T4_ILI9341::BacklightControl5(uint8_t dim1, uint8_t dim2)
{
}


void T4_ILI9341::BacklightControl7(uint8_t pwm_div)
{
}


void T4_ILI9341::BacklightControl8(bool ledpwmpol, bool ledonpol, bool ledonr)
{
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
		data | 0b10000000;
	}
	Cmd8Data1(ILI9341_CMD2_VCOM_CTL_2, data);
}



void T4_ILI9341::NvMemoryWrite(PGMAddr pgmAddress, uint8_t pgmData)
{
}


void T4_ILI9341::NvMemoryProtectionKey(uint32_t key)
{
}


NvMemoryStatus T4_ILI9341::NvStatusRead()
{
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
}



void T4_ILI9341::NegativeGammaCorrection(GammaCorrection table)
{
}



void T4_ILI9341::DigitalGammaControl1(GammaColorCorrection1 table)
{
}


void T4_ILI9341::DigitalGammaControl2(GammaColorCorrection2 table)
{
}



void T4_ILI9341::InterfaceControl(bool myEor, bool mxEor, bool mvEor, bool bgrEor, uint8_t mdt, bool weMode,
						Endianess endian, DisplayOperationMode dm, GramInterfaceMode rm,
						bool rim, ColorFormat65K epf)
{
}



// T4_ILI9341::Extend register commands
void T4_ILI9341::PowerCtrlA(uint8_t vcore, uint8_t ddvdh)
{
}


void T4_ILI9341::PowerCtrlB(uint8_t powerCtrl, bool pceq, bool drvEna, 
			uint8_t drvVml, uint8_t drvVmh, bool esdProtection)
{
}


void T4_ILI9341::DriverTimingCtrlA1(bool eq, bool now, bool cr, PreChargeTimingControl pc)
{
}


void T4_ILI9341::DriverTimingCtrlA2(bool eqe, bool nowe, bool cre, PreChargeTimingControl pc)
{
}

 // what is this??
void T4_ILI9341::DriverTimingCtrlB(uint8_t vgSwT1, uint8_t vgSwT2, uint8_t vgSwT3, uint8_t vgSwT4)
{
}


void T4_ILI9341::PowerSequenceCtrl(uint8_t cp1SoftStart, uint8_t cp23SoftStart, uint8_t enVcl,
						uint8_t enDdvdh, uint8_t enVgh, uint8_t enVgl, bool ddvdhEnable)
{
}


void T4_ILI9341::Enable3Gamma(bool enable)
{
}


void T4_ILI9341::PumpRatioCtrl(PumpRatio ratio)
{
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
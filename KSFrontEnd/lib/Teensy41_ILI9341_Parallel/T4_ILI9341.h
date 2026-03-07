// ============================================================================
// ILI9341 display driver code parallel interface meant for teensy 4.1
// ----------------------------------------------------------------------------
// ============================================================================
// Include
// ============================================================================
#include <stdint.h>
#include "ILI9341_Types.h"

#ifndef ILI9341_PARALLEL_H
#define ILI9341_PARALLEL_H

namespace ILI9341
{

// ============================================================================
// T4_ILI9341
// ============================================================================
class T4_ILI9341
{
public:
	constexpr static int WIDTH = 240;
	constexpr static int HEIGHT = 320;

	// Init
	T4_ILI9341(uint8_t cs, uint8_t cd, uint8_t wr, uint8_t rd);

	// Command functions are named based off the datasheet. Many of this inconsistent but oh well...
	// e.g. some are "get" some are "read", some have "read" at the start, others at the end...
	// Level 1 commands

	void SoftwareReset();
	
	DisplayIdentification ReadDisplayIdentification();
	DisplayStatus ReadDisplayStatus();
	DisplayPowerMode ReadDisplayPowerMode();
	DisplayMADCTL ReadDisplayMADCTL();
	PixelFormat ReadDisplayPixelFormat();
	uint8_t ReadDisplayImageFormat();
	DisplaySignalMode ReadDisplaySignalMode();
	DisplaySelfDiagnostic ReadDisplaySelfDiagnostic();

	void EnterSleepMode();
	void SleepOut();
	void PartialModeOn();
	void NormalDisplayModeOn();
	void DisplayInversionOff();
	void DisplayInversionOn();

	void GammaSet(uint8_t gammaIndex);
   
	void DisplayOff();
	void DisplayOn();

	void ColumnAddrSet(uint16_t startCol, uint16_t endCol);
	void PageAddrSet(uint16_t startPage, uint16_t endPage);
	void MemoryWrite(uint16_t* buff, uint32_t size);

	void ColorSet(ColorLUT table);

	uint8_t* MemoryRead(uint32_t size); // you take ownership of this pointer

	void PartialArea(uint16_t startRow, uint16_t endRow);

	void VerticalScrollingDefinition(uint16_t topFixedArea, 
									uint16_t vertialScrollArea,
									uint16_t bottomFixedArea);

	void TearingEffectLineOff();
	void TearingEffectLineOn(TearingEffectMode mode);

	void MemoryAccessCtrl(RowOrder rowOrder, 
						ColOrder colOrder, 
						RowColExchange exchange, 
						RowOrder verticalRefresh,
						RgbOrder rgbOrder,
						ColOrder horizontalRefresh);
	
	void VerticalScrollStartAddr(uint16_t start);

	void IdleModeOff();
	void IdleModeOn();

	void PixelFormatSet(PixelFormat format);

	void WriteMemoryContinue(uint16_t* buff, uint32_t size);
	uint16_t* ReadMemoryContinue(uint32_t size); // you take ownership of this pointer

	void SetTearScanline(uint16_t scanLine);
	uint16_t GetScanline();

	void WriteDisplayBrightness(uint8_t brightness);
	uint8_t ReadDisplayBrightness();

	void WriteCtrlDisplay(bool brightnessBlock, bool displayDimming, bool backlightOnOff);
	CtrlDisplay ReadCtrlDisplay();

	void WriteAdaptiveBrightness(AdaptiveBrightnessCtrl mode);
	AdaptiveBrightnessCtrl ReadAdaptiveBrightness();

	void WriteCabcMinBrightness(uint8_t brightness);
	uint8_t ReadCabcMinBrightness();

	uint8_t ReadId1();
	uint8_t ReadId2();
	uint8_t ReadId3();

	// Level 2 commands
	void RgbInterfaceSignalCtrl(RgbBypassMode bypass, 
								SyncPolarity vspl, 
								SyncPolarity hspl, 
								TransitionPolarity dotclk, 
								bool eplLow);

	void FrameRateCtrl(uint8_t divisionRatio, uint8_t rtn);
	void FrameRateCtrlIdle(uint8_t divisionRatio, uint8_t rtn);
	void FrameRateCtrlPartial(uint8_t divisionRatio, uint8_t rtn);

	void DisplayInversionCtrl(bool fullColor, bool idle, bool partial);
	void BlankPorchCtrl(uint8_t vfp, uint8_t vbp, uint8_t hfp, uint8_t hbp);

	void DisplayFunctionCtrl(ScanMode scanMode, 
							SourceOutput pt,
							ShiftDirectionOfSourceDriver ss,
							LiquidCrystalType rev,
							uint8_t isc,
							GateOutputScanDiraction gs,
							bool sm,
							uint8_t nl,
							uint8_t pcdiv); // No idea what these params stand for...

	void EntryModeSet(bool lowVoltageDetection, GateOuputLevel output);

	// You must read datasheet to understand these...
	void BacklightControl1(uint8_t levelUi);
	void BacklightControl2(uint8_t levelStillPicture, uint8_t levelMovingImage);
	void BacklightControl3(uint8_t dthUi);
	void BacklightControl4(uint8_t dth_st, uint8_t dth_mv);
	void BacklightControl5(uint8_t dim1, uint8_t dim2);
	void BacklightControl7(uint8_t pwm_div);
	void BacklightControl8(bool ledpwmpol, bool ledonpol, bool ledonr);

	void PowerCtrl1(uint8_t gvddLevel);
	void PowerCtrl2(uint8_t stepUpFactor);

	void VcomCtrl1(uint8_t vcomhVoltage, uint8_t vcomlVoltage);
	void VcomCtrl2(bool nVM, uint8_t vcomOffset);

	void NvMemoryWrite(PGMAddr pgmAddress, uint8_t pgmData);
	void NvMemoryProtectionKey(uint32_t key);
	NvMemoryStatus NvStatusRead();
	Id4 ReadId4();

	void PositiveGammaCorrection(GammaCorrection table);

	void NegativeGammaCorrection(GammaCorrection table);
	
	void DigitalGammaControl1(GammaColorCorrection1 table);
	void DigitalGammaControl2(GammaColorCorrection2 table);

	void InterfaceControl(bool myEor, bool mxEor, bool mvEor, bool bgrEor, uint8_t mdt, bool weMode,
							Endianess endian, DisplayOperationMode dm, GramInterfaceMode rm,
							bool rim, ColorFormat65K epf);

	// Extend register commands
	void PowerCtrlA(uint8_t vcore, uint8_t ddvdh);
	void PowerCtrlB(uint8_t powerCtrl, bool pceq, bool drvEna, 
				uint8_t drvVml, uint8_t drvVmh, bool esdProtection);
	void DriverTimingCtrlA1(bool eq, bool now, bool cr, PreChargeTimingControl pc);
	void DriverTimingCtrlA2(bool eqe, bool nowe, bool cre, PreChargeTimingControl pc); // what is this??
	void DriverTimingCtrlB(uint8_t vgSwT1, uint8_t vgSwT2, uint8_t vgSwT3, uint8_t vgSwT4);
	void PowerSequenceCtrl(uint8_t cp1SoftStart, uint8_t cp23SoftStart, uint8_t enVcl,
							uint8_t enDdvdh, uint8_t enVgh, uint8_t enVgl, bool ddvdhEnable);
	void Enable3Gamma(bool enable);
	void PumpRatioCtrl(PumpRatio ratio);

private:
	void Cmd8Data0(uint8_t cmd);
	void Cmd8Data1(uint8_t cmd, uint8_t d0);
	void Cmd8Data2(uint8_t cmd, uint8_t d0, uint8_t d1);
	void Cmd8Data3(uint8_t cmd, uint8_t d0, uint8_t d1, uint8_t d2);
	void Cmd8Data4(uint8_t cmd, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
	void Cmd8DataN(uint8_t cmd, uint8_t* d, uint32_t n);
	
	void BeginReadMode(uint8_t cmd);
	uint8_t ReadNext8();
	void EndReadMode();

	uint8_t mPinCS = 0xFF; // Chip select
	uint8_t mPinCD = 0xFF; // Data/Command pin
	uint8_t mPinWrite = 0xFF;
	uint8_t mPinRead = 0xFF;
};

}
#endif //ILI9341_PARALLEL_H

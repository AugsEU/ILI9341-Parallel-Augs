// ============================================================================
// ILI9341 display structures that are given back to us
// ----------------------------------------------------------------------------
// ============================================================================
// Include
// ============================================================================
#include <stdint.h>

#ifndef ILI9341_TYPES_H
#define ILI9341_TYPES_H

namespace ILI9341
{

enum RowOrder : bool
{
	TopToBottom,
	BottomToTop
};

enum ColOrder : bool
{
	LeftToRight,
	RightToLeft
};

enum RowColExchange : bool
{
	Normal,
	Reverse
};

enum RgbOrder : bool
{
	RGB,
	BGR
};

enum PixelFormat : bool
{
	Form16Bit,
	Form18Bit
};

enum TearingEffectMode : bool
{
	VBlankOnly,
	VBlankHBlank,
};

enum AdaptiveBrightnessCtrl : uint8_t
{
	Off,
	UserInterfaceImage,
	StillPicture,
	MovingImage
};

enum RgbBypassMode : bool
{
	DirectToShiftRegister,
	Memory
};

enum SyncPolarity : bool
{
	LowLevelSyncClock,
	HighLevelSyncClock
};

enum TransitionPolarity : bool
{
	RisingEdge,
	FallingEdge
};

enum ScanMode : bool
{
	NormalScan,
	IntervalScan
};

enum SourceOutput : uint8_t
{
	V64_V0,
	V0_V63,
	AGND,
	HiZ
};

enum ShiftDirectionOfSourceDriver : bool
{
	S1_S720,
	S720_S1
};

enum LiquidCrystalType : bool
{
	NormallyBlack,
	NormallyWhite
};

enum GateOutputScanDiraction : bool
{
	G1_G320,
	G320_G1
};

enum GateOuputLevel : uint8_t
{
	VGH_0,
	VGH_1, // Are these the same?
	VGL,
	Nominal
};

enum PGMAddr : uint8_t
{
	Id1Programming,
	Id2Programming,
	Id3Programming,
	VmfProgramming
};

enum class Endianess : uint8_t
{
	BigEndian,
	LittleEndian
};

enum class DisplayOperationMode : uint8_t
{
	InternalClockOperation,
	RgbInterface,
	VsyncInterface,
	SettingDiabled
};

enum class GramInterfaceMode : uint8_t
{
	VsyncInterface,
	RgbInterface
};

enum class ColorFormat65K : uint8_t
{
	LsbMsb = 0b00, // R5 copied to R0, B5 copied to B0
	LsbZero = 0b01, // R0 and B0 always 0 
	LsbOne = 0b10, // R0 and B0 always 1
	LsbCompare = 0b11 // G0 copied to R0 and B0 conditionally (see datasheet)
};

enum class PreChargeTimingControl : uint8_t
{
	Default2Unit = 0b00,
	Default1Unit = 0b01,
	DefaultPreCharge = 0b10
	/* Reserved = 0b11 */
};

enum class PumpRatio : uint8_t
{
	VCIx2 = 0b10,
	VCIx3 = 0b11
};

// Special return types
struct DisplayIdentification
{
	uint8_t mManufacturerId = 0;
	uint8_t mDriverVersion = 0;
	uint8_t mDriverId = 0;
};

struct DisplayStatus
{
	// Packed bit fields. Recommended to use utility methods below.
	uint32_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline bool BoosterVolatageEnabled() const { return READ_BOOL(31); }
	inline RowOrder RowAddrOrder() const { return READ_BOOL(30) ? BottomToTop : TopToBottom; }
	inline ColOrder ColAddrOrder() const { return READ_BOOL(29) ? RightToLeft : LeftToRight; }
	inline RowColExchange RowColExchange() const { return READ_BOOL(28) ? Reverse : Normal; }
	inline RowOrder VerticalRefresh() const { return READ_BOOL(27) ? BottomToTop : TopToBottom; }
	inline RgbOrder RgbOrder() const { return READ_BOOL(26) ? BGR : RGB; }
	inline ColOrder HorizontalRefresh() const { return READ_BOOL(25) ? RightToLeft : LeftToRight; }
	inline PixelFormat PixelFormat() const { return READ_BOOL(21) ? Form18Bit : Form16Bit; }
	inline bool IdleMode() const { return READ_BOOL(29); }
	inline bool SleepOut() const { return READ_BOOL(17); }
	inline bool DisplayInNormalMode() const { return READ_BOOL(16); }
	inline bool DisplayOn() const { return READ_BOOL(10); }
	inline bool TearingEffectOn() const { return READ_BOOL(9); }
	inline uint8_t GammaCurveIndex() const
	{
		uint32_t gammaBits = mPackedBits & (0xE0);
		gammaBits >>= 5;
		return (uint8_t)gammaBits;
	}
	inline TearingEffectMode TearingEffectMode() const { return READ_BOOL(5) ? VBlankHBlank : VBlankOnly; }

	#undef READ_BOOL
};

struct DisplayPowerMode
{
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline bool BoosterEnabled() const { return READ_BOOL(7); }
	inline bool IdleMode() const { return READ_BOOL(6); }
	inline bool PartialMode() const { return READ_BOOL(5); }
	inline bool SleepOut() const { return READ_BOOL(4); }
	inline bool DisplayInNormalMode() const { return READ_BOOL(3); }
	inline bool DisplayOn() const { return READ_BOOL(2); }

	#undef READ_BOOl
};

struct DisplayMADCTL
{
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline RowOrder RowAddrOrder() const { return READ_BOOL(7) ? BottomToTop : TopToBottom; }
	inline ColOrder ColAddrOrder() const { return READ_BOOL(6) ? RightToLeft : LeftToRight; }
	inline RowColExchange RowColExchange() const { return READ_BOOL(5) ? Reverse : Normal; }
	inline RowOrder VerticalRefresh() const { return READ_BOOL(4) ? BottomToTop : TopToBottom; }
	inline RgbOrder RgbOrder() const { return READ_BOOL(3) ? BGR : RGB; }
	inline ColOrder HorizontalRefresh() const { return READ_BOOL(2) ? RightToLeft : LeftToRight; }

	#undef READ_BOOl
};

struct DisplaySignalMode
{
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline bool TearingEffectOn() const { return READ_BOOL(7); }
	inline TearingEffectMode TearingEffectMode() const { return READ_BOOL(6) ? VBlankHBlank : VBlankOnly; }
	inline bool HorizontalSyncOn() const { return READ_BOOL(5); }
	inline bool VerticalSyncOn() const { return READ_BOOL(4); }
	inline bool PixelClockOn() const { return READ_BOOL(3); }
	inline bool DataEnable() const { return READ_BOOL(2); }

	#undef READ_BOOl
};

struct DisplaySelfDiagnostic
{
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline bool RegisterLoadingDetection() const { return READ_BOOL(7); }
	inline bool FunctionalityDetection() const { return READ_BOOL(7); }

	#undef READ_BOOl
};

struct CtrlDisplay
{	
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline bool BrightnessControlBlock() const { return READ_BOOL(5); }
	inline bool DisplayDimming() const { return READ_BOOL(3); }
	inline bool BacklightOnOff() const { return READ_BOOL(2); }

	#undef READ_BOOl
};

struct NvMemoryStatus
{
	uint8_t mId2Count = 0;
	uint8_t mId1Count = 0;
	bool mBusy = false;
	uint8_t mVmfCount = 0;
	uint8_t mId3Count = 0;
};

struct Id4
{
	uint8_t mIcVersion = 0;
	uint16_t mIcModelName = 0;
	
	bool IsValid()
	{
		// Our specific thing should always return this...
		return mIcVersion == 0 && mIcModelName == 0x9341;
	}
};

// Complete 16-bit to 18-bit color table
struct ColorLUT
{
	uint8_t mReds[32]; // 5-bits per color
	uint8_t mGreens[64]; // 10-bits per color
	uint8_t mBlues[32]; // 5-bits per color
};

struct GammaCorrection
{
	uint8_t mVp63;
	uint8_t mVp62;
	uint8_t mVp61;
	uint8_t mVp59;
	uint8_t mVp57;
	uint8_t mVp50;
	uint8_t mVp43;
	uint8_t mVp27;
	uint8_t mVp36;
	uint8_t mVp20;
	uint8_t mVp13;
	uint8_t mVp6;
	uint8_t mVp4;
	uint8_t mVp2;
	uint8_t mVp1;
	uint8_t mVp0;
};

struct GammaColorCorrection1
{
	uint8_t mRedCurve[16];
	uint8_t mBlueCurve[16];
};

struct GammaColorCorrection2
{
	uint8_t mRedCurve[64];
	uint8_t mBlueCurve[64];
};

}

#endif // ILI9341_TYPES_H

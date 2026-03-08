// ============================================================================
// ILI9341 display structures that are given back to us
// ----------------------------------------------------------------------------
// ============================================================================
// Include
// ============================================================================
#include <stdint.h>
#include <stdlib.h>

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
	Off = 0b00,
	UserInterfaceImage = 0b01,
	StillPicture = 0b10,
	MovingImage = 0b11
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

enum ScanMode : uint8_t
{
	NormalScan = 0b00,
	IntervalScan = 0b10
};

enum SourceOutput : uint8_t
{
	V64_V0 = 0b00,
	V0_V63 = 0b01,
	AGND   = 0b10,
	HiZ    = 0b11
};

enum ShiftDirectionOfSourceDriver : uint8_t
{
	S1_S720 = 0b0,
	S720_S1 = 0b1
};

enum LiquidCrystalType : uint8_t
{
	NormallyBlack = 0b0,
	NormallyWhite = 0b1
};

enum GateOutputScanDiraction : uint8_t
{
	G1_G320 = 0b0,
	G320_G1 = 0b1
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
	Id1Programming = 0b000,
	Id2Programming = 0b001,
	Id3Programming = 0b010,
	VmfProgramming = 0b100
};

enum class Endianess : uint8_t
{
	BigEndian = 0,
	LittleEndian = 1
};

enum class DisplayOperationMode : uint8_t
{
	InternalClockOperation = 0b00,
	RgbInterface = 0b01,
	VsyncInterface = 0b10,
	SettingDiabled = 0b11
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

enum class RgbInterfaceRCM : uint8_t
{
	DeMode = 0b10,
	SyncMode = 0b11
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

	inline bool GetBoosterVolatageEnabled() const { return READ_BOOL(31); }
	inline RowOrder GetRowAddrOrder() const { return READ_BOOL(30) ? BottomToTop : TopToBottom; }
	inline ColOrder GetColAddrOrder() const { return READ_BOOL(29) ? RightToLeft : LeftToRight; }
	inline RowColExchange GetRowColExchange() const { return READ_BOOL(28) ? Reverse : Normal; }
	inline RowOrder GetVerticalRefresh() const { return READ_BOOL(27) ? BottomToTop : TopToBottom; }
	inline RgbOrder GetRgbOrder() const { return READ_BOOL(26) ? BGR : RGB; }
	inline ColOrder GetHorizontalRefresh() const { return READ_BOOL(25) ? RightToLeft : LeftToRight; }
	inline PixelFormat GetPixelFormat() const { return READ_BOOL(21) ? Form18Bit : Form16Bit; }
	inline bool GetIdleMode() const { return READ_BOOL(29); }
	inline bool GetSleepOut() const { return READ_BOOL(17); }
	inline bool GetDisplayInNormalMode() const { return READ_BOOL(16); }
	inline bool GetDisplayOn() const { return READ_BOOL(10); }
	inline bool GetTearingEffectOn() const { return READ_BOOL(9); }
	inline uint8_t GetGammaCurveIndex() const
	{
		uint32_t gammaBits = mPackedBits & (0xE0);
		gammaBits >>= 5;
		return (uint8_t)gammaBits;
	}
	inline TearingEffectMode GetTearingEffectMode() const { return READ_BOOL(5) ? VBlankHBlank : VBlankOnly; }

	#undef READ_BOOL
};

struct DisplayPowerMode
{
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline bool GetBoosterEnabled() const { return READ_BOOL(7); }
	inline bool GetIdleMode() const { return READ_BOOL(6); }
	inline bool GetPartialMode() const { return READ_BOOL(5); }
	inline bool GetSleepOut() const { return READ_BOOL(4); }
	inline bool GetDisplayInNormalMode() const { return READ_BOOL(3); }
	inline bool GetDisplayOn() const { return READ_BOOL(2); }

	#undef READ_BOOl
};

struct DisplayMADCTL
{
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline RowOrder GetRowAddrOrder() const { return READ_BOOL(7) ? BottomToTop : TopToBottom; }
	inline ColOrder GetColAddrOrder() const { return READ_BOOL(6) ? RightToLeft : LeftToRight; }
	inline RowColExchange GetRowColExchange() const { return READ_BOOL(5) ? Reverse : Normal; }
	inline RowOrder GetVerticalRefresh() const { return READ_BOOL(4) ? BottomToTop : TopToBottom; }
	inline RgbOrder GetRgbOrder() const { return READ_BOOL(3) ? BGR : RGB; }
	inline ColOrder GetHorizontalRefresh() const { return READ_BOOL(2) ? RightToLeft : LeftToRight; }

	#undef READ_BOOl
};

struct DisplaySignalMode
{
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline bool GetTearingEffectOn() const { return READ_BOOL(7); }
	inline TearingEffectMode GetTearingEffectMode() const { return READ_BOOL(6) ? VBlankHBlank : VBlankOnly; }
	inline bool GetHorizontalSyncOn() const { return READ_BOOL(5); }
	inline bool GetVerticalSyncOn() const { return READ_BOOL(4); }
	inline bool GetPixelClockOn() const { return READ_BOOL(3); }
	inline bool GetDataEnable() const { return READ_BOOL(2); }

	#undef READ_BOOl
};

struct DisplaySelfDiagnostic
{
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline bool GetRegisterLoadingDetection() const { return READ_BOOL(7); }
	inline bool GetFunctionalityDetection() const { return READ_BOOL(7); }

	#undef READ_BOOl
};

struct CtrlDisplay
{	
	// Packed bit fields. Recommended to use utility methods below.
	uint8_t mPackedBits = 0;

	#define READ_BOOL(x) ((mPackedBits & (1u << x)) != 0)

	inline bool GetBrightnessControlBlock() const { return READ_BOOL(5); }
	inline bool GetDisplayDimming() const { return READ_BOOL(3); }
	inline bool GetBacklightOnOff() const { return READ_BOOL(2); }

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
	uint8_t mVp27_36;
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
	static constexpr size_t NUM_ENTRIES = 16;
	uint8_t mRedCurve[NUM_ENTRIES];
	uint8_t mBlueCurve[NUM_ENTRIES];
};

struct GammaColorCorrection2
{
	static constexpr size_t NUM_ENTRIES = 64;
	uint8_t mRedCurve[NUM_ENTRIES];
	uint8_t mBlueCurve[NUM_ENTRIES];
};

}

#endif // ILI9341_TYPES_H

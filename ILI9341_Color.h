// ============================================================================
// ILI9341 display color format
// ----------------------------------------------------------------------------
#pragma once
// ============================================================================
// Include
// ============================================================================
#include <stdint.h>
#include "ILI9341_Constants.h"

namespace ILI9341
{

// Format 5R 6G 5B
typedef uint16_t ILIColor;

inline void SetRed(ILIColor& col, uint8_t r)
{
	col = ((col & ~(ILI9341_RED)) | (r << 11));
}

inline void SetGreen(ILIColor& col, uint8_t g)
{
	col = ((col & ~(ILI9341_GREEN)) | (g << 5));
}

inline void SetBlue(ILIColor& col, uint8_t b)
{
	col = ((col & ~(ILI9341_BLUE)) | (b));
}

inline uint8_t GetRed(ILIColor& col)
{
	return (col & ILI9341_RED) >> 11;
}

inline uint8_t GetGreen(ILIColor& col)
{
	return (col & ILI9341_GREEN) >> 5;
}

inline uint8_t GetBlue(ILIColor& col)
{
	return (col & ILI9341_BLUE);
}

inline ILIColor DarkerColor(ILIColor col)
{
	// Quickly half all 3 color values
	col >>= 1;
	col &= 0b1111101111101111;

	return col;
}

}
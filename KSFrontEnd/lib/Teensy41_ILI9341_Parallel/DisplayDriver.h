// ============================================================================
// ILI9341 display driver. Use to abstract functions from the raw device 
// commands.
// ----------------------------------------------------------------------------
// ============================================================================
// Include
// ============================================================================
#include <stdint.h>
#include "T4_ILI9341.h"

#ifndef ILI9341_DISPLAY_DRIVER_H
#define ILI9341_DISPLAY_DRIVER_H

namespace ILI9341
{

// ============================================================================
// DisplayDriver - Abstract display driver.
// ============================================================================
class DisplayDriver
{
public:
	enum class Status
    {
        Offline,
        Online // Device successfully setup and ready to use.
    };

	/// @brief Create new display driver
	/// @param device ILI device to interface with.
	DisplayDriver(T4_ILI9341 device);

	/// @brief Begin driver after the screen has been created
    /// @return Error code.
    int Begin();

	/// @brief Directly write color to every pixel in LCD.
	/// @param col Color to write
	/// @return Error code.
	void ForceClear(ILIColor col);

	// Drawing methods which use abstract methods below.

	/// @brief Draw 1px line between sx and sy
	/// @param sx start x
	/// @param sy stary y
	/// @param ex end x
	/// @param ey end y
	/// @param col line color
	void DrawLine(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, ILIColor col);

	// Abstract methods

	/// @brief Draw a single pixel
	/// @param x x-coordinate
	/// @param y y-coordinate
	/// @param col packed 16-bit color
	virtual void DrawPixel(uint16_t x, uint16_t y, ILIColor col) = 0;

	/// @brief Draw rectangle
	/// @param x top-left x-coordinate
	/// @param y top-left y-coordinate
	/// @param w width
	/// @param h height
	/// @param col packed 16-bit color
	virtual void DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ILIColor col);

protected:

	/// @brief Attempt to connect to device
	/// @return Error code
	int EstablishConnection();

	T4_ILI9341 mDevice;
	Status mStatus;
};

}

#endif // ILI9341_DISPLAY_DRIVER_H
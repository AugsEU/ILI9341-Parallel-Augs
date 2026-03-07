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

	int TestWritePixel(uint16_t x, uint16_t y, uint16_t col);

private:

	/// @brief Attempt to connect to device
	/// @return Error code
	int EstablishConnection();

	T4_ILI9341 mDevice;
	Status mStatus;
};

}

#endif // ILI9341_DISPLAY_DRIVER_H
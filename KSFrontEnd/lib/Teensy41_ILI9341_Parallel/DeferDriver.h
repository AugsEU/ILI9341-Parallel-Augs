// ============================================================================
// ILI9341 deferred display driver. Writes to buffer then later renders out to
// the screen.
// ----------------------------------------------------------------------------
// ============================================================================
// Include
// ============================================================================
#include <stdint.h>
#include "DisplayDriver.h"
#include "T4_ILI9341.h"

#ifndef ILI9341_DEFER_DRIVER_H
#define ILI9341_DEFER_DRIVER_H

namespace ILI9341
{

// ============================================================================
// DeferDriver. ILI9341 driver that writes to buffer then later renders out to
// the screen later (i.e. it is deferred).
// ============================================================================
class DeferDriver final : public DisplayDriver
{

};

}

#endif // ILI9341_DEFER_DRIVER_H
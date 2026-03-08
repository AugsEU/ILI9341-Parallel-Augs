// ============================================================================
// Functions to display things on the screen.
// ----------------------------------------------------------------------------
// ============================================================================
// Include
// ============================================================================
#include <Arduino.h>

#ifndef SCREEN_DISPLAY_H
#define SCREEN_DISPLAY_H

// ============================================================================
// Public constants
// ============================================================================
constexpr uint16_t SC_BLACK   = 0x0000;
constexpr uint16_t SC_BLUE    = 0x001F;
constexpr uint16_t SC_RED     = 0xF800;
constexpr uint16_t SC_GREEN   = 0x07E0;
constexpr uint16_t SC_CYAN    = 0x07FF;
constexpr uint16_t SC_MAGENTA = 0xF81F;
constexpr uint16_t SC_YELLOW  = 0xFFE0;
constexpr uint16_t SC_WHITE   = 0xFFFF;

#define USE_ADAFRUIT_LIBRARY 0



// ============================================================================
// Public functions
// ============================================================================
void SetupScreenDisplay();

void ScreenDisplayUpdate();
#endif //SCREEN_DISPLAY_H
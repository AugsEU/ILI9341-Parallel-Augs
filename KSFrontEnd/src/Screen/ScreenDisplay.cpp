// ============================================================================
// Include
// ============================================================================
#include "Screen/ScreenDisplay.h"
#include "Screen/ScreenTest.h"

#if USE_ADAFRUIT_LIBRARY
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#else // USE_ADAFRUIT_LIBRARY
#include <DeferDriver.h>
#include <ILI9341_Constants.h>
#endif // USE_ADAFRUIT_LIBRARY





// ============================================================================
// Constants
// ============================================================================
#define LCD_CS 9 // Chip Select
#define LCD_CD 10 // Command/Data
#define LCD_WR 11 // LCD Write
#define LCD_RD 12 // LCD Read
#define LCD_RESET 0 // Reset (disabled)





// ============================================================================
// Global variables
// ============================================================================
#if USE_ADAFRUIT_LIBRARY
Adafruit_TFTLCD gTftScreen(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#else // USE_ADAFRUIT_LIBRARY
ILI9341::T4_ILI9341 gDevice(LCD_CS, LCD_CD, LCD_WR, LCD_RD);
ILI9341::DeferDriver gDriver(gDevice);
#endif // USE_ADAFRUIT_LIBRARY



// ============================================================================
// Public functions
// ============================================================================
void SetupScreenDisplay(void) 
{
#if USE_ADAFRUIT_LIBRARY
 	gTftScreen.reset();
	gTftScreen.begin(0x9341);
#else // USE_ADAFRUIT_LIBRARY
	int err = gDriver.Begin();
	if(err != ILI9341_OK)
	{
		Serial.printf("Display error. %x", (uint32_t)err);
	}
#endif // USE_ADAFRUIT_LIBRARY

#if SCREEN_TEST_ENABLE
	RunScreenTest();
#endif // SCREEN_TEST_ENABLE
}

void ScreenDisplayUpdate()
{

}


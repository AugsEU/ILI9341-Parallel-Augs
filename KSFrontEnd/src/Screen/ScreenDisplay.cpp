// ============================================================================
// Include
// ============================================================================
#include "Screen/ScreenDisplay.h"
#include "Screen/ScreenTest.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <DisplayDriver.h>

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
//Adafruit_TFTLCD gTftScreen(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
ILI9341::T4_ILI9341 gDevice(LCD_CS, LCD_CD, LCD_WR, LCD_RD);
ILI9341::DisplayDriver gDriver(gDevice);




// ============================================================================
// Public functions
// ============================================================================
void SetupScreenDisplay(void) 
{
	//gTftScreen.reset();

	while(!Serial);
	int err = gDriver.Begin();
	Serial.printf("error: %d", err);

	delay(100);
	//gDriver.TestWritePixel(10, 10, SC_MAGENTA);
	gDriver.TestClear(SC_BLACK);
	//gDriver.TestClear(SC_BLUE);
	for(int x = 0; x < gDevice.WIDTH; x++)
	{
		for(int y = 0; y < gDevice.HEIGHT; y++)
		{
			gDriver.TestWritePixel(x, y, ((x+y)/2) % 5 == 0 ? SC_GREEN : SC_BLACK);
		}
	}
	//gDriver.TestWritePixel(11, 10, SC_MAGENTA);
	//gDriver.TestWritePixel(12, 10, SC_MAGENTA);
	//gDriver.TestWritePixel(10, 11, 0x0000);

	//uint16_t identifier = gTftScreen.readID();
	//gTftScreen.begin(identifier);

#if SCREEN_TEST_ENABLE
	//RunScreenTest();
#endif // SCREEN_TEST_ENABLE
}

void ScreenDisplayUpdate()
{

}


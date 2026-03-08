// ============================================================================
// Include
// ============================================================================
#include <Screen/ScreenTest.h>
#include <Screen/ScreenDisplay.h>

#if USE_ADAFRUIT_LIBRARY
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#else // USE_ADAFRUIT_LIBRARY
#include <DeferDriver.h>
#include <ILI9341_Constants.h>
#include <T4_ILI9341.h>
#endif // USE_ADAFRUIT_LIBRARY

#if SCREEN_TEST_ENABLE
// ============================================================================
// Macros
// ============================================================================
#if USE_ADAFRUIT_LIBRARY
#define BEGIN_PROFILE gTftScreen.fillScreen(SC_BLACK);\
					unsigned long start = micros()

#define GET_WIDTH() gTftScreen.width()
#define GET_HEIGHT() gTftScreen.height()

#else // USE_ADAFRUIT_LIBRARY
#define BEGIN_PROFILE gDriver.ForceClear(SC_BLACK);\
					  unsigned long start = micros()

#define GET_WIDTH() ILI9341::T4_ILI9341::WIDTH
#define GET_HEIGHT() ILI9341::T4_ILI9341::HEIGHT

#endif // USE_ADAFRUIT_LIBRARY

#define END_PROFILE start = micros() - start; \
					  Serial.printf("%s, %f \n", __PRETTY_FUNCTION__, (float)start / 1000.0f)




// ============================================================================
// Global variables
// ============================================================================
#if USE_ADAFRUIT_LIBRARY
extern Adafruit_TFTLCD gTftScreen;
#else // USE_ADAFRUIT_LIBRARY
extern ILI9341::DeferDriver gDriver;
#endif // USE_ADAFRUIT_LIBRARY

// ============================================================================
// Private func decl
// ============================================================================
void TestFillScreen();
void TestFilledRoundRects();
void TestRoundRects();
void TestFilledTriangles();
void TestTriangles();
void TestCircles();
void TestFilledCircles();
void TestFilledRects();
void TestRects();
void TestFastLines();
void TestLines();
void TestText();

// ============================================================================
// Public functions
// ============================================================================
void RunScreenTest()
{
	// Wait for serial
	while(!Serial) {}

	TestFilledRoundRects();
	TestRoundRects();
	TestFilledTriangles();
	TestTriangles();
	TestCircles();
	TestFilledCircles();
	TestFilledRects();
	TestRects();
	TestFastLines();
	TestLines();
	TestText();
}


// ============================================================================
// Private functions
// ============================================================================
void TestFillScreen() 
{
	BEGIN_PROFILE;

#if USE_ADAFRUIT_LIBRARY
	gTftScreen.fillScreen(SC_BLACK);
	gTftScreen.fillScreen(SC_RED);
	gTftScreen.fillScreen(SC_GREEN);
	gTftScreen.fillScreen(SC_BLUE);
	gTftScreen.fillScreen(SC_BLACK);
#else // USE_ADAFRUIT_LIBRARY
	gDriver.ForceClear(SC_BLACK);
	gDriver.ForceClear(SC_RED);
	gDriver.ForceClear(SC_GREEN);
	gDriver.ForceClear(SC_BLUE);
	gDriver.ForceClear(SC_BLACK);
	gDriver.RenderAllPixels();
#endif // USE_ADAFRUIT_LIBRARY

	END_PROFILE;
}

void TestText() 
{
	BEGIN_PROFILE;

#if USE_ADAFRUIT_LIBRARY
	gTftScreen.setCursor(0, 0);
	gTftScreen.setTextColor(SC_WHITE);  gTftScreen.setTextSize(1);
	gTftScreen.println("Hello World!");
	gTftScreen.setTextColor(SC_YELLOW); gTftScreen.setTextSize(2);
	gTftScreen.println(1234.56);
	gTftScreen.setTextColor(SC_RED);    gTftScreen.setTextSize(3);
	gTftScreen.println(0xDEADBEEF, HEX);
	gTftScreen.println();
	gTftScreen.setTextColor(SC_GREEN);
	gTftScreen.setTextSize(5);
	gTftScreen.println("Groop");
	gTftScreen.setTextSize(2);
	gTftScreen.println("I implore thee,");
	gTftScreen.setTextSize(1);
	gTftScreen.println("my foonting turlingdromes.");
	gTftScreen.println("And hooptiously drangle me");
	gTftScreen.println("with crinkly bindlewurdles,");
	gTftScreen.println("Or I will rend thee");
	gTftScreen.println("in the gobberwarts");
	gTftScreen.println("with my blurglecruncheon,");
	gTftScreen.println("see if I don't!");
#else // USE_ADAFRUIT_LIBRARY
#endif // USE_ADAFRUIT_LIBRARY

	END_PROFILE;
}

void TestLines() 
{
	int w = GET_WIDTH();
	int h = GET_HEIGHT();

	int x1, x2, y1, y2;
	
	BEGIN_PROFILE;

	x1 = y1 = 0;
	y2  = h - 1;
	for(x2=0; x2<w; x2+=6)
	{
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
#else // USE_ADAFRUIT_LIBRARY
		gDriver.DrawLine(x1, y1, x2, y2, SC_CYAN);
#endif // USE_ADAFRUIT_LIBRARY
	}

	x2 = w - 1;
	for(y2=0; y2<h; y2+=6)
	{
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
#else // USE_ADAFRUIT_LIBRARY
		gDriver.DrawLine(x1, y1, x2, y2, SC_CYAN);
#endif // USE_ADAFRUIT_LIBRARY
	}

	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;
	for(x2=0; x2<w; x2+=6)
	{
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
#else // USE_ADAFRUIT_LIBRARY
		gDriver.DrawLine(x1, y1, x2, y2, SC_CYAN);
#endif // USE_ADAFRUIT_LIBRARY
	}

	x2 = 0;
	for(y2=0; y2<h; y2+=6)
	{
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
#else // USE_ADAFRUIT_LIBRARY
		gDriver.DrawLine(x1, y1, x2, y2, SC_CYAN);
#endif // USE_ADAFRUIT_LIBRARY
	}

	x1 = 0;
	y1 = h - 1;
	y2 = 0;
	for(x2=0; x2<w; x2+=6)
	{
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
#else // USE_ADAFRUIT_LIBRARY
		gDriver.DrawLine(x1, y1, x2, y2, SC_CYAN);
#endif // USE_ADAFRUIT_LIBRARY
	}
	x2 = w - 1;
	for(y2=0; y2<h; y2+=6)
	{
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
#else // USE_ADAFRUIT_LIBRARY
		gDriver.DrawLine(x1, y1, x2, y2, SC_CYAN);
#endif // USE_ADAFRUIT_LIBRARY
	}

	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;
	for(x2=0; x2<w; x2+=6)
	{
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
#else // USE_ADAFRUIT_LIBRARY
		gDriver.DrawLine(x1, y1, x2, y2, SC_CYAN);
#endif // USE_ADAFRUIT_LIBRARY
	}
	x2 = 0;
	for(y2=0; y2<h; y2+=6)
	{
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
#else // USE_ADAFRUIT_LIBRARY
		gDriver.DrawLine(x1, y1, x2, y2, SC_CYAN);
#endif // USE_ADAFRUIT_LIBRARY
	}

#if USE_ADAFRUIT_LIBRARY
#else // USE_ADAFRUIT_LIBRARY
	gDriver.RenderAllPixels();
#endif // USE_ADAFRUIT_LIBRARY

	END_PROFILE;
}

void TestFastLines()
{
	int x, y, w = GET_WIDTH(), h = GET_HEIGHT();
	BEGIN_PROFILE;

#if USE_ADAFRUIT_LIBRARY
	for(y=0; y<h; y+=5) gTftScreen.drawFastHLine(0, y, w, SC_RED);
	for(x=0; x<w; x+=5) gTftScreen.drawFastVLine(x, 0, h, SC_GREEN);
#else // USE_ADAFRUIT_LIBRARY
	for(y=0; y<h; y+=5) gDriver.DrawLine(0, y, 0, w, SC_RED);
	for(x=0; x<w; x+=5) gDriver.DrawLine(x, 0, h, 0, SC_GREEN);
#endif // USE_ADAFRUIT_LIBRARY

	END_PROFILE;
}

void TestRects()
{
	int           n, i, i2,
					cx = GET_WIDTH()  / 2,
					cy = GET_HEIGHT() / 2;
	n = min(GET_WIDTH(), GET_HEIGHT());

	BEGIN_PROFILE;

	for(i=2; i<n; i+=6) {
		i2 = i / 2;
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawRect(cx-i2, cy-i2, i, i, SC_WHITE);
#else // USE_ADAFRUIT_LIBRARY
		// Todo: Replace this with rect outline
		gDriver.DrawRect(cx-i2, cy-i2, i, i, SC_WHITE);
#endif // USE_ADAFRUIT_LIBRARY
	}

#if USE_ADAFRUIT_LIBRARY
#else // USE_ADAFRUIT_LIBRARY
	gDriver.RenderAllPixels();
#endif // USE_ADAFRUIT_LIBRARY

	END_PROFILE;
}

void TestFilledRects()
{
	int           n, i, i2,
					cx = GET_WIDTH()  / 2+1,
					cy = GET_HEIGHT() / 2+1;
	n = min(GET_WIDTH(), GET_HEIGHT());

	BEGIN_PROFILE;

	for(i=n; i>0; i-=6)
	{
		i2 = i / 2;

#if USE_ADAFRUIT_LIBRARY
		gTftScreen.fillRect(cx-i2, cy-i2, i, i, SC_MAGENTA);

		// Outlines are not included in timing results
		gTftScreen.drawRect(cx-i2, cy-i2, i, i, SC_WHITE);
#else // USE_ADAFRUIT_LIBRARY
		gDriver.DrawRect(cx-i2, cy-i2, i, i, SC_WHITE);

#endif // USE_ADAFRUIT_LIBRARY

	}

	END_PROFILE;
}

void TestFilledCircles()
{
	uint8_t radius = 10;
	uint16_t color = SC_GREEN;
	int x, y, w = GET_WIDTH(), h = GET_HEIGHT(), r2 = radius * 2;

	BEGIN_PROFILE;

	for(x=radius; x<w; x+=r2)
	{
		for(y=radius; y<h; y+=r2)
		{
#if USE_ADAFRUIT_LIBRARY
			gTftScreen.fillCircle(x, y, radius, color);
#else // USE_ADAFRUIT_LIBRARY
			// Todo...
			(void)color;
#endif // USE_ADAFRUIT_LIBRARY
		}
	}

	END_PROFILE;
}

void TestCircles() 
{
	uint8_t radius = 10;
	uint16_t color = SC_RED;
	int           x, y, r2 = radius * 2,
					w = GET_WIDTH()  + radius,
					h = GET_HEIGHT() + radius;


	BEGIN_PROFILE;
	for(x=0; x<w; x+=r2)
	{
		for(y=0; y<h; y+=r2)
		{
#if USE_ADAFRUIT_LIBRARY
			gTftScreen.drawCircle(x, y, radius, color);
#else // USE_ADAFRUIT_LIBRARY
			(void)color;
#endif // USE_ADAFRUIT_LIBRARY
		}
	}

	END_PROFILE;
}

void TestTriangles()
{
	int n, i, cx = GET_WIDTH()  / 2 - 1,
				cy = GET_HEIGHT() / 2 - 1;

	BEGIN_PROFILE;

	n     = min(cx, cy);
	for(i=0; i<n; i+=5) {
#if USE_ADAFRUIT_LIBRARY
			gTftScreen.drawTriangle(
			cx    , cy - i, // peak
			cx - i, cy + i, // bottom left
			cx + i, cy + i, // bottom right
			gTftScreen.color565(0, 0, i));
#else // USE_ADAFRUIT_LIBRARY
			// Todo
#endif // USE_ADAFRUIT_LIBRARY
	}

	END_PROFILE;
}

void TestFilledTriangles() 
{
	int           i, cx = GET_WIDTH()  / 2 - 1,
					cy = GET_HEIGHT() / 2 - 1;

	BEGIN_PROFILE;

	for(i=min(cx,cy); i>10; i-=5) 
	{
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
		gTftScreen.color565(0, i, i));

		gTftScreen.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
		gTftScreen.color565(i, i, 0));
#else // USE_ADAFRUIT_LIBRARY
		// Todo
#endif // USE_ADAFRUIT_LIBRARY
	}

	END_PROFILE;
}

void TestRoundRects()
{
	int w, i, i2,
		cx = GET_WIDTH()  / 2 - 1,
		cy = GET_HEIGHT() / 2 - 1;

	w = min(GET_WIDTH(), GET_HEIGHT());
	
	BEGIN_PROFILE;
	for(i=0; i<w; i+=6)
	{
		i2 = i / 2;
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.drawRoundRect(cx-i2, cy-i2, i, i, i/8, gTftScreen.color565(i, 0, 0));
#else // USE_ADAFRUIT_LIBRARY
		// Todo
		(void)cx;
		(void)cy;
		(void)i2;
#endif // USE_ADAFRUIT_LIBRARY
	}

	END_PROFILE;
}

void TestFilledRoundRects()
{
	int           i, i2,
					cx = GET_WIDTH()  / 2 - 1,
					cy = GET_HEIGHT() / 2 - 1;

	
	BEGIN_PROFILE;
	for(i=min(GET_WIDTH(), GET_HEIGHT()); i>20; i-=6)
	{
		i2 = i / 2;
#if USE_ADAFRUIT_LIBRARY
		gTftScreen.fillRoundRect(cx-i2, cy-i2, i, i, i/8, gTftScreen.color565(0, i, 0));
#else // USE_ADAFRUIT_LIBRARY
		// Todo
		(void)cx;
		(void)cy;
		(void)i2;
#endif // USE_ADAFRUIT_LIBRARY
	}

	END_PROFILE;
}
#endif // SCREEN_TEST_ENABLE
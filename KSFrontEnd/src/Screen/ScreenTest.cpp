// ============================================================================
// Include
// ============================================================================
#include <Screen/ScreenTest.h>
#include <Screen/ScreenDisplay.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#if SCREEN_TEST_ENABLE
// ============================================================================
// Macros
// ============================================================================
#define BEGIN_PROFILE gTftScreen.fillScreen(SC_BLACK);\
					  unsigned long start = micros()

#define END_PROFILE start = micros() - start; \
					  Serial.printf("%s, %f \n", __PRETTY_FUNCTION__, (float)start / 1000.0f)

// ============================================================================
// Global variables
// ============================================================================
extern Adafruit_TFTLCD gTftScreen;


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

	gTftScreen.fillScreen(SC_BLACK);
	gTftScreen.fillScreen(SC_RED);
	gTftScreen.fillScreen(SC_GREEN);
	gTftScreen.fillScreen(SC_BLUE);
	gTftScreen.fillScreen(SC_BLACK);

	END_PROFILE;
}

void TestText() 
{
	BEGIN_PROFILE;

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

	END_PROFILE;
}

void TestLines() 
{
	int w = gTftScreen.width();
	int h = gTftScreen.height();

	int x1, x2, y1, y2;
	
	BEGIN_PROFILE;

	x1 = y1 = 0;
	y2  = h - 1;
	for(x2=0; x2<w; x2+=6)
	{
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
	}

	x2 = w - 1;
	for(y2=0; y2<h; y2+=6)
	{
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
	}

	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;
	for(x2=0; x2<w; x2+=6)
	{
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
	}

	x2 = 0;
	for(y2=0; y2<h; y2+=6)
	{
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
	}

	x1 = 0;
	y1 = h - 1;
	y2 = 0;
	for(x2=0; x2<w; x2+=6)
	{
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
	}
	x2 = w - 1;
	for(y2=0; y2<h; y2+=6)
	{
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
	}

	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;
	for(x2=0; x2<w; x2+=6)
	{
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
	}
	x2 = 0;
	for(y2=0; y2<h; y2+=6)
	{
		gTftScreen.drawLine(x1, y1, x2, y2, SC_CYAN);
	}

	END_PROFILE;
}

void TestFastLines()
{
	int x, y, w = gTftScreen.width(), h = gTftScreen.height();
	BEGIN_PROFILE;

	for(y=0; y<h; y+=5) gTftScreen.drawFastHLine(0, y, w, SC_RED);
	for(x=0; x<w; x+=5) gTftScreen.drawFastVLine(x, 0, h, SC_GREEN);

	END_PROFILE;
}

void TestRects()
{
  int           n, i, i2,
				cx = gTftScreen.width()  / 2,
				cy = gTftScreen.height() / 2;

  BEGIN_PROFILE;
  n = min(gTftScreen.width(), gTftScreen.height());
  for(i=2; i<n; i+=6) {
	i2 = i / 2;
	gTftScreen.drawRect(cx-i2, cy-i2, i, i, SC_WHITE);
  }

  END_PROFILE;
}

void TestFilledRects()
{
	int           n, i, i2,
					cx = gTftScreen.width()  / 2 - 1,
					cy = gTftScreen.height() / 2 - 1;

	BEGIN_PROFILE;

	n = min(gTftScreen.width(), gTftScreen.height());
	for(i=n; i>0; i-=6)
	{
			i2 = i / 2;

			gTftScreen.fillRect(cx-i2, cy-i2, i, i, SC_MAGENTA);

			// Outlines are not included in timing results
			gTftScreen.drawRect(cx-i2, cy-i2, i, i, SC_WHITE);
	}

	END_PROFILE;
}

void TestFilledCircles()
{
	uint8_t radius = 10;
	uint16_t color = SC_GREEN;
	int x, y, w = gTftScreen.width(), h = gTftScreen.height(), r2 = radius * 2;

	BEGIN_PROFILE;

	for(x=radius; x<w; x+=r2)
	{
		for(y=radius; y<h; y+=r2)
		{
			gTftScreen.fillCircle(x, y, radius, color);
		}
	}

	END_PROFILE;
}

void TestCircles() 
{
	uint8_t radius = 10;
	uint16_t color = SC_RED;
	int           x, y, r2 = radius * 2,
					w = gTftScreen.width()  + radius,
					h = gTftScreen.height() + radius;

	BEGIN_PROFILE;
	for(x=0; x<w; x+=r2)
	{
		for(y=0; y<h; y+=r2)
		{
			gTftScreen.drawCircle(x, y, radius, color);
		}
	}

	END_PROFILE;
}

void TestTriangles()
{
	int n, i, cx = gTftScreen.width()  / 2 - 1,
				cy = gTftScreen.height() / 2 - 1;

	BEGIN_PROFILE;

	n     = min(cx, cy);
	for(i=0; i<n; i+=5) {
		gTftScreen.drawTriangle(
		cx    , cy - i, // peak
		cx - i, cy + i, // bottom left
		cx + i, cy + i, // bottom right
		gTftScreen.color565(0, 0, i));
	}

	END_PROFILE;
}

void TestFilledTriangles() 
{
	int           i, cx = gTftScreen.width()  / 2 - 1,
					cy = gTftScreen.height() / 2 - 1;

	BEGIN_PROFILE;

	for(i=min(cx,cy); i>10; i-=5) 
	{
		gTftScreen.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
		gTftScreen.color565(0, i, i));

		gTftScreen.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
		gTftScreen.color565(i, i, 0));
	}

	END_PROFILE;
}

void TestRoundRects()
{
	int w, i, i2,
		cx = gTftScreen.width()  / 2 - 1,
		cy = gTftScreen.height() / 2 - 1;

	w = min(gTftScreen.width(), gTftScreen.height());
	
	BEGIN_PROFILE;
	for(i=0; i<w; i+=6)
	{
		i2 = i / 2;
		gTftScreen.drawRoundRect(cx-i2, cy-i2, i, i, i/8, gTftScreen.color565(i, 0, 0));
	}

	END_PROFILE;
}

void TestFilledRoundRects()
{
	int           i, i2,
					cx = gTftScreen.width()  / 2 - 1,
					cy = gTftScreen.height() / 2 - 1;

	
	BEGIN_PROFILE;
	for(i=min(gTftScreen.width(), gTftScreen.height()); i>20; i-=6)
	{
		i2 = i / 2;
		gTftScreen.fillRoundRect(cx-i2, cy-i2, i, i, i/8, gTftScreen.color565(0, i, 0));
	}

	END_PROFILE;
}
#endif // SCREEN_TEST_ENABLE
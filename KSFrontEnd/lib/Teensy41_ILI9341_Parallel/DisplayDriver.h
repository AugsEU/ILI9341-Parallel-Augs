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

	enum class CircleSection
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};



	/// @brief Create new display driver
	/// @param device ILI device to interface with.
	DisplayDriver(T4_ILI9341 device);



	/// @brief Begin driver after the screen has been created
    /// @return Error code.
    int Begin();



	/// @brief Draw a single pixel
	/// @param x x-coordinate
	/// @param y y-coordinate
	/// @param col packed 16-bit color
	virtual void DrawPixel(uint16_t x, uint16_t y, ILIColor col) = 0;



	/// @brief Directly write color to every pixel in LCD.
	/// @param col Color to write
	/// @return Error code.
	virtual void ForceClear(ILIColor col);



	/// @brief Draw 1px line between sx and sy
	/// @param sx start x
	/// @param sy stary y
	/// @param ex end x
	/// @param ey end y
	/// @param col line color
	void DrawLine(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, ILIColor col);



	/// @brief Draw rectangle outline
	/// @param x top-left x-coordinate
	/// @param y top-left y-coordinate
	/// @param w width
	/// @param h height
	/// @param col packed 16-bit color
	void DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ILIColor col);



	/// @brief Draw rectangle fill
	/// @param x top-left x-coordinate
	/// @param y top-left y-coordinate
	/// @param w width
	/// @param h height
	/// @param col packed 16-bit color
	virtual void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, ILIColor col);



	/// @brief Draw circle outline
	/// @param x center x-coordinate
	/// @param y center y-coordinate
	/// @param r radius
	/// @param color outline color
	void DrawCircle(uint16_t x, uint16_t y, uint16_t r, ILIColor color);



	/// @brief Draw a quarter circle
	/// @tparam Section Circle quadrant to draw
	/// @param x center x-coordinate
	/// @param y center y-coordinate
	/// @param r radius
	/// @param color outline color
	template<CircleSection Section>
	void DrawCircleQuadrant(uint16_t x, uint16_t y, uint16_t r, ILIColor color);



	/// @brief Fill circle
	/// @param x center x-coordinate
	/// @param y center y-coordinate
	/// @param r radius
	/// @param color fill color
	void FillCircle(uint16_t x, uint16_t y, uint16_t r, ILIColor color);



	/// @brief Draw rectangle outline with rounded corners
	/// @param x top-left x-coordinate 
	/// @param y top-right x-coordinate
	/// @param w width
	/// @param h height
	/// @param radius corner radius
	/// @param color outline color
	void DrawRoundedRect(int16_t x, int16_t y, int16_t w, int16_t h,
                     		int16_t radius, uint16_t color);


							
	/// @brief Set text cursor to absolute position.
	/// @param x cursor x-coordinate
	/// @param y cursor y-coordinate
	inline void SetCursor(uint16_t x, uint16_t y) { mCursorX = x; mCursorY = y; }



	/// @brief Set the font's color for subsequent draws
	/// @param col Font's color
	inline void SetTextColor(ILIColor col) { mFontColor = col; }



	/// @brief Set the font's size for subsquent draws
	/// @param mult Multiplier to apply to the font's size.
	inline void SetTextSize(uint16_t mult) { mFontSize = mult; }



	/// @brief Set pixel data for font.
	/// @param data Pixel data where each byte is one 8-pixel column. Chars 
	///             assumed to be 5 pixels wide. Begin data at 0x21 to 0x7F
	inline void SetFont(const uint8_t* data) { mFontData = data; }



	/// @brief Draw a string using previously set parameters.
	/// @param string String to draw.
	void DrawText(const char* string);



	/// @brief Draw a character at a point using previously set font params.
	///        Does not move cursor.
	/// @param x char x-coordinate
	/// @param y char y-coordinate
	/// @param c character to draw. Must be 0x21 to 0x7F
	void DrawChar(uint16_t x, uint16_t y, char c);

protected:

	/// @brief Attempt to connect to device
	/// @return Error code
	int EstablishConnection();

	T4_ILI9341 mDevice;
	Status mStatus;
	uint16_t mCursorX;
	uint16_t mCursorY;
	uint16_t mFontSize;
	ILIColor mFontColor;
	const uint8_t* mFontData;
};

// ============================================================================
// Inline functions
// ============================================================================
template<DisplayDriver::CircleSection Section>
void DisplayDriver::DrawCircleQuadrant(uint16_t x, uint16_t y, uint16_t r, ILIColor color)
{
    // Cardinal points for this quadrant
    if constexpr (Section == CircleSection::TopLeft || Section == CircleSection::TopRight)
        DrawPixel(x, y - r, color);
    if constexpr (Section == CircleSection::BottomLeft || Section == CircleSection::BottomRight)
        DrawPixel(x, y + r, color);
    if constexpr (Section == CircleSection::TopRight || Section == CircleSection::BottomRight)
        DrawPixel(x + r, y, color);
    if constexpr (Section == CircleSection::TopLeft || Section == CircleSection::BottomLeft)
        DrawPixel(x - r, y, color);

    int32_t dx = 0;
    int32_t dy = r;
    int32_t s  = r * r;
    int32_t r2 = s;

    while (dx < dy)
    {
        dx++;
        dy--;
        s += 2 * (dx - dy) + 2;
        if (s < r2)
        {
            s += 2 * dy + 1;
            dy++;
        }

        // +dx, +dy  → bottom right
        if constexpr (Section == CircleSection::BottomRight)
            DrawPixel(x + dx, y + dy, color);

        // -dx, +dy  → bottom left
        if constexpr (Section == CircleSection::BottomLeft)
            DrawPixel(x - dx, y + dy, color);

        // +dx, -dy  → top right
        if constexpr (Section == CircleSection::TopRight)
            DrawPixel(x + dx, y - dy, color);

        // -dx, -dy  → top left
        if constexpr (Section == CircleSection::TopLeft)
            DrawPixel(x - dx, y - dy, color);

        // +dy, +dx  → bottom right
        if constexpr (Section == CircleSection::BottomRight)
            DrawPixel(x + dy, y + dx, color);

        // -dy, +dx  → bottom left
        if constexpr (Section == CircleSection::BottomLeft)
            DrawPixel(x - dy, y + dx, color);

        // +dy, -dx  → top right
        if constexpr (Section == CircleSection::TopRight)
            DrawPixel(x + dy, y - dx, color);

        // -dy, -dx  → top left
        if constexpr (Section == CircleSection::TopLeft)
            DrawPixel(x - dy, y - dx, color);
    }
}


}

#endif // ILI9341_DISPLAY_DRIVER_H
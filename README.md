
# ILI9341 8-bit parallel 

This is a high-performance library designed to interface with the 

Currently supported microcontrollers:
   * Teensy 4.1

Adding more microcontrollers should be as easy as defining macros in ILI9341_Macros.h

#### This library is still in beta! Do not expect the APIs to remain stable. ###

## Usage

**Wiring:** map PORTD (pins 1-8) to the D1, D2, D3 ... D7 pins on the ILI9341. The other pins can be defined in code.

First define the driver and device

```cpp
ILI9341::Device gDevice(LCD_CS, LCD_CD, LCD_WR, LCD_RD);
ILI9341::DeferDriver gDriver(gDevice);
```

then initialise on program launch

```cpp
int err = gDriver.Begin();
if(err != ILI9341_OK)
{
	Serial.printf("Display error. %x", (uint32_t)err);
}
```

Then use it to draw things:

```cpp
gDriver.FillRoundedRect(cx-i2, cy-i2, i, i, i/8, (i*73)^0x32AF);
```

Finally if you are using a deferred driver you need to call

```cpp
gDriver.RenderAllPixels();
```

to actually push any changes to the screen. Or call `RenderPixels` to render only a specified amount of pixels. This can be useful when you are doing time-sensitive code and don't want the draw functions to block for too long.

## Code structure

The `ILI9341::Device` represents the raw ILI9341 hardware. You can use this class to directly call the commands as specified in the data sheet. Each command is a singular method.

The `ILI9341::DisplayDriver` is an abstract class which wraps around the device and provides an easy-to-use interface for drawing. It also implements many graphics functions. The way in which it draws pixels is decided by the child classes.

* `ILI9341::DeferDriver` this driver stores a backbuffer of pixel data locally, and only pushes the pixel data to the device when you call `RenderAllPixels` or `RenderPixels`

Currently there is only one driver. To do: add an "immediate driver", this will save memory but have much worse performance. Also considering adding an "8 color driver" which uses some LUT table tricks to get performance at the cost of only allowing 8 colors.
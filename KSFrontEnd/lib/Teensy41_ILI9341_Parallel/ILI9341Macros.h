// ============================================================================
// Macros used internally for ILI9341 library
// ----------------------------------------------------------------------------
// ============================================================================
// Includes
// ============================================================================
#include "ILI9341_Constants.h"

// ============================================================================
// Macros
// ============================================================================
#if defined(ARDUINO_TEENSY41)
	// Write to data bus assuming the port is available to write.
	#define ILI9341_WRITE_DATA_BUS(d) PORTD = (d)
	#define ILI9341_READ_DATA_BUS() ((uint8_t)PIND)

	#define ILI9341_START_READ() DDRD = 0b00000000
	#define ILI9341_END_READ() DDRD = 0b11111111

	#define ILI9341_WRITE_PIN_FAST(d, v) digitalWriteFast(d, v)
	#define ILI9341_STROBE_PIN(d) do {                             \
										digitalWriteFast(d, LOW);  \
										digitalWriteFast(d, HIGH); \
									} while(false)

#else
	#error Platform not supported. Please implement above macros.
#endif

#define ILI9341_CHECK_ERR_ABORT(err) if(err != ILI9341_OK) return err
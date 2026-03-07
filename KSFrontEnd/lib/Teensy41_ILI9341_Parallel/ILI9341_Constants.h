// ============================================================================
// ILI9341 display driver constants and magic numbers
// ----------------------------------------------------------------------------
// ============================================================================
// Includes
// ============================================================================
#include <stdint.h>

#ifndef ILI9341_CONSTANTS_H
#define ILI9341_CONSTANTS_H

// ============================================================================
// Errors
// ============================================================================

#define ILI9341_OK                      0x00000000 // All is good..
#define ILI9341_INVALID_ID              0x80010000 // Got sent back an ID that wasn't 9341
#define ILI9341_NOT_INITIALISED         0x80010001 // Didn't run initialisation sequence

#define ILI9341_CHECK_ERR_ABORT(err) if(err != ILI9341_OK) return err

// ============================================================================
// Macros
// ============================================================================
#if defined(ARDUINO_TEENSY41)
	// Write to data bus assuming the port is available to write.
	#define ILI9341_WRITE_DATA_BUS(d) PORTD = (d)
	#define ILI9341_READ_DATA_BUS() ((uint8_t)PIND)

	#define ILI9341_START_READ() DDRD = 0b00000000
	#define ILI9341_END_READ() DDRD = 0b11111111

	#define ILI9341_WRITE_PIN_FAST(d, v) digitalWrite(d, v)
	#define ILI9341_STROBE_PIN(d) do {                             \
										digitalWrite(d, LOW);  \
										digitalWrite(d, HIGH); \
									} while(false)

#else
	#error Platform not supported. Please implement above macros.
#endif

// ============================================================================
// Commands
// ============================================================================

// Level 1 commands
#define ILI9341_CMD_NOP 0x00
#define ILI9341_CMD_SOFTWARE_RESET 0x01
#define ILI9341_CMD_READ_DISPLAY_ID_INFO 0x04
#define ILI9341_CMD_READ_DISPLAY_STATUS 0x09
#define ILI9341_CMD_READ_DISPLAY_POWER_MODE 0x0A
#define ILI9341_CMD_READ_DISPLAY_MADCTL 0x0B
#define ILI9341_CMD_READ_DISPLAY_PIXEL_FORMAT 0x0C
#define ILI9341_CMD_READ_DISPLAY_IMAGE_FORMAT 0x0D
#define ILI9341_CMD_READ_DISPLAY_SIGNAL_MODE 0x0E
#define ILI9341_CMD_READ_DISPLAY_SELF_DIAG_RESULT 0x0F
#define ILI9341_CMD_ENTER_SLEEP_MODE 0x10
#define ILI9341_CMD_SLEEP_OUT 0x11
#define ILI9341_CMD_PARTIAL_ON 0x12
#define ILI9341_CMD_NORMAL_ON 0x13
#define ILI9341_CMD_DISPLAY_INVERT_OFF 0x20
#define ILI9341_CMD_DISPLAY_INVERT_ON 0x21
#define ILI9341_CMD_GAMMA_SET 0x26
#define ILI9341_CMD_DISPLAY_OFF 0x28
#define ILI9341_CMD_DISPLAY_ON 0x29
#define ILI9341_CMD_COL_ADDR_SET 0x2A
#define ILI9341_CMD_PAGE_ADDR_SET 0x2B
#define ILI9341_CMD_MEMORY_WRITE 0x2C
#define ILI9341_CMD_COLOR_SET 0x2D
#define ILI9341_CMD_MEMORY_READ 0x2E
#define ILI9341_CMD_PARTIAL_AREA_SET 0x30
#define ILI9341_CMD_VERT_SCROLL_DEFN 0x33
#define ILI9341_CMD_TEARING_EFX_OFF 0x34
#define ILI9341_CMD_TEARING_EFX_ON 0x35
#define ILI9341_CMD_MEM_ACCESS_CTL 0x36
#define ILI9341_CMD_VERT_SCROLL_START_ADDR 0x37
#define ILI9341_CMD_IDLE_OFF 0x38
#define ILI9341_CMD_IDLE_ON 0x39
#define ILI9341_CMD_PIXEL_FMT_SET 0x3A
#define ILI9341_CMD_MEMORY_WRITE_CONT 0x3C
#define ILI9341_CMD_MEMORY_READ_CONT 0x3E
#define ILI9341_CMD_SET_TEAR_SCANLINE 0x44
#define ILI9341_CMD_GET_SCANLINE 0x45
#define ILI9341_CMD_WRITE_DISPLAY_BRIGHT 0x51
#define ILI9341_CMD_READ_DISPLAY_BRIGHT 0x52
#define ILI9341_CMD_WRITE_CTRL_DISPLAY 0x53
#define ILI9341_CMD_READ_CTRL_DISPLAY 0x54
#define ILI9341_CMD_WRITE_CONTENT_ADAPT_BRIGHT 0x55
#define ILI9341_CMD_READ_CONTENT_ADAPT_BRIGHT 0x56
#define ILI9341_CMD_WRITE_CABC_MIN_BRIGHT 0x5E
#define ILI9341_CMD_READ_CABC_MIN_BRIGHT 0x5F
#define ILI9341_CMD_READ_ID1 0xDA
#define ILI9341_CMD_READ_ID2 0xDB
#define ILI9341_CMD_READ_ID3 0xDC

// Level 2 commands
#define ILI9341_CMD2_RBG_INTERFACE_SIG_CTL 0xB0
#define ILI9341_CMD2_FRAME_RATE_CTL_NORM 0xB1
#define ILI9341_CMD2_FRAME_RATE_CTL_IDLE 0xB2
#define ILI9341_CMD2_FRAME_RATE_CTL_PARTIAL 0xB3
#define ILI9341_CMD2_DISPLAY_INVERT_CTL 0xB4
#define ILI9341_CMD2_BLANK_PORCH_CTL 0xB5
#define ILI9341_CMD2_DISPLAY_FUNCT_CTL 0xB6
#define ILI9341_CMD2_ENTRY_MODE_SET 0xB7
#define ILI9341_CMD2_BACKLIGHT_CTL_1 0xB8
#define ILI9341_CMD2_BACKLIGHT_CTL_2 0xB9
#define ILI9341_CMD2_BACKLIGHT_CTL_3 0xBA
#define ILI9341_CMD2_BACKLIGHT_CTL_4 0xBB
#define ILI9341_CMD2_BACKLIGHT_CTL_5 0xBC
#define ILI9341_CMD2_BACKLIGHT_CTL_7 0xBE
#define ILI9341_CMD2_BACKLIGHT_CTL_8 0xBF
#define ILI9341_CMD2_POWER_CTL_1 0xC0
#define ILI9341_CMD2_POWER_CTL_2 0xC1
#define ILI9341_CMD2_VCOM_CTL_1 0xC5
#define ILI9341_CMD2_VCOM_CTL_2 0xC7
#define ILI9341_CMD2_NV_MEM_WRITE 0xD0
#define ILI9341_CMD2_NV_MEM_PROTECT_KEY 0xD1
#define ILI9341_CMD2_NV_MEM_STATUS_READ 0xD2
#define ILI9341_CMD2_READ_ID4 0xD3
#define ILI9341_CMD2_POS_GAMMA_CORRECTION 0xE0
#define ILI9341_CMD2_NEG_GAMMA_CORRECTION 0xE1
#define ILI9341_CMD2_DIGITAL_GAMMA_CTL_1 0xE2
#define ILI9341_CMD2_DIGITAL_GAMMA_CTL_2 0xE3
#define ILI9341_CMD2_INTERFACE_CTL 0xF6


#define ILI9341_EXT_REG_CMD_POWER_CTL_A 0xCB
#define ILI9341_EXT_REG_CMD_POWER_CTL_B 0xCF
#define ILI9341_EXT_REG_CMD_DRIVER_TIME_CTL_A1 0xE8
#define ILI9341_EXT_REG_CMD_DRIVER_TIME_CTL_A2 0xE9
#define ILI9341_EXT_REG_CMD_DRIVER_TIME_CTL_B 0xEA
#define ILI9341_EXT_REG_CMD_POWER_ON_SEQ_CTL 0xED
#define ILI9341_EXT_REG_CMD_ENABLE_3G 0xF2
#define ILI9341_EXT_REG_CMD_PUMP_RATIO_CTL 0xF7

#endif // ILI9341_CONSTANTS_H
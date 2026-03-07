// ============================================================================
// Main source file. Top level entry point and loop.
// ----------------------------------------------------------------------------
// ============================================================================
// Include
// ============================================================================
#include <Arduino.h>
#include "USBHost_t36.h"

#include <Midi/NoteMaps.h>
#include <Usart/TxBackend.h>
#include <Screen/ScreenDisplay.h>


// ============================================================================
// Constants
// ============================================================================
#define SHOW_KEYBOARD_DATA 1





// ============================================================================
// Forward decl
// ============================================================================
void OnPress(int key);
void OnRawPress(uint8_t keycode);
void OnRawRelease(uint8_t keycode);





// ============================================================================
// Globals
// ============================================================================
USBHost gUsbHost;
KeyboardController gUsbKeyboard(gUsbHost);
USBHIDParser gHidParser(gUsbHost);





// ============================================================================
// Private Funcs
// ============================================================================

/// @brief Called on launch
void setup()
{
	// SERIAL
	TxBackendBegin();

	// USB
	gUsbHost.begin();
	gUsbKeyboard.attachRawPress(OnRawPress);
	gUsbKeyboard.attachRawRelease(OnRawRelease);
	gUsbKeyboard.forceHIDProtocol();
	
	// Screen
	SetupScreenDisplay();
}



/// @brief Called in a loop
void loop()
{
	ScreenDisplayUpdate();
	gUsbHost.Task();
}




/// @brief Called when a key is pressed
void OnRawPress(uint8_t keycode) 
{
#if SHOW_KEYBOARD_DATA
	Serial.print("OnRawPress keycode: ");
	Serial.println(keycode, HEX);
#endif

	uint8_t noteNum = KeyCodeToNoteNum(keycode, NoteLayout::Isometric);
	TxBackendNotePress(noteNum);
}



/// @brief Called on each key release
void OnRawRelease(uint8_t keycode)
{
#if SHOW_KEYBOARD_DATA
	Serial.print("OnRawRelease keycode: ");
	Serial.println(keycode, HEX);
#endif

	uint8_t noteNum = KeyCodeToNoteNum(keycode, NoteLayout::Isometric);
	TxBackendNoteRelease(noteNum);
}
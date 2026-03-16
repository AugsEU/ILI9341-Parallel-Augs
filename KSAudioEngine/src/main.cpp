// ============================================================================
// Includes
// ============================================================================
#include <I2S/Teensy4i2s.h>
#include <Wire.h>
#include <SPI.h>
#include <arm_math.h>
#include <WaveGen.h>

#include <Usart/RxFrontEnd.h>
#include "Subtractive/SubWaveGen.h"

// ============================================================================
// Constants
// ============================================================================
#define ENABLE_CPU_PROFILE_REPORT 1


// ============================================================================
// Globals
// ============================================================================

void setup(void)
{
	// Begin serial
	Serial.begin(9600);
	RxFrontEndBegin();

	Subtractive::SynthInit();

	// Start the I2S interrupts
	BeginI2s();
}

void loop(void)
{
	// Should this be interrupt based instead?
	RxFrontEndPoll();

#if ENABLE_CPU_PROFILE_REPORT
	static uint32_t lastReport = 0;
	uint32_t timeNow = millis();
	if(timeNow - lastReport > 1000)
	{
		float avg = Timers::GetPeak(Timers::TIMER_TOTAL);
		float period = Timers::GetAvgPeriod();
		float percent = avg / period * 100;
		Serial.print("CPU Usage: ");
		Serial.print(percent, 4);
		Serial.print("%");
		Serial.print(" -- Processing period: ");
		Serial.print(period/1000, 3);
		Serial.println("ms");
		lastReport = timeNow;
	}
#endif // ENABLE_CPU_PROFILE_REPORT
}

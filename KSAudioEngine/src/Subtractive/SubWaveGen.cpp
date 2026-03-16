// ============================================================================
// Includes
// ============================================================================
#include "SubWaveGen.h"
#include "Oscillator.h"
#include "Midi.h"
#include "Tuning.h"
#include "SubParams.h"
#include "SubVoice.h"
#include "NLFilter.h"
#include "Voice.h"
#include "Parameters.h"
#include "Utilities/QwertyMath.h"
#include "I2S/AudioConfig.h"


namespace Subtractive
{

// ============================================================================
// Constants
// ============================================================================
#define DELAY_BUFFER_LEN 48000
#define DELAY_GLITCH_SIZE 2000
#define LOUDNESS_ALPHA (0.001f)

#define CLICK_PHASE_INC (262.62f * SAMPLE_PERIOD)
#define CLICK_VOLUME (500)




// ============================================================================
// Globals
// ============================================================================
static uint16_t gDelayBuffer[DELAY_BUFFER_LEN];
static uint32_t gDelayWriteHead;
static int32_t gDelayReadOffset = 0;
static int32_t gDelayReadOffsetOffset = 0;

static Oscillator gLFO;
static Oscillator gLFOWobbler;
static float_t gCurrLoudness = 0.0f;

// ============================================================================
// Public functions
// ============================================================================

/// @brief Initialise synth.
void SynthInit(void)
{
	for(int i = 0; i < VOICE_POLYPHONY; i++)
	{
		VoiceInit(&gVoices[i].mSubVoice);
	}

	gDelayWriteHead = 0;
	for(int i = 0; i < DELAY_BUFFER_LEN; i++)
	{
		gDelayBuffer[i] = 0;
	}

	OscInit(&gLFO);
	OscInit(&gLFOWobbler);

	ZeroOutParams();
	InitFilter();

	// Default test preset
	SetFloatParam(ASP_GAIN, 1.4f);
	SetIntParam(ASP_DCO_WAVE_TYPE_1, OSC_MODE_SAW);
	SetFloatParam(ASP_DCO_TUNE_1, 1.0f);
	SetFloatParam(ASP_DCO_VOL_1, 0.75f);
	SetFloatParam(ASP_DCO_WS_1, 0.5f);

	SetIntParam(ASP_DCO_WAVE_TYPE_2, OSC_MODE_SQUARE);
	SetFloatParam(ASP_DCO_TUNE_2, 1.5f);
	SetFloatParam(ASP_DCO_VOL_1, 0.75f);
	SetFloatParam(ASP_DCO_WS_1, 0.2f);
	
	SetFloatParam(ASP_ENV_ATTACK1, 0.002);
	SetFloatParam(ASP_ENV_DECAY1, 0.000005f);
	SetFloatParam(ASP_ENV_SUSTAIN1, 0.76f);
	SetFloatParam(ASP_ENV_RELEASE1, 0.000005f);

	SetFloatParam(ASP_ENV_ATTACK2, 0.00002);
	SetFloatParam(ASP_ENV_DECAY2, 0.00005f);
	SetFloatParam(ASP_ENV_SUSTAIN2, 0.96f);
	SetFloatParam(ASP_ENV_RELEASE2, 0.00005f);

	SetFloatParam(ASP_LFO_RATE, 4.0f * SAMPLE_PERIOD);
	SetFloatParam(ASP_LFO_OSC1_VOLUME, 0.25f);
	SetFloatParam(ASP_LFO_VCF_CUTOFF, -0.2f);

	SetIntParam(ASP_VCF_MODE, FILTER_MODE_LP);
	SetFloatParam(ASP_VCF_RES, 0.2f);
	SetFloatParam(ASP_VCF_CUTOFF, 0.5f);
}



/// @brief Fill sound buffer with number of samples.
/// Note: Buffer has to be twice as long as number of samples.
void FillSoundBuffer(uint16_t* buf, uint16_t samples)
{
	uint16_t pos;
	uint16_t* outp = buf;
	int32_t value, noDelayValue;
	int32_t delayValue;
	uint32_t rndValue = GetNextRand();

	// Delay
	uint32_t delayMode = GetIntParam(ASP_DELAY_MODE);
	
	if(delayMode == DELAY_MODE_GLITCH)
	{
		if(rndValue % 13 == 0)
		{
			if(rndValue % 3 == 0 && gDelayReadOffsetOffset < DELAY_GLITCH_SIZE)
			{
				gDelayReadOffsetOffset += DELAY_GLITCH_SIZE / 3;
			}
			else if (rndValue % 3 == 1 && gDelayReadOffsetOffset > -DELAY_GLITCH_SIZE)
			{
				gDelayReadOffsetOffset -= DELAY_GLITCH_SIZE / 3;
			}
		}
	}
	else
	{
		gDelayReadOffsetOffset = 0;
	}

	int32_t delayReadOffset = GetFloatParam(ASP_DELAY_TIME) * DELAY_BUFFER_LEN + gDelayReadOffsetOffset;
	if(delayMode == DELAY_MODE_OFF)
	{
		gDelayReadOffset = 0;
		delayReadOffset = 0;
	}
	else
	{
		if(delayReadOffset < 0) 
		{
			delayReadOffset = 0;
			gDelayReadOffsetOffset = 0;
		}
		else if(delayReadOffset >= DELAY_BUFFER_LEN)
		{
			delayReadOffset =  DELAY_BUFFER_LEN - 1;
			gDelayReadOffsetOffset = 0;
		}
	}
	int32_t delayFeedbackVol = (uint32_t)(GetFloatParam(ASP_DELAY_FEEDBACK) * 32768.0f);
	uint16_t delayGlide = (uint16_t)(GetFloatParam(ASP_DELAY_SHEAR) * 3.0f) + 2;

	uint32_t delayReadHead;

	// DCO
	uint32_t waveType1 = GetIntParam(ASP_DCO_WAVE_TYPE_1);
	uint32_t waveType2 = GetIntParam(ASP_DCO_WAVE_TYPE_2);
	float_t tune1 = GetFloatParam(ASP_DCO_TUNE_1);
	float_t tune2 = GetFloatParam(ASP_DCO_TUNE_2);
	float_t shape1;// = 1.5f * GetFloatParam(ASP_DCO_WS_1) - 0.25f;
	float_t shape2;// = 1.5f * GetFloatParam(ASP_DCO_WS_2) - 0.25f;
	float_t shape1Lfo = GetFloatParam(ASP_LFO_OSC1_SHAPE);
	float_t shape2Lfo = GetFloatParam(ASP_LFO_OSC2_SHAPE);

	// VCF
	SetFilterType(GetIntParam(ASP_VCF_MODE));
	float_t filterFreqMod, filterFreq = GetFloatParam(ASP_VCF_CUTOFF);
	float_t filterRes = GetFloatParam(ASP_VCF_RES);
	float_t filterFreqLfo = GetFloatParam(ASP_LFO_VCF_CUTOFF);
	float_t filterResLfo = GetFloatParam(ASP_LFO_VCF_RES);
	float_t filterFollow = GetFloatParam(ASP_VCF_FOLLOW);

	// LFO
	float_t lfoValue;
	uint32_t lfoWaveSelect = GetIntParam(ASP_LFO_WAVE_TYPE);
	float_t lfoPhaseInc = GetFloatParam(ASP_LFO_RATE);
	float_t lfoWobblePhaseInc = lfoPhaseInc * 0.061804697157f;
	float_t lfoWobble = GetFloatParam(ASP_LFO_WOBBLE);

	// Drive & Gain
	float_t gain = GetFloatParam(ASP_GAIN);
	float_t drive = GetFloatParam(ASP_DRIVE);

	for(int i = 0; i < VOICE_POLYPHONY; i++)
	{
		VoicePrepSampleBlock(&gVoices[i].mSubVoice);
	}

	for (pos = 0; pos < samples; pos++)
	{		
		/*--- LFO ---*/
		OscPhaseInc(&gLFO, lfoPhaseInc * ComputeLfoMult(OscSine(&gLFOWobbler), lfoWobble));
		OscPhaseInc(&gLFOWobbler, lfoWobblePhaseInc);
		switch (lfoWaveSelect)
		{
		default:
		case OSC_MODE_SINE:
			lfoValue = OscSine(&gLFO);
			break;
		case OSC_MODE_SQUARE:
			lfoValue = OscSquareLF(&gLFO);
			break;
		case OSC_MODE_SAW:
			lfoValue = OscSawTooth(&gLFO, lfoPhaseInc);
			break;
		}

		/*--- Generate waveform ---*/
		float_t	y = 0.0f;
		shape1 = 1.5f * GetFloatParam(ASP_DCO_WS_1) * ComputeLfoMult(lfoValue, shape1Lfo) - 0.25f;
		shape2 = 1.5f * GetFloatParam(ASP_DCO_WS_2) * ComputeLfoMult(lfoValue, shape2Lfo) - 0.25f;

		for(int i = 0; i < VOICE_POLYPHONY; i++)
		{
			y += VoiceGetSample(&gVoices[i].mSubVoice, waveType1, waveType2, tune1, tune2, shape1, shape2, lfoValue);
		}

		/*--- Measure loudness ---*/
		float_t sampLoud = fabsf(y) * 6.0f;
		if(sampLoud > 1.0f) sampLoud = 1.0f;
		gCurrLoudness = LOUDNESS_ALPHA * sampLoud + (1.0f - LOUDNESS_ALPHA) * gCurrLoudness;

		/*--- Filter ---*/
		y *= (1.0f / (VOICE_POLYPHONY + 1.0f)); // Normalise
		filterFreqMod = ComputeLfoMult(lfoValue, filterFreqLfo);
		filterFreqMod *=  ComputeLoudnessMult(gCurrLoudness, filterFollow);
		SetFilterFreq(filterFreq * filterFreqMod);
		SetFilterRes(filterRes * ComputeLfoMult(lfoValue, filterResLfo));
		y = CalcFilterSample(y);

		/*--- Drive & Gain ---*/
		y = drive * DrivenSample(y) + (1.0f - drive) * y;
		y *= gain;
		
		/*--- Delay ---*/
		value = (int32_t)((32767.0f) * y);

		// Delay read
		if ((pos % delayGlide) == 0)
		{
			if(gDelayReadOffset < delayReadOffset)
			{
				gDelayReadOffset += 1;
			}
			else if (gDelayReadOffset > delayReadOffset)
			{
				gDelayReadOffset -= 1;
			}
		}

		noDelayValue = value;
		if(gDelayReadOffset > 0)
		{
			delayReadHead = (gDelayWriteHead + DELAY_BUFFER_LEN - gDelayReadOffset) % DELAY_BUFFER_LEN;
			delayValue = (int16_t)gDelayBuffer[delayReadHead];
			value += delayValue;
		}

		/*--- Write to buffer ---*/
		if (value < -32768)
		{
			value = -32768;
		}
		else if (value > 32767)
		{
			value = 32767;
		}

		*outp++ = (uint16_t)(int16_t)value;
		*outp++ = (uint16_t)(int16_t)value;

		/*--- Delay write ---*/
		if (delayMode == DELAY_MODE_SLAPBACK)
		{
			value = noDelayValue; // Just write delay without feedback
		}

		value *= delayFeedbackVol;
		value /= 32768;

		gDelayBuffer[gDelayWriteHead] = (uint16_t)(int16_t)value;
		gDelayWriteHead = (gDelayWriteHead + 1) % DELAY_BUFFER_LEN;
	}
}

}
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DelayUnit 
{

public:

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		samplerate = sampleRate;
		circularBufferSize = samplerate * 2.0;
		circularBufferL = new float[circularBufferSize];
		circularBufferR = new float[circularBufferSize];
		
		for (int sample = 0; sample < circularBufferSize; sample++)
		{
			circularBufferL[sample] = 0.0;
			circularBufferR[sample] = 0.0;
		}
		circularBufferWritePosition = 0;
	}

	void process(AudioSampleBuffer &buffer)
	{
		float* outputL = buffer.getWritePointer(0);
		float* outputR = buffer.getWritePointer(1);

		const float* inputL = buffer.getReadPointer(0);
		const float* inputR = buffer.getReadPointer(1);
		
		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			circularBufferWritePosition++;

			if (circularBufferWritePosition > circularBufferSize - 1)
			{
				circularBufferWritePosition = 0;
			}

			circularBufferL[circularBufferWritePosition] = inputL[sample];
			circularBufferR[circularBufferWritePosition] = inputR[sample];

			int circularBufferReadPosition = circularBufferWritePosition - delayTimeInSamples;

			if (circularBufferReadPosition < 0)
			{
				circularBufferReadPosition += circularBufferSize;
			}

			outputL[sample] = circularBufferL[circularBufferReadPosition];
			outputR[sample] = circularBufferR[circularBufferReadPosition];
		}
	}

	void setDelayTimeInSamples(int time)
	{
		delayTimeInSamples = time;
	}

	void setDelayTimeInMS(int time)
	{
		delayTimeInSamples = time * (samplerate / 1000);
		DBG("Delay is " + (String)delayTimeInSamples);
	}

private:

	float samplerate = 48000;
	float *circularBufferL;
	float *circularBufferR;

	int circularBufferSize = 0;
	int circularBufferWritePosition = 0;

	int delayTimeInSamples = 100.0;
};
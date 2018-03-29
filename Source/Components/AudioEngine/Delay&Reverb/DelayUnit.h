#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DelayUnit 
{

public:

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		samplerate = sampleRate;
		circularBufferSize = sampleRate * 2.0;
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

		DBG("DELAY TIME IS " + (String)delayTimeInSamples);
		
		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			circularBufferWritePosition++;

			if (circularBufferWritePosition >= circularBufferSize)
			{
				circularBufferWritePosition = 0;
			}

			circularBufferL[circularBufferWritePosition] = inputL[sample];
			circularBufferR[circularBufferWritePosition] = inputR[sample];

			int circularBufferReadPosition = circularBufferWritePosition - delayTimeInSamples; //force 1,2,3,4

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
	ScopedPointer<float> circularBufferL;
	ScopedPointer<float> circularBufferR;

	int circularBufferSize = 0;
	int circularBufferWritePosition = 0;

	int delayTimeInSamples = 100.0;
};
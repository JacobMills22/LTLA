
#include "DelayUnit.h"

	void DelayUnit::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		// Initalise samplerate, circular buffer size and 
		// create two arrays for each channel initialised to 0.0
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

	void DelayUnit::process(AudioSampleBuffer &buffer)
	{
		float* outputL = buffer.getWritePointer(0);
		float* outputR = buffer.getWritePointer(1);

		const float* inputL = buffer.getReadPointer(0);
		const float* inputR = buffer.getReadPointer(1);

		// For each sample
		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			// Increment write position
			circularBufferWritePosition++;

			// If write position is greater than the buffer size, wrap around back to 0.
			if (circularBufferWritePosition >= circularBufferSize)
			{
				circularBufferWritePosition = 0;
			}

			// Set the current write position of the buffer to the input.
			circularBufferL[circularBufferWritePosition] = inputL[sample];
			circularBufferR[circularBufferWritePosition] = inputR[sample];

			// Set the read posiiton to the current posiiton - the delay amount (the delayed output).
			int circularBufferReadPosition = circularBufferWritePosition - delayTimeInSamples; 

			// If when delaying the read position it reaches 0 wrap around the buffer by adding the buffer size
			// to the negative value.
			if (circularBufferReadPosition < 0)
			{
				circularBufferReadPosition += circularBufferSize;
			}

			// Set the output to the delayed read posiiton.
			outputL[sample] = circularBufferL[circularBufferReadPosition];
			outputR[sample] = circularBufferR[circularBufferReadPosition];
		}
	}

	void DelayUnit::setDelayTimeInSamples(int time)
	{
		delayTimeInSamples = time;
	}

	void DelayUnit::setDelayTimeInMS(int time)
	{
		delayTimeInSamples = time * (samplerate / 1000);
	}

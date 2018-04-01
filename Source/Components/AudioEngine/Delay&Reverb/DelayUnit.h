#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/** This class performs a very simple delay 
	on an audio buffer, delay time can be set
	in samples ot ms. */

class DelayUnit 
{

public:

	/** prepares the audio for playback*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);

	/** Process the delay unit. */
	void process(AudioSampleBuffer &buffer);

	/** Sets the delay time in samples. */
	void setDelayTimeInSamples(int time);

	/** Sets the delay time in ms. */
	void setDelayTimeInMS(int time);

private:

	float samplerate = 48000;
	ScopedPointer<float> circularBufferL;
	ScopedPointer<float> circularBufferR;

	int circularBufferSize = 0;
	int circularBufferWritePosition = 0;

	int delayTimeInSamples = 100.0;
};
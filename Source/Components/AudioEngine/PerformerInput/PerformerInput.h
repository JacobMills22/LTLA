#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Taken from JUCE tutorial https://docs.juce.com/master/tutorial_processing_audio_input.html

/** This class is used to get the audio microphone input
	from a performer*/

class PerformerInput : public AudioSource
{
public:

	/** Initialise the class, decide which input channel to use, and the maximum output channels in use*/
	void initialise(int actualInputChannel, int maximumOutputChannels);

	/** Set the device manager to use. */
	void setDeviceManagerToUse(AudioDeviceManager *deviceManager);

	/** Makes a copy of the provided buffer to use as input.*/
	void setInputBuffer(AudioSampleBuffer buffer);

	/** Standard JUCE Function: Prepares all DSP modules for audio playback */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	/** Standard JUCE Function: Releases resources for all DSP modules*/
	void releaseResources() override;

	/**	Standard JUCE Function: Processes each DSP module*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

	/** Sets the output level (0.0 to 1.0) */
	void setLevel(float value);

	void process(AudioSampleBuffer buffer);


private:

	AudioIODevice* device;
	AudioSampleBuffer inputBuffer;

	int actualInputChannel = 0;
	int maxOutputChannels = 0;
	float level = 0.2;
};
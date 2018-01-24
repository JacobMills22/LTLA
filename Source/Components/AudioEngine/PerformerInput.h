#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class PerformerInput : public AudioSource
{
public:

	void initialise(int ActualInputChannel, int MaxOutputChannels);

	/** Standard JUCE Function: Prepares all DSP modules for audio playback */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	/** Standard JUCE Function: Releases resources for all DSP modules*/
	void releaseResources() override;

	/**	Standard JUCE Function: Processes each DSP module*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

	void setLevel(float value);

	void process(AudioSampleBuffer buffer);

	void setDeviceManagerToUse(AudioDeviceManager *deviceManager);

	void setInputBuffer(AudioSampleBuffer buffer);

private:

	AudioIODevice* device;
	AudioSampleBuffer inputBuffer;

	int actualInputChannel = 0;
	int maxOutputChannels = 0;
	float level = 0.2;
};
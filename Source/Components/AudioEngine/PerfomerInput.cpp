
#include "PerformerInput.h"

void PerformerInput::initialise(int ActualInputChannel, int MaxOutputChannels)
{
	actualInputChannel = ActualInputChannel;
	maxOutputChannels = MaxOutputChannels;
}

/** Standard JUCE Function: Prepares all DSP modules for audio playback */
void PerformerInput::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	inputBuffer.setSize(2, samplesPerBlockExpected, false, false, false);
}

/** Standard JUCE Function: Releases resources for all DSP modules*/
void PerformerInput::releaseResources()
{

}

/**	Standard JUCE Function: Processes each DSP module*/
void PerformerInput::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	//DBG("PERFORMER PROCESS : actualInputChannel = " + (String)actualInputChannel + " / maxOutputChannels = " + (String)maxOutputChannels);

	const BigInteger activeInputChannels = device->getActiveInputChannels();
	const BigInteger activeOutputChannels = device->getActiveOutputChannels();
	const int maxInputChannels = activeInputChannels.getHighestBit() + 1;
	const int maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

	for (int channel = 0; channel < maxOutputChannels; ++channel)
	{
		if ((!activeOutputChannels[channel]) || maxInputChannels == 0)
		{
			bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
		}
		else
		{
			const int actualInputChannel = channel % maxInputChannels; // [1]

			if (!activeInputChannels[channel]) // [2]
			{
				bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
			}
			else // [3]
			{
				const float* inBuffer = inputBuffer.getReadPointer(actualInputChannel, bufferToFill.startSample);

				float* outBuffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

				for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
				{
					outBuffer[sample] = inBuffer[sample] * level;
				}
			}
		}
	}
}

void PerformerInput::setLevel(float value)
{
	level = value;
}

void PerformerInput::process(AudioSampleBuffer buffer)
{
	for (int channel = 0; channel < maxOutputChannels; channel++)
	{
		const float* inBuffer = buffer.getReadPointer(actualInputChannel);
		float* outBuffer = buffer.getWritePointer(channel);

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			outBuffer[sample] = inBuffer[sample] * 0.2;
		}
	}
}

void PerformerInput::setDeviceManagerToUse(AudioDeviceManager *deviceManager)
{
	device = deviceManager->getCurrentAudioDevice();
}

void PerformerInput::setInputBuffer(AudioSampleBuffer buffer)
{
	inputBuffer.makeCopyOf(buffer, true);
}


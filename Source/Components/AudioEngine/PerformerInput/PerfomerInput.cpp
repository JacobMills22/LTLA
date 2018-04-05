
#include "PerformerInput.h"

void PerformerInput::initialise(int inputChannel, int maximumOutputChannels)
{
	actualInputChannel = inputChannel;
	maxOutputChannels = maximumOutputChannels;
}

// Standard JUCE Function: Prepares all DSP modules for audio playback 
void PerformerInput::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	inputBuffer.setSize(2, samplesPerBlockExpected, false, false, false);
}

// Standard JUCE Function: Releases resources for all DSP modules
void PerformerInput::releaseResources()
{
}

//	Standard JUCE Function: Processes each DSP module
void PerformerInput::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	// Taken from JUCE tutorial https://docs.juce.com/master/tutorial_processing_audio_input.html

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
			const int actualInputChannel = channel % maxInputChannels;

			if (!activeInputChannels[channel])
			{
				bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
			}
			else
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

void PerformerInput::process(AudioSampleBuffer &buffer)
{
	for (int channel = 0; channel < maxOutputChannels; channel++)
	{
		const float* inBuffer = buffer.getReadPointer(actualInputChannel);
		float* outBufferL = buffer.getWritePointer(0);
		float* outBufferR = buffer.getWritePointer(1);


		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			outBufferL[sample] = inBuffer[sample] * 0.5;
			outBufferR[sample] = outBufferL[sample];
		}
	}
}

void PerformerInput::setLevel(float value)
{
	level = value;
}

void PerformerInput::setDeviceManagerToUse(AudioDeviceManager *deviceManager)
{
	device = deviceManager->getCurrentAudioDevice();
}

void PerformerInput::setInputBuffer(AudioSampleBuffer buffer)
{
	inputBuffer.makeCopyOf(buffer, true);
}


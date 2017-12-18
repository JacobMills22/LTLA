#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPanelObject.h"

class AutoPanner : public AudioSource,
				   public AudioPanelObject,
				   public Slider::Listener,
	public Timer
{

public:

	AutoPanner()
	{
		addAndMakeVisible(panningSlider);
		panningSlider.setRange(0.0, 1.0, 0.01);
		panningSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		panningSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
		panningSlider.addListener(this);

		startTimer(50);
	}

	/** Initialises the resampling audio sources parameters.*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)override
	{

	}

	/** Processes the audio data (Resampling Audio Source)*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		float* OutputL = bufferToFill.buffer->getWritePointer(0);
		float* OutputR = bufferToFill.buffer->getWritePointer(1);


		for (int sample = 0; sample < bufferToFill.numSamples; sample++)
		{
			OutputL[sample] *= pow(sin((1 - panAmount) * (float_Pi * 0.5)), 2);
			OutputR[sample] *= pow(sin(panAmount * (float_Pi * 0.5)), 2);
		}
	}

	/** Releases the resampling audio sources resources. */
	void releaseResources() override
	{

	}

	void resized() override
	{
		float centreX = getWidth() * 0.5;
		float centreY = getHeight() * 0.5;
		float sliderWidth = getWidth() * 0.3;

		panningSlider.setBounds(centreX - (sliderWidth * 0.5), centreY - (sliderWidth * 0.5), sliderWidth, sliderWidth);
	}

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &panningSlider)
		{
			setPanAmount(slider->getValue());
		}
	}

	void timerCallback() override
	{
		panningSlider.setValue(panAmount, dontSendNotification);
	}


	void setPanAmount(float value)
	{
		panAmount = value;
	}

private:

	Slider panningSlider;
	Label panningLabel;

	float panAmount = 0.0;

};
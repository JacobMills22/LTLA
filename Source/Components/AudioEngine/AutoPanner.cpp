
#include "AutoPanner.h"

	AutoPanner::AutoPanner()
	{
		addAndMakeVisible(panningSlider);
		panningSlider.setRange(0.0, 1.0, 0.01);
		panningSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		panningSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
		panningSlider.addListener(this);

		addAndMakeVisible(enablePanButton);
		enablePanButton.addListener(this);
		enablePanButton.setToggleState(false, dontSendNotification);
		enablePanButton.setButtonText("Auto Panning Disabled");

		addAndMakeVisible(resetPanButton);
		resetPanButton.addListener(this);
		resetPanButton.setButtonText("Reset");

		startTimer(50);
	}

	/** Initialises the resampling audio sources parameters.*/
	void AutoPanner::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		// Not nessesarily needed since this class just processes existing data.
	}

	/** Processes the audio data (Resampling Audio Source)*/
	void AutoPanner::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
	{
		float* OutputL = bufferToFill.buffer->getWritePointer(0);
		float* OutputR = bufferToFill.buffer->getWritePointer(1);


		for (int sample = 0; sample < bufferToFill.numSamples; sample++)
		{
			// 6dB Pan Law
			OutputL[sample] *= pow(sin((1 - panAmount) * (float_Pi * 0.5)), 2) * 0.5;
			OutputR[sample] *= pow(sin(panAmount * (float_Pi * 0.5)), 2) * 0.5;
		}
	}

	/** Releases the resampling audio sources resources. */
	void AutoPanner::releaseResources()
	{
		// Not nessesarily needed since this class just processes existing data.
	}

	void AutoPanner::resized()
	{
		float centreX = getWidth() * 0.5;
		float centreY = getHeight() * 0.5;
		float sliderWidth = getWidth() * 0.3;

		panningSlider.setBounds(centreX - (sliderWidth * 0.5), centreY - (sliderWidth * 0.8), sliderWidth, sliderWidth);
		enablePanButton.setBounds(centreX - (sliderWidth * 0.7), centreY - (sliderWidth * 1.0), sliderWidth * 1.5, sliderWidth * 0.25);
		resetPanButton.setBounds(centreX - (sliderWidth * 0.25), centreY * 1.1, sliderWidth * 0.5, sliderWidth * 0.15);
	}

	void AutoPanner::sliderValueChanged(Slider* slider) 
	{
		if (slider == &panningSlider)
		{
			panAmount = slider->getValue();
		}
	}

	void AutoPanner::buttonClicked(Button* button)
	{
		if (button == &enablePanButton)
		{

			if (enablePanButton.getToggleState() == true)
			{
				enablePanButton.setButtonText("Auto Panning Enabled");
				enablePanState = true;
			}
			else
			{
				enablePanButton.setButtonText("Auto Panning Disabled");
				enablePanState = false;
			}
		}
		else if (button == &resetPanButton)
		{
			panAmount = 0.5;
			panningSlider.setValue(0.5, dontSendNotification);
		}
	}


	void AutoPanner::timerCallback()
	{
		panningSlider.setValue(panAmount, dontSendNotification);
	}


	void AutoPanner::setPanAmount(float value)
	{
		if (enablePanState == true)
		{
			panAmount = value;
		}
	}

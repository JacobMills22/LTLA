
#include "AutoPanner.h"

	AutoPanner::AutoPanner() : autoPannerValueTree("autoPannerValueTree")
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

		autoPannerValueTree.setProperty("EnableAutoPan", false, nullptr);

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
		// Using process function instead.
	}

	void AutoPanner::process(AudioSampleBuffer &buffer)
	{
		float* OutputL = buffer.getWritePointer(0);
		float* OutputR = buffer.getWritePointer(1);

		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			// 6dB Pan Law
			//	OutputL[sample] *= pow(sin((1 - panAmount) * (float_Pi * 0.5)), 2) * 1.98;
			//	OutputR[sample] *= pow(sin(panAmount * (float_Pi * 0.5)), 2) * 1.98;

			// 3dB Pan Law
			OutputL[sample] *= sin((1 - panAmount) * (float_Pi * 0.5));
			OutputR[sample] *= sin((panAmount) * (float_Pi * 0.5));

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

		panningSlider.setBounds(centreX - (sliderWidth * 0.5), centreY - (sliderWidth * 0.7), sliderWidth, sliderWidth);
		enablePanButton.setBounds(centreX - sliderWidth * 0.4, centreY - (sliderWidth * 0.85), sliderWidth * 1.5, sliderWidth * 0.25);
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
				//enablePanState = true;
				autoPannerValueTree.setProperty("EnableAutoPan", true, nullptr);
			}
			else
			{
				enablePanButton.setButtonText("Auto Panning Disabled");
				//enablePanState = false;
				autoPannerValueTree.setProperty("EnableAutoPan", false, nullptr);
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
		if (autoPannerValueTree.getPropertyAsValue("EnableAutoPan", nullptr) == true)
		{
			panAmount = value;
		}
	}

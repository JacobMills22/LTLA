
#include "AutoPanner.h"

	AutoPanner::AutoPanner() : autoPannerValueTree("autoPannerValueTree")
	{
		autoPannerValueTree.setProperty("EnableAutoPan", false, nullptr);

		addAndMakeVisible(panningSlider);
		panningSlider.setRange(0.0, 1.0, 0.01);
		panningSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		panningSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
		panningSlider.addListener(this);

		addAndMakeVisible(enablePanButton);
		enablePanButton.addListener(this);
		enablePanButton.setToggleState(false, dontSendNotification);
		enablePanButton.setButtonText("Enable Auto Panning");
		enablePanButton.getToggleStateValue().referTo(autoPannerValueTree.getPropertyAsValue("EnableAutoPan", nullptr));

		addAndMakeVisible(resetPanButton);
		resetPanButton.addListener(this);
		resetPanButton.setButtonText("Reset");

		startTimer(50);
	}

	void AutoPanner::setPanAmount(float value)
	{
		if (enablePanState == true)
		{
			panAmount = value;
		}
	}

	void AutoPanner::process(AudioSampleBuffer &buffer)
	{
		float* OutputL = buffer.getWritePointer(0);
		float* OutputR = buffer.getWritePointer(1);

		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			// Researched and experimented with different pan laws
			// Future developers can easily switch the methods out if desired.

			// -3dB Pan Law
			//OutputL[sample] *= sin((1 - panAmount) * halfPi);
			//OutputR[sample] *= sin((panAmount) * halfPi);

			// -4.5 dB center law :
			OutputL[sample] *= pow(sin((1 - panAmount) * halfPi), 1.5);
			OutputR[sample] *= pow(sin(panAmount * halfPi), 1.5);

			// -6dB Pan Law
			//	OutputL[sample] *= pow(sin((1 - panAmount) * halfPi), 2) * 1.98;
			//	OutputR[sample] *= pow(sin(panAmount * halfPi, 2) * 1.98;
		}
	}

	void AutoPanner::resized()
	{
		float centreX = getWidth() * 0.5;
		float centreY = getHeight() * 0.4;
		float sliderWidth = getWidth() * 0.2;

		panningSlider.setBounds(centreX - (sliderWidth * 0.5), centreY - (sliderWidth * 0.7), sliderWidth, sliderWidth);
		enablePanButton.setBounds(centreX - sliderWidth * 0.4, centreY - (sliderWidth * 0.85), sliderWidth * 1.5, sliderWidth * 0.2);
		resetPanButton.setBounds(centreX - (sliderWidth * 0.25), centreY * 1.25, sliderWidth * 0.5, sliderWidth * 0.15);
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
			enablePanButton.getToggleState() == true ? enablePanState = true : enablePanState = false;
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
	
	ValueTree AutoPanner::getValueTree()
	{
		return autoPannerValueTree;
	}

	void AutoPanner::snapshotFired()
	{
		// Update the enable button and state.
		enablePanState = autoPannerValueTree.getPropertyAsValue("EnableAutoPan", nullptr).getValue();
		enablePanButton.setToggleState(enablePanState, dontSendNotification);

		// A a snapshot has been fired which doesn't have aurto panning enabled
		// Reset the pan amount to the centre.
		if (enablePanState == false)
		{
			setPanAmount(0.5);
			panningSlider.setValue(0.5);
		}
	}

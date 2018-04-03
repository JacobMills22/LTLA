
#include "AutoFilter.h"

	AutoFilter::AutoFilter() : autoFilterValueTree("autoFilterValueTree")
	{
		// Parameter Initialisation
		for (int filterNum = 0; filterNum < numOfFilters; filterNum++)
		{
			addAndMakeVisible(cutoffSlider[filterNum]);
			cutoffSlider[filterNum].setRange(0.0, 24000.0, 10.0);
			cutoffSlider[filterNum].setSliderStyle(Slider::RotaryVerticalDrag);
			cutoffSlider[filterNum].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[filterNum].getWidth() * 0.5, 30);
			cutoffSlider[filterNum].addListener(this);

			addAndMakeVisible(qFactorSlider[filterNum]);
			qFactorSlider[filterNum].addListener(this);
			qFactorSlider[filterNum].setRange(0.8, 10.0, 0.1);
			qFactorSlider[filterNum].setSliderStyle(Slider::LinearVertical);
			qFactorSlider[filterNum].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[filterNum].getWidth() * 0.5, 30);

			addAndMakeVisible(enableButton[filterNum]);
			enableButton[filterNum].addListener(this);
			enableButton[filterNum].setToggleState(false, dontSendNotification);
			
			enableState[filterNum] = false;
		}

		cutoffSlider[lowPassID].setValue(1.0);
		cutoffSlider[highPassID].setValue(20.0);
		cutoffSlider[lowPassID].setSkewFactor(0.23);
		cutoffSlider[highPassID].setSkewFactor(0.45);

		enableButton[lowPassID].setButtonText("LowPass Enable");
		enableButton[highPassID].setButtonText("HighPass Enable");

		autoFilterValueTree.setProperty("LowPassEnabled", false, nullptr);
		autoFilterValueTree.setProperty("HighPassEnabled", false, nullptr);
		autoFilterValueTree.setProperty("LowPassCutoff", 20000.0, nullptr);
		autoFilterValueTree.setProperty("HighPassCutoff", 20.0, nullptr);
		autoFilterValueTree.setProperty("LowPassQ", 1.0, nullptr);
		autoFilterValueTree.setProperty("HighPassQ", 1.0, nullptr);

		enableButton[lowPassID].getToggleStateValue().referTo(autoFilterValueTree.getPropertyAsValue("LowPassEnabled", nullptr));
		enableButton[highPassID].getToggleStateValue().referTo(autoFilterValueTree.getPropertyAsValue("HighPassEnabled", nullptr));
		cutoffSlider[lowPassID].getValueObject().referTo(autoFilterValueTree.getPropertyAsValue("LowPassCutoff", nullptr));
		cutoffSlider[highPassID].getValueObject().referTo(autoFilterValueTree.getPropertyAsValue("HighPassCutoff", nullptr));
		qFactorSlider[lowPassID].getValueObject().referTo(autoFilterValueTree.getPropertyAsValue("LowPassQ", nullptr));
		qFactorSlider[highPassID].getValueObject().referTo(autoFilterValueTree.getPropertyAsValue("HighPassQ", nullptr));

		lowpassFilter.setParameters(20000.0, 1.0);
		highpassFilter.setParameters(20.0, 1.0);
		
	}

	void AutoFilter::process(AudioSampleBuffer &buffer)
	{
		// If enabled process the lowpass filter.
		if (enableState[lowPassID] == true)
			lowpassFilter.process(buffer);

		// If enabled process the highpass filter.
		if (enableState[highPassID] == true)
			highpassFilter.process(buffer);
	}

	void AutoFilter::resized() 
	{
		// Set bounds of components
		float sliderWidth = getWidth() * 0.3;
		float sliderHeight = getHeight() * 0.55;

		float gap = getWidth() * 0.01;

		cutoffSlider[lowPassID].setBounds(getWidth() * 0.1, (getHeight() * 0.65) - sliderHeight, sliderWidth, sliderHeight);
		cutoffSlider[highPassID].setBounds(getWidth() * 0.1 + gap + sliderWidth, (getHeight() * 0.65) - sliderHeight, sliderWidth, sliderHeight);

		qFactorSlider[lowPassID].setBounds(0, cutoffSlider[lowPassID].getY(), getWidth() * 0.07, cutoffSlider[lowPassID].getHeight());
		qFactorSlider[highPassID].setBounds(cutoffSlider[highPassID].getRight() + getWidth() * 0.01, cutoffSlider[highPassID].getY(), getWidth() * 0.07, cutoffSlider[highPassID].getHeight());

		cutoffSlider[lowPassID].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[lowPassID].getWidth() * 0.25, 20);
		cutoffSlider[highPassID].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[highPassID].getWidth() * 0.25, 20);

		float centerXLowPass = getWidth() * 0.2;
		float centerXHighPass = getWidth() * 0.5;

		enableButton[lowPassID].setBounds(centerXLowPass, cutoffSlider[lowPassID].getY() - 20, 120, 30);
		enableButton[highPassID].setBounds(centerXHighPass, cutoffSlider[highPassID].getY() - 20, 120, 30);

	}

	void AutoFilter::sliderValueChanged(Slider* slider) 
	{
		// Update the parameters of the low or high pass filter depending on the slider.

		if (slider == &cutoffSlider[lowPassID] || slider == &qFactorSlider[lowPassID])
		{
			double lowpassCutoff = cutoffSlider[lowPassID].getValue();
			double lowpassQ = qFactorSlider[lowPassID].getValue();
			lowpassFilter.setParameters(lowpassCutoff, lowpassQ);
		}
		else if (slider == &cutoffSlider[highPassID] || slider == &qFactorSlider[highPassID])
		{
			double highpassCutoff = cutoffSlider[highPassID].getValue();
			double highpassQ = qFactorSlider[highPassID].getValue();
			highpassFilter.setParameters(highpassCutoff, highpassQ);
		}
	}

	void AutoFilter::buttonClicked(Button* button) 
	{
		// Update the enable state.
		if (button == &enableButton[lowPassID])
			enableState[lowPassID] = enableButton[lowPassID].getToggleState();
		else if (button == &enableButton[highPassID])
			enableState[highPassID] = enableButton[highPassID].getToggleState();
	}

	void AutoFilter::setSampleRate(double value)
	{
		// Set the sample rate of the filters.
		sampleRate = value;
		lowpassFilter.setSampleRate(sampleRate);
		highpassFilter.setSampleRate(sampleRate);
	}

	ValueTree AutoFilter::getValueTree()
	{
		return autoFilterValueTree;
	}

	void AutoFilter::snapshotFired()
	{
		// update the enable state to the new states in the newly fired snapshot.
		enableState[lowPassID] = enableButton[lowPassID].getToggleState();
		enableState[highPassID] = enableButton[highPassID].getToggleState();
		lowpassFilter.setParameters(cutoffSlider[lowPassID].getValue(), qFactorSlider[lowPassID].getValue());
		highpassFilter.setParameters(cutoffSlider[highPassID].getValue(), qFactorSlider[highPassID].getValue());
	}

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/AudioEngine/AudioPanelObject.h"
#include <math.h>
#include "FilterProcesses.h"

class AutoFilter : public AudioPanelObject,
	public Slider::Listener,
	public Button::Listener
{
public:

	AutoFilter() : autoFilterValueTree("autoFilterValueTree")
	{
		for (int filterNum = 0; filterNum < numOfFilters; filterNum++)
		{
			 addAndMakeVisible(cutoffSlider[filterNum]);
			 addAndMakeVisible(enableButton[filterNum]);

			 cutoffSlider[filterNum].setRange(0.0, 24000.0, 10.0);
			 cutoffSlider[filterNum].setSliderStyle(Slider::RotaryVerticalDrag);
			 cutoffSlider[filterNum].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[filterNum].getWidth() * 0.5, 30);
			 cutoffSlider[filterNum].addListener(this);

			 addAndMakeVisible(qFactorSlider[filterNum]);
			 qFactorSlider[filterNum].addListener(this);
			 qFactorSlider[filterNum].setRange(0.8, 10.0, 0.1);
			 qFactorSlider[filterNum].setSliderStyle(Slider::LinearVertical);
			 qFactorSlider[filterNum].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[filterNum].getWidth() * 0.5, 30);

		}
		
		cutoffSlider[highPassID].setRange(0.0, 10000, 10.0);

		cutoffSlider[lowPassID].setValue(1.0);
		cutoffSlider[highPassID].setValue(20.0);

		cutoffSlider[lowPassID].setSkewFactor(0.23);
		cutoffSlider[highPassID].setSkewFactor(0.45);

		enableButton[lowPassID].addListener(this);
		enableButton[highPassID].addListener(this);

		enableButton[lowPassID].setButtonText("LowPass Enable");
		enableButton[highPassID].setButtonText("HighPass Enable");
		enableButton[lowPassID].setToggleState(false, dontSendNotification);
		enableButton[highPassID].setToggleState(false, dontSendNotification);

		enableState[lowPassID] = false;
		enableState[highPassID] = false;

		autoFilterValueTree.setProperty("LowPassEnabled", false, nullptr);
		autoFilterValueTree.setProperty("HighPassEnabled", false, nullptr);

		autoFilterValueTree.setProperty("LowPassCutoff", 20000.0, nullptr);
		autoFilterValueTree.setProperty("HighPassCutoff", 20.0, nullptr);
		
		enableButton[lowPassID].getToggleStateValue().referTo(autoFilterValueTree.getPropertyAsValue("LowPassEnabled", nullptr));
		enableButton[highPassID].getToggleStateValue().referTo(autoFilterValueTree.getPropertyAsValue("HighPassEnabled", nullptr));

		cutoffSlider[lowPassID].getValueObject().referTo(autoFilterValueTree.getPropertyAsValue("LowPassCutoff", nullptr));
		cutoffSlider[highPassID].getValueObject().referTo(autoFilterValueTree.getPropertyAsValue("HighPassCutoff", nullptr));

	}

	void process(AudioSampleBuffer &buffer)
	{
		if (enableState[lowPassID] == true)
			lowpassFilter.process(buffer);

		if (enableState[highPassID] == true)
			highpassFilter.process(buffer);
	}
	
	void resized() override
	{
		float sliderWidth = getWidth() * 0.3;
		float sliderHeight = getHeight() * 0.55;

		float gap = getWidth() * 0.01;
		
		cutoffSlider[lowPassID].setBounds(getWidth() * 0.1, (getHeight() * 0.65) - sliderHeight, sliderWidth, sliderHeight);
		cutoffSlider[highPassID].setBounds(getWidth() * 0.1 + gap + sliderWidth , (getHeight() * 0.65) - sliderHeight, sliderWidth, sliderHeight);

		qFactorSlider[lowPassID].setBounds(0, cutoffSlider[lowPassID].getY(), getWidth() * 0.07, cutoffSlider[lowPassID].getHeight());
		qFactorSlider[highPassID].setBounds(cutoffSlider[highPassID].getRight() + getWidth() * 0.01, cutoffSlider[highPassID].getY(), getWidth() * 0.07, cutoffSlider[highPassID].getHeight());
		
		cutoffSlider[lowPassID].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[lowPassID].getWidth() * 0.25, 20);
		cutoffSlider[highPassID].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[highPassID].getWidth() * 0.25, 20);

		float centerXLowPass = getWidth() * 0.2;
		float centerXHighPass = getWidth() * 0.5;
					
		enableButton[lowPassID].setBounds(centerXLowPass, cutoffSlider[lowPassID].getY() - 20, 120, 30);
		enableButton[highPassID].setBounds(centerXHighPass, cutoffSlider[highPassID].getY() - 20, 120, 30);

	}

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &cutoffSlider[lowPassID])
		{
			double value = cutoffSlider[lowPassID].getValue();
			double lowpassQ = qFactorSlider[lowPassID].getValue();
			lowpassFilter.setCutoff(value, sampleRate, lowpassQ);
		}
		else if (slider == &cutoffSlider[highPassID])
		{
			double value = cutoffSlider[highPassID].getValue();
			double highPassQ = qFactorSlider[highPassID].getValue();
			highpassFilter.setCutoff(value, sampleRate, highPassQ);
		}
		else if (slider == &qFactorSlider[lowPassID])
		{
			double lowpassCutoff = cutoffSlider[lowPassID].getValue();
			double lowpassQ = qFactorSlider[lowPassID].getValue();
			lowpassFilter.setCutoff(lowpassCutoff, sampleRate, lowpassQ);
		}
		else if (slider == &qFactorSlider[highPassID])
		{
			double highpassCutoff = cutoffSlider[highPassID].getValue();
			double highpassQ = qFactorSlider[highPassID].getValue();
			highpassFilter.setCutoff(highpassCutoff, sampleRate, highpassQ);
			DBG("HIGHPASS Q IS " + (String)highpassQ);
		}
	}

	void buttonClicked(Button* button) override
	{
		if (button == &enableButton[lowPassID])
		{
			enableState[lowPassID] = enableButton[lowPassID].getToggleState();
		}
		else if (button == &enableButton[highPassID])
		{
			enableState[highPassID] = enableButton[highPassID].getToggleState();
		}
	}

	void setSampleRate(double value)
	{
		sampleRate = value;
	}

	ValueTree getValueTree()
	{
		return autoFilterValueTree;
	}

	void snapshotFired()
	{
		enableState[lowPassID] = enableButton[lowPassID].getToggleState();
		enableState[highPassID] = enableButton[highPassID].getToggleState();
	}

	enum { lowPassID, highPassID, numOfFilters };

private:

	Slider cutoffSlider[numOfFilters];
	Slider qFactorSlider[numOfFilters];
	ToggleButton enableButton[numOfFilters];

	IIRFilter  test;

	ValueTree autoFilterValueTree;

	double cutoffFrequency[2];
	double sampleRate = 48000;

	LowpassIIRFilter lowpassFilter;
	HighpassIIRFilter highpassFilter;

	bool enableState[2];
};
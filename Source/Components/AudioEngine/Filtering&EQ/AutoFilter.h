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
		for (int filterNum = 0; filterNum < NumOfFilters; filterNum++)
		{
			 addAndMakeVisible(cutoffSlider[filterNum]);
			 addAndMakeVisible(enableButton[filterNum]);

			 cutoffSlider[filterNum].setRange(0.0, 20000.0, 10.0);
			 cutoffSlider[filterNum].setSliderStyle(Slider::RotaryVerticalDrag);
			 cutoffSlider[filterNum].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[filterNum].getWidth() * 0.5, 30);
			 cutoffSlider[filterNum].addListener(this);
		}

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

		addAndMakeVisible(cutoffLabel);
		cutoffLabel.setText("label", dontSendNotification);

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

		cutoffSlider[lowPassID].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[lowPassID].getWidth() * 0.25, 20);
		cutoffSlider[highPassID].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[highPassID].getWidth() * 0.25, 20);

		float centerXLowPass = getWidth() * 0.2;
		float centerXHighPass = getWidth() * 0.5;
					
		enableButton[lowPassID].setBounds(centerXLowPass, cutoffSlider[lowPassID].getY() - 20, 120, 30);
		enableButton[highPassID].setBounds(centerXHighPass, cutoffSlider[highPassID].getY() - 20, 120, 30);

		cutoffLabel.setBounds(330, 100, 100, 40);
	}

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &cutoffSlider[lowPassID])
		{
			double cutoffValue = (cutoffSlider[lowPassID].getValue() - 0.0) / (20000.0 - 0.0);
			lowpassFilter.setFirstCoefficient(cutoffValue);

			DBG((String)lowpassFilter.calculateCutoff(sampleRate));
		}
		else if (slider == &cutoffSlider[highPassID])
		{
			double cutoffValue = (cutoffSlider[highPassID].getValue() / 24350.0);
		
			//cutoffValue = sqrt(cutoffValue); 
			highpassFilter.setFirstCoefficient(cutoffValue);

			cutoffLabel.setText((String)highpassFilter.calculateCutoff(sampleRate), dontSendNotification);

			DBG((String)cutoffValue);
			DBG((String)highpassFilter.calculateCutoff(sampleRate));
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

	enum { lowPassID, highPassID, NumOfFilters };

private:

	Slider cutoffSlider[NumOfFilters];
	ToggleButton enableButton[NumOfFilters];

	ValueTree autoFilterValueTree;

	double cutoffFrequency[2];
	double sampleRate = 48000;

	LowpassIIRFilter lowpassFilter;
	HighpassIIRFilter highpassFilter;

	Label cutoffLabel;

	bool enableState[2];
};
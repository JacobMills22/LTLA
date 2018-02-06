#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPanelObject.h"

class AutoFilter : public AudioPanelObject,
	public Slider::Listener,
	public Timer,
	public Button::Listener
{
public:

	AutoFilter() : autoFilterValueTree("autoFilterValueTree")
	{
		for (int filterNum = 0; filterNum < NumOfFilters; filterNum++)
		{
			 addAndMakeVisible(cutoffSlider[filterNum]);
			 addAndMakeVisible(enableButton[filterNum]);

			 cutoffSlider[filterNum].setRange(20.0, 20000.0, 1);
			 cutoffSlider[filterNum].setSliderStyle(Slider::RotaryVerticalDrag);
			 cutoffSlider[filterNum].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[filterNum].getWidth() * 0.5, 30);
			 cutoffSlider[filterNum].addListener(this);
		}

		cutoffSlider[lowPassID].setValue(20000.0);
		cutoffSlider[highPassID].setValue(20.0);

		cutoffSlider[lowPassID].setSkewFactor(0.23);
		cutoffSlider[highPassID].setSkewFactor(0.23);

		enableButton[lowPassID].addListener(this);
		enableButton[highPassID].addListener(this);
		enableButton[lowPassID].setButtonText("LowPass Enable");
		enableButton[highPassID].setButtonText("HighPass Enable");
		enableButton[lowPassID].setToggleState(false, dontSendNotification);
		enableButton[highPassID].setToggleState(false, dontSendNotification);

		//enableState[lowPassID] = false;
		//enableState[highPassID] = false;

		autoFilterValueTree.setProperty("LowPassEnabled", false, nullptr);
		autoFilterValueTree.setProperty("HighPassEnabled", false, nullptr);

		autoFilterValueTree.setProperty("LowPassCutoff", 20000.0, nullptr);
		autoFilterValueTree.setProperty("HighPassCutoff", 20.0, nullptr);
	}

	void initialise(double SampleRate)
	{
		cutoffFrequency[lowPassID] = 20000.0;
		cutoffFrequency[highPassID] = 20.0;

		autoFilterValueTree.setProperty("LowPassCutoff", 20000.0, nullptr);
		autoFilterValueTree.setProperty("HighPassCutoff", 20.0, nullptr);

		autoFilterValueTree.setProperty("LowPassEnabled", false, nullptr);
		autoFilterValueTree.setProperty("HighPassEnabled", false, nullptr);

		filter[lowPassID].reset();
		filter[highPassID].reset();
		
		filterCoefficients[lowPassID].makeLowPass(sampleRate, autoFilterValueTree.getPropertyAsValue("LowPassCutoff", nullptr).getValue());
		filterCoefficients[highPassID].makeHighPass(sampleRate, autoFilterValueTree.getPropertyAsValue("HighPassCutoff", nullptr).getValue());

		filter[lowPassID].setCoefficients(filterCoefficients[lowPassID]);
		filter[highPassID].setCoefficients(filterCoefficients[highPassID]);

		sampleRate = SampleRate;
	}

	void updateCutoff(double lowPassCutoff, double highPassCutoff)
	{
		cutoffFrequency[lowPassID] = lowPassCutoff;
		cutoffFrequency[highPassID] = highPassCutoff;

		filterCoefficients[lowPassID] = filterCoefficients[lowPassID].makeLowPass(sampleRate, cutoffFrequency[lowPassID]);
		filterCoefficients[highPassID] = filterCoefficients[highPassID].makeHighPass(sampleRate, cutoffFrequency[highPassID]);

		filter[lowPassID].setCoefficients(filterCoefficients[lowPassID]);
		filter[highPassID].setCoefficients(filterCoefficients[highPassID]);
	}

	void process(AudioSampleBuffer &buffer)
	{
		float* OutputL = buffer.getWritePointer(0);
		float* OutputR = buffer.getWritePointer(1);

		bool lowPassEnabled = autoFilterValueTree.getPropertyAsValue("LowPassEnabled", nullptr).getValue();
		bool highPassEnabled = autoFilterValueTree.getPropertyAsValue("HighPassEnabled", nullptr).getValue();

		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			if (lowPassEnabled == true)
			{
				OutputL[sample] = filter[lowPassID].processSingleSampleRaw(OutputL[sample]);
				OutputR[sample] = filter[lowPassID].processSingleSampleRaw(OutputR[sample]);
			}

			if (highPassEnabled == true)
			{
				OutputL[sample] = filter[highPassID].processSingleSampleRaw(OutputL[sample]);
				OutputR[sample] = filter[highPassID].processSingleSampleRaw(OutputR[sample]);
			}
		}
	}

	void resized() override
	{
		float sliderWidth = getWidth() * 0.4;
		float sliderHeight = getHeight() * 0.55;

		float gap = getWidth() * 0.01;
		
		cutoffSlider[lowPassID].setBounds(getWidth() * 0.1, (getHeight() * 0.65) - sliderHeight, sliderWidth, sliderHeight);
		cutoffSlider[highPassID].setBounds(getWidth() * 0.1 + gap + sliderWidth , (getHeight() * 0.65) - sliderHeight, sliderWidth, sliderHeight);

		cutoffSlider[lowPassID].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[lowPassID].getWidth() * 0.25, 20);
		cutoffSlider[highPassID].setTextBoxStyle(Slider::TextBoxBelow, false, cutoffSlider[highPassID].getWidth() * 0.25, 20);

		//float centerXLowPass = cutoffSlider[lowPassID].getX() + (cutoffSlider[lowPassID].getBounds().getWidth() * 0.2);
		float centerXLowPass = getWidth() * 0.2;
		float centerXHighPass = getWidth() * 0.6;
			
		enableButton[lowPassID].setBounds(centerXLowPass, cutoffSlider[lowPassID].getY() - 20, 120, 30);
		enableButton[highPassID].setBounds(centerXHighPass, cutoffSlider[highPassID].getY() - 20, 120, 30);

	}

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &cutoffSlider[lowPassID])
		{
			updateCutoff(cutoffSlider[lowPassID].getValue(), cutoffFrequency[highPassID]);
			autoFilterValueTree.setProperty("LowPassCutoff", cutoffSlider[lowPassID].getValue(), nullptr);
		}
		else if (slider == &cutoffSlider[highPassID])
		{
			updateCutoff(cutoffFrequency[lowPassID], cutoffSlider[highPassID].getValue());
			autoFilterValueTree.setProperty("HighPassCutoff", cutoffSlider[highPassID].getValue(), nullptr);
		}
	}

	void buttonClicked(Button* button) override
	{
		if (button == &enableButton[lowPassID])
		{
			//enableState[lowPassID] = enableButton[lowPassID].getToggleState();
			autoFilterValueTree.setProperty("LowPassEnabled", enableButton[lowPassID].getToggleState(), nullptr);
		}
		else if (button == &enableButton[highPassID])
		{
			//enableState[highPassID] = enableButton[highPassID].getToggleState();
			autoFilterValueTree.setProperty("HighPassEnabled", enableButton[highPassID].getToggleState(), nullptr);

		}
	}

	void timerCallback() override
	{

	}

	void setSampleRate(double value)
	{
		sampleRate = value;
	}

	void paint(Graphics& g) override
	{

	}

	ValueTree getValueTree()
	{
		return autoFilterValueTree;
	}

	void snapshotFired()
	{
		cutoffSlider[lowPassID].setValue(autoFilterValueTree.getPropertyAsValue("LowPassCutoff", nullptr).getValue());
		cutoffSlider[highPassID].setValue(autoFilterValueTree.getPropertyAsValue("HighPassCutoff", nullptr).getValue());

		enableButton[lowPassID].setToggleState(autoFilterValueTree.getPropertyAsValue("LowPassEnabled", nullptr).getValue(), dontSendNotification);
		enableButton[highPassID].setToggleState(autoFilterValueTree.getPropertyAsValue("HighPassEnabled", nullptr).getValue(), dontSendNotification);
		
	}

	enum { lowPassID, highPassID, NumOfFilters };

private:

	IIRFilter filter[2];
	IIRCoefficients filterCoefficients[2];
	Slider cutoffSlider[2];
	ToggleButton enableButton[2];

	ValueTree autoFilterValueTree;

	double cutoffFrequency[2];
	double sampleRate = 48000;
	//bool enableState[2];
};
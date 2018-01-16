#pragma once

#include "AudioPanelObject.h"

class AudioMeter : public AudioPanelObject,
				   public Timer
{

public:

	AudioMeter()
	{
		peakMeterData[0] = 0.2;
		peakMeterData[1] = 0.7;

		peak[0] = 0.0;
		peak[1] = 0.0;

		startTimer(15);
	}

	~AudioMeter()
	{
		stopTimer();
	}

	void resized() override
	{
		float meterSpacing = (float)getWidth() * 0.05;
		float meterWidth = ((float)getWidth() * 0.5) - (meterSpacing * 0.5);
		meterHeight = getHeight();
		
		float meterRightX = meterWidth + meterSpacing;

		leftMeter.setBounds(      0,      meterHeight * peakMeterData[0], meterWidth, meterHeight * 2);
		rightMeter.setBounds(meterRightX, meterHeight * peakMeterData[1], meterWidth, meterHeight * 2);
	}

	void timerCallback() override
	{
		resized();
	}

	void setMeterData(int channel, float value)
	{
		
		value *= 5;

		if (value > peak[channel])
		{
			peakMeterData[channel] = 1.0 - value;
			peak[channel] = value;
		}
		else
		{
			peak[channel] -= 0.01;
			peakMeterData[channel] = 1.0 - peak[channel];
		}
	}

	void paint(Graphics& g) override
	{
		g.fillAll(Colours::grey);
		g.setColour(Colours::black);
		g.drawRect(0, 0, getWidth(), getHeight(), 1.0);

		g.setColour(Colours::blue);
		g.fillRect(leftMeter);
		g.setColour(Colours::black);
		g.drawRect(leftMeter);

		g.setColour(Colours::green);
		g.fillRect(rightMeter);
		g.setColour(Colours::black);
		g.drawRect(rightMeter);
	
	}

private:

	float peakMeterData[2];
	juce::Rectangle<float> leftMeter;
	juce::Rectangle<float> rightMeter;
	float meterHeight = 0;
	float peak[2];


};
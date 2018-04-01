#pragma once

#include "../AudioPanelObject.h"

/** Very simple audio metering class, could use some
	improvements to display level in dB rather than 
	basic amplitude */

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
		// Resize the meters 
		float meterSpacing = (float)getWidth() * 0.05;
		float meterWidth = ((float)getWidth() * 0.5) - (meterSpacing * 0.5);
		meterHeight = getHeight();
		
		float meterRightX = meterWidth + meterSpacing;

		// Height is scaled by the amplitide peak.
		leftMeter.setBounds(      0,      meterHeight * peakMeterData[0], meterWidth, meterHeight * 2);
		rightMeter.setBounds(meterRightX, meterHeight * peakMeterData[1], meterWidth, meterHeight * 2);
	}

	void timerCallback() override
	{
		// Periodically update the meter display.
		resized();
	}

	void setMeterData(int channel, float value)
	{
		// Scale meter data
		value *= 10;

		// Decay meter display
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
		// Fill background and draw outline
		g.fillAll(Colours::grey);
		g.setColour(Colours::black);
		g.drawRect(0, 0, getWidth(), getHeight(), 1.0);

		// Fill meters and draw outline around each meter
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
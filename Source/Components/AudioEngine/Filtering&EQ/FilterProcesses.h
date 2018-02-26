#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>

class FilterProcess
{
public:

	void process(AudioSampleBuffer &buffer)
	{
		float* outputL = buffer.getWritePointer(0);
		float* outputR = buffer.getWritePointer(1);

		secondCoefficient = 1.0 - firstCoefficient;

		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			outputL[sample] *= secondCoefficient;
			outputR[sample] *= secondCoefficient;

			previousSampleL *= firstCoefficient;
			previousSampleR *= firstCoefficient;

			outputL[sample] += previousSampleL;
			outputR[sample] += previousSampleR;

			previousSampleL = outputL[sample];
			previousSampleR = outputR[sample];
		}
	}

	void setFirstCoefficient(float value)
	{
		firstCoefficient = value;
		secondCoefficient = 1.0 - firstCoefficient;
	}

	float getFirstCoefficient()
	{
		return firstCoefficient;
	}

	float getSecondCoefficient()
	{
		return secondCoefficient;
	}

private:

	float previousSampleL = 0.0;
	float previousSampleR = 0.0;

	float firstCoefficient = 0.1;
	float secondCoefficient = 1.0;

};

class LowpassIIRFilter
{

public:

	void process(AudioSampleBuffer &buffer)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].process(buffer);
		}
	}

	void setFirstCoefficient(float value)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].setFirstCoefficient(value);
		}
	}

	double calculateCutoff(float sampleRate)
	{
		double cutoff = 0.0;
		cutoff = acos(1.0 - ((filterSeries[0].getFirstCoefficient() * filterSeries[0].getFirstCoefficient()) / (2.0 * filterSeries[0].getSecondCoefficient()))) * (sampleRate / (double_Pi * 2.0));
		return cutoff;
	}

private:

	enum { numOfFilters = 3 };
	FilterProcess filterSeries[numOfFilters];

};

class HighpassIIRFilter
{

public:

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		rawInputBuffer.setSize(2, samplesPerBlockExpected, false, false, false);
	}

	void process(AudioSampleBuffer &buffer)
	{
		rawInputBuffer.makeCopyOf(buffer, false);

		lowpassFilter.process(buffer);

		float* outputL = buffer.getWritePointer(0);
		float* outputR = buffer.getWritePointer(1);

		float* rawInputL = rawInputBuffer.getWritePointer(0);
		float* rawInputR = rawInputBuffer.getWritePointer(1);

		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			outputL[sample] = rawInputL[sample] - outputL[sample];
			outputR[sample] = rawInputR[sample] - outputR[sample];
		}
	}

	void setFirstCoefficient(float value)
	{
		lowpassFilter.setFirstCoefficient(value);
	}

	double calculateCutoff(float sampleRate)
	{
		return lowpassFilter.calculateCutoff(sampleRate);
	}

private:

	LowpassIIRFilter lowpassFilter;
	AudioSampleBuffer rawInputBuffer;

};

class BandPassFilter
{

public:

	void process(AudioSampleBuffer &buffer)
	{
		lowpassFilter.process(buffer);
		highpassFilter.process(buffer);
	}

	void setFirstCoefficient(float value)
	{
		firstCoefficient = value;
		lowpassFilter.setFirstCoefficient(value - bandWidth);
		highpassFilter.setFirstCoefficient(value + bandWidth);
	}

	void setBandWidth(float value)
	{
		bandWidth = value;
		setFirstCoefficient(firstCoefficient);
	}

	float getFirstCoefficient()
	{
		return firstCoefficient;
	}

	float getBandWidth()
	{
		return bandWidth;
	}

	void printBandWidthInHz()
	{
		DBG("Band Width is between " + (String)lowpassFilter.calculateCutoff(48000) + " and " + (String)highpassFilter.calculateCutoff(48000));
	}

	void printCutOffFreqInHz(float sampleRate)
	{
		DBG((String)lowpassFilter.calculateCutoff(sampleRate));
	}

private:

	LowpassIIRFilter lowpassFilter;
	HighpassIIRFilter highpassFilter;

	float bandWidth = 0.001;
	float firstCoefficient = 0.0;
};
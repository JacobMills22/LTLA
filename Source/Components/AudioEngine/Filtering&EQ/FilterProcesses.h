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

		const float* inputL = buffer.getReadPointer(0);
		const float* inputR = buffer.getReadPointer(1);
		
		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			outputL[sample] = (coefficientA0 * inputL[sample]) + (coefficientA1 * delayedSampleX1L) + (coefficientA2 * delayedSampleX2L)
				                                               - (coefficientB1 * delayedSampleY1L) - (coefficientB2 * delayedSampleY2L);

			outputR[sample] = (coefficientA0 * inputR[sample]) + (coefficientA1 * delayedSampleX1R) + (coefficientA2 * delayedSampleX2R)
				                                               - (coefficientB1 * delayedSampleY1R) - (coefficientB2 * delayedSampleY2R);

			delayedSampleX2L = delayedSampleX1L;
			delayedSampleX1L = inputL[sample];

			delayedSampleY2L = delayedSampleY1L;
			delayedSampleY1L = outputL[sample];

			delayedSampleX2R = delayedSampleX1R;
			delayedSampleX1R = inputR[sample];

			delayedSampleY2R = delayedSampleY1R;
			delayedSampleY1R = outputR[sample];

		}
	}

	void setCutoffinHz(double cutoff, double sampleRate, double Qfactor)
	{

		if (filterType == lowpass)
		{
			double omega = (double_Pi * 2.0 * cutoff) / sampleRate;

			coefficientB2 = (2.0 * Qfactor - sin(omega)) / (2.0 * Qfactor + sin(omega));

			coefficientB1 = -(1.0 + coefficientB2) * cos(omega);

			coefficientA0 = 0.25 * (1.0 + coefficientB1 + coefficientB2);
			coefficientA1 = 2.0 * coefficientA0;
			coefficientA2 = coefficientA0;
		}
		else if (filterType == highpass)
		{
			double omega = (double_Pi * 2.0 * cutoff) / sampleRate;

			coefficientB2 = (2.0 * Qfactor - sin(omega)) / (2.0 * Qfactor + sin(omega));

			coefficientB1 = -(1.0 + coefficientB2) * cos(omega);

			coefficientA0 = 0.25 * (1.0 - coefficientB1 + coefficientB2);
			coefficientA1 = -(2.0 * coefficientA0);
			coefficientA2 = coefficientA0;
		}
	}

	void makeLowPass()
	{
		filterType = lowpass;
	}

	void makeHighPass()
	{
		filterType = highpass;
	}

private:

	double coefficientB1 = 0.0;
	double coefficientB2 = 0.0;
	double coefficientA0 = 0.0;
	double coefficientA1 = 0.0;
	double coefficientA2 = 0.0;

	double delayedSampleX1L = 0.0;
	double delayedSampleY1L = 0.0;

	double delayedSampleX1R = 0.0;
	double delayedSampleY1R = 0.0;

	double delayedSampleX2L = 0.0;
	double delayedSampleY2L = 0.0;

	double delayedSampleX2R = 0.0;
	double delayedSampleY2R = 0.0;

	enum { lowpass, highpass };
	int filterType = lowpass;
	
};

class LowpassIIRFilter
{

public:

	LowpassIIRFilter()
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].makeLowPass();
		}
	}

	void process(AudioSampleBuffer &buffer)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].process(buffer);
		}
	}

	void setCutoff(double cutoff, double sampleRate, double qFactor)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].setCutoffinHz(cutoff, sampleRate, qFactor);
		}
	}
	
private:

	enum { numOfFilters = 3 };
	FilterProcess filterSeries[numOfFilters];
};

class HighpassIIRFilter
{

public:

	HighpassIIRFilter()
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].makeHighPass();
		}
	}

	void process(AudioSampleBuffer &buffer)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].process(buffer);
		}
	}

	void setCutoff(double cutoff, double sampleRate, double qFactor)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].setCutoffinHz(cutoff, sampleRate, qFactor);
		}
	}

private:

	enum { numOfFilters = 3 };
	FilterProcess filterSeries[numOfFilters];
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
	//	lowpassFilter.setFirstCoefficient(value - bandWidth);
	//	highpassFilter.setFirstCoefficient(value + bandWidth);
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

private:

	LowpassIIRFilter lowpassFilter;
	HighpassIIRFilter highpassFilter;

	float bandWidth = 0.001;
	float firstCoefficient = 0.0;
};
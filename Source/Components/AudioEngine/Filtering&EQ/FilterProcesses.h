#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

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
			
			delayedSampleX2L = delayedSampleX1L;
			delayedSampleX1L = inputL[sample];

			delayedSampleY2L = delayedSampleY1L;
			delayedSampleY1L = outputL[sample];

			outputR[sample] = (coefficientA0 * inputR[sample]) + (coefficientA1 * delayedSampleX1R) + (coefficientA2 * delayedSampleX2R) 
					                                           - (coefficientB1 * delayedSampleY1R) - (coefficientB2 * delayedSampleY2R);

			delayedSampleX2R = delayedSampleX1R;
			delayedSampleX1R = inputR[sample];

			delayedSampleY2R = delayedSampleY1R;
			delayedSampleY1R = outputR[sample];
		}
	}

	float processPeaking(double x0)
	{
		double y0 = (coefficientA0 * x0) + (coefficientA1 * delayedSampleX1L) + (coefficientA2 * delayedSampleX2L) 
										 - (coefficientB1 * delayedSampleY1L) - (coefficientB2 * delayedSampleY2L);

		delayedSampleX2L = delayedSampleX1L;
		delayedSampleX1L = x0;

		delayedSampleY2L = delayedSampleY1L;
		delayedSampleY1L = y0;

		return y0;
	}

	void setCutoffinHz(double cutoff, double sampleRate, double Qfactor, double gain, bool test = false)
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
		else if (filterType == bandpass)
		{
			double omega = (double_Pi * 2.0 * cutoff) / sampleRate;

			coefficientB2 = tan((double_Pi / 4) - (omega / (2.0 * Qfactor)));

			coefficientB1 = -(1.0 + coefficientB2) * cos(omega);

			coefficientA0 = 0.5 * (1.0 - coefficientB2);
			coefficientA1 = 0.0;
			coefficientA2 = -coefficientA0;
		}
		else if (filterType == peaking)
		{
			double omega = double_Pi * 2.0 * cutoff / sampleRate;

			double gainIndec = pow(10, (gain / 20));
			double g0 = 0.5 * (gainIndec - 1.0);

			double g1 = 2.0 / (1.0 + g0);
			
			coefficientB2 = (1.0 - (g1 * tan(omega / (2.0 * Qfactor)))) / (1.0 + (g1 * tan(omega / (2.0 * Qfactor))));

			coefficientB1 = -(1.0 + coefficientB2) * cos(omega);
			
			coefficientA0 = 1.0 + g0 * (1.0 - coefficientB2);
			coefficientA1 = coefficientB1;
			coefficientA2 = 1.0 + (coefficientB2 - coefficientA0);
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

	void makeBandPass()
	{
		filterType = bandpass;
	}

	void makePeaking()
	{
		filterType = peaking;
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

	enum { lowpass, highpass, bandpass, peaking};
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
			filterSeries[filterID].setCutoffinHz(cutoff, sampleRate, qFactor, 0.0);
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
			filterSeries[filterID].setCutoffinHz(cutoff, sampleRate, qFactor, 0.0);
		}
	}

private:

	enum { numOfFilters = 3 };
	FilterProcess filterSeries[numOfFilters];
};

class BandPassFilter
{

public:

	BandPassFilter()
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].makeBandPass();
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
			filterSeries[filterID].setCutoffinHz(cutoff, sampleRate, qFactor, 0.0);
		}
	}

private:

	enum { numOfFilters = 3 };
	FilterProcess filterSeries[numOfFilters];
};

class PeakingFilter
{

public:

	PeakingFilter()
	{
		filterSeries.makePeaking();
	}

	float process(double x0)
	{
		return filterSeries.processPeaking(x0);
	}

	void setCutoff(double cutoff, double sampleRate, double qFactor, double gain)
	{
		filterSeries.setCutoffinHz(cutoff, sampleRate, qFactor, gain);
	}

private:

	FilterProcess filterSeries;
};
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
			outputL[sample] = (coefficientA0 * inputL[sample]) + (coefficientA1 * delayedSampleX1L) + (coefficientA2 * delayedSampleX2L) - (coefficientB1 * delayedSampleY1L) - (coefficientB2 * delayedSampleY2L);
			
			delayedSampleX2L = delayedSampleX1L;
			delayedSampleX1L = inputL[sample];

			delayedSampleY2L = delayedSampleY1L;
			delayedSampleY1L = outputL[sample];

			outputR[sample] = (coefficientA0 * inputR[sample]) + (coefficientA1 * delayedSampleX1R) + (coefficientA2 * delayedSampleX2R) - (coefficientB1 * delayedSampleY1R) - (coefficientB2 * delayedSampleY2R);

			delayedSampleX2R = delayedSampleX1R;
			delayedSampleX1R = inputR[sample];

			delayedSampleY2R = delayedSampleY1R;
			delayedSampleY1R = outputR[sample];

		}
	}

	void processChannel(AudioSampleBuffer &buffer, int channel)
	{
		float* output = buffer.getWritePointer(channel);

		const float* input = buffer.getReadPointer(channel);

		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			output[sample] = (coefficientA0 * input[sample]) + (coefficientA1 * delayedSampleX1L) + (coefficientA2 * delayedSampleX2L) - (coefficientB1 * delayedSampleY1L) - (coefficientB2 * delayedSampleY2L);
			
			delayedSampleX2L = delayedSampleX1L;
			delayedSampleX1L = input[sample];

			delayedSampleY2L = delayedSampleY1L;
			delayedSampleY1L = output[sample];

		}
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

			double fc = cutoff;
			double sr = sampleRate;
			double g = gain;
			double q = Qfactor;

			double a0, a1, a2, b1, b2;

			const double theta = (2 * float_Pi * fc) / sr;
			const double g0 = 0.5 * (g - 1.0);
			const double g1 = 2.0 / (1 + g0);

			b2 = (1 - (g1 * tan(theta / (2 * q)))) / (1 + (g1 * tan(theta / (2 * q))));
			b1 = -(1 + b2) * cos(theta);
			a0 = 1 + g0 * (1 - b2);
			a1 = b1;
			a2 = 1 + (b2 - a0);

			coefficientB2 = b2;
			coefficientB1 = b1;
			coefficientA0 = a0;
			coefficientA1 = a1;
			coefficientA2 = a2;

			/*
			//cutoff = cutoff * 0.5;

			double omega = double_Pi * 2.0 * cutoff / sampleRate;

			double gainIndec = pow(10, (gain / 20));
			double g0 = 0.5 * (gainIndec - 1.0);

			double g1 = 2.0 / (1.0 + g0);
			
			//double twoQ = 2.0 * Qfactor;

			coefficientB2 = (1.0 - (g1 * tan(omega / (2.0 * Qfactor)))) / (1.0 + (g1 * tan(omega / (2.0 * Qfactor))));

			coefficientB1 = -(1.0 + coefficientB2) * cos(omega);
			
			coefficientA0 = 1.0 + g0 * (1.0 - coefficientB2);
			coefficientA1 = coefficientB1;
			coefficientA2 = 1.0 + (coefficientB2 - coefficientA0);
			*/

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
	//	for (int filterID = 0; filterID < numOfFilters; filterID++)
	//	{
			filterSeries.makePeaking();
	//	}
	}

	void process(AudioSampleBuffer &buffer)
	{
	//	for (int filterID = 0; filterID < numOfFilters; filterID++)
	//	{
			filterSeries.process(buffer);
	//	}
	}

	void setCutoff(double cutoff, double sampleRate, double qFactor, double gain)
	{
	//	for (int filterID = 0; filterID < numOfFilters; filterID++)
	//	{
			filterSeries.setCutoffinHz(cutoff, sampleRate, qFactor, gain);
	//	}
	}

private:

//	enum { numOfFilters = 1 };
	FilterProcess filterSeries;
};
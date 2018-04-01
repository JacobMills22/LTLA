
#include "FilterProcesses.h"

	//====================================================
	//	FilterProcess
	//====================================================

	void FilterProcess::process(AudioSampleBuffer &buffer)
	{
		float* outputL = buffer.getWritePointer(0);
		float* outputR = buffer.getWritePointer(1);

		const float* x0L = buffer.getReadPointer(0);
		const float* x0R = buffer.getReadPointer(1);

		for (int s = 0; s < buffer.getNumSamples(); s++)
		{
			// Left Channel
			outputL[s] = (a0 * x0L[s]) + (a1 * x1L) + (a2 * x2L) - (b1 * y1L) - (b2 * y2L);

			x2L = x1L;
			x1L = x0L[s];

			y2L = y1L;
			y1L = outputL[s];

			// Right Channel
			outputR[s] = (a0 * x0R[s]) + (a1 * x1R) + (a2 * x2R) - (b1 * y1R) - (b2 * y2R);

			x2R = x1R;
			x1R = x0R[s];

			y2R = y1R;
			y1R = outputR[s];
		}
	}

	float FilterProcess::processPeaking(double x0)
	{
		double y0 = (a0 * x0) + (a1 * x1L) + (a2 * x2L) - (b1 * y1L) - (b2 * y2L);

		x2L = x1L;
		x1L = x0;

		y2L = y1L;
		y1L = y0;

		return y0;
	}

	void FilterProcess::setCoefficients(double cutoff, double Qfactor, double gain, double sampleRate)
	{
		double omega = (double_Pi * 2.0 * cutoff) / sampleRate;

		if (filterType == lowpass)
		{
			b2 = (2.0 * Qfactor - sin(omega)) / (2.0 * Qfactor + sin(omega));

			b1 = -(1.0 + b2) * cos(omega);

			a0 = 0.25 * (1.0 + b1 + b2);
			a1 = 2.0 * a0;
			a2 = a0;
		}
		else if (filterType == highpass)
		{
			b2 = (2.0 * Qfactor - sin(omega)) / (2.0 * Qfactor + sin(omega));

			b1 = -(1.0 + b2) * cos(omega);

			a0 = 0.25 * (1.0 - b1 + b2);
			a1 = -(2.0 * a0);
			a2 = a0;
		}
		else if (filterType == bandpass)
		{
			b2 = tan((double_Pi / 4) - (omega / (2.0 * Qfactor)));

			b1 = -(1.0 + b2) * cos(omega);

			a0 = 0.5 * (1.0 - b2);
			a1 = 0.0;
			a2 = -a0;
		}
		else if (filterType == peaking)
		{
			// Convert gain in dB to gain in decimal
			double gainInDecimal = pow(10, (gain / 20));

			double g0 = 0.5 * (gainInDecimal - 1.0);
			double g1 = 2.0 / (1.0 + g0);

			b2 = (1.0 - (g1 * tan(omega / (2.0 * Qfactor)))) / (1.0 + (g1 * tan(omega / (2.0 * Qfactor))));

			b1 = -(1.0 + b2) * cos(omega);

			a0 = 1.0 + g0 * (1.0 - b2);
			a1 = b1;
			a2 = 1.0 + (b2 - a0);
		}
	}

	void FilterProcess::makeLowPass()
	{
		filterType = lowpass;
	}

	void FilterProcess::makeHighPass()
	{
		filterType = highpass;
	}

	void FilterProcess::makeBandPass()
	{
		filterType = bandpass;
	}

	void FilterProcess::makePeaking()
	{
		filterType = peaking;
	}

	//====================================================
	//	LowpassIIRFilter
	//====================================================

	LowpassIIRFilter::LowpassIIRFilter()
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].makeLowPass();
		}
	}

	void LowpassIIRFilter::process(AudioSampleBuffer &buffer)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].process(buffer);
		}
	}

	void LowpassIIRFilter::setParameters(double cutoff, double qFactor)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].setCoefficients(cutoff, qFactor, 0.0, sampleRate);
		}
	}

	void LowpassIIRFilter::setSampleRate(double newSampleRate)
	{
		sampleRate = newSampleRate;
	}

	//====================================================
	//	HighpassIIRFilter
	//====================================================

	HighpassIIRFilter::HighpassIIRFilter()
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].makeHighPass();
		}
	}

	void HighpassIIRFilter::process(AudioSampleBuffer &buffer)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].process(buffer);
		}
	}

	void HighpassIIRFilter::setParameters(double cutoff, double qFactor)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].setCoefficients(cutoff, qFactor, 0.0, sampleRate);
		}
	}

	void HighpassIIRFilter::setSampleRate(double newSampleRate)
	{
		sampleRate = newSampleRate;
	}

	//====================================================
	//	BandPassFilter
	//====================================================

	BandPassFilter::BandPassFilter()
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].makeBandPass();
		}
	}

	void BandPassFilter::process(AudioSampleBuffer &buffer)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].process(buffer);
		}
	}

	void BandPassFilter::setParameters(double cutoff, double qFactor)
	{
		for (int filterID = 0; filterID < numOfFilters; filterID++)
		{
			filterSeries[filterID].setCoefficients(cutoff, qFactor, 0.0, sampleRate);
		}
	}
	
	void BandPassFilter::setSampleRate(double newSampleRate)
	{
		sampleRate = newSampleRate;
	}
	
	//====================================================
	//	PeakingFilter
	//====================================================

	PeakingFilter:: PeakingFilter()
	{
		filter.makePeaking();
	}

	float PeakingFilter::process(double x0)
	{
		return filter.processPeaking(x0);
	}

	void PeakingFilter::setParameters(double cutoff, double qFactor, double gain)
	{
		filter.setCoefficients(cutoff, qFactor, gain, sampleRate);
	}

	void PeakingFilter::setSampleRate(double newSamplerate)
	{
		sampleRate = newSamplerate;
	}

	//====================================================
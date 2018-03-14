#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SimpleFade
{

public:

	void initialiseFade(double start, double end, unsigned long long lengthInSamples)
	{
		if (end <= 0.0)
		{
			end = 0.01;
		}

		if (start <= 0.0)
		{
			start = 0.01;
		}

		multiplier = 1.0 + (log(end) - log(start)) / (lengthInSamples);
		currentFade = start;
		timesProcessed = 0;
		fadeLengthInSamples = lengthInSamples;
	}

	double process()
	{
		if (timesProcessed < fadeLengthInSamples)
		{
			currentFade *= multiplier;
			timesProcessed++;
		}
		return currentFade;
	}

	void setNeedsReinitialising(bool state)
	{
		needsInitialising = state;
	}

	bool getNeedsReinitialising()
	{
		return needsInitialising;
	}

private:

	double multiplier = 0.0;
	double currentFade = 0.0001;
	int fadeLengthInSamples;
	int timesProcessed = 0;

	bool needsInitialising = true;
};
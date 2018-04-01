#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Based off this guide - http://www.martin-finke.de/blog/articles/audio-plugins-011-envelopes/

/** This simple class is used to fade an audio buffer between two levels,
	It must be initialised with the start and end levels (0.0 to 1.0)
	and the duration that the fade will take in samples. The process 
	function must then be multiplied by the current output sample. */

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

private:

	double multiplier = 0.0;
	double currentFade = 0.0001;
	int fadeLengthInSamples;
	int timesProcessed = 0;
};
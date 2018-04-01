#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

/** The following filter designs have been implemented using 
    the designs from the Audio Processes Book by David Creasey 
	Available from https://www.dawsonera.com/abstract/9781315657813 */

/** This file contains a FilterProcess class capable
	of producing various filtering methods. Seperate
	classes for each filter are also created which makes 
	implementation of the various filters easier. */

// =======================================

/** This class implements a single difference equation and 
    calculates the required coefficients for various filters.
	The difference equation represents a IIR biquad filter.*/

class FilterProcess
{
public:

	/** Processes the audio using difference equation with calculated coefficients */
	void process(AudioSampleBuffer &buffer);

	/** Slightly different process function for the only used for a peaking filter. 
	This function only processes a single sample on a single channel so two instances of
	this class need to be used for stereo processing. */
	float processPeaking(double x0);

	/** Calculates the coefficients for the filter using the provided parameters*/
	void setCoefficients(double cutoff, double Qfactor, double gain, double sampleRate);

	/** Turns the filter into a Low Pass */
	void makeLowPass();

	/** Turns the filter into a High Pass */
	void makeHighPass();

	/** Turns the filter into a Band Pass */
	void makeBandPass();

	/** Turns the filter into a peaking filter */
	void makePeaking();

private:

	double b1 = 0.0;
	double b2 = 0.0;
	double a0 = 0.0;
	double a1 = 0.0;
	double a2 = 0.0;

	double x1L = 0.0;
	double y1L = 0.0;

	double x1R = 0.0;
	double y1R = 0.0;

	double x2L = 0.0;
	double y2L = 0.0;

	double x2R = 0.0;
	double y2R = 0.0;

	enum { lowpass, highpass, bandpass, peaking};
	int filterType = lowpass;
	
};

/** This class uses the FilterProcess class to 
	create a IIR biquad lowpass filtering effect 
	with variable Q factor and cutoff frequency.
	To use ensure the sample rate has been set before
	setting any parameters and set parameters before
	any audio is processed.*/

class LowpassIIRFilter
{

public:

	/** Constructor used to set the filter to be a lowpass. */
	LowpassIIRFilter();

	/** Processes the audio buffer */
	void process(AudioSampleBuffer &buffer);

	/** Sets the cutoff and Q factor parameters. */
	void setParameters(double cutoff, double qFactor);

	/** Sets the sample rate, this should be called before processing. */
	void setSampleRate(double newSampleRate);
	
private:

	enum { numOfFilters = 3 };
	FilterProcess filterSeries[numOfFilters];
	double sampleRate = 48000;
};

/** This class uses the FilterProcess class to
	create a IIR biquad highpass filtering effect
	with variable Q factor and cutoff frequency.
	To use ensure the sample rate has been set before
	setting any parameters and set parameters before
	any audio is processed.*/

class HighpassIIRFilter
{

public:

	/** Constructor used to set the filter to be a highpass. */
	HighpassIIRFilter();

	/** Processes the audio buffer */
	void process(AudioSampleBuffer &buffer);

	/** Sets the cutoff and Q factor parameters. */
	void setParameters(double cutoff, double qFactor);

	/** Sets the sample rate, this should be called before processing. */
	void setSampleRate(double newSampleRate);

private:

	enum { numOfFilters = 3 };
	FilterProcess filterSeries[numOfFilters];
	double sampleRate = 48000;
};

/** This class uses the FilterProcess class to
	create a IIR biquad bandpass filtering effect
	with variable Q factor and cutoff frequency.
	To use ensure the sample rate has been set before
	setting any parameters and set parameters before
	any audio is processed.*/

class BandPassFilter
{

public:

	/** Constructor used to set the filter to be a bandpass. */
	BandPassFilter();

	/** Processes the audio buffer */
	void process(AudioSampleBuffer &buffer);

	/** Sets the cutoff and Q factor parameters. */
	void setParameters(double cutoff, double qFactor);

	/** Sets the sample rate, this should be called before processing. */
	void setSampleRate(double newSampleRate);

private:

	enum { numOfFilters = 3 };
	FilterProcess filterSeries[numOfFilters];
	double sampleRate = 48000;

};

/** This class uses the FilterProcess class to
	create a IIR biquad peaking filtering effect
	with variable Q factor, gain centre frequency.
	To use ensure the sample rate has been set before
	setting any parameters and set parameters before
	any audio is processed.
	
	Note: The process function only processes a single 
	sample on a single channel, so two instances of 
	this class need to be used for stereo processing. */
	
class PeakingFilter
{

public:

	/** Constructor used to set the filter to be a peaking filter. */
	PeakingFilter();

	/** returns a single processed version of the provided audio sample */
	float process(double x0);

	/** Sets the cutoff and Q factor parameters. */
	void setParameters(double cutoff, double qFactor, double gain);

	/** Sets the sample rate, this should be called before processing. */
	void setSampleRate(double newSampleRate);

private:

	FilterProcess filter;
	double sampleRate = 48000;

};
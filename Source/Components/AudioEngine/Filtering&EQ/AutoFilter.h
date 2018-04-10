#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/AudioEngine/AudioPanelObject.h"
#include <math.h>
#include "FilterProcesses.h"

/** This class represents the whole auto filtering module,
	and contains two filters (highpass and lowpass) with
	variable cutoff, Q factor and enable states. */
 
class AutoFilter : public AudioPanelObject,
				   public Slider::Listener,
	               public Button::Listener
{
public:

	/** Constructor, initialisation. */
	AutoFilter();

	/** Processes audio using both filters if enabled. */
	void process(AudioSampleBuffer &buffer);
	
	/** Standard JUCE resize function, used to set the bounds of various components. */
	void resized() override;

	/** Called when a sliders value changes, used to set the filters parameters. */
	void sliderValueChanged(Slider* slider) override;

	/** Called when a button is clicked, used to enable or disable the filtering. */
	void buttonClicked(Button* button) override;

	/** Sets the sample rate of the filters. */
	void setSampleRate(double value);

	/** Returns the value tree of the AutoFilters */
	ValueTree getValueTree();

	/** Called when a snapshot is fired, used to update parameter positions/states. */
	void snapshotFired();

private:

	enum FilterIDs { lowPassID, highPassID, numOfFilters };

	LowpassIIRFilter lowpassFilter;
	HighpassIIRFilter highpassFilter;

	Slider cutoffSlider[numOfFilters];
	Slider qFactorSlider[numOfFilters];
	ToggleButton enableButton[numOfFilters];
	Label qfactorLabel[numOfFilters];

	ValueTree autoFilterValueTree;

	double cutoffFrequency[2];
	double sampleRate = 48000;
	bool enableState[2];
};
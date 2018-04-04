#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/AudioEngine/AudioPanelObject.h"

/** This component simply pans an audio buffer by
using the process and setPanAmount functions. The
pan amount can be updated bya  performers position
so a timer is used to upfdate the slider and pan amount. */

class AutoPanner : public AudioPanelObject,
				   public Slider::Listener,
				   public Timer,
	               public Button::Listener
{ 

public:

/** Constructor: Initialisation. */
	AutoPanner();
	
/** Sets the pan amount. */
	void setPanAmount(float value);

/** Applys the panning to the buffer */
	void process(AudioSampleBuffer &buffer);

/** Sets the bounds of the auto-panner panels objects*/
	void resized() override;

/** Called when a slider is changed, used to update the pan amount.*/
	void sliderValueChanged(Slider* slider) override;

/** Called when a button is pressed, used to enable/disable the auto-panning 
	and reset the pan amount. Note: Only disables the automatic element of the panning,
	not the pan itself. */
	void buttonClicked(Button* button) override;

/** Updates the pan amount slider*/
	void timerCallback() override;

/** Returns the Autopanner valueTree */
	ValueTree getValueTree();

/** Called when a snapshot is fired, used to set parameters */
	void snapshotFired();

private:

	Slider panningSlider;
	Label panningLabel;
	ToggleButton enablePanButton;
	TextButton resetPanButton;

	float panAmount = 0.5;
	bool enablePanState = false;
	double halfPi = double_Pi * 0.5;

	ValueTree autoPannerValueTree;

};
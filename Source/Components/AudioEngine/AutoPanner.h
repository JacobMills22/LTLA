#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPanelObject.h"

class AutoPanner : public AudioSource,
				   public AudioPanelObject,
				   public Slider::Listener,
				   public Timer,
	               public Button::Listener
{ 

public:

/** Constructor: Initialisation. */
	AutoPanner();

/** Initialises the  audio source, Not nessesarily needed since this class just processes existing data. */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)override;

/** Processes the audio data (Resampling Audio Source)*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

/** Releases the audio sources resources. Not nessesarily needed since this class just processes existing data. */
	void releaseResources() override;

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

/** Sets the pan amount. */
	void setPanAmount(float value);
	
	void process(AudioSampleBuffer &buffer);

	ValueTree getValueTree()
	{
		return autoPannerValueTree;
	}

	void snapshotFired()
	{
		bool enableAutoPanState = autoPannerValueTree.getPropertyAsValue("EnableAutoPan", nullptr).getValue();

		enablePanButton.setToggleState(enableAutoPanState, dontSendNotification);
		if (enableAutoPanState == false)
		{
			setPanAmount(0.5);
			panningSlider.setValue(0.5);
		}
	}

private:

	Slider panningSlider;
	Label panningLabel;
	ToggleButton enablePanButton;
	TextButton resetPanButton;

	float panAmount = 0.5;
	bool enablePanState = false;

	ValueTree autoPannerValueTree;

};
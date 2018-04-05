#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../AudioEngine/FilePlayer/FilePlayer.h"
#include "AutoPanner\AutoPanner.h"
#include "PerformerInput\PerformerInput.h"
#include "Filtering&EQ\AutoFilter.h"
#include "Filtering&EQ\AutoEQ.h"
#include "Delay&Reverb\AutoReverb.h"
#include "Utilities\SimpleFade.h"

/** A new LTLAAudioPanel class is created or removed every time a 
	stage area is added or removed. This class contains one instance
	of all Audio modules and is used to process incomming aduio. */

class LTLAAudioPanel : public AudioSource,
					   public Component,
					   public Button::Listener,
					   public ComboBox::Listener,
	                   public Slider::Listener
{

public:

/** Constructor, Initialises GUI elements*/
	LTLAAudioPanel();

/** Standard JUCE Function: Prepares all DSP modules for audio playback */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

/** Standard JUCE Function: Releases resources for all DSP modules*/
	void releaseResources() override;

/**	Standard JUCE Function: Processes each DSP module*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

	void setChannelsToProcess(int leftChannel, int rightChannel)
	{
		leftChannelValue = leftChannel;
		leftChannelValue = rightChannel;
	}

/**	Standard JUCE Function: sets the bounds of DSP modules */
	void resized() override;

/**	Button callback, opens and closes the audio panels various DSP modules*/
	void buttonClicked(Button* button) override;

/** Called when a combobox value is changed, for example the audio input is set here */
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

	void sliderValueChanged(Slider* slider) override;
		
/** Closes all of the panels of all the DSP modules (Not the audioPanel itself) */
	void closeAllPanels();

/** Tells the audio panel whether a performer is inside its corressponding areaID */
	void setPerformerInsideAreaState(int performerNum, bool state);

/** Returns true if a performer is inside the audio panels corressponding areaID */
	bool getPerformerInsideAreaState(int performerNum);

/** Sets the audio input ID */
	void setAudioInputID(int ID);

/** Gets the audio input ID */
	int getAudioInputID();

	// VALUE TREE

/** Sets the valuetree for this class to use*/
	ValueTree getValueTree();

	// FILEPLAYER FUNCTIONS

/** Starts file player playback at the specified position in ms */
	void startFilePlayerPlayback(int playBackPosition);

/** Stops file player playback */
	void stopFilePlayerPlayback();

/** Returns the performer option ID which will cause the fileplayer to trigger playback (Can be both performers) */
	int getPerfromerToTrigger();

/** Returns true if the file player is set to retrigger audio samples when a performer re-enters the area. */
	bool getFilePlayerRetriggerState();

/** Returns the option ID which determines whether audio playback will stop or continue when a performer exits the area*/
	int getFilePlayerPerformerExitOption();

/** Returns the current playback state of the file player*/
	bool getFilePlayerPlayBackState();

	// AUTOPANNER FUNCTIONS

/** Sets the Auto-Panner Amount */
	void setAutoPannerAmount(float value);

/** Called when a snapshot is fired and is used to uodate GUI parameters */
	void snapshotFired();

	void processPerfomer(AudioSampleBuffer &buffer, int performerID)
	{
		if (getAudioInputID() == Performer1 || getAudioInputID() == Performer2)
		{
			//int performerID = getAudioInputID() - 2; // Store the performer ID (0 or 1)

			rawInputBuffer.makeCopyOf(buffer); // Store a raw copy of the buffer
																		 // Process audio.
			autoPanner.process(buffer);
			autoFilter.process(buffer);
			autoEQ.process(buffer);
			autoReverb.process(buffer);

			// If the performer has just enetered the area.
			if (performerInsideArea[performerID] == true && performerPreviouslyEnteredArea[performerID] == false)
			{
				// Initialise the crossfade, (fade in the processed buffer and fade out the raw buffer)
				processedBufferFade.initialiseFade(0.0, 1.0, fadeTimeInSamples);
				rawBufferFade.initialiseFade(1.0, 0.0, fadeTimeInSamples);

				// Initialise the short unmute fade if enabled.
				if (autoMuteState == true)
				{
					autoMuteFade.initialiseFade(0.0, 1.0, samplerate * 0.1);
				}
			}

			// If the performer has just exited the area.
			if (performerInsideArea[performerID] == false && performerPreviouslyExitedArea[performerID] == false)
			{
				// Initialise the crossfade, (fade in the raw buffer and fade out the processed buffer)
				processedBufferFade.initialiseFade(1.0, 0.0, fadeTimeInSamples);
				rawBufferFade.initialiseFade(0.0, 1.0, fadeTimeInSamples);

				// Initialise the short mute fade if enabled.
				if (autoMuteState == true)
				{
					autoMuteFade.initialiseFade(1.0, 0.0, samplerate * 0.1);
				}
			}

			float* outputL = buffer.getWritePointer(0);
			float* outputR = buffer.getWritePointer(1);

			float* rawInputL = rawInputBuffer.getWritePointer(0);
			float* rawInputR = rawInputBuffer.getWritePointer(1);

			// Set the output to the sum of both crossfaded buffers.
			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				outputL[sample] = (outputL[sample] * processedBufferFade.process()) + (rawInputL[sample] * rawBufferFade.process());
				outputR[sample] = (outputR[sample] * processedBufferFade.process()) + (rawInputR[sample] * rawBufferFade.process());
			}

			// Mute or unmute the output if enabled 
			if (autoMuteState == true)
			{
				for (int sample = 0; sample < buffer.getNumSamples(); sample++)
				{
					outputL[sample] = (outputL[sample] * autoMuteFade.process());
					outputR[sample] = (outputR[sample] * autoMuteFade.process());
				}
			}

			// Update the previously entered or exited states.
			performerInsideArea[performerID] == true ? performerPreviouslyEnteredArea[performerID] = true : performerPreviouslyEnteredArea[performerID] = false;
			performerInsideArea[performerID] == false ? performerPreviouslyExitedArea[performerID] = true : performerPreviouslyExitedArea[performerID] = false;

		}
	}

	enum { FilePlayerInput = 1, Performer1, Performer2 };

private:

	enum { buttonFilePlayerID, buttonAutoPannerID, buttonAutoFilterID, buttonAutoEQID, buttonAutoReverbID, numOfButtons };
	enum {inputLabel, fadeTimeLabel, numOfLabels};

	FilePlayer filePlayer;
	AutoPanner autoPanner;
	AutoFilter autoFilter;
	AutoEQ autoEQ;
	AutoReverb autoReverb;
	ValueTree audioPanelValueTree;

	ComboBox inputComboBox;
	Value audioInputID;
	Slider areaFadeTimeSlider;
	TextButton audioPanelButton[numOfButtons];
	ToggleButton autoMuteButton;
	bool autoMuteState = false;
	Label labels[numOfLabels];

	int currentlySelectedAreaID = 0;

	bool performerInsideArea[2];
	bool performerPreviouslyEnteredArea[2];
	bool performerPreviouslyExitedArea[2];

	AudioSampleBuffer rawInputBuffer;
	SimpleFade processedBufferFade;
	SimpleFade rawBufferFade;
	SimpleFade autoMuteFade;
	float samplerate = 48000;
	float fadeTimeInSamples;

	int leftChannelValue = 0;
	int RightChannelValue = 1;

};
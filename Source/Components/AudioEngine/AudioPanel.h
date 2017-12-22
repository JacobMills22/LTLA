#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../AudioEngine/FilePlayer.h"
#include "AutoPanner.h"

class LTLAAudioPanel : public AudioSource,
					   public Component,
					   public Button::Listener
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

/**	Standard JUCE Function: sets the bounds of DSP modules */
	void resized() override;

/**	Button callback, opens and closes the audio panels various DSP modules*/
	void buttonClicked(Button* button) override;

/** Closes all of the panels of all the DSP modules (Not the audioPanel itself) */
	void closeAllPanels();

	// FILEPLAYER FUNCTIONS

/** Starts file player playback at the specified position in ms */
	void startFilePlayerPlayback(int playBackPosition);

/** Stops file player playback */
	void stopFilePlayerPlayback();

/** Returns the performer option ID which will cause the fileplayer to trigger playback (Can be both performers) */
	int getPerfromerToTrigger();

	bool getFilePlayerRetriggerState();

/** Returns the option ID which determines whether audio playback will stop or continue when a performer exits the area*/
	int getFilePlayerPerformerExitOption();

/** Returns the current playback state of the file player*/
	bool getFilePlayerPlayBackState();

	// AUTOPANNER FUNCTIONS

/** Sets the Auto-Panner Amount */
	void setAutoPannerAmount(float value);

private:

	FilePlayer filePlayer;
	AutoPanner autoPanner;

	enum {buttonFilePlayerID, buttonAutoPannerID, numOfButtons};
	TextButton audioPanelButton[numOfButtons];

	int currentlySelectedAreaID = 0;
};
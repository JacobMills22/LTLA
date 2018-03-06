#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../AudioEngine/FilePlayer.h"
#include "AutoPanner.h"
#include "PerformerInput.h"
#include "Filtering&EQ\AutoFilter.h"
#include "Filtering&EQ\AutoEQ.h"
#include "Delay&Reverb\AutoReverb.h"


class LTLAAudioPanel : public AudioSource,
					   public Component,
					   public Button::Listener,
					   public ComboBox::Listener
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

/** Called when a combobox value is changed, for example the audio input is set here */
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	
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

	enum { FilePlayerInput = 1, Performer1, Performer2 };

private:

	FilePlayer filePlayer;
	AutoPanner autoPanner;
	AutoFilter autoFilter;
	AutoEQ autoEQ;
	AutoReverb autoReverb;
	ValueTree audioPanelValueTree;

	ComboBox inputComboBox;

	enum {buttonFilePlayerID, buttonAutoPannerID, buttonAutoFilterID, buttonAutoEQID, buttonAutoReverbID, numOfButtons};
	TextButton audioPanelButton[numOfButtons];

	int currentlySelectedAreaID = 0;
	//int audioInputID = FilePlayerInput;

	bool performerInsideArea[2];


};
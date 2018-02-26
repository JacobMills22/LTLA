#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPanelObject.h"

class FilePlayer : public AudioSource,
	public Button::Listener,
	public FilenameComponentListener,
	public Slider::Listener,
	public Timer,
	public ComboBox::Listener,
	public AudioPanelObject
{

public:
/** Constructor */
	FilePlayer();

/** Destructor */
	~FilePlayer();
	
	/** GUI Functions*/

/** Responds to button clicks such as the play button */
	void buttonClicked(Button* button) override;

/** Responds to slider changes such as the playback position */
	void sliderValueChanged(Slider* slider) override;

/** Responds to periodic calls, used to update the playback positon slider */
	void timerCallback() override;

/** Called when a combobox option gets changed. */
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

/** Called when the component is resized. */
	void resized() override;

	/** Audio Functions*/

/** Initialises the resampling audio sources parameters.*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)override;

/** Processes the audio data (Resampling Audio Source)*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

/** Releases the resampling audio sources resources. */
	void releaseResources()override;

/** Loads and stores an audio file into an audio transport source.*/
	void loadAudioFile(File& file);

/** Calls loadAudioFile when the filechooser chooses a new file.*/
	void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged) override;

	/** Getters & Setters */

/** Sets the playback state. */
	void setPlayBackState(bool state);

/** Gets the playback state. */
	bool getPlaybackState();

/** Starts the audio playback */
	void startPlayback(int playBackPosition);

/** Stops the audio playback */
	void stopPlayback();

/** Sets the Sample Rate. */
	void setSampleRate(int sampleRate);

/** Gets the Sample Rate. */
	int getSampleRate();

/** Set the playback level modifier */
	void setLevel(float level);

/** Get the playback level modifier */
	float getLevel();

/** Returns the performer option ID which will cause the fileplayer to trigger playback (Can be both performers) */
	int getPerformerWhichTriggers();

/** Returns the option ID which determines whether audio playback will stop or continue when a performer exits the area*/
	int getPlaybackOnPerformerExitOption();

/** Returns the Retrigger option state */
	bool getReTriggerState();

	void snapshotFired()
	{
		//setLevel(getLevel());

		if (filePlayerValueTree.getPropertyAsValue("AudioFilePath", nullptr).toString() != "NULL")
		{
			reloadAudioFile();
		}
	}

	ValueTree getValueTree()
	{
		return filePlayerValueTree;
	}

	void reloadAudioFile()
	{
		File file(filePlayerValueTree.getPropertyAsValue("AudioFilePath", nullptr).toString());
		loadAudioFile(file);
		fileChooser->setCurrentFile(file, true, dontSendNotification);
	}

private:

	// TRACKING BASED PARAMETERS (VALUETREE PARAMETERS)
	enum { performerWhichTriggersBoxID, performerExitBoxID, retriggerBoxID, numOfComboBoxes };
	ComboBox filePlayerComboBox[numOfComboBoxes];
	float playbackLevel = 0.0;
	int PerformerWhichTriggers = 0;
	enum { continuePlaybackID = 1, stopPlaybackID };
	int playbackOnPerformerExit = stopPlaybackID;
	bool retriggerState = false;

	// AUDIO VARIABLES/OBJECTS
	AudioFormatReaderSource* audioFormatReaderSource;
	AudioTransportSource audioTransportSource;
	ResamplingAudioSource* resamplingAudioSource;
	TimeSliceThread thread;
	float playBackPosition = 0.0;
	bool playbackState = false;
	int SampleRate = 44100;

	// GUI VARIABLES/OBJECTS
	enum { playButtonID, stopButtonID, numOfButtons };
	TextButton GUIButtons[numOfButtons];
	enum { levelSliderID, numOfSliderIDs};
	Slider fileplayerSlider[numOfSliderIDs];
	ScopedPointer<FilenameComponent> fileChooser;
	enum {levelLabelID, triggeredByLabelID, performerExitLabelID, retriggerLabelID, numOfLabels};
	Label filePlayerLabel[numOfLabels];

	// VALUE TREE
	ValueTree filePlayerValueTree;
	Value audioFilePath;

};
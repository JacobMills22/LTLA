#include "FilePlayer.h"


FilePlayer::FilePlayer() : thread("AudioFileStreamThread"), filePlayerValueTree("FilePlayerValueTree")
{
	//Audio Initialisation
	thread.startThread();
	audioFormatReaderSource = NULL;
	resamplingAudioSource = new ResamplingAudioSource(&audioTransportSource, false);

	//GUI Initialisation

	// Format Manager initialisation
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();

	// File chooser initialisation
	fileChooser = new FilenameComponent("AudioFile", File::nonexistent, true, false, false,
		formatManager.getWildcardForAllFormats(), String::empty, "Choose an audio file");

	fileChooser->addListener(this);
	addAndMakeVisible(fileChooser);

	// Button initialisation
	for (int ButtonID = 0; ButtonID < numOfButtons; ButtonID++)
	{
		addAndMakeVisible(GUIButtons[ButtonID]);
		GUIButtons[ButtonID].addListener(this);
	}
	GUIButtons[playButtonID].setButtonText("Play");
	GUIButtons[stopButtonID].setButtonText("Stop");
	
	// Slider initialisation
	for (int sliderNum = 0; sliderNum < numOfSliderIDs; sliderNum++)
	{
		addAndMakeVisible(fileplayerSlider[sliderNum]);
		fileplayerSlider[sliderNum].setRange(0.0, 1.0, 0.01);
		fileplayerSlider[sliderNum].addListener(this);
	}

	fileplayerSlider[levelSliderID].setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	fileplayerSlider[levelSliderID].setSkewFactor(1.5, false);
	fileplayerSlider[levelSliderID].setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);

	// Label Initialisation

	for (int labelNum = 0; labelNum < numOfLabels; labelNum++)
	{
		addAndMakeVisible(filePlayerLabel[labelNum]);
		filePlayerLabel[labelNum].setJustificationType(Justification::centred);
	}
	filePlayerLabel[levelLabelID].setText("Level", dontSendNotification);
	filePlayerLabel[triggeredByLabelID].setText("Triggered by", dontSendNotification);
	filePlayerLabel[performerExitLabelID].setText("Stop When Perfromer Exits", dontSendNotification);
	filePlayerLabel[retriggerLabelID].setText("Retrigger", dontSendNotification);

	// ComboBox Initialisation

	for (int boxNum = 0; boxNum < numOfComboBoxes; boxNum++)
	{
		addAndMakeVisible(filePlayerComboBox[boxNum]);
		filePlayerComboBox[boxNum].addListener(this);
	}
	filePlayerComboBox[performerWhichTriggersBoxID].addItem("Perfromer 1", 1);
	filePlayerComboBox[performerWhichTriggersBoxID].addItem("Perfromer 2", 2);
	filePlayerComboBox[performerWhichTriggersBoxID].addItem("Any Performer", 3);
	filePlayerComboBox[performerWhichTriggersBoxID].setSelectedId(1, dontSendNotification);

	filePlayerComboBox[performerExitBoxID].addItem("Continue Playback", continuePlaybackID);
	filePlayerComboBox[performerExitBoxID].addItem("Stop Playback", stopPlaybackID);
	filePlayerComboBox[performerExitBoxID].setSelectedId(stopPlaybackID, dontSendNotification);

	filePlayerComboBox[retriggerBoxID].addItem("No", 1);
	filePlayerComboBox[retriggerBoxID].addItem("Yes", 2);
	filePlayerComboBox[retriggerBoxID].setSelectedId(1, dontSendNotification);

	PerformerWhichTriggers = 0;

	filePlayerValueTree.setProperty("Level", 0.0, nullptr);
	filePlayerValueTree.setProperty("PerformerWhichTriggers", 1, nullptr);
	filePlayerValueTree.setProperty("playbackOnPerformerExit", stopPlaybackID, nullptr);
	filePlayerValueTree.setProperty("Retrigger", 1, nullptr);
	filePlayerValueTree.setProperty("AudioFilePath", "NULL", nullptr);

	fileplayerSlider[levelSliderID].getValueObject().referTo(filePlayerValueTree.getPropertyAsValue("Level", nullptr));
	filePlayerComboBox[performerWhichTriggersBoxID].getSelectedIdAsValue().referTo(filePlayerValueTree.getPropertyAsValue("PerformerWhichTriggers", nullptr));
	filePlayerComboBox[performerExitBoxID].getSelectedIdAsValue().referTo(filePlayerValueTree.getPropertyAsValue("playbackOnPerformerExit", nullptr));
	filePlayerComboBox[retriggerBoxID].getSelectedIdAsValue().referTo(filePlayerValueTree.getPropertyAsValue("Retrigger", nullptr));
	audioFilePath.referTo(filePlayerValueTree.getPropertyAsValue("AudioFilePath", nullptr));

	// Start timer
	startTimer(100);
}

FilePlayer::~FilePlayer()
{
	stopTimer();
	audioTransportSource.setSource(0);
	deleteAndZero(audioFormatReaderSource);
	thread.stopThread(100);
	delete resamplingAudioSource;
}

// GUI

void FilePlayer::buttonClicked(Button* button)
{
	if (button == &GUIButtons[playButtonID])	// If Play button was clicked,
	{											// Start/Pause audio Playback.
		if (getPlaybackState() == false)
		{
			startPlayback(playBackPosition);
		}
		else
		{
			setPlayBackState(false);
			GUIButtons[playButtonID].setButtonText("Play");
		}
	}
	else if (button == &GUIButtons[stopButtonID])	// If Stop button was clicked,
	{												// stop audio playback and reset psoition to zero.
		stopPlayback();
	}
}

void FilePlayer::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == &filePlayerComboBox[performerWhichTriggersBoxID])
	{
		PerformerWhichTriggers = filePlayerComboBox[performerWhichTriggersBoxID].getSelectedId() - 1;
		//filePlayerValueTree.setProperty("PerformerWhichTriggers", filePlayerComboBox[performerWhichTriggersBoxID].getSelectedId() - 1, nullptr);
	}
	else if (comboBoxThatHasChanged == &filePlayerComboBox[performerExitBoxID])
	{
		playbackOnPerformerExit = filePlayerComboBox[performerExitBoxID].getSelectedId();
		//filePlayerValueTree.setProperty("playbackOnPerformerExit", filePlayerComboBox[performerExitBoxID].getSelectedId(), nullptr);
	}
	else if (comboBoxThatHasChanged == &filePlayerComboBox[retriggerBoxID])
	{
		retriggerState = filePlayerComboBox[retriggerBoxID].getSelectedId() - 1;
		//filePlayerValueTree.setProperty("Retrigger", filePlayerComboBox[retriggerBoxID].getSelectedId() - 1, nullptr);

	}
}


void FilePlayer::startPlayback(int playBackPosition)
{
		audioTransportSource.setPosition(playBackPosition);
		setPlayBackState(true);
		GUIButtons[playButtonID].setButtonText("Pause");	
}

void FilePlayer::stopPlayback()
{
	setPlayBackState(false);
	playBackPosition = 0;
	audioTransportSource.setPosition(playBackPosition);
	GUIButtons[playButtonID].setButtonText("Play");
}


void FilePlayer::sliderValueChanged(Slider* slider)
{
 if (slider == &fileplayerSlider[levelSliderID])	   // If slider is the level slider.
	{
		setLevel(fileplayerSlider[levelSliderID].getValue());  // Set the level modifier to the sliders value.
	}  
}

void FilePlayer::timerCallback()
{
	if (getPlaybackState() == true) // If audio is playing
	{
		// Update the playback slider to represent the current position in the audio file.
		playBackPosition = audioTransportSource.getCurrentPosition();
	}
	// If the playback position has reached the end of the audio file.
	if (playBackPosition >= audioTransportSource.getLengthInSeconds() - 0.1) 
	{
		// Reset audio playback to be at the start of the file.
		GUIButtons[playButtonID].setButtonText("Play");
		playBackPosition = 0;
		setPlayBackState(false);
	}
}

void FilePlayer::resized()
{
	// Set bounds of components.
	fileChooser->setBounds (0, 0, getWidth(), getHeight() * 0.10);
	fileplayerSlider[levelSliderID].setBounds(getWidth() * 0.1, fileChooser->getBottom() + (getWidth() * 0.03), getWidth() * 0.15, getHeight() * 0.23);
	GUIButtons[playButtonID].setBounds(fileplayerSlider[levelSliderID].getRight() + (getWidth() * 0.05), fileChooser->getBottom() + (getHeight() * 0.08), getWidth() * 0.25, getHeight() * 0.15);
	GUIButtons[stopButtonID].setBounds(       GUIButtons[playButtonID].getRight() + (getWidth() * 0.05), fileChooser->getBottom() + (getHeight() * 0.08), getWidth() * 0.25, getHeight() * 0.15);
	filePlayerLabel[levelLabelID].setBounds(fileplayerSlider[levelSliderID].getX(), fileplayerSlider[levelSliderID].getY() * 0.75, fileplayerSlider[levelSliderID].getWidth(), fileplayerSlider[levelSliderID].getHeight() * 0.25);

	filePlayerLabel[triggeredByLabelID].setBounds(0, fileplayerSlider[levelSliderID].getBottom() + (getHeight() * 0.01), getWidth() * 0.25, getHeight() * 0.1);
	filePlayerComboBox[performerWhichTriggersBoxID].setBounds(filePlayerLabel[triggeredByLabelID].getX(), filePlayerLabel[triggeredByLabelID].getBottom(), getWidth() * 0.25, getHeight() * 0.1);

	filePlayerLabel[performerExitLabelID].setBounds(filePlayerLabel[triggeredByLabelID].getRight() + (getWidth() * 0.03), filePlayerLabel[triggeredByLabelID].getY(), getWidth() * 0.40, getHeight() * 0.1);
	filePlayerComboBox[performerExitBoxID].setBounds(filePlayerLabel[performerExitLabelID].getX(), filePlayerLabel[performerExitLabelID].getBottom(), getWidth() * 0.40, getHeight() * 0.1);

	filePlayerLabel[retriggerLabelID].setBounds(filePlayerLabel[performerExitLabelID].getRight() + (getWidth() * 0.03), filePlayerLabel[triggeredByLabelID].getY(), getWidth() * 0.20, getHeight() * 0.1);
	filePlayerComboBox[retriggerBoxID].setBounds(filePlayerLabel[retriggerLabelID].getX(), filePlayerLabel[performerExitLabelID].getBottom(), getWidth() * 0.20, getHeight() * 0.1);

}

// AUDIO

void FilePlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	resamplingAudioSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void FilePlayer::releaseResources()
{
	resamplingAudioSource->releaseResources();
}

void FilePlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	resamplingAudioSource->getNextAudioBlock(bufferToFill);

	float *outputL = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample); // Writeable Left Channel Output Array.
	float *outputR = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample); // Writeable Right Channel Output Array.

	for (int sample = 0; sample < bufferToFill.numSamples; sample++) // For each sample.
	{
		outputL[sample] *= playbackLevel * 0.5;	// Scale audio down.
		outputR[sample] *= playbackLevel * 0.5;	// Scale audio down.
	}
}

void FilePlayer::loadAudioFile(File& file)
{										// File is loading so
	setPlayBackState(false);			// Stop audio playback
	GUIButtons[playButtonID].setButtonText("Play");
	playBackPosition = 0;
	//fileplayerSlider->setValue(0.0, dontSendNotification);
	audioTransportSource.setSource(0);  // Remove the source from the audio transport source.
	deleteAndZero(audioFormatReaderSource); // Remove audioformatreadersource

	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();	// Adds basic audio formats

	AudioFormatReader* reader = formatManager.createReaderFor(file); // Creates a reader for the audio file.

	if (reader != 0)	// If the reader has a file to read
	{
		audioFormatReaderSource = new AudioFormatReaderSource(reader, true);	// Create a AudioFormatReaderSource

		// Set the audioTransportSource to be ready to play the audio file on the audio thread.
		audioTransportSource.setSource(audioFormatReaderSource, getSampleRate(), &thread, reader->sampleRate);
		//filePlayerValueTree.setProperty("AudioFilePath", file.getFullPathName(), nullptr);
		audioFilePath = file.getFullPathName();
	}
}

void FilePlayer::filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged)
{
	// If a new audio file is selected, load it.
	if (fileComponentThatHasChanged == fileChooser)
	{
		File audioFile(fileChooser->getCurrentFile().getFullPathName());

		if (audioFile.existsAsFile())
		{
			loadAudioFile(audioFile);
		}
	}
}


// GETTERS & SETTERS

void FilePlayer::setPlayBackState(bool state)
{
	if (state == true)
	{
		audioTransportSource.start();
	}
	else
	{
		audioTransportSource.stop();
	}
}

bool FilePlayer::getPlaybackState()
{
	return audioTransportSource.isPlaying();
}

void FilePlayer::setSampleRate(int sampleRate)
{
	SampleRate = sampleRate;
}

int FilePlayer::getSampleRate()
{
	return SampleRate;
}


void FilePlayer::setLevel(float level)
{
	//filePlayerValueTree.setProperty("Level", level, nullptr);
	playbackLevel = level;
	fileplayerSlider[levelSliderID].setValue(level, dontSendNotification);
}

float FilePlayer::getLevel()
{
	return playbackLevel;
}

int FilePlayer::getPerformerWhichTriggers()
{
	return PerformerWhichTriggers;
	//return filePlayerValueTree.getPropertyAsValue("PerformerWhichTriggers", nullptr).getValue();
}

int FilePlayer::getPlaybackOnPerformerExitOption()
{
	return playbackOnPerformerExit;
	//return filePlayerValueTree.getPropertyAsValue("playbackOnPerformerExit", nullptr).getValue();
}

bool FilePlayer::getReTriggerState()
{
	return retriggerState;
	//return filePlayerValueTree.getPropertyAsValue("Retrigger", nullptr).getValue();
}


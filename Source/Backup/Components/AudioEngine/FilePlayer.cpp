#include "FilePlayer.h"


FilePlayer::FilePlayer() : thread("AudioFileStreamThread")
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
			audioTransportSource.setPosition(playBackPosition);
			setPlayBackState(true);
			GUIButtons[playButtonID].setButtonText("Pause");
		}
		else
		{
			setPlayBackState(false);
			GUIButtons[playButtonID].setButtonText("Play");
		}
	}
	else if (button == &GUIButtons[stopButtonID])	// If Stop button was clicked,
	{												// stop audio playback and reset psoition to zero.
		setPlayBackState(false);
		playBackPosition = 0;
		audioTransportSource.setPosition(playBackPosition);
		GUIButtons[playButtonID].setButtonText("Play");
	}
}

void FilePlayer::openPanel()
{
	setVisible(true);
}

void FilePlayer::closePanel()
{
	setVisible(false);
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
		fileplayerSlider->setValue(0.0, dontSendNotification);
	}
}

void FilePlayer::resized()
{
	// Set bounds of components.
	fileChooser->setBounds (0, 0, getWidth(), getHeight() * 0.15);
	fileplayerSlider[levelSliderID].setBounds(0, fileChooser->getBottom(), getWidth(), getHeight() * 0.33);
	GUIButtons[playButtonID].setBounds(5, fileplayerSlider[levelSliderID].getBottom(), getWidth() * 0.4, getHeight() * 0.2);
	GUIButtons[stopButtonID].setBounds(GUIButtons[playButtonID].getRight() + 5, fileplayerSlider[levelSliderID].getBottom(), getWidth() * 0.4, getHeight() * 0.2);
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
		outputL[sample] *= getLevel() * 0.5;	// Scale audio down.
		outputR[sample] *= getLevel() * 0.5;	// Scale audio down.
	}
}

void FilePlayer::loadAudioFile(File& file)
{										// File is loading so
	setPlayBackState(false);			// Stop audio playback
	GUIButtons[playButtonID].setButtonText("Play");
	playBackPosition = 0;
	fileplayerSlider->setValue(0.0, dontSendNotification);
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
	playbackLevel = level;
}

float FilePlayer::getLevel()
{
	return playbackLevel;
}

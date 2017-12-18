#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../AudioEngine/FilePlayer.h"
#include "AutoPanner.h"

class LTLAAudioPanel : public AudioSource,
					   public Component,
					   public Button::Listener
{

public:

	LTLAAudioPanel()
	{
		addAndMakeVisible(filePlayer);
		addAndMakeVisible(autoPanner);

		for (int buttonNum = 0; buttonNum < numOfButtons; buttonNum++)
		{
			addAndMakeVisible(audioPanelButton[buttonNum]);
			audioPanelButton[buttonNum].addListener(this);
			audioPanelButton[buttonNum].setToggleState(false, dontSendNotification);
		}
		audioPanelButton[buttonFilePlayerID].setButtonText("File Player");
		audioPanelButton[buttonAutoPannerID].setButtonText("AutoPanner");

		filePlayer.closePanel();
		autoPanner.closePanel();
	}

	~LTLAAudioPanel()
	{
		
	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)  override
	{
	    filePlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
		autoPanner.prepareToPlay(samplesPerBlockExpected, sampleRate);
	}

	void releaseResources() override
	{
		filePlayer.releaseResources();
		autoPanner.releaseResources();
	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		filePlayer.getNextAudioBlock(bufferToFill);
		autoPanner.getNextAudioBlock(bufferToFill);
	}

	void resized() override
	{
		audioPanelButton[buttonFilePlayerID].setBounds(5, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
		audioPanelButton[buttonAutoPannerID].setBounds(audioPanelButton[buttonFilePlayerID].getRight() + getWidth() * 0.01, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
		filePlayer.setBounds(getBounds().getCentreX() * 0.55, audioPanelButton[buttonFilePlayerID].getBottom(), getWidth() * 0.5, getHeight() * 0.8);
		autoPanner.setBounds(filePlayer.getBounds());
	}

	void buttonClicked(Button* button) override
	{
		closeAllPanels();

		if (button == &audioPanelButton[buttonFilePlayerID])
		{
			audioPanelButton[buttonFilePlayerID].setToggleState(!audioPanelButton[buttonFilePlayerID].getToggleState(), dontSendNotification);
			audioPanelButton[buttonFilePlayerID].getToggleState() == false ? filePlayer.closePanel() : filePlayer.openPanel();

		}
		else if (button == &audioPanelButton[buttonAutoPannerID])
		{
			audioPanelButton[buttonAutoPannerID].setToggleState(!audioPanelButton[buttonAutoPannerID].getToggleState(), dontSendNotification);
			audioPanelButton[buttonAutoPannerID].getToggleState() == false ? autoPanner.closePanel() : autoPanner.openPanel();
		}
	}

	// FILEPLAYER FUNCTIONS

	void startFilePlayerPlayback(int playBackPosition)
	{
		filePlayer.startPlayback(playBackPosition);
	}

	void stopFilePlayerPlayback()
	{
		filePlayer.stopPlayback();
	}

	int getPerfromerToTrigger()
	{
		if (&filePlayer != nullptr)
		{
			return filePlayer.getPerformerWhichTriggers();
		}
		else { return 0; }
	}

	int getFilePlayerPerformerExitOption()
	{
		return filePlayer.getPlaybackOnPerformerExitOption();
	}

	bool getFilePlayerPlayBackState()
	{
		return filePlayer.getPlaybackState();
	}

	bool getFilePlayerRetriggerState()
	{
		return filePlayer.getReTriggerState();
	}

	// AUTOPANNER FUNCTIONS

	void setAutoPannerAmount(float value)
	{
		autoPanner.setPanAmount(value);
	}

	void closeAllPanels()
	{
		for (int buttomNum = 0; buttomNum < numOfButtons; buttomNum++)
		{
			audioPanelButton[buttomNum].setToggleState(false, dontSendNotification);
		}

		filePlayer.closePanel();
		autoPanner.closePanel();
	}

private:

	FilePlayer filePlayer;
	AutoPanner autoPanner;

	enum {buttonFilePlayerID, buttonAutoPannerID, numOfButtons};
	TextButton audioPanelButton[numOfButtons];

	int currentlySelectedAreaID = 0;
};
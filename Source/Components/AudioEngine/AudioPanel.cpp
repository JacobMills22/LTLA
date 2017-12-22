
#include "AudioPanel.h"

	LTLAAudioPanel::LTLAAudioPanel()
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

	void LTLAAudioPanel::prepareToPlay(int samplesPerBlockExpected, double sampleRate)  
	{
		filePlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
		autoPanner.prepareToPlay(samplesPerBlockExpected, sampleRate);
	}

	void LTLAAudioPanel::releaseResources()
	{
		filePlayer.releaseResources();
		autoPanner.releaseResources();
	}

	void LTLAAudioPanel::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
	{
		filePlayer.getNextAudioBlock(bufferToFill);
		autoPanner.getNextAudioBlock(bufferToFill);
	}

	void LTLAAudioPanel::resized()
	{
		audioPanelButton[buttonFilePlayerID].setBounds(5, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
		audioPanelButton[buttonAutoPannerID].setBounds(audioPanelButton[buttonFilePlayerID].getRight() + getWidth() * 0.01, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
		filePlayer.setBounds(getBounds().getCentreX() * 0.55, audioPanelButton[buttonFilePlayerID].getBottom(), getWidth() * 0.5, getHeight() * 0.8);
		autoPanner.setBounds(filePlayer.getBounds());
	}

	void LTLAAudioPanel::buttonClicked(Button* button)
	{
		closeAllPanels(); // Close all the Panels before opening new ones.

		if (button == &audioPanelButton[buttonFilePlayerID]) // Open file player panel if selected.
		{
			audioPanelButton[buttonFilePlayerID].setToggleState(!audioPanelButton[buttonFilePlayerID].getToggleState(), dontSendNotification);
			audioPanelButton[buttonFilePlayerID].getToggleState() == false ? filePlayer.closePanel() : filePlayer.openPanel();

		}
		else if (button == &audioPanelButton[buttonAutoPannerID]) // Open Auto-panner panel if selected.
		{
			audioPanelButton[buttonAutoPannerID].setToggleState(!audioPanelButton[buttonAutoPannerID].getToggleState(), dontSendNotification);
			audioPanelButton[buttonAutoPannerID].getToggleState() == false ? autoPanner.closePanel() : autoPanner.openPanel();
		}
	}

	void LTLAAudioPanel::closeAllPanels()
	{
		for (int buttomNum = 0; buttomNum < numOfButtons; buttomNum++)
		{
			audioPanelButton[buttomNum].setToggleState(false, dontSendNotification);
		}

		filePlayer.closePanel();
		autoPanner.closePanel();
	}

	// FILEPLAYER FUNCTIONS

	void LTLAAudioPanel::startFilePlayerPlayback(int playBackPosition)
	{
		filePlayer.startPlayback(playBackPosition);
	}

	void LTLAAudioPanel::stopFilePlayerPlayback()
	{
		filePlayer.stopPlayback();
	}

	int LTLAAudioPanel::getPerfromerToTrigger()
	{
	//	if (&filePlayer != nullptr) // Guard to prevent this accessing unallocated memory
		{
			return filePlayer.getPerformerWhichTriggers();
		}
	//	else { return 0; }
	}


	bool LTLAAudioPanel::getFilePlayerRetriggerState()
	{
		return filePlayer.getReTriggerState();
	}

	int LTLAAudioPanel::getFilePlayerPerformerExitOption()
	{
		return filePlayer.getPlaybackOnPerformerExitOption();
	}

	bool LTLAAudioPanel::getFilePlayerPlayBackState()
	{
		return filePlayer.getPlaybackState();
	}

	// AUTOPANNER FUNCTIONS

	void LTLAAudioPanel::setAutoPannerAmount(float value)
	{
		autoPanner.setPanAmount(value);
	}


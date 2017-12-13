#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../AudioEngine/FilePlayer.h"

class LTLAAudioPanel : public AudioSource,
					   public Component,
					   public Button::Listener
{

public:

	LTLAAudioPanel()
	{
		addAndMakeVisible(filePlayer);

		for (int buttonNum = 0; buttonNum < numOfButtons; buttonNum++)
		{
			addAndMakeVisible(audioPanelButton[buttonNum]);
			audioPanelButton[buttonNum].addListener(this);
			audioPanelButton[buttonNum].setToggleState(false, dontSendNotification);
		}
		audioPanelButton[ButtonFilePlayerID].setButtonText("File Player");

		filePlayer.closePanel();
	}

	~LTLAAudioPanel()
	{
		
	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)  override
	{
		
	    filePlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
		
	}

	void releaseResources() override
	{
		filePlayer.releaseResources();
	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		filePlayer.getNextAudioBlock(bufferToFill);
	}

	void resized() override
	{
		audioPanelButton[ButtonFilePlayerID].setBounds(5, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
		filePlayer.setBounds(getBounds().getCentreX() * 0.55, audioPanelButton[ButtonFilePlayerID].getBottom(), getWidth() * 0.5, getHeight() * 0.8);
	}

	void buttonClicked(Button* button) override
	{
		if (button == &audioPanelButton[ButtonFilePlayerID])
		{
			audioPanelButton[ButtonFilePlayerID].setToggleState(!audioPanelButton[ButtonFilePlayerID].getToggleState(), dontSendNotification);
			
			if (audioPanelButton[ButtonFilePlayerID].getToggleState() == false)
			{
				filePlayer.closePanel();
			}
			else
			{
				filePlayer.openPanel();
			}

		}
	}

	void paint(Graphics& g) override
	{
		//g.fillAll(Colours::pink);
	}


private:

	FilePlayer filePlayer;
	enum {ButtonFilePlayerID, numOfButtons};
	TextButton audioPanelButton[numOfButtons];

	int currentlySelectedAreaID = 0;
};

#include "AudioPanel.h"

	LTLAAudioPanel::LTLAAudioPanel() : audioPanelValueTree("audioPanelValueTree")
	{
		performerInsideArea[0] = false;
		performerInsideArea[1] = false;
		performerPreviouslyExitedArea[0] = false;
		performerPreviouslyExitedArea[1] = false;
				
		addAndMakeVisible(filePlayer);
		addAndMakeVisible(autoPanner);
		addAndMakeVisible(autoFilter);
		addAndMakeVisible(autoEQ);
		addAndMakeVisible(autoReverb);

		for (int buttonNum = 0; buttonNum < numOfButtons; buttonNum++)
		{
			addAndMakeVisible(audioPanelButton[buttonNum]);
			audioPanelButton[buttonNum].addListener(this);
			audioPanelButton[buttonNum].setToggleState(false, dontSendNotification);
		}
		audioPanelButton[buttonFilePlayerID].setButtonText("File Player");
		audioPanelButton[buttonAutoPannerID].setButtonText("AutoPanner");
		audioPanelButton[buttonAutoFilterID].setButtonText("Filters");
		audioPanelButton[buttonAutoEQID].setButtonText("EQ");
		audioPanelButton[buttonAutoReverbID].setButtonText("Reverb");

		addAndMakeVisible(autoMuteButton);
		autoMuteButton.addListener(this);
		autoMuteButton.setButtonText("Auto Mute");

		addAndMakeVisible(inputComboBox);
		inputComboBox.addItem("File Player", 1);
		inputComboBox.addItem("Performer 1", 2);
		inputComboBox.addItem("Performer 2", 3);
		inputComboBox.addListener(this);
		inputComboBox.setSelectedId(1, dontSendNotification);

		addAndMakeVisible(areaFadeTimeSlider);
		areaFadeTimeSlider.addListener(this);
		areaFadeTimeSlider.setRange(0.0, 5, 0.1);
		areaFadeTimeSlider.setSliderStyle(Slider::RotaryVerticalDrag);
		areaFadeTimeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
		areaFadeTimeSlider.setValue(0.3, sendNotification);

		for (int labelID = 0; labelID < numOfLabels; labelID++)
		{
			addAndMakeVisible(labels[labelID]);
			labels[labelID].setJustificationType(Justification::centred);
		}

		labels[inputLabel].setText("Area Input", dontSendNotification);
		labels[fadeTimeLabel].setText("Fade Time", dontSendNotification);

		filePlayer.closePanel();
		autoPanner.closePanel();
		autoFilter.closePanel();
		autoEQ.closePanel();
		autoReverb.closePanel();


		audioPanelValueTree.setProperty("InputSource", FilePlayerInput, nullptr);

		audioPanelValueTree.addChild(filePlayer.getValueTree(), 0, nullptr);
		audioPanelValueTree.addChild(autoPanner.getValueTree(), 0, nullptr);
		audioPanelValueTree.addChild(autoFilter.getValueTree(), 0, nullptr);

	}

	void LTLAAudioPanel::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		samplerate = sampleRate;

		rawInputBuffer.setSize(2, samplesPerBlockExpected, false, false, false);

		filePlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
		autoPanner.prepareToPlay(samplesPerBlockExpected, sampleRate);

		//autoFilter.initialise(sampleRate);
		autoFilter.setSampleRate(sampleRate);
		autoEQ.prepareToPlay(samplesPerBlockExpected, sampleRate);
		autoReverb.prepareToPlay(samplesPerBlockExpected, sampleRate);
	}

	void LTLAAudioPanel::releaseResources()
	{
		filePlayer.releaseResources();
		autoPanner.releaseResources();

	}

	void LTLAAudioPanel::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
	{
		if (getAudioInputID() == Performer1 || getAudioInputID() == Performer2)
		{
			int performerID = getAudioInputID() - 2;

			rawInputBuffer.makeCopyOf(*bufferToFill.buffer);

			autoPanner.process(*bufferToFill.buffer);
			autoFilter.process(*bufferToFill.buffer);
			autoEQ.process(*bufferToFill.buffer);
			autoReverb.process(*bufferToFill.buffer);

			if (performerInsideArea[performerID] == true && performerPreviouslyEnteredArea[performerID] == false)
			{
				processedBufferFade.initialiseFade(0.0, 1.0, fadeTimeInSamples);
				rawBufferFade.initialiseFade(1.0, 0.0, fadeTimeInSamples);

				if (autoMuteState == true)
				{
					autoMuteFade.initialiseFade(0.0, 1.0, samplerate * 0.1);
				}
			}

			if (performerInsideArea[performerID] == false && performerPreviouslyExitedArea[performerID] == false)
			{
				processedBufferFade.initialiseFade(1.0, 0.0, fadeTimeInSamples);
				rawBufferFade.initialiseFade(0.0, 1.0, fadeTimeInSamples);

				if (autoMuteState == true)
				{
					autoMuteFade.initialiseFade(1.0, 0.0, samplerate * 0.1);
				}
			}

			float* outputL = bufferToFill.buffer->getWritePointer(0);
			float* outputR = bufferToFill.buffer->getWritePointer(1);

			float* rawInputL = rawInputBuffer.getWritePointer(0);
			float* rawInputR = rawInputBuffer.getWritePointer(1);

			for (int sample = 0; sample < bufferToFill.buffer->getNumSamples(); sample++)
			{
				outputL[sample] = (outputL[sample] * processedBufferFade.process()) + (rawInputL[sample] * rawBufferFade.process());
				outputR[sample] = (outputR[sample] * processedBufferFade.process()) + (rawInputR[sample] * rawBufferFade.process());
			}

			if (autoMuteState == true)
			{
				for (int sample = 0; sample < bufferToFill.buffer->getNumSamples(); sample++)
				{
					outputL[sample] = (outputL[sample] * autoMuteFade.process());
					outputR[sample] = (outputR[sample] * autoMuteFade.process());
				}
			}

				performerInsideArea[performerID] == true ? performerPreviouslyEnteredArea[performerID] = true : performerPreviouslyEnteredArea[performerID] = false;
				performerInsideArea[performerID] == false ? performerPreviouslyExitedArea[performerID] = true : performerPreviouslyExitedArea[performerID] = false;

		}
		else if (getAudioInputID() == FilePlayerInput)
		{
			filePlayer.getNextAudioBlock(bufferToFill);
			autoFilter.process(*bufferToFill.buffer);
			autoEQ.process(*bufferToFill.buffer);
			autoReverb.process(*bufferToFill.buffer);
		}
	}

	void LTLAAudioPanel::resized()
	{
		audioPanelButton[buttonFilePlayerID].setBounds(getWidth() * 0.16, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
		audioPanelButton[buttonAutoPannerID].setBounds(audioPanelButton[buttonFilePlayerID].getRight() + getWidth() * 0.01, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
		audioPanelButton[buttonAutoFilterID].setBounds(audioPanelButton[buttonAutoPannerID].getRight() + getWidth() * 0.01, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
		audioPanelButton[buttonAutoEQID].setBounds(audioPanelButton[buttonAutoFilterID].getRight() + getWidth() * 0.01, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
		audioPanelButton[buttonAutoReverbID].setBounds(audioPanelButton[buttonAutoEQID].getRight() + getWidth() * 0.01, getHeight() * 0.01, getWidth() * 0.1, getHeight() * 0.05);
				
		inputComboBox.setBounds(5, getHeight() * 0.08, getWidth() * 0.15, getHeight() * 0.1);
		areaFadeTimeSlider.setBounds(5, getHeight() * 0.25, getWidth() * 0.15, getHeight() * 0.3);
		autoMuteButton.setBounds(5 + (inputComboBox.getWidth() * 0.5) - (getWidth() * 0.1 * 0.5), getHeight() * 0.5, getWidth() * 0.1, getHeight() * 0.3);

		filePlayer.setBounds(getWidth() * 0.25, audioPanelButton[buttonFilePlayerID].getBottom() + (getHeight() * 0.1), getWidth() * 0.6, getHeight() * 0.8);
		autoPanner.setBounds(filePlayer.getBounds());
		autoFilter.setBounds(filePlayer.getBounds());
		autoEQ.setBounds(filePlayer.getBounds());
		autoReverb.setBounds(filePlayer.getBounds());

		labels[inputLabel].setBounds(5, 0, getWidth() * 0.15, 30);
		labels[fadeTimeLabel].setBounds(5, getHeight() * 0.19, getWidth() * 0.15, 30);


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
		else if (button == &audioPanelButton[buttonAutoFilterID]) // Open Auto-Filter panel if selected.
		{
			audioPanelButton[buttonAutoFilterID].setToggleState(!audioPanelButton[buttonAutoFilterID].getToggleState(), dontSendNotification);
			audioPanelButton[buttonAutoFilterID].getToggleState() == false ? autoFilter.closePanel() : autoFilter.openPanel();
		}
		else if (button == &audioPanelButton[buttonAutoEQID]) // Open Auto-EQ panel if selected.
		{
			audioPanelButton[buttonAutoEQID].setToggleState(!audioPanelButton[buttonAutoEQID].getToggleState(), dontSendNotification);
			audioPanelButton[buttonAutoEQID].getToggleState() == false ? autoEQ.closePanel() : autoEQ.openPanel();
		}
		else if (button == &audioPanelButton[buttonAutoReverbID]) // Open Auto-Reverb panel if selected.
		{
			audioPanelButton[buttonAutoReverbID].setToggleState(!audioPanelButton[buttonAutoReverbID].getToggleState(), dontSendNotification);
			audioPanelButton[buttonAutoReverbID].getToggleState() == false ? autoReverb.closePanel() : autoReverb.openPanel();
		}
		else if (button == &autoMuteButton)
		{
			autoMuteState = autoMuteButton.getToggleState();
			//autoMuteState == true ? autoMuteFade.initialiseFade(1.0, 0.0, (samplerate * 0.1)) : autoMuteFade.initialiseFade(0.0, 1.0, (samplerate * 0.1));
		}
	}

	void LTLAAudioPanel::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
	{
		if (comboBoxThatHasChanged == &inputComboBox)
		{
			setAudioInputID(inputComboBox.getSelectedId());
		}
	}

	void LTLAAudioPanel::sliderValueChanged(Slider* slider)
	{
		if (slider == &areaFadeTimeSlider)
		{
			fadeTimeInSamples = samplerate * areaFadeTimeSlider.getValue();
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
		autoFilter.closePanel();
		autoEQ.closePanel();
		autoReverb.closePanel();
	}

	void LTLAAudioPanel::setPerformerInsideAreaState(int performerNum, bool state)
	{
		performerInsideArea[performerNum] = state;
	}

	bool LTLAAudioPanel::getPerformerInsideAreaState(int performerNum)
	{
		return performerInsideArea[performerNum];
	}

	void LTLAAudioPanel::setAudioInputID(int ID)
	{
		audioPanelValueTree.setProperty("InputSource", ID, nullptr);
	}

	int LTLAAudioPanel::getAudioInputID()
	{
		return audioPanelValueTree.getPropertyAsValue("InputSource", nullptr).getValue();
	}

	ValueTree LTLAAudioPanel::getValueTree()
	{
		return audioPanelValueTree;
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

	// SNAPSHOT FUNCTIONS

	void LTLAAudioPanel::snapshotFired()
	{
		inputComboBox.setSelectedId(audioPanelValueTree.getPropertyAsValue("InputSource", nullptr).getValue(), dontSendNotification);
		filePlayer.snapshotFired();
		autoPanner.snapshotFired();
		autoFilter.snapshotFired();
	}


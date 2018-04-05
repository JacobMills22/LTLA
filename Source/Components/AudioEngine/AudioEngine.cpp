
#include "AudioEngine.h"

	LTLAAudioEngine::~LTLAAudioEngine()
	{
		audioPanel.clear(true);
		areaData.clear(true);
	}

	void LTLAAudioEngine::initialiseEngine(int SamplesPerBlock, double sampleRate)
	{
		samplesPerBlock = SamplesPerBlock;
		samplerate = sampleRate;
		
		setMeterData(0, 0.0);
		setMeterData(1, 0.0);

		startTimer(15);
	}

	void LTLAAudioEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
	{
		// Initalise buffers
		rawInputBuffer.setSize(2, samplesPerBlockExpected, false, false, false);
		processedPerformerBuffer[0].setSize(2, samplesPerBlockExpected, false, false, false);
		processedPerformerBuffer[1].setSize(2, samplesPerBlockExpected, false, false, false);

		rawPerformerBuffer[0].setSize(2, samplesPerBlockExpected, false, false, false);
		rawPerformerBuffer[1].setSize(2, samplesPerBlockExpected, false, false, false);
		
		filePlayerBuffer.setSize(2, samplesPerBlockExpected, false, false, false);

		if (audioPanel.size() > 0)
		{
			audioPanel.getLast()->prepareToPlay(samplesPerBlockExpected, sampleRate);
			mixerAudioSource.addInputSource(audioPanel.getLast(), false);
		}
		else
		{
			mixerAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
		}

		performerInput[0].prepareToPlay(samplesPerBlockExpected, sampleRate);
		performerInput[1].prepareToPlay(samplesPerBlockExpected, sampleRate);

	}

	void LTLAAudioEngine::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) 
	{
		// Store a raw copy of the input buffer
		rawInputBuffer.makeCopyOf(*bufferToFill.buffer, true);
		rawPerformerBuffer[0].makeCopyOf(*bufferToFill.buffer, true);
		rawPerformerBuffer[1].makeCopyOf(*bufferToFill.buffer, true);

		// Set the Input to the copy of the input buffer and process
	//	performerInput.setInputBuffer(rawInputBuffer);
	//	performerInput.getNextAudioBlock(bufferToFill);

		performerInput[0].process(rawPerformerBuffer[0]);
		performerInput[1].process(rawPerformerBuffer[1]);


		// Since fileplayers use a resamplingaudiosource and doesn't just process data
		// all fileplayers need to be added to a mixeraudiosource and removed if 
		// the audiopanel doesn't have its input set to fileplayer.

		// For each audiopanel which has its input set to performer 1 or 2
		for (int panel = 0; panel < audioPanel.size(); panel++)
		{
			if (audioPanel[panel]->getAudioInputID() == 2)
			{
				// Remove the fileplayer audiosource from the mixer and then process
				mixerAudioSource.removeInputSource(audioPanel[panel]);
			//	audioPanel[panel]->getNextAudioBlock(bufferToFill);
				audioPanel[panel]->processPerfomer(rawPerformerBuffer[0], 0);
			}
		}
		// Copy the processed audio into a buffer.
		processedPerformerBuffer[0].makeCopyOf(rawPerformerBuffer[0], false);
		bufferToFill.clearActiveBufferRegion();


		for (int panel = 0; panel < audioPanel.size(); panel++)
		{
			if (audioPanel[panel]->getAudioInputID() == 3)
			{
				// Remove the fileplayer audiosource from the mixer and then process
				mixerAudioSource.removeInputSource(audioPanel[panel]);
			//	audioPanel[panel]->getNextAudioBlock(bufferToFill);
				audioPanel[panel]->processPerfomer(rawPerformerBuffer[1], 1);

			}
		}

		processedPerformerBuffer[1].makeCopyOf(rawPerformerBuffer[1], false);
		bufferToFill.clearActiveBufferRegion();
		
		// For each audiopanel which has its input set to Fileplayer
		for (int panel = 0; panel < audioPanel.size(); panel++)
		{
			if (audioPanel[panel]->getAudioInputID() == 1)
			{
				// add the audio panel with the fileplayer to the mixer.
				mixerAudioSource.addInputSource(audioPanel[panel], false);
			}
		}
		// process all fileplayers and make a copy of the buffer.
		mixerAudioSource.getNextAudioBlock(bufferToFill);
		filePlayerBuffer.makeCopyOf(*bufferToFill.buffer, false);

		if (usingTwoMics == false)
		{
			rawPerformerBuffer[1].clear();
		}
		
		float* OutputL = bufferToFill.buffer->getWritePointer(0);
		float* OutputR = bufferToFill.buffer->getWritePointer(1);

		float* ProcessedPerformer1L = rawPerformerBuffer[0].getWritePointer(0);
		float* ProcessedPerformer1R = rawPerformerBuffer[0].getWritePointer(1);

		float* ProcessedPerformer2L = rawPerformerBuffer[1].getWritePointer(0);
		float* ProcessedPerformer2R = rawPerformerBuffer[1].getWritePointer(1);

		float* filePlayerBufferL = filePlayerBuffer.getWritePointer(0);
		float* filePlayerBufferR = filePlayerBuffer.getWritePointer(1);
		
		for (int sample = 0; sample < bufferToFill.numSamples; sample++)
		{
			// Set the output to be the processed performer audio and the fileplayer audio.
			OutputL[sample] = ProcessedPerformer1L[sample] + ProcessedPerformer2L[sample] + filePlayerBufferL[sample] * 0.5;
			OutputR[sample] = ProcessedPerformer1R[sample] + ProcessedPerformer2R[sample] + filePlayerBufferR[sample] * 0.5;
		}
		
		for (int sample = 0; sample < bufferToFill.numSamples; sample++)
		{
			// Set meter data.
			if (OutputL[sample] <= 1.0 && OutputL[sample] >= 0.0)
			setMeterData(0, OutputL[sample]);

			if (OutputR[sample] <= 1.0 && OutputR[sample] >= 0.0)
			setMeterData(1, OutputR[sample]);
		}
	}

	void LTLAAudioEngine::releaseResources() 
	{
		for (int panel = 0; panel < audioPanel.size(); panel++)
		{
			audioPanel[panel]->releaseResources();
		}
		stopTimer();
	}

	void LTLAAudioEngine::addNewStageAreaAudioPanel()
	{
		audioPanel.add(new LTLAAudioPanel); // Add additional AudioPanel, contains all audio processes
		areaData.add(new AreaDataStruct);   // Add new areaData set, contains tracking related data from GUI
		audioPanel.getLast()->prepareToPlay(samplesPerBlock, samplerate); // Initialisation.
		prepareToPlay(samplesPerBlock, samplerate);
		addAndMakeVisible(audioPanel.getLast());
		audioPanel.getLast()->setVisible(true);
		areaData.getLast()->performerExitedAreaState[0] = false;
		areaData.getLast()->performerExitedAreaState[1] = false;
		areaData.getLast()->performerEnteredAreaState[0] = false;
		areaData.getLast()->performerEnteredAreaState[1] = false;
	}

	void LTLAAudioEngine::reopenAudioPanel(int selectedArea)
	{
		for (int stageAreaAudioID = 0; stageAreaAudioID < audioPanel.size(); stageAreaAudioID++)
		{
			audioPanel[stageAreaAudioID]->setVisible(false); // Set all to be invisible
		}
		audioPanel[selectedArea]->setVisible(true); // Set currently selected to be visible
		audioPanel[selectedArea]->toFront(false);   // and bring it to the front of the screen.
		resized();
	}

	void LTLAAudioEngine::setStateOfAllPanels(bool state)
	{
		for (int stageAreaAudioID = 0; stageAreaAudioID < audioPanel.size(); stageAreaAudioID++)
		{
			audioPanel[stageAreaAudioID]->setVisible(state);
		}
	}

	void LTLAAudioEngine::resized()
	{
		for (int stageAreaAudioID = 0; stageAreaAudioID < audioPanel.size(); stageAreaAudioID++)
		{
			audioPanel[stageAreaAudioID]->setBounds(0, 0, getBounds().getWidth(), getBounds().getHeight());
		}
	}

	void LTLAAudioEngine::setAreaIDContainingPerformerState(int perfromerID, int areaID, bool state)
	{
		areaData[areaID]->areaContainsPerformer[perfromerID] = state;
		audioPanel[areaID]->setPerformerInsideAreaState(perfromerID, state);
	}

	void LTLAAudioEngine::setPerformerEnteredAreaState(int perfromerID, bool state, int areaID)
	{
		if (areaData.size() > 0) // Guard to prevent this being called on unallocated memory.
		{
			areaData[areaID]->performerEnteredAreaState[perfromerID] = state;
		}
	}

	bool LTLAAudioEngine::getPerformerEnteredAreaState(int perfromerID, int areaID)
	{
		if (areaData.size() > 0) // Guard to prevent this being called on unallocated memory.
		{
			return areaData[areaID]->performerEnteredAreaState[perfromerID];
		}
	}

	void LTLAAudioEngine::setPerformerExitedAreaState(int perfromerID, bool state, int areaID)
	{
		if (areaData.size() > 0) // Guard to prevent this being called on unallocated memory.
		{
			areaData[areaID]->performerExitedAreaState[perfromerID] = state;
		}
	}

	bool LTLAAudioEngine::getPerformerExitedAreaState(int perfromerID, int areaID)
	{
		if (areaData.size() > 0) // Guard to prevent this being called on unallocated memory.
		{
			return areaData[areaID]->performerExitedAreaState[perfromerID];
		}
	}

	void LTLAAudioEngine::timerCallback()
	{
		if (areaData.size() > 0) // Guard to prevent this being called on unallocated memory.
		{
			for (int performer = 0; performer < 2; performer++) // for each performer
			{
				for (int area = 0; area < areaData.size(); area++) // for each area that has been added
				{
					// call performerEnteredArea if the enter state is true and they are still in the area
					// and if either retrigger mode is turned on or the fileplayer has finished previous playback.
					if (areaData[area]->performerEnteredAreaState[performer] == true && areaData[area]->areaContainsPerformer[performer] == true)
					{						
						if (audioPanel[area]->getFilePlayerRetriggerState() == true || audioPanel[area]->getFilePlayerPlayBackState() == false)
						{
							performerEnteredArea(performer, area);
						}
					}
					else if (areaData[area]->performerExitedAreaState[performer] == true && areaData[area]->areaContainsPerformer[performer] == false)
					{
						performerExitedArea(performer, area); // call performerExitedArea if the exit state is true and they aren't in the area
					}

					if (areaData[area]->areaContainsPerformer[performer] == true)
					{
						audioPanel[area]->setPerformerInsideAreaState(performer, true);
					}
				}
			}
		}
	}

	void LTLAAudioEngine::performerEnteredArea(int performer, int areaID)
	{
		if (audioPanel[areaID]->getFilePlayerRetriggerState() == true)
		{

		}

		// Trigger audio playback on the relevent audioPanel if the "triggered by" option is either set to both
		// performers or if it corresponds to the performer which is allowed to trigger the playback.
		if (audioPanel[areaID]->getPerfromerToTrigger() == triggeredByBothPerformers)
		{
			audioPanel[areaID]->startFilePlayerPlayback(0);
		}
		else if (audioPanel[areaID]->getPerfromerToTrigger() == performer)
		{
			audioPanel[areaID]->startFilePlayerPlayback(0);
		}
	}

	void LTLAAudioEngine::performerExitedArea(int performer, int areaID)
	{
		// If the "stop when performer exits" parameter is enabled, stop audio playback on the relevent audioPanel
		// if the "triggered by" option is either set to both  performers or if it corresponds to the performer 
		// which is allowed to trigger the playback.
		if (audioPanel[areaID]->getFilePlayerPerformerExitOption() == stopPlaybackID)
		{
			if (audioPanel[areaID]->getPerfromerToTrigger() == triggeredByBothPerformers)
			{
				if (areaData[areaID]->performerExitedAreaState[0] == true && areaData[areaID]->performerExitedAreaState[1] == true)
				{
					audioPanel[areaID]->stopFilePlayerPlayback();
				}
			}
			else if (audioPanel[areaID]->getPerfromerToTrigger() == performer)
			{
				audioPanel[areaID]->stopFilePlayerPlayback();
			}
		}
	}

	void LTLAAudioEngine::snapshotFired()
	{
		for (int panel = 0; panel < audioPanel.size(); panel++)
		{
			audioPanel[panel]->snapshotFired();
		}
	}

	void LTLAAudioEngine::setAutoPannerAmount(float value, int areaID, int performerID)
	{
		// Performer ID is either a 0 or a 1, getAudioInputID refers to the fileplayer (1), performer 1 (2) and performer 2 (3).
		if (performerID == (audioPanel[areaID]->getAudioInputID() - 2))
		{
			audioPanel[areaID]->setAutoPannerAmount(value);
		}
	}

	void LTLAAudioEngine::setMeterData(int channel, float value)
	{
		meterData[channel] = value;
	}

	float LTLAAudioEngine::getMeterData(int channel)
	{
		return meterData[channel];
	}

	void LTLAAudioEngine::setPerformerParameters(int ActualInputChannel, int MaxOutputChannels)
	{
	//	performerInput.initialise(ActualInputChannel, MaxOutputChannels);
	}

	void LTLAAudioEngine::setDeviceManagerToUse(AudioDeviceManager *deviceManager)
	{
		performerInput[0].setDeviceManagerToUse(deviceManager);
		performerInput[1].setDeviceManagerToUse(deviceManager);
		
		AudioIODevice* device = deviceManager->getCurrentAudioDevice();
	
		if (messageBoxHasBeenDisplayed == false)
		{
			usingTwoMics = messageBox.showYesNoBox(AlertWindow::QuestionIcon, "How many Inputs?", "Are you using two microphones?", nullptr, nullptr);
			messageBoxHasBeenDisplayed = true;
		}

		if (usingTwoMics == true)
		{
			performerInput[0].initialise(0, 2);
			performerInput[1].initialise(1, 2);
		}
		else
		{
			performerInput[0].initialise(0, 2);
		}

		/*
		const BigInteger activeInputChannels = device->getActiveInputChannels();
		const BigInteger activeOutputChannels = device->getActiveOutputChannels();
		const int maxInputChannels = activeInputChannels.getHighestBit() + 1;
		const int maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

		if (maxInputChannels == 1 || maxInputChannels == 2)
		{
			performerInput[0].initialise(0, 2);
			DBG("INITIALISING MIC 1");
		}
		else if (maxInputChannels >= 3)
		{
			performerInput[1].initialise(1, 2);
			DBG("INITIALISING MIC 2");

		}
		*/
	}

	

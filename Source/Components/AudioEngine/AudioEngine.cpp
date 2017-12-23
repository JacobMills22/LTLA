
#include "AudioEngine.h"

	void LTLAAudioEngine::initialiseEngine(int SamplesPerBlock, double sampleRate)
	{
		samplesPerBlock = SamplesPerBlock;
		samplerate = sampleRate;

		setMeterData(0, 0.0);
		setMeterData(1, 0.0);

		startTimer(30);
	}

	void LTLAAudioEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
	{
		mixerAudioSource.addInputSource(audioPanel.getLast(), true);
		mixerAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	}

	void LTLAAudioEngine::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) 
	{
		mixerAudioSource.getNextAudioBlock(bufferToFill);

		float* OutputL = bufferToFill.buffer->getWritePointer(0);
		float* OutputR = bufferToFill.buffer->getWritePointer(1);


		for (int sample = 0; sample < bufferToFill.numSamples; sample++)
		{
			if (OutputL[sample] <= 1.0 && OutputL[sample] >= 0.0)
				setMeterData(0, OutputL[sample]);

			if (OutputR[sample] <= 1.0 && OutputR[sample] >= 0.0)
				setMeterData(1, OutputR[sample]);
			
		}
	}

	void LTLAAudioEngine::releaseResources() 
	{
		mixerAudioSource.releaseResources();
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

	void LTLAAudioEngine::setAutoPannerAmount(float value, int areaID)
	{
		audioPanel[areaID]->setAutoPannerAmount(value);
	}

	void LTLAAudioEngine::setMeterData(int channel, float value)
	{
		meterData[channel] = value;
	}

	float LTLAAudioEngine::getMeterData(int channel)
	{
		return meterData[channel];
	}

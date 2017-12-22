#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPanel.h"

class LTLAAudioEngine : public AudioSource,
					    public Component,
					    public Timer
{

public:

	void initialiseEngine( int SamplesPerBlock, double sampleRate)
	{
		samplesPerBlock = SamplesPerBlock;
		samplerate = sampleRate;

		startTimer(30);
	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		mixerAudioSource.addInputSource(audioPanel.getLast(), true);
		mixerAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		mixerAudioSource.getNextAudioBlock(bufferToFill);
	}

	void releaseResources() override
	{
		mixerAudioSource.releaseResources();
		stopTimer();
	}

	void addNewStageAreaAudioPanel()
	{
		audioPanel.add(new LTLAAudioPanel);
		areaData.add(new AreaDataStruct);
		audioPanel.getLast()->prepareToPlay(samplesPerBlock, samplerate);
		prepareToPlay(samplesPerBlock, samplerate);
		addAndMakeVisible(audioPanel.getLast());
		audioPanel.getLast()->setVisible(true);
		areaData.getLast()->performerExitedAreaState[0] = false;
		areaData.getLast()->performerExitedAreaState[1] = false;
		areaData.getLast()->performerEnteredAreaState[0] = false;
		areaData.getLast()->performerEnteredAreaState[1] = false;

	}

	void reopenAudioPanel(int selectedArea)
	{
		for (int stageAreaAudioID = 0; stageAreaAudioID < audioPanel.size(); stageAreaAudioID++)
		{
			audioPanel[stageAreaAudioID]->setVisible(false);
		}
		audioPanel[selectedArea]->setVisible(true);
		audioPanel[selectedArea]->toFront(false);
		resized();
	}

	void setStateOfAllPanels(bool state)
	{
		for (int stageAreaAudioID = 0; stageAreaAudioID < audioPanel.size(); stageAreaAudioID++)
		{
			audioPanel[stageAreaAudioID]->setVisible(state);
		}
	}

	void resized() override
	{
		for (int stageAreaAudioID = 0; stageAreaAudioID < audioPanel.size(); stageAreaAudioID++)
		{
			audioPanel[stageAreaAudioID]->setBounds(0, 0, getBounds().getWidth(), getBounds().getHeight());
		}
	}

	void setAreaIDContainingPerformerState(int perfromerID, int areaID, bool state)
	{
		areaData[areaID]->areaContainsPerformer[perfromerID] = state;
	}

	void setPerformerEnteredAreaState(int perfromerID, bool state, int areaID)
	{
		if (areaData.size() > 0)
		{
			areaData[areaID]->performerEnteredAreaState[perfromerID] = state;
		}
	}

	bool getPerformerEnteredAreaState(int perfromerID, int areaID)
	{
		if (areaData.size() > 0)
		{
			return areaData[areaID]->performerEnteredAreaState[perfromerID];
		}
	}

	void setPerformerExitedAreaState(int perfromerID, bool state, int areaID)
	{
		if (areaData.size() > 0)
		{
			areaData[areaID]->performerExitedAreaState[perfromerID] = state;
		}
	}

	bool getPerformerExitedAreaState(int perfromerID, int areaID)
	{
		if (areaData.size() > 0)
		{
			return areaData[areaID]->performerExitedAreaState[perfromerID];
		}
	}

	void timerCallback() override
	{
		if (areaData.size() > 0)
		{
			for (int performer = 0; performer < 2; performer++)
			{
				for (int area = 0; area < areaData.size(); area++)
				{
					if (areaData[area]->performerEnteredAreaState[performer] == true && areaData[area]->areaContainsPerformer[performer] == true)
					{
						performerEnteredArea(performer, area);
					}
					else if (areaData[area]->performerExitedAreaState[performer] == true && areaData[area]->areaContainsPerformer[performer] == false)
					{
						performerExitedArea(performer, area);
					}
				}
			}
		}
	}

	void performerEnteredArea(int performer, int areaID)
	{
		if (audioPanel[areaID]->getFilePlayerRetriggerState() == true)
		{
			if (audioPanel[areaID]->getPerfromerToTrigger() == 2) // 2 = Both performer 1 and 2
			{
				audioPanel[areaID]->startFilePlayerPlayback(0);
			}
			else if (audioPanel[areaID]->getPerfromerToTrigger() == performer)
			{
				audioPanel[areaID]->startFilePlayerPlayback(0);
			}
		}
		else if (audioPanel[areaID]->getFilePlayerPlayBackState() == false)
		{
			if (audioPanel[areaID]->getPerfromerToTrigger() == 2) // 2 = Both performer 1 and 2
			{
				audioPanel[areaID]->startFilePlayerPlayback(0);
			}
			else if (audioPanel[areaID]->getPerfromerToTrigger() == performer)
			{
				audioPanel[areaID]->startFilePlayerPlayback(0);
			}
		}
	}

	void performerExitedArea(int performer, int areaID)
	{
		if (audioPanel[areaID]->getFilePlayerPerformerExitOption() == stopPlaybackID)
		{
			if (audioPanel[areaID]->getPerfromerToTrigger() == 2) // 2 = Both performer 1 and 2
			{
				audioPanel[areaID]->stopFilePlayerPlayback();
			}
			else if (audioPanel[areaID]->getPerfromerToTrigger() == performer)
			{
				audioPanel[areaID]->stopFilePlayerPlayback();
			}
		}
	}

	void setAutoPannerAmount(float value, int areaID)
	{
		audioPanel[areaID]->setAutoPannerAmount(value);
	}

private:

	struct AreaDataStruct
	{
		int areaID = 0;
		bool performerEnteredAreaState[2];
		bool performerExitedAreaState[2];
		bool areaContainsPerformer[2];
	};

	MixerAudioSource mixerAudioSource;
	OwnedArray<LTLAAudioPanel> audioPanel;
	OwnedArray<AreaDataStruct> areaData;
	int samplesPerBlock = 0;
	double samplerate = 0;

	float autoPannerAmount = 0;

	enum { continuePlaybackID = 1, stopPlaybackID };


};
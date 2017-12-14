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

		performerExitedAreaState[0] = false;
		performerExitedAreaState[1] = false;

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
		audioPanel.getLast()->prepareToPlay(samplesPerBlock, samplerate);
		prepareToPlay(samplesPerBlock, samplerate);
		addAndMakeVisible(audioPanel.getLast());
		audioPanel.getLast()->setVisible(true);
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

	void setAreaIDWhichContainsPerformer(int perfromerID, int AreaID)
	{
		areaIDWithPerformer[perfromerID] = AreaID;
	}

	void setPerformerEnteredAreaState(int perfromerID, bool state)
	{
		performerEnteredAreaState[perfromerID] = state;
	}

	bool getPerformerEnteredAreaState(int perfromerID)
	{
		return performerEnteredAreaState[perfromerID];
	}

	void setPerformerExitedAreaState(int perfromerID, bool state)
	{
		performerExitedAreaState[perfromerID] = state;
	}

	bool getPerformerExitedAreaState(int perfromerID)
	{
		return performerExitedAreaState[perfromerID];
	}

	void timerCallback() override
	{
		for (int performer = 0; performer < 2; performer++)
		{
			if (performerEnteredAreaState[performer] == true)
			{
				performerEnteredArea(performer);
			}
			else if (performerExitedAreaState[performer] == true)
			{
				performerExitedArea(performer);
			}
		}
	}

	void performerEnteredArea(int performer)
	{
		if (audioPanel[areaIDWithPerformer[performer]]->getFilePlayerRetriggerState() == true)
		{
			DBG("TRUE");
			if (audioPanel[areaIDWithPerformer[performer]]->getPerfromerToTrigger() == 2) // 2 = Both performer 1 and 2
			{
				audioPanel[areaIDWithPerformer[performer]]->startFilePlayerPlayback(0);
			}
			else if (audioPanel[areaIDWithPerformer[performer]]->getPerfromerToTrigger() == performer)
			{
				audioPanel[areaIDWithPerformer[performer]]->startFilePlayerPlayback(0);
			}
		}
		else if (audioPanel[areaIDWithPerformer[performer]]->getFilePlayerPlayBackState() == false)
		{
			DBG("FALSE");

			if (audioPanel[areaIDWithPerformer[performer]]->getPerfromerToTrigger() == 2) // 2 = Both performer 1 and 2
			{
				audioPanel[areaIDWithPerformer[performer]]->startFilePlayerPlayback(0);
			}
			else if (audioPanel[areaIDWithPerformer[performer]]->getPerfromerToTrigger() == performer)
			{
				audioPanel[areaIDWithPerformer[performer]]->startFilePlayerPlayback(0);
			}
		}
	}

	void performerExitedArea(int performer)
	{
		if (audioPanel[areaIDWithPerformer[performer]]->getFilePlayerPerformerExitOption() == stopPlaybackID)
		{
			if (audioPanel[areaIDWithPerformer[performer]]->getPerfromerToTrigger() == 2) // 2 = Both performer 1 and 2
			{
				audioPanel[areaIDWithPerformer[performer]]->stopFilePlayerPlayback();
			}
			else if (audioPanel[areaIDWithPerformer[performer]]->getPerfromerToTrigger() == performer)
			{
				audioPanel[areaIDWithPerformer[performer]]->stopFilePlayerPlayback();
			}
		}
	}


private:

	MixerAudioSource mixerAudioSource;
	OwnedArray<LTLAAudioPanel> audioPanel;
	int samplesPerBlock = 0;
	double samplerate = 0;

	bool performerEnteredAreaState[2];
	bool performerExitedAreaState[2];

	int areaIDWithPerformer[2];

	enum { continuePlaybackID = 1, stopPlaybackID };


};
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
		mixerAudioSource.addInputSource(stageAreaAudioPanel.getLast(), true);
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
		stageAreaAudioPanel.add(new LTLAAudioPanel);
		stageAreaAudioPanel.getLast()->prepareToPlay(samplesPerBlock, samplerate);
		prepareToPlay(samplesPerBlock, samplerate);
		addAndMakeVisible(stageAreaAudioPanel.getLast());
		stageAreaAudioPanel.getLast()->setVisible(true);
	}

	void reopenAudioPanel(int selectedArea)
	{
		for (int stageAreaAudioID = 0; stageAreaAudioID < stageAreaAudioPanel.size(); stageAreaAudioID++)
		{
			stageAreaAudioPanel[stageAreaAudioID]->setVisible(false);
		}
		stageAreaAudioPanel[selectedArea]->setVisible(true);
		stageAreaAudioPanel[selectedArea]->toFront(false);
		resized();
	}

	void setStateOfAllPanels(bool state)
	{
		for (int stageAreaAudioID = 0; stageAreaAudioID < stageAreaAudioPanel.size(); stageAreaAudioID++)
		{
			stageAreaAudioPanel[stageAreaAudioID]->setVisible(state);
		}
	}

	void resized() override
	{
		for (int stageAreaAudioID = 0; stageAreaAudioID < stageAreaAudioPanel.size(); stageAreaAudioID++)
		{
			stageAreaAudioPanel[stageAreaAudioID]->setBounds(0, 0, getBounds().getWidth(), getBounds().getHeight());
		}
	}

	void setAreaIDWhichContainsPerformer(int perfromerID, int AreaID)
	{
		areaIDWhichContainsPerformer[perfromerID] = AreaID;
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
		for (int perfromerNum = 0; perfromerNum < 2; perfromerNum++)
		{
			if (performerEnteredAreaState[perfromerNum] == true)
			{
				if (stageAreaAudioPanel[areaIDWhichContainsPerformer[perfromerNum]]->getPerfromerToTrigger() == 2) // 2 = Both performer 1 and 2
				{
					stageAreaAudioPanel[areaIDWhichContainsPerformer[perfromerNum]]->startFilePlayerPlayback(0);
				}
				else if (stageAreaAudioPanel[areaIDWhichContainsPerformer[perfromerNum]]->getPerfromerToTrigger() == perfromerNum)
				{
					stageAreaAudioPanel[areaIDWhichContainsPerformer[perfromerNum]]->startFilePlayerPlayback(0);
				}
			}
		}

	}


private:

	MixerAudioSource mixerAudioSource;
	OwnedArray<LTLAAudioPanel> stageAreaAudioPanel;
	int samplesPerBlock = 0;
	double samplerate = 0;

	bool performerEnteredAreaState[2];
	bool performerExitedAreaState[2];

	int areaIDWhichContainsPerformer[2];

};
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPanel.h"
#include "PerformerInput.h"

class LTLAAudioEngine : public AudioSource,
					    public Component,
					    public Timer
{

public:

	~LTLAAudioEngine();

/** Initialises Audio Engine */
	void initialiseEngine(int SamplesPerBlock, double sampleRate);

/** Adds inputs from each AudioPanel to the AudioMixerSource and prepares for playback */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

/** Processes the AudioMixerSource */
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

/** Released the resources of the AudioMixerSource */
	void releaseResources() override;

/** Called whenever the user adds a new stage area. 
	Adds and initialises a new AudioPanel containing all audio processes.
	Adds and initialises a new set of Areadata which contains tracking data provided by TrackingGUI/MainComponent */
	void addNewStageAreaAudioPanel();

/** Makes all AudioPanels invisible and then makes the currently selected AudioPanel visible */
	void reopenAudioPanel(int selectedArea);

/** Sets the visibillity (true/false) of all AudioPanels, typically called when the EditAreaParameters menu is closed*/
	void setStateOfAllPanels(bool state);

/** Standard JUCE resized function, used to set the bounds of all objects */
	void resized() override;

/** AreaData: Sets whether a performer is stood inside a specific area */
	void setAreaIDContainingPerformerState(int perfromerID, int areaID, bool state);

/** AreaData: Updates the entered area state of a specific performer and area */
	void setPerformerEnteredAreaState(int perfromerID, bool state, int areaID);

/** AreaData: Gets the entered area state of a specific performer and area */
	bool getPerformerEnteredAreaState(int perfromerID, int areaID);

/** AreaData: Updates the exited area state of a specific performer and area */
	void setPerformerExitedAreaState(int perfromerID, bool state, int areaID);

/** AreaData: Gets the exited area state of a specific performer and area */
	bool getPerformerExitedAreaState(int perfromerID, int areaID);

/** Standard JUCE Timer callback.
	Loops through each performer and area and updates various states to control audio playback*/
	void timerCallback() override;

/** Called in timercallback when a performer enters an area, starts audio playback if required*/
	void performerEnteredArea(int performer, int areaID);

/** Called in timercallback when a performer exits an area, stops audio playback if required*/
	void performerExitedArea(int performer, int areaID);

/** Updates the AutoPanning amount of an area */
	void setAutoPannerAmount(float value, int areaID);

	void setMeterData(int channel, float value);

	float getMeterData(int channel);

	void setPerformerParameters(int ActualInputChannel, int MaxOutputChannels);

	void setDeviceManagerToUse(AudioDeviceManager *deviceManager);
	

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
	float meterData[2];

	PerformerInput performerInput;
	AudioSampleBuffer rawInputBuffer, processedPerformerBuffer, filePlayerBuffer;

	enum { continuePlaybackID = 1, stopPlaybackID, triggeredByBothPerformers = 2 };

};
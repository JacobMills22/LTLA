#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/AudioEngine/AudioPanelObject.h"
#include "DelayUnit.h"
#include "../Filtering&EQ/FilterProcesses.h"

/** This file includes all nessesary classes used 
	in the AutoReverb, the Reverb has been implemented using 
    the design from the Audio Processes Book by David Creasey 
	Available from https://www.dawsonera.com/abstract/9781315657813 */

/** This class takes four buffers and applies a matrix 
	multiplication on each buffer causing a sophisticated
	difference in amplitude in each buffer */

class FeedBackMatrixMultiplier
{

public:

	/** Initialises each matrix elements multiplication value. */
	void initialise();

	/** Processes all four buffers using the matrix. */
	void process(AudioSampleBuffer &buffer0, AudioSampleBuffer &buffer1, AudioSampleBuffer &buffer2, AudioSampleBuffer &buffer3);

private:

	float matrix[4][4];

};

/** This class generates a fairly complex series of
	early reflections which will be used within reverb. */

class EarlyReflectionsGenerator
{

public:
	
	/** Constructor, intialises delay times and gain values.*/
	EarlyReflectionsGenerator();

	/** Prepares for audio playback by intialising filters and buffers*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);

	/** Processs the early reflections */
	void process(AudioSampleBuffer &buffer);

	/** Sets the cutoff of both filters*/
	void setFilterCutoff(float lowpassCutoff, float highpassCutoff);

private: 

	float samplerate = 48000;
	LowpassIIRFilter lowpassFilter;
	HighpassIIRFilter highpassFilter;

	enum { numOfReflections = 2};
	float delayTime[numOfReflections];
	float gain[numOfReflections];

	AudioSampleBuffer delayedBuffer[numOfReflections];
	DelayUnit delayUnit[numOfReflections];

};

/** This class generates later reflections and is fed
	audio from the early reflections generator, multiple
	instances of this class can be used to create more dense
	and complex sounds.*/

class LaterReflectionsGenerator
{

public:

	/** Prepares for audio playback by intialising filters, buffers and more. */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);

	/** Initialises the delay paths, filters and gain using the provided arguments. */
	void initialise(float delayTime1, float delayTime2, float delayTime3, float delayTime4, float delayModifier, float cutoff, float audioGain);

	/** Processes the later reflections, this should be fed a buffer of early reflections. */
	void process(AudioSampleBuffer &buffer);

	/** Sets the feedback modifier (decimal scale). */
	void setFeedbackModifier(float amount);

private:

	enum { numOfDelays = 4 };
	AudioSampleBuffer delayPathBuffer[numOfDelays];
	AudioSampleBuffer feedbackMatrixBuffer[numOfDelays];

	FeedBackMatrixMultiplier feedBackMatrixMultiplier;
	DelayUnit delayPath[numOfDelays];
	LowpassIIRFilter lowpassFilter[numOfDelays];
	float gain = 0.25;

	float samplerate = 48000;
	float feedbackModifier = 1.0;
};

/** This class represents the whole AutoReverb and brings
	all of the above classes together. */

class AutoReverb : public AudioPanelObject,
	public Slider::Listener,
	public Button::Listener,
	public Label::Listener
{

public:

	/** Constructor */
	AutoReverb();

	/** Called when a snapshot is fired, used to update values stored in the recalled snapshot.*/
	void snapshotFired();
	
	/** Prepares all audio components for playback */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);

	/** Processes the audio, applying the reverb effect. */
	void process(AudioSampleBuffer &buffer);

	/** Called when a slider is changed, used to set various parameters*/
	void sliderValueChanged(Slider* slider) override;

	/** Called when a button is clicked, used to set toggle the bypass state */
	void buttonClicked(Button* button) override;

	/** Called when a labels text is changed */
	void labelTextChanged(Label* labelThatHasChanged) override;


	/** Sets the bounds of all components */
	void resized() override;

	/** Returns the AutoReverbs valuetree */
	ValueTree getValueTree();

private:

	enum Stages { direct, earlyReflections, lateReflections1, lateReflections2, numOfStages };
	enum SliderParameters {preDelay, delayModifierID, feedbackAmount, numOfSliderParams};
	enum ParameterLabels { filterLabel, gainLabel, directGain, earlyReflectionsLabel, lateReflections1Label, lateReflections2Label, 
		   preDelayLabel, feedbackLabel, roomSizeLabel, numOfLabels };
	
	Slider gainSliders[numOfStages];
	Slider filterCutoffSlider; // Two Value Verticle
	Slider sliderParam[numOfSliderParams];

	ToggleButton bypassButton;
	float gain[numOfStages];
	
	AudioSampleBuffer stageBuffer[numOfStages];
	EarlyReflectionsGenerator earlyReflectionsGenerator;
	LaterReflectionsGenerator laterReflectionsGenerator[2];
	DelayUnit preDelayUnit;

	Label labels[numOfLabels];

	float delayModifier = 10.0;
	bool bypassState = false;

	ValueTree autoReverbValueTree;

};
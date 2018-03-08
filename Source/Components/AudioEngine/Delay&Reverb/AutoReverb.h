#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/AudioEngine/AudioPanelObject.h"
#include "DelayUnit.h"
#include "../Filtering&EQ/FilterProcesses.h"

class FeedBackMatrixMultiplier
{

public:

	void initialise()
	{
		matrix[0][0] = 0.0;
		matrix[0][1] = 1.0;
		matrix[0][2] = 1.0;
		matrix[0][3] = -1.0;

		matrix[1][0] = -1.0;
		matrix[1][1] = 0.0;
		matrix[1][2] = -1.0;
		matrix[1][3] = 1.0;

		matrix[2][0] = -1.0;
		matrix[2][1] = 1.0;
		matrix[2][2] = 0.0;
		matrix[2][3] = -1.0;

		matrix[3][0] = 1.0;
		matrix[3][1] = -1.0;
		matrix[3][2] = 1.0;
		matrix[3][3] = 0.0;
	}

	void process(AudioSampleBuffer &buffer0, AudioSampleBuffer &buffer1, AudioSampleBuffer &buffer2, AudioSampleBuffer &buffer3)
	{
		float* output0L = buffer0.getWritePointer(0);
		float* output0R = buffer0.getWritePointer(1);

		float* output1L = buffer1.getWritePointer(0);
		float* output1R = buffer1.getWritePointer(1);

		float* output2L = buffer2.getWritePointer(0);
		float* output2R = buffer2.getWritePointer(1);

		float* output3L = buffer3.getWritePointer(0);
		float* output3R = buffer3.getWritePointer(1);
		
		const float* input0L = buffer0.getReadPointer(0);
		const float* input0R = buffer0.getReadPointer(1);

		const float* input1L = buffer1.getReadPointer(0);
		const float* input1R = buffer1.getReadPointer(1);

		const float* input2L = buffer2.getReadPointer(0);
		const float* input2R = buffer2.getReadPointer(1);

		const float* input3L = buffer3.getReadPointer(0);
		const float* input3R = buffer3.getReadPointer(1);

		for (int sample = 0; sample < buffer0.getNumSamples(); sample++)
		{
			output0L[sample] = (matrix[0][0] * input0L[sample]) + (matrix[0][1] * input1L[sample] + (matrix[0][2] * input2L[sample]) + (matrix[0][3] * input3L[sample]));
			output0R[sample] = (matrix[0][0] * input0R[sample]) + (matrix[0][1] * input1R[sample] + (matrix[0][2] * input2R[sample]) + (matrix[0][3] * input3R[sample]));

			output1L[sample] = (matrix[1][0] * input0L[sample]) + (matrix[1][1] * input1L[sample] + (matrix[1][2] * input2L[sample]) + (matrix[1][3] * input3L[sample]));
			output1R[sample] = (matrix[1][0] * input0R[sample]) + (matrix[1][1] * input1R[sample] + (matrix[1][2] * input2R[sample]) + (matrix[1][3] * input3R[sample]));

			output2L[sample] = (matrix[2][0] * input0L[sample]) + (matrix[2][1] * input1L[sample] + (matrix[2][2] * input2L[sample]) + (matrix[2][3] * input3L[sample]));
			output2R[sample] = (matrix[2][0] * input0R[sample]) + (matrix[2][1] * input1R[sample] + (matrix[2][2] * input2R[sample]) + (matrix[2][3] * input3R[sample]));

			output3L[sample] = (matrix[3][0] * input0L[sample]) + (matrix[3][1] * input1L[sample] + (matrix[3][2] * input2L[sample]) + (matrix[3][3] * input3L[sample]));
			output3R[sample] = (matrix[3][0] * input0R[sample]) + (matrix[3][1] * input1R[sample] + (matrix[3][2] * input2R[sample]) + (matrix[3][3] * input3R[sample]));
		}
	}

private:

	float matrix[4][4];

};


class EarlyReflectionsGenerator
{

public:
	
	EarlyReflectionsGenerator()
	{
		delayUnit[0].setDelayTimeInMS(2.0);
		delayUnit[1].setDelayTimeInMS(5.15);
		delayUnit[2].setDelayTimeInMS(5.94);
		delayUnit[3].setDelayTimeInMS(7.54);
		delayUnit[4].setDelayTimeInMS(8.878);
		delayUnit[5].setDelayTimeInMS(10.422);
		delayUnit[6].setDelayTimeInMS(13.9);
		delayUnit[7].setDelayTimeInMS(17.1);
		
		gain[0] = 0.609;
		gain[1] = 0.262;
		gain[2] = 0.3;
		gain[3] = 0.47;
		gain[4] = 0.29;
		gain[5] = 0.042;
		gain[6] = 0.1;
		gain[7] = 0.2;

	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		samplerate = sampleRate;

		for (int reflection = 0; reflection < numOfReflections; reflection++)
		{
			delayedBuffer[reflection].setSize(2, samplesPerBlockExpected, false, false, false);
			delayUnit[reflection].prepareToPlay(samplesPerBlockExpected, sampleRate);
		}

		highpassFilter.setCutoff(500, samplerate, 0.72);

		lowpassFilter.setCutoff(15000, samplerate, 0.72);
	}

	void process(AudioSampleBuffer &buffer)
	{
		highpassFilter.process(buffer);

		lowpassFilter.process(buffer);

		for (int reflection = 0; reflection < numOfReflections; reflection++)
		{
			delayedBuffer[reflection].makeCopyOf(buffer, true);
			delayUnit[reflection].process(delayedBuffer[reflection]);
		
			float* outputL = buffer.getWritePointer(0);
			float* outputR = buffer.getWritePointer(1);

			float* delayedOutputL = delayedBuffer[reflection].getWritePointer(0);
			float* delayedOutputR = delayedBuffer[reflection].getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				outputL[sample] += (delayedOutputL[sample] * gain[reflection]);
				outputR[sample] += (delayedOutputR[sample] * gain[reflection]);
			}
		
		} 

	}

	void setFilterCutoff(float lowpassCutoff, float highpassCutoff)
	{
		highpassFilter.setCutoff(highpassCutoff, samplerate, 0.72);

		lowpassFilter.setCutoff(lowpassCutoff, samplerate, 0.72);

		DBG("HighPass C = " + (String)highpassCutoff + " LowPass C = " + (String)lowpassCutoff);
	}

private: 

	float samplerate = 48000;
	LowpassIIRFilter lowpassFilter;
	HighpassIIRFilter highpassFilter;

	enum { numOfReflections = 8};
	float delayTime[numOfReflections];
	float gain[numOfReflections];

	AudioSampleBuffer delayedBuffer[numOfReflections];
	DelayUnit delayUnit[numOfReflections];

};

class LaterReflectionsGenerator
{

public:

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			delayPath[pathNum].prepareToPlay(samplesPerBlockExpected, sampleRate);
			delayPathBuffer[pathNum].setSize(2, samplesPerBlockExpected, false, false, false);
			feedbackMatrixBuffer[pathNum].setSize(2, samplesPerBlockExpected, false, false, false);
		}

		feedBackMatrixMultiplier.initialise();
		samplerate = sampleRate;
	}

	void initialise(float delayTime1, float delayTime2, float delayTime3, float delayTime4, float cutoff, float audioGain)
	{
		delayPath[0].setDelayTimeInMS(delayTime1);
		delayPath[1].setDelayTimeInMS(delayTime2);
		delayPath[2].setDelayTimeInMS(delayTime3);
		delayPath[3].setDelayTimeInMS(delayTime4);

		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			lowpassFilter[pathNum].setCutoff(cutoff, samplerate, 0.71);
		}

		gain = audioGain;
	}

	void process(AudioSampleBuffer &buffer)
	{		
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			delayPathBuffer[pathNum].makeCopyOf(buffer, true);
			feedbackMatrixBuffer[pathNum].makeCopyOf(buffer, true);

			float* DelayPathOutputL = delayPathBuffer[pathNum].getWritePointer(0);
			float* DelayPathOutputR = delayPathBuffer[pathNum].getWritePointer(1);

			float* feedbackOutputL = feedbackMatrixBuffer[pathNum].getWritePointer(0);
			float* feedbackOutputR = feedbackMatrixBuffer[pathNum].getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				DelayPathOutputL[sample] += feedbackOutputL[sample];
				DelayPathOutputR[sample] += feedbackOutputR[sample];
			}
		
			//=======================================================
		
			delayPath[pathNum].process(delayPathBuffer[pathNum]);
			lowpassFilter[pathNum].process(delayPathBuffer[pathNum]);

			DelayPathOutputL = delayPathBuffer[pathNum].getWritePointer(0);
			DelayPathOutputR = delayPathBuffer[pathNum].getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				DelayPathOutputL[sample] *= gain;
				DelayPathOutputR[sample] *= gain;
			}
			//=======================================================
		}
		
		
		float* OutputL = buffer.getWritePointer(0);
		float* OutputR = buffer.getWritePointer(1);

		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			float* DelayPathOutputL = delayPathBuffer[pathNum].getWritePointer(0);
			float* DelayPathOutputR = delayPathBuffer[pathNum].getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				OutputL[sample] += DelayPathOutputL[sample];
				OutputR[sample] += DelayPathOutputR[sample];
			}
		}
		
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			feedbackMatrixBuffer[pathNum].makeCopyOf(delayPathBuffer[pathNum]);
		}
		
		feedBackMatrixMultiplier.process(feedbackMatrixBuffer[0], feedbackMatrixBuffer[1], feedbackMatrixBuffer[2], feedbackMatrixBuffer[3]);
		
	}

private:

	enum { numOfDelays = 4 };
	AudioSampleBuffer delayPathBuffer[numOfDelays];
	AudioSampleBuffer feedbackMatrixBuffer[numOfDelays];

	FeedBackMatrixMultiplier feedBackMatrixMultiplier;
	DelayUnit delayPath[numOfDelays];
	LowpassIIRFilter lowpassFilter[numOfDelays];
	float gain = 0.25;

	bool test = false;

	float samplerate = 48000;

};

class AutoReverb : public AudioPanelObject,
	public Slider::Listener,
	public Button::Listener
{

public:

	AutoReverb()
	{
		for (int stage = 0; stage < numOfStages; stage++)
		{
			addAndMakeVisible(gainSliders[stage]);
			gainSliders[stage].addListener(this);
			gainSliders[stage].setRange(0.0, 1.0, 0.01);

			gain[stage] = 0.0;
		}

		addAndMakeVisible(filterCutoffSlider);
		filterCutoffSlider.addListener(this);
		filterCutoffSlider.setSliderStyle(Slider::TwoValueVertical);
		filterCutoffSlider.setRange(20, 20000, 1);
		filterCutoffSlider.setSkewFactor(0.23);
		filterCutoffSlider.setMinAndMaxValues(500, 15000, dontSendNotification);

		addAndMakeVisible(bypassButton);
		bypassButton.addListener(this);
		bypassButton.setButtonText("Bypass");
	}

	void paint(Graphics &g) override
	{
		//g.fillAll(Colours::pink);
	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		earlyReflectionsGenerator.prepareToPlay(samplesPerBlockExpected, sampleRate);

		laterReflectionsGenerator[0].prepareToPlay(samplesPerBlockExpected, sampleRate);
		laterReflectionsGenerator[1].prepareToPlay(samplesPerBlockExpected, sampleRate);

		laterReflectionsGenerator[0].initialise(6.040 * delayModifier, 8.864 * delayModifier, 17.39 * delayModifier, 48.87 * delayModifier, 4000, 0.25);
		laterReflectionsGenerator[1].initialise(34.27 * delayModifier, 61.72 * delayModifier, 74.6 * delayModifier, 96.13 * delayModifier, 900, 0.35);

		for (int stage = 0; stage < numOfStages; stage++)
		{
			stageBuffer[stage].setSize(2, samplesPerBlockExpected, false, false, false);
		}
	}

	void process(AudioSampleBuffer &buffer)
	{
		if (bypassState == false)
		{
			for (int stage = 0; stage < numOfStages; stage++)
			{
				stageBuffer[stage].makeCopyOf(buffer, true);
			}

			earlyReflectionsGenerator.process(stageBuffer[earlyReflections]);

			stageBuffer[lateReflections1].makeCopyOf(stageBuffer[earlyReflections], true);
			laterReflectionsGenerator[0].process(stageBuffer[lateReflections1]);

			stageBuffer[lateReflections2].makeCopyOf(stageBuffer[lateReflections1], true);
			laterReflectionsGenerator[1].process(stageBuffer[lateReflections2]);


			float* outputL = buffer.getWritePointer(0);
			float* outputR = buffer.getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				//	outputL[sample] = 0.0;
				//	outputR[sample] = 0.0;
			}

			for (int stage = 0; stage < numOfStages; stage++)
			{
				float* stageOutputL = stageBuffer[stage].getWritePointer(0);
				float* stageOutputR = stageBuffer[stage].getWritePointer(1);

				for (int sample = 0; sample < buffer.getNumSamples(); sample++)
				{
					outputL[sample] += (stageOutputL[sample] * gain[stage]);
					outputR[sample] += (stageOutputR[sample] * gain[stage]);
				}
			}
		}
	}

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &gainSliders[direct])
		{
			gain[direct] = gainSliders[direct].getValue();
		}
		else if (slider == &gainSliders[earlyReflections])
		{
			gain[earlyReflections] = gainSliders[earlyReflections].getValue();
		}
		else if (slider == &gainSliders[lateReflections1])
		{
			gain[lateReflections1] = gainSliders[lateReflections1].getValue();
		}
		else if (slider == &gainSliders[lateReflections2])
		{
			gain[lateReflections2] = gainSliders[lateReflections2].getValue();
		}
		else if (slider == &filterCutoffSlider)
		{
			earlyReflectionsGenerator.setFilterCutoff(filterCutoffSlider.getMaxValue(), filterCutoffSlider.getMinValue());
		}
	}

	void buttonClicked(Button* button) override
	{
		if (button == &bypassButton)
		{
			bypassState = bypassButton.getToggleState();
		}
	}

	void resized() override
	{
		gainSliders[direct].setBounds(          getWidth() * 0.1, 0, 150, 30);
		gainSliders[earlyReflections].setBounds(getWidth() * 0.1, 40, 150, 30);
		gainSliders[lateReflections1].setBounds(getWidth() * 0.1, 80, 150, 30);
		gainSliders[lateReflections2].setBounds(getWidth() * 0.1, 120, 150, 30);
		filterCutoffSlider.setBounds(10, 0, 30, getHeight() * 0.5);
		bypassButton.setBounds(gainSliders[direct].getRight() + 10, 0, 70, 30);

	}

private:

	enum { direct, earlyReflections, lateReflections1, lateReflections2, numOfStages };

	Slider gainSliders[numOfStages];
	Slider filterCutoffSlider;
	ToggleButton bypassButton;
	float gain[numOfStages];

	AudioSampleBuffer stageBuffer[numOfStages];
	EarlyReflectionsGenerator earlyReflectionsGenerator;
	LaterReflectionsGenerator laterReflectionsGenerator[2];

	float delayModifier = 10.0;
	bool bypassState = false;
};
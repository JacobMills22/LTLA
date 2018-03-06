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
		matrix[0][1] = 0.0;
		matrix[0][2] = 1.0;
		matrix[0][3] = 1.0;
		matrix[0][4] = -1.0;

		matrix[1][1] = -1.0;
		matrix[1][2] = 0.0;
		matrix[1][3] = -1.0;
		matrix[1][4] = 1.0;

		matrix[2][1] = -1.0;
		matrix[2][2] = 1.0;
		matrix[2][3] = 0.0;
		matrix[2][4] = -1.0;

		matrix[3][1] = 1.0;
		matrix[3][2] = -1.0;
		matrix[3][3] = 1.0;
		matrix[3][4] = 0.0;
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
			output0L[sample] = (matrix[0][0] * input0L[sample]) + (matrix[0][1] * input0L[sample] + (matrix[0][2] * input0L[sample]) + (matrix[0][3] * input0L[sample]));
			output0R[sample] = (matrix[0][0] * input0R[sample]) + (matrix[0][1] * input0R[sample] + (matrix[0][2] * input0R[sample]) + (matrix[0][3] * input0R[sample]));

			output1L[sample] = (matrix[1][0] * input1L[sample]) + (matrix[1][1] * input1L[sample] + (matrix[1][2] * input1L[sample]) + (matrix[1][3] * input1L[sample]));
			output1R[sample] = (matrix[1][0] * input1R[sample]) + (matrix[1][1] * input1R[sample] + (matrix[1][2] * input1R[sample]) + (matrix[1][3] * input1R[sample]));

			output2L[sample] = (matrix[2][0] * input2L[sample]) + (matrix[2][1] * input2L[sample] + (matrix[2][2] * input2L[sample]) + (matrix[2][3] * input2L[sample]));
			output2R[sample] = (matrix[2][0] * input2R[sample]) + (matrix[2][1] * input2R[sample] + (matrix[2][2] * input2R[sample]) + (matrix[2][3] * input2R[sample]));

			output3L[sample] = (matrix[3][0] * input3L[sample]) + (matrix[3][1] * input3L[sample] + (matrix[3][2] * input3L[sample]) + (matrix[3][3] * input3L[sample]));
			output3R[sample] = (matrix[3][0] * input3R[sample]) + (matrix[3][1] * input3R[sample] + (matrix[3][2] * input3R[sample]) + (matrix[3][3] * input3R[sample]));
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
		DBG("CALLED IN PTP X2");
		delayUnit[0].setDelayTimeInMS(20.078);
		delayUnit[1].setDelayTimeInMS(50.154);

		gain[0] = 0.609;
		gain[1] = 0.262;
	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		samplerate = sampleRate;

		for (int reflection = 0; reflection < numOfReflections; reflection++)
		{
			delayedBuffer[reflection].setSize(2, samplesPerBlockExpected, false, false, false);
			delayUnit[reflection].prepareToPlay(samplesPerBlockExpected, sampleRate);
		}
	}

	void process(AudioSampleBuffer &buffer)
	{		
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

class LaterReflectionsGenerator
{

public:

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			delayPath[pathNum].prepareToPlay(samplesPerBlockExpected, sampleRate);
			delayedBuffer[pathNum].setSize(2, samplesPerBlockExpected, false, false, false);
			previousDelayedBuffer[pathNum].setSize(2, samplesPerBlockExpected, false, false, false);
		}

		feedBackMatrixMultiplier.initialise();
		samplerate = sampleRate;
	}

	void initialise(float delayTime1, float delayTime2, float delayTime3, float delayTime4, float cutoff, float audioGain)
	{
		DBG("CALLED IN INIT X4");
		delayPath[0].setDelayTimeInMS(delayTime1);
		delayPath[1].setDelayTimeInMS(delayTime2);
		delayPath[2].setDelayTimeInMS(delayTime3);
		delayPath[3].setDelayTimeInMS(delayTime4);

		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			lowpassFilter[pathNum].setCutoff(cutoff, samplerate, 1.0);
		}

		gain = audioGain;
	}

	void process(AudioSampleBuffer &buffer)
	{
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			delayedBuffer[pathNum].makeCopyOf(buffer, true);
			previousDelayedBuffer[pathNum].makeCopyOf(buffer, true);
		}

		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			float* delayOutputL = delayedBuffer[pathNum].getWritePointer(0);
			float* delayOutputR = delayedBuffer[pathNum].getWritePointer(1);

			float* previousDelayOutputL = previousDelayedBuffer[pathNum].getWritePointer(0);
			float* previousDelayOutputR = previousDelayedBuffer[pathNum].getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				delayOutputL[sample] += previousDelayOutputL[sample];
				delayOutputR[sample] += previousDelayOutputR[sample];
			}

			delayPath[pathNum].process(delayedBuffer[pathNum]);

			lowpassFilter[pathNum].process(delayedBuffer[pathNum]);

	//		float* delayOutputL = delayedBuffer[pathNum].getWritePointer(0);
	//		float* delayOutputR = delayedBuffer[pathNum].getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				delayOutputL[sample] *= gain;
				delayOutputR[sample] *= gain;
			}
		}

		float* outputL = buffer.getWritePointer(0);
		float* outputR = buffer.getWritePointer(1);

		feedBackMatrixMultiplier.process(delayedBuffer[0], delayedBuffer[1], delayedBuffer[2], delayedBuffer[3]);
		
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			previousDelayedBuffer[pathNum].makeCopyOf(delayedBuffer[pathNum], true);

			float* delayOutputL = delayedBuffer[pathNum].getWritePointer(0);
			float* delayOutputR = delayedBuffer[pathNum].getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				outputL[sample] += delayOutputL[sample];
				outputR[sample] += delayOutputR[sample];
			}
		}
	}

private:

	enum { numOfDelays = 4 };
	AudioSampleBuffer delayedBuffer[numOfDelays];
	AudioSampleBuffer previousDelayedBuffer[numOfDelays];

	FeedBackMatrixMultiplier feedBackMatrixMultiplier;
	DelayUnit delayPath[numOfDelays];
	LowpassIIRFilter lowpassFilter[numOfDelays];
	float gain = 0.25;

	float samplerate = 48000;

};

class AutoReverb : public AudioPanelObject,
	public Slider::Listener
{

public:

	AutoReverb()
	{
		laterReflectionsGenerator[0].initialise(6.040, 8.864, 17.39, 48.87, 4000, 0.25);
		laterReflectionsGenerator[1].initialise(34.27, 61.72, 74.6, 96.13, 900, 0.35);

		for (int stage = 0; stage < numOfStages; stage++)
		{
			addAndMakeVisible(gainSliders[stage]);
			gainSliders[stage].addListener(this);
			gainSliders[stage].setRange(0.0, 1.0, 0.01);

			gain[stage] = 0.0;
		}
	}

	void paint(Graphics &g) override
	{
		g.fillAll(Colours::pink);
	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		earlyReflectionsGenerator.prepareToPlay(samplesPerBlockExpected, sampleRate);

		laterReflectionsGenerator[0].prepareToPlay(samplesPerBlockExpected, sampleRate);
		laterReflectionsGenerator[1].prepareToPlay(samplesPerBlockExpected, sampleRate);

		for (int stage = 0; stage < numOfStages; stage++)
		{
			stageBuffer[stage].setSize(2, samplesPerBlockExpected, false, false, false);
		}
	}

	void process(AudioSampleBuffer &buffer)
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

		for (int stage = 0; stage < numOfStages; stage++)
		{
			float* stageOutputL = stageBuffer[stage].getWritePointer(0);
			float* stageOutputR = stageBuffer[stage].getWritePointer(1);

			float* outputL = buffer.getWritePointer(0);
			float* outputR = buffer.getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				outputL[sample] += (stageOutputL[sample] * gain[stage]);
				outputR[sample] += (stageOutputR[sample] * gain[stage]);
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
	}

	void resized() override
	{
		gainSliders[direct].setBounds(          0, 0, 150, 30);
		gainSliders[earlyReflections].setBounds(0, 50, 150, 30);
		gainSliders[lateReflections1].setBounds(0, 100, 150, 30);
		gainSliders[lateReflections2].setBounds(0, 150, 150, 30);

	}

private:

	enum { direct, earlyReflections, lateReflections1, lateReflections2, numOfStages };

	Slider gainSliders[numOfStages];
	float gain[numOfStages];

	AudioSampleBuffer stageBuffer[numOfStages];
	EarlyReflectionsGenerator earlyReflectionsGenerator;
	LaterReflectionsGenerator laterReflectionsGenerator[2];

};
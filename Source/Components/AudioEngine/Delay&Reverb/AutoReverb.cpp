
#include "AutoReverb.h"

	// =============================================
	// FeedBackMatrixMultiplier
	// =============================================

	void FeedBackMatrixMultiplier::initialise()
	{
		// Initialise the multiplication values, these can be changed here at any time.
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

	void FeedBackMatrixMultiplier::process(AudioSampleBuffer &buffer0, AudioSampleBuffer &buffer1, AudioSampleBuffer &buffer2, AudioSampleBuffer &buffer3)
	{
		// Applys the matrix multiplier to each incoming buffer.

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

	// =============================================
	// EarlyReflectionsGenerator
	// =============================================

	EarlyReflectionsGenerator::EarlyReflectionsGenerator()
	{
		// Initialise delay times
		delayUnit[0].setDelayTimeInMS(2.0);
		delayUnit[1].setDelayTimeInMS(5.15);

		// Initialise gains
		gain[0] = 0.609;
		gain[1] = 0.262;
	}

	void EarlyReflectionsGenerator::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		// Set sample rate
		samplerate = sampleRate;

		// Initialise buffer sizes and prepare the delay units for playback
		for (int reflection = 0; reflection < numOfReflections; reflection++)
		{
			delayedBuffer[reflection].setSize(2, samplesPerBlockExpected, false, false, false);
			delayUnit[reflection].prepareToPlay(samplesPerBlockExpected, sampleRate);
		}

		// Set filters samplerate and initial parameters
		highpassFilter.setSampleRate(sampleRate);
		lowpassFilter.setSampleRate(sampleRate);

		highpassFilter.setParameters(500, 0.72);
		lowpassFilter.setParameters(15000, 0.72);
	}

	void EarlyReflectionsGenerator::process(AudioSampleBuffer &buffer)
	{
		// Filter the incomming audio
		highpassFilter.process(buffer);
		lowpassFilter.process(buffer);

		for (int reflection = 0; reflection < numOfReflections; reflection++)
		{
			delayedBuffer[reflection].makeCopyOf(buffer, true); // Store buffer
			delayUnit[reflection].process(delayedBuffer[reflection]); // Delayed/Process stored buffer

			float* outputL = buffer.getWritePointer(0);
			float* outputR = buffer.getWritePointer(1);

			float* delayedOutputL = delayedBuffer[reflection].getWritePointer(0);
			float* delayedOutputR = delayedBuffer[reflection].getWritePointer(1);

			// Sum all reflections scaled by each gain modifier
			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				outputL[sample] += (delayedOutputL[sample] * gain[reflection]);
				outputR[sample] += (delayedOutputR[sample] * gain[reflection]);
			}
		}
	}

	void EarlyReflectionsGenerator::setFilterCutoff(float lowpassCutoff, float highpassCutoff)
	{
		// Set parameters of filters
		highpassFilter.setParameters(highpassCutoff, 0.72);
		lowpassFilter.setParameters(lowpassCutoff, 0.72);
	}

	// =============================================
	// LaterReflectionsGenerator
	// =============================================
	
	void LaterReflectionsGenerator::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		// Initialise and prepare delay paths, buffers, feedback matrix and lowpass filter
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			delayPath[pathNum].prepareToPlay(samplesPerBlockExpected, sampleRate);
			delayPathBuffer[pathNum].setSize(2, samplesPerBlockExpected, false, false, false);
			feedbackMatrixBuffer[pathNum].setSize(2, samplesPerBlockExpected, false, false, false);
			lowpassFilter[pathNum].setSampleRate(sampleRate);
		}

		feedBackMatrixMultiplier.initialise();
		samplerate = sampleRate;
	}

	void LaterReflectionsGenerator::initialise(float delayTime1, float delayTime2, float delayTime3, float delayTime4, float delayModifier, float cutoff, float audioGain)
	{
		// Set delay times.
		delayPath[0].setDelayTimeInMS(delayTime1 * delayModifier);
		delayPath[1].setDelayTimeInMS(delayTime2 * delayModifier);
		delayPath[2].setDelayTimeInMS(delayTime3 * delayModifier);
		delayPath[3].setDelayTimeInMS(delayTime4 * delayModifier);

		// Update filter cutoff
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			lowpassFilter[pathNum].setParameters(cutoff, 0.71);
		}

		// Set the gain
		gain = audioGain;
	}

	void LaterReflectionsGenerator::process(AudioSampleBuffer &buffer)
	{
		// For each delay path
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			// Store copies of input buffers
			delayPathBuffer[pathNum].makeCopyOf(buffer, true);
			feedbackMatrixBuffer[pathNum].makeCopyOf(buffer, true);

			float* delayPathOutputL = delayPathBuffer[pathNum].getWritePointer(0);
			float* delayPathOutputR = delayPathBuffer[pathNum].getWritePointer(1);

			float* feedbackOutputL = feedbackMatrixBuffer[pathNum].getWritePointer(0);
			float* feedbackOutputR = feedbackMatrixBuffer[pathNum].getWritePointer(1);

			// Sum the Delay paths and output of the feedback matrix, scaled by the feedback modifier
			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				delayPathOutputL[sample] += (feedbackOutputL[sample] * feedbackModifier);
				delayPathOutputR[sample] += (feedbackOutputR[sample] * feedbackModifier);
			}

			//================== Delay Path Block==================

			// Delay the stored delay path buffer
			delayPath[pathNum].process(delayPathBuffer[pathNum]);

			// Filter the stored delay path buffer
			lowpassFilter[pathNum].process(delayPathBuffer[pathNum]);

			// Scale the amplitide of the delay paths output
			delayPathOutputL = delayPathBuffer[pathNum].getWritePointer(0);
			delayPathOutputR = delayPathBuffer[pathNum].getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				delayPathOutputL[sample] *= gain;
				delayPathOutputR[sample] *= gain;
			}
			//=======================================================
		}

		// Sum the delay paths together and send to output.

		float* OutputL = buffer.getWritePointer(0);
		float* OutputR = buffer.getWritePointer(1);

		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			float* delayPathOutputL = delayPathBuffer[pathNum].getWritePointer(0);
			float* delayPathOutputR = delayPathBuffer[pathNum].getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				OutputL[sample] += delayPathOutputL[sample];
				OutputR[sample] += delayPathOutputR[sample];
			}
		}

		// Store the delay paths outputs in the feedback Matrix buffers
		for (int pathNum = 0; pathNum < numOfDelays; pathNum++)
		{
			feedbackMatrixBuffer[pathNum].makeCopyOf(delayPathBuffer[pathNum]);
		}

		// Process the feedback matrix ready for use on the next block.
		feedBackMatrixMultiplier.process(feedbackMatrixBuffer[0], feedbackMatrixBuffer[1], feedbackMatrixBuffer[2], feedbackMatrixBuffer[3]);

	}

	void LaterReflectionsGenerator::setFeedbackModifier(float amount)
	{
		feedbackModifier = amount;
	}

	// =============================================
	//	AutoReverb
	// =============================================
	
	AutoReverb::AutoReverb() : autoReverbValueTree("AutoReverbValueTree")
	{
		// Initialisation

		for (int stage = 0; stage < numOfStages; stage++)
		{
			addAndMakeVisible(gainSliders[stage]);
			gainSliders[stage].addListener(this);
			gainSliders[stage].setRange(0.0, 1.0, 0.01);
			gainSliders[stage].setTextBoxStyle(Slider::NoTextBox, false, 50, 15);
			gainSliders[stage].setSliderStyle(Slider::LinearVertical);

			gain[stage] = 0.0;
		}

		for (int param = 0; param < numOfSliderParams; param++)
		{
			addAndMakeVisible(sliderParam[param]);
			sliderParam[param].addListener(this);
			sliderParam[param].setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
		}

		sliderParam[preDelay].setRange(5, 200, 1);
		sliderParam[delayModifierID].setRange(1, 20, 1);
		sliderParam[feedbackAmount].setRange(0.0, 5.0, 0.1);

		for (int labelID = 0; labelID < numOfLabels; labelID++)
		{
			addAndMakeVisible(labels[labelID]);
			labels[labelID].addListener(this);
		}

		labels[filterLabel].setText("Filtering", dontSendNotification);
		labels[gainLabel].setText("Stage Gain", dontSendNotification);
		labels[directGain].setText("Direct", dontSendNotification);
		labels[earlyReflectionsLabel].setText("ER", dontSendNotification);
		labels[lateReflections1Label].setText("LR1", dontSendNotification);
		labels[lateReflections2Label].setText("LR2", dontSendNotification);
		labels[preDelayLabel].setText("Pre-Delay", dontSendNotification);
		labels[feedbackLabel].setText("Feedback Amount", dontSendNotification);
		labels[roomSizeLabel].setText("Room Size", dontSendNotification);

		addAndMakeVisible(filterCutoffSlider);
		filterCutoffSlider.addListener(this);
		filterCutoffSlider.setSliderStyle(Slider::TwoValueVertical);
		filterCutoffSlider.setRange(20, 20000, 1);
		filterCutoffSlider.setSkewFactor(0.23);
		filterCutoffSlider.setMinAndMaxValues(100, 15000, dontSendNotification);

		addAndMakeVisible(bypassButton);
		bypassButton.addListener(this);
		bypassButton.setButtonText("Bypass");

		bypassButton.setToggleState(true, sendNotification);
		gainSliders[direct].setValue(0.5, sendNotification);
		gainSliders[earlyReflections].setValue(0.2, sendNotification);
		gainSliders[lateReflections1].setValue(0.7, sendNotification);
		gainSliders[lateReflections2].setValue(0.9, sendNotification);
		sliderParam[feedbackAmount].setValue(1.0, sendNotification);
		sliderParam[preDelay].setValue(20, sendNotification);
		sliderParam[delayModifierID].setValue(10, sendNotification);

		autoReverbValueTree.setProperty("Bypassed", true, nullptr);
		autoReverbValueTree.setProperty("LowPassCutoff", 100.0, nullptr);
		autoReverbValueTree.setProperty("HighPassCutoff", 15000.0, nullptr);
		autoReverbValueTree.setProperty("DirectGain", 0.5, nullptr);
		autoReverbValueTree.setProperty("ER_Gain", 0.2, nullptr);
		autoReverbValueTree.setProperty("LR1_Gain", 0.7, nullptr);
		autoReverbValueTree.setProperty("LR2_Gain", 0.9, nullptr);
		autoReverbValueTree.setProperty("FeedbackAmount", 1.0, nullptr);
		autoReverbValueTree.setProperty("Pre-Delay", 20.0, nullptr);
		autoReverbValueTree.setProperty("DelayModifier", 10.0, nullptr);

		bypassButton.getToggleStateValue().referTo(autoReverbValueTree.getPropertyAsValue("Bypassed", nullptr));
		filterCutoffSlider.getMinValueObject().referTo(autoReverbValueTree.getPropertyAsValue("LowPassCutoff", nullptr));
		filterCutoffSlider.getMaxValueObject().referTo(autoReverbValueTree.getPropertyAsValue("HighPassCutoff", nullptr));
		gainSliders[direct].getValueObject().referTo(autoReverbValueTree.getPropertyAsValue("DirectGain", nullptr));
		gainSliders[earlyReflections].getValueObject().referTo(autoReverbValueTree.getPropertyAsValue("ER_Gain", nullptr));
		gainSliders[lateReflections1].getValueObject().referTo(autoReverbValueTree.getPropertyAsValue("LR1_Gain", nullptr));
		gainSliders[lateReflections2].getValueObject().referTo(autoReverbValueTree.getPropertyAsValue("LR2_Gain", nullptr));
		sliderParam[feedbackAmount].getValueObject().referTo(autoReverbValueTree.getPropertyAsValue("FeedbackAmount", nullptr));
		sliderParam[preDelay].getValueObject().referTo(autoReverbValueTree.getPropertyAsValue("Pre-Delay", nullptr));
		sliderParam[delayModifierID].getValueObject().referTo(autoReverbValueTree.getPropertyAsValue("DelayModifier", nullptr));

	}

	void AutoReverb::snapshotFired()
	{
		// Snapshot has been fired so update bypass state to the state stored in the recalled snapshot.
		bypassState = bypassButton.getToggleState();

		// Update gain values to the values stored in the recalled snapshot.
		gain[direct] = gainSliders[direct].getValue();
		gain[earlyReflections] = gainSliders[earlyReflections].getValue();
		gain[lateReflections1] = gainSliders[lateReflections1].getValue();
		gain[lateReflections2] = gainSliders[lateReflections2].getValue();

		// Reinitialise reflection generators and delayunits using parameter values stored in the recalled snapshot.
		earlyReflectionsGenerator.setFilterCutoff(filterCutoffSlider.getMaxValue(), filterCutoffSlider.getMinValue());
		preDelayUnit.setDelayTimeInMS(sliderParam[preDelay].getValue());
		delayModifier = sliderParam[delayModifierID].getValue();

		laterReflectionsGenerator[0].initialise(6.040, 8.864, 17.39, 48.87, delayModifier, 4000, 0.25);
		laterReflectionsGenerator[1].initialise(34.27, 61.72, 74.6, 96.13, delayModifier, 900, 0.35);

		laterReflectionsGenerator[0].setFeedbackModifier(sliderParam[feedbackAmount].getValue());
		laterReflectionsGenerator[1].setFeedbackModifier(sliderParam[feedbackAmount].getValue()); //fo

	}

	void AutoReverb::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		// Initialise and prepare all reflection generators, delay units and buffers.
		earlyReflectionsGenerator.prepareToPlay(samplesPerBlockExpected, sampleRate);

		laterReflectionsGenerator[0].prepareToPlay(samplesPerBlockExpected, sampleRate);
		laterReflectionsGenerator[1].prepareToPlay(samplesPerBlockExpected, sampleRate);

		laterReflectionsGenerator[0].initialise(6.040, 8.864, 17.39, 48.87, delayModifier, 4000, 0.25);
		laterReflectionsGenerator[1].initialise(34.27, 61.72, 74.6, 96.13, delayModifier, 900, 0.35);

		preDelayUnit.prepareToPlay(samplesPerBlockExpected, sampleRate);
		preDelayUnit.setDelayTimeInMS(5);

		for (int stage = 0; stage < numOfStages; stage++)
		{
			stageBuffer[stage].setSize(2, samplesPerBlockExpected, false, false, false);
		}
	}

	void AutoReverb::process(AudioSampleBuffer &buffer)
	{
		// If bypass is disabled
		if (bypassState == false)
		{
			// Store a copy of the input buffer for each stage to use.
			for (int stage = 0; stage < numOfStages; stage++)
			{
				stageBuffer[stage].makeCopyOf(buffer, true);
			}

			// Process the pre-delay.
			preDelayUnit.process(stageBuffer[earlyReflections]);

			// Process the early reflections.
			earlyReflectionsGenerator.process(stageBuffer[earlyReflections]);

			// Process the first set of late reflections.
			stageBuffer[lateReflections1].makeCopyOf(stageBuffer[earlyReflections], true);
			laterReflectionsGenerator[0].process(stageBuffer[lateReflections1]);

			// Process the second set pf late reflections.
			stageBuffer[lateReflections2].makeCopyOf(stageBuffer[lateReflections1], true);
			laterReflectionsGenerator[1].process(stageBuffer[lateReflections2]);
			
			// Clear the output.
			float* outputL = buffer.getWritePointer(0);
			float* outputR = buffer.getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				outputL[sample] = 0.0;
				outputR[sample] = 0.0;
			}

			// Sum and output each stage scaled by the gain of each stage.
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

	void AutoReverb::sliderValueChanged(Slider* slider)
	{
		// Update parameter values based on which slider was changed
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
		else if (slider == &sliderParam[preDelay])
		{
			preDelayUnit.setDelayTimeInMS(sliderParam[preDelay].getValue());
		}
		else if (slider == &sliderParam[delayModifierID])
		{
			delayModifier = sliderParam[delayModifierID].getValue();

			laterReflectionsGenerator[0].initialise(6.040, 8.864, 17.39, 48.87, delayModifier, 4000, 0.25);
			laterReflectionsGenerator[1].initialise(34.27, 61.72, 74.6, 96.13, delayModifier, 900, 0.35);
		}
		else if (slider == &sliderParam[feedbackAmount])
		{
			laterReflectionsGenerator[0].setFeedbackModifier(sliderParam[feedbackAmount].getValue());
			laterReflectionsGenerator[1].setFeedbackModifier(sliderParam[feedbackAmount].getValue());
		}
	}

	void AutoReverb::buttonClicked(Button* button) 
	{
		// Update the bypass state
		if (button == &bypassButton)
		{
			bypassState = bypassButton.getToggleState();
		}
	}

	void AutoReverb::labelTextChanged(Label* labelThatHasChanged)
	{
	}

	void AutoReverb::resized() 
	{
		// Set bounds of all components
		float sliderWidth = getWidth() * 0.2;
		float sliderHeight = getHeight() * 0.15;

		float verticalSliderWidth = getWidth() * 0.05;
		float verticalSliderHeight = getHeight() * 0.5;

		float sliderY = getHeight() * 0.1;

		float buttonWidth = getWidth() * 0.15;

		gainSliders[direct].setBounds(getWidth() * 0.2, sliderY, verticalSliderWidth, verticalSliderHeight);
		gainSliders[earlyReflections].setBounds(getWidth() * 0.3, sliderY, verticalSliderWidth, verticalSliderHeight);
		gainSliders[lateReflections1].setBounds(getWidth() * 0.4, sliderY, verticalSliderWidth, verticalSliderHeight);
		gainSliders[lateReflections2].setBounds(getWidth() * 0.5, sliderY, verticalSliderWidth, verticalSliderHeight);

		sliderParam[preDelay].setBounds(getWidth() * 0.6, getHeight() * 0.13, sliderWidth, sliderHeight);
		sliderParam[feedbackAmount].setBounds(getWidth() * 0.6, getHeight() * 0.4, sliderWidth, sliderHeight);
		sliderParam[delayModifierID].setBounds(getWidth() * 0.8, getHeight() * 0.4, sliderWidth, sliderHeight);
		
		filterCutoffSlider.setBounds(getWidth() * 0.03, sliderY, sliderWidth * 0.2, verticalSliderHeight);
		bypassButton.setBounds(getWidth() - (buttonWidth * 1.3), getHeight() * 0.1, buttonWidth, sliderHeight);

		float labelY = sliderY + verticalSliderHeight * 0.9;

		labels[filterLabel].setBounds(getWidth() * 0.001, 0, 80, 30);
		labels[gainLabel].setBounds(getWidth() * 0.3, 0, 80, 30);
		labels[directGain].setBounds(getWidth() * 0.18, labelY, 50, 30);
		labels[earlyReflectionsLabel].setBounds(getWidth() * 0.305, labelY, 50, 30);
		labels[lateReflections1Label].setBounds(getWidth() * 0.398, labelY, 50, 30);
		labels[lateReflections2Label].setBounds(getWidth() * 0.498, labelY, 50, 30);

		labels[preDelayLabel].setBounds(getWidth() * 0.62, 0, 100, 30);
		labels[feedbackLabel].setBounds(getWidth() * 0.60, getHeight() * 0.3, 100, 30);
		labels[roomSizeLabel].setBounds(getWidth() * 0.82, getHeight() * 0.3, 100, 30);

	}

	ValueTree AutoReverb::getValueTree()
	{
		return autoReverbValueTree;
	}
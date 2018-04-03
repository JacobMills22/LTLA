
#include "AutoEQ.h"

	AutoEQ::AutoEQ() : autoEQValueTree("autoEQValueTree")
	{
		// Parameter initialisation

		for (int parameter = 0; parameter < numOfSliders; parameter++)
		{
			addAndMakeVisible(eqSlider[parameter]);
			eqSlider[parameter].setSliderStyle(Slider::RotaryVerticalDrag);
			eqSlider[parameter].addListener(this);
			eqSlider[parameter].setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
			eqSliderValueLabel[parameter].setEditable(true, true, false);


			addAndMakeVisible(eqSliderValueLabel[parameter]);
			eqSliderValueLabel[parameter].addListener(this);

			addAndMakeVisible(eqSliderNameLabel[parameter]);
			eqSliderNameLabel[parameter].addListener(this);
			eqSliderNameLabel[parameter].setJustificationType(Justification::centred);
		}

		eqSlider[frequencySliderID].setRange(20, 20000, 1.0);
		eqSlider[frequencySliderID].setSkewFactor(0.23);
		eqSlider[frequencySliderID].setValue(20, dontSendNotification);
		eqSliderValueLabel[frequencySliderID].setText("20 Hz", dontSendNotification);
		eqSliderNameLabel[frequencySliderID].setText("Frequency", dontSendNotification);

		eqSlider[qFactorSliderID].setRange(1.0, 10.0, 0.1);
		eqSlider[qFactorSliderID].setValue(1.0, dontSendNotification);
		eqSliderValueLabel[qFactorSliderID].setText("1.0", dontSendNotification);
		eqSliderNameLabel[qFactorSliderID].setText("Q Factor", dontSendNotification);

		eqSlider[gainSliderID].setRange(-18.0, 18.0, 0.5);
		eqSlider[gainSliderID].setValue(0.0, dontSendNotification);
		eqSliderValueLabel[gainSliderID].setText("0.0 dB", dontSendNotification);
		eqSliderNameLabel[gainSliderID].setText("Gain", dontSendNotification);

		addAndMakeVisible(bypassButton);
		bypassButton.addListener(this);
		bypassButton.setButtonText("Bypass");
		bypassButton.setToggleState(true, sendNotification);

		for (int band = 0; band < numOfBands; band++)
			eqBand[band].sliderValues.qFactor.setValue(2.0);

		autoEQValueTree.setProperty("Bypassed", true, nullptr);
		autoEQValueTree.setProperty("Band1_Frequency", 0, nullptr);
		autoEQValueTree.setProperty("Band1_QFactor", 2.0, nullptr);
		autoEQValueTree.setProperty("Band1_Gain", 0, nullptr);
		autoEQValueTree.setProperty("Band2_Frequency", 0, nullptr);
		autoEQValueTree.setProperty("Band2_QFactor", 2.0, nullptr);
		autoEQValueTree.setProperty("Band2_Gain", 0, nullptr);
		autoEQValueTree.setProperty("Band3_Frequency", 0, nullptr);
		autoEQValueTree.setProperty("Band3_QFactor", 2.0, nullptr);
		autoEQValueTree.setProperty("Band3_Gain", 0, nullptr);
		autoEQValueTree.setProperty("Band4_Frequency", 0, nullptr);
		autoEQValueTree.setProperty("Band4_QFactor", 2.0, nullptr);
		autoEQValueTree.setProperty("Band4_Gain", 0, nullptr);

		bypassButton.getToggleStateValue().referTo(autoEQValueTree.getPropertyAsValue("Bypassed", nullptr));
		eqBand[0].sliderValues.frequency.referTo(autoEQValueTree.getPropertyAsValue("Band1_Frequency", nullptr));
		eqBand[0].sliderValues.qFactor.referTo(autoEQValueTree.getPropertyAsValue("Band1_QFactor", nullptr));
		eqBand[0].sliderValues.gain.referTo(autoEQValueTree.getPropertyAsValue("Band1_Gain", nullptr));
		eqBand[1].sliderValues.frequency.referTo(autoEQValueTree.getPropertyAsValue("Band2_Frequency", nullptr));
		eqBand[1].sliderValues.qFactor.referTo(autoEQValueTree.getPropertyAsValue("Band2_QFactor", nullptr));
		eqBand[1].sliderValues.gain.referTo(autoEQValueTree.getPropertyAsValue("Band2_Gain", nullptr));
		eqBand[2].sliderValues.frequency.referTo(autoEQValueTree.getPropertyAsValue("Band3_Frequency", nullptr));
		eqBand[2].sliderValues.qFactor.referTo(autoEQValueTree.getPropertyAsValue("Band3_QFactor", nullptr));
		eqBand[2].sliderValues.gain.referTo(autoEQValueTree.getPropertyAsValue("Band3_Gain", nullptr));
		eqBand[3].sliderValues.frequency.referTo(autoEQValueTree.getPropertyAsValue("Band4_Frequency", nullptr));
		eqBand[3].sliderValues.qFactor.referTo(autoEQValueTree.getPropertyAsValue("Band4_QFactor", nullptr));
		eqBand[3].sliderValues.gain.referTo(autoEQValueTree.getPropertyAsValue("Band4_Gain", nullptr));
	}

	void AutoEQ::initialiseEQBands(float sampleRate)
	{
		// EQBands Default Values
		eqBand[0].uiCentreFreqModifier = 0.19;
		eqBand[1].uiCentreFreqModifier = 0.40;
		eqBand[2].uiCentreFreqModifier = 0.53;
		eqBand[3].uiCentreFreqModifier = 0.66;

		eqBand[0].uiBandWidthModifier = 0.0025;
		eqBand[1].uiBandWidthModifier = 0.0025;
		eqBand[2].uiBandWidthModifier = 0.0625;
		eqBand[3].uiBandWidthModifier = 0.125;

		eqBand[0].sliderValues.frequency = 100;
		eqBand[1].sliderValues.frequency = 1000;
		eqBand[2].sliderValues.frequency = 2500;
		eqBand[3].sliderValues.frequency = 5000;

		eqBand[0].colour = Colours::red;
		eqBand[1].colour = Colours::green;
		eqBand[2].colour = Colours::blue;
		eqBand[3].colour = Colours::yellow;

		for (int chan = 0; chan < 2; chan++)
		{
			eqBand[0].peakingFilter[chan].setParameters(100, 2.0, 0.0);
			eqBand[1].peakingFilter[chan].setParameters(1000, 2.0, 0.0);
			eqBand[2].peakingFilter[chan].setParameters(2500, 2.0, 0.0);
			eqBand[3].peakingFilter[chan].setParameters(5000, 2.0, 0.0);
		}

		for (int band = 0; band < numOfBands; band++)
		{
			eqBand[band].freqUIRange.start = 20.0;
			eqBand[band].freqUIRange.end = 20000.0;
			eqBand[band].freqUIRange.interval = 1.0;
			eqBand[band].freqUIRange.skew = 0.30;
		}
	}

	// ========================
	// Audio Functions
	// ========================

	void AutoEQ::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		// Prepare audio for playback
		eqSampleRate = sampleRate;

		for (int band = 0; band < numOfBands; band++)
		{
			eqBand[band].peakingFilter[0].setSampleRate(sampleRate);
			eqBand[band].peakingFilter[1].setSampleRate(sampleRate);
		}

		initialiseEQBands(sampleRate);
	}

	void AutoEQ::process(AudioSampleBuffer &buffer)
	{
		// Process all EQ bands if bypass is disabled
		if (bypassState == false)
		{
			float* outputL = buffer.getWritePointer(0);
			float* outputR = buffer.getWritePointer(1);

			for (int sample = 0; sample < buffer.getNumSamples(); sample++)
			{
				for (int band = 0; band < numOfBands; band++)
				{
					outputL[sample] = eqBand[band].peakingFilter[0].process(outputL[sample]);
					outputR[sample] = eqBand[band].peakingFilter[0].process(outputR[sample]);
				}
			}
		}
	}

	// ========================
	// UI Functions
	// ========================

	void AutoEQ::resized() 
	{
		// Set bounds for components

		eqLeft = getWidth() * 0.01;
		eqRight = (getWidth() * 0.7) + eqLeft;

		bypassButton.setBounds(eqRight + 1, -5, 100, 30);

		eqSlider[frequencySliderID].setBounds(eqRight + 50, getHeight() * 0.08, 50, 50);
		eqSlider[qFactorSliderID].setBounds(eqRight + 50, getHeight() * 0.28, 50, 50);
		eqSlider[gainSliderID].setBounds(eqRight + 50, getHeight() * 0.48, 50, 50);

		eqSliderNameLabel[frequencySliderID].setBounds(eqSlider[frequencySliderID].getX() - 45, eqSlider[frequencySliderID].getY() + 15, 55, 20);
		eqSliderNameLabel[qFactorSliderID].setBounds(eqSlider[qFactorSliderID].getX() - 45, eqSlider[qFactorSliderID].getY() + 15, 50, 20);
		eqSliderNameLabel[gainSliderID].setBounds(eqSlider[gainSliderID].getX() - 45, eqSlider[gainSliderID].getY() + 15, 50, 20);

		eqSliderValueLabel[frequencySliderID].setBounds(eqSlider[frequencySliderID].getX() + 45, eqSlider[frequencySliderID].getY() + 15, 60, 20);
		eqSliderValueLabel[qFactorSliderID].setBounds(eqSlider[qFactorSliderID].getX() + 45, eqSlider[qFactorSliderID].getY() + 15, 60, 20);
		eqSliderValueLabel[gainSliderID].setBounds(eqSlider[gainSliderID].getX() + 45, eqSlider[gainSliderID].getY() + 15, 60, 20);
	}

	void AutoEQ::paint(Graphics &g) 
	{
		eqLeft = getWidth() * 0.01;
		eqRight = (getWidth() * 0.7) + eqLeft;
		float eqCentreY = ((getHeight() * 0.65) * 0.5);
		eqWidth = eqRight - eqLeft;
		eqHeight = getHeight() * 0.65;

		Path eqCurve; // The main EQ curved line
		Point<float> mainEQLine[2]; // Start and end of the main EQ curved line
		juce::Rectangle<float> eqBackground; // The background
		juce::Rectangle<float> eqInvisibleBorder[3]; // Three squares surrounding the EQ to hide the EQ line if it spills out of the EQ bounds.
		
		// Set main line start coordinates.
		mainEQLine[eqLineStart].setXY(eqLeft, eqCentreY);

		for (int band = 0; band < numOfBands; band++)
		{
			// Set X and Y of each bands start, centre and end points.
			eqBand[band].uiGraphPoints[eqLineBandCentre].setXY((eqWidth * eqBand[band].uiCentreFreqModifier) + eqLeft, eqHeight * eqBand[band].uiGainModifierCurve);
			eqBand[band].uiGraphPoints[eqLineBandStart].setXY(eqBand[band].uiGraphPoints[eqLineBandCentre].getX() - (eqWidth * 0.4 * 0.25), eqCentreY);
			eqBand[band].uiGraphPoints[eqLineBandEnd].setXY(eqBand[band].uiGraphPoints[eqLineBandCentre].getX() + (eqWidth * 0.4 * 0.25), eqCentreY);

			// Calculate each bands curve width and height.
			// Note: To draw a nice curve for a single band two curves need to be used, therefore the curveWidth is actually half the
			// full width since the first curve goes from the start to the centre and the second curves goes from the centre to the end.
			float curveWidth = eqBand[band].uiGraphPoints[eqLineBandCentre].getX() - eqBand[band].uiGraphPoints[eqLineBandStart].getX();
			float curveHeight = eqBand[band].uiGraphPoints[eqLineBandCentre].getY() - eqCentreY;

			// Bezier handles are used to manipulate the way in which the lines curve,
			// The XY coordinates for both handles are calculated here.
			float bezierHandleLeftX = eqBand[band].uiGraphPoints[eqLineBandStart].getX() + (curveWidth * eqBand[band].uiBandWidthModifier);
			float bezierHandleLeftY = eqCentreY + (curveHeight * (1.0 - eqBand[band].uiBandWidthModifier));

			float bezierHandleRightX = eqBand[band].uiGraphPoints[eqLineBandCentre].getX() + (curveWidth * (1.0 - eqBand[band].uiBandWidthModifier));
			float bezierHandleRightY = eqCentreY + (curveHeight * (1.0 - eqBand[band].uiBandWidthModifier));

			// Bezier handles coordinates are set here.
			eqBand[band].uiGraphPoints[BezierHandleLeft].setXY(bezierHandleLeftX, bezierHandleLeftY);
			eqBand[band].uiGraphPoints[BezierHandleRight].setXY(bezierHandleRightX, bezierHandleRightY);
		}

		// Set main line end coordinates.
		mainEQLine[eqLineEnd].setXY(eqRight, eqCentreY);

		// Clear existing curve before it is redrawn.
		eqCurve.clear();

		// Fill background and draw a black outline
		g.setColour(Colours::dimgrey.darker(1.5));
		eqBackground.setBounds(eqLeft, 0, getWidth() * 0.7, getHeight() * 0.65);
		g.fillRect(eqBackground);
		g.setColour(Colours::black);
		g.drawRect(eqBackground);

		// Draw a line to each band, using a quadtraic curve manipulated by bezier handles.
		g.setColour(Colours::white);
		eqCurve.startNewSubPath(mainEQLine[eqLineStart]);

		eqCurve.lineTo(eqBand[0].uiGraphPoints[eqLineBandStart]);
		eqCurve.quadraticTo(eqBand[0].uiGraphPoints[BezierHandleLeft], eqBand[0].uiGraphPoints[eqLineBandCentre]);
		eqCurve.quadraticTo(eqBand[0].uiGraphPoints[BezierHandleRight], eqBand[0].uiGraphPoints[eqLineBandEnd]);

		eqCurve.lineTo(eqBand[1].uiGraphPoints[eqLineBandStart]);
		eqCurve.quadraticTo(eqBand[1].uiGraphPoints[BezierHandleLeft], eqBand[1].uiGraphPoints[eqLineBandCentre]);
		eqCurve.quadraticTo(eqBand[1].uiGraphPoints[BezierHandleRight], eqBand[1].uiGraphPoints[eqLineBandEnd]);

		eqCurve.lineTo(eqBand[2].uiGraphPoints[eqLineBandStart]);
		eqCurve.quadraticTo(eqBand[2].uiGraphPoints[BezierHandleLeft], eqBand[2].uiGraphPoints[eqLineBandCentre]);
		eqCurve.quadraticTo(eqBand[2].uiGraphPoints[BezierHandleRight], eqBand[2].uiGraphPoints[eqLineBandEnd]);

		eqCurve.lineTo(eqBand[3].uiGraphPoints[eqLineBandStart]);
		eqCurve.quadraticTo(eqBand[3].uiGraphPoints[BezierHandleLeft], eqBand[3].uiGraphPoints[eqLineBandCentre]);
		eqCurve.quadraticTo(eqBand[3].uiGraphPoints[BezierHandleRight], eqBand[3].uiGraphPoints[eqLineBandEnd]);

		eqCurve.lineTo(mainEQLine[eqLineEnd]);

		// Fill the EQ path light blue and draw a white eq line on top.
		PathStrokeType strokeType(1.0, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded);
		g.setColour(Colours::lightblue);
		g.setOpacity(0.5);
		g.fillPath(eqCurve);
		g.setColour(Colours::white);
		g.setOpacity(1.0);
		g.strokePath(eqCurve, strokeType);

		// set the bounds for the invisible borders, this hides the any clipping of the line outside the bounds.
		eqInvisibleBorder[0].setBounds(eqRight, 0, 100, eqHeight * 1.2);
		eqInvisibleBorder[1].setBounds(0, 0, eqLeft, eqHeight * 1.2);
		eqInvisibleBorder[2].setBounds(0, eqHeight, eqWidth * 1.25, 20);

		// for each band
		for (int band = 0; band < numOfBands; band++)
		{
			// Update the handle position (not to be confused with the bezier handles, these are also known as eq nodes).
			eqBand[band].handlePosition.x = eqBand[band].uiGraphPoints[eqLineBandCentre].getX() - 10;
			eqBand[band].handlePosition.y = (eqHeight * eqBand[band].uiGainModifierHandle) - 10;

			// Fill the circlular handles with the bands colour and draw a black outline around it.
			g.setColour(eqBand[band].colour);
			g.setOpacity(0.5);
			g.fillEllipse(eqBand[band].handlePosition.x, eqBand[band].handlePosition.y, 20, 20);
			g.setOpacity(1.0);
			if (eqBand[band].selected == true)
			{
				g.setColour(Colours::black);
				g.drawEllipse(eqBand[band].handlePosition.x, eqBand[band].handlePosition.y, 20, 20, 1.0);
			}
		}

		// Fill the invisible borders with the same colour as the background, 
		// this hides the any clipping of the line outside the bounds.
		g.setColour(Colours::darkgrey.darker());
		g.fillRect(eqInvisibleBorder[0]);
		g.fillRect(eqInvisibleBorder[1]);
		g.fillRect(eqInvisibleBorder[2]);
	}

	void AutoEQ::sliderValueChanged(Slider* slider) 
	{
		// For each band
		for (int band = 0; band < numOfBands; band++)
		{
			if (eqBand[band].selected == true)
			{
				// If the band is selected get the slider values.
				double centreFrequency = eqSlider[frequencySliderID].getValue();
				double qFactor = eqSlider[qFactorSliderID].getValue();
				double gainSliderValue = eqSlider[gainSliderID].getValue();
				
				if (slider == &eqSlider[frequencySliderID] || slider == &eqSlider[qFactorSliderID] || slider == &eqSlider[gainSliderID])
				{
					// If either the fequency, Q factor or gain slider was changed, 
					// update eq bands left and right filter parameters
					eqBand[band].peakingFilter[0].setParameters(centreFrequency, qFactor, gainSliderValue);
					eqBand[band].peakingFilter[1].setParameters(centreFrequency, qFactor, gainSliderValue);

					// Depending on which slider has been changed, store the relevent values and update the value label text
					if (slider == &eqSlider[frequencySliderID])
					{
						eqBand[band].sliderValues.frequency = centreFrequency;
						eqSliderValueLabel[frequencySliderID].setText((String)centreFrequency + " Hz", dontSendNotification);
					}
					else if (slider == &eqSlider[qFactorSliderID])
					{
						eqBand[band].sliderValues.qFactor = qFactor;
						eqSliderValueLabel[qFactorSliderID].setText((String)qFactor, dontSendNotification);
					}
					else if (slider == &eqSlider[gainSliderID])
					{
						eqBand[band].sliderValues.gain = gainSliderValue;
						eqSliderValueLabel[gainSliderID].setText((String)gainSliderValue + " dB", dontSendNotification);
					}

					// Scale UI modifiers based on new parameters.
					eqBand[band].uiCentreFreqModifier = eqBand[band].freqUIRange.convertTo0to1(centreFrequency);
					eqBand[band].uiBandWidthModifier = (qFactor / 10) - 0.1;
					eqBand[band].uiGainModifierHandle = rangeRemapper.convert(gainSliderValue, 18.0, -18.0, 0.95, 0.05);
					eqBand[band].uiGainModifierCurve = eqBand[band].uiGainModifierHandle;
				}

			}
		}
	}

	void AutoEQ::labelTextChanged(Label* labelThatHasChanged) 
	{
		// Depending on which label has been changed, set the value of the slider to the
		// new value in the label and update the label text.
		if (labelThatHasChanged == &eqSliderValueLabel[frequencySliderID])
		{
			float value = eqSliderValueLabel[frequencySliderID].getText().getFloatValue();
			eqSlider[frequencySliderID].setValue(value, sendNotification);

			eqSliderValueLabel[frequencySliderID].setText(eqSliderValueLabel[frequencySliderID].getText() + " Hz", dontSendNotification);
		}
		else if (labelThatHasChanged == &eqSliderValueLabel[qFactorSliderID])
		{
			float value = eqSliderValueLabel[qFactorSliderID].getText().getFloatValue();
			eqSlider[qFactorSliderID].setValue(value, sendNotification);
		}
		else if (labelThatHasChanged == &eqSliderValueLabel[gainSliderID])
		{
			float value = eqSliderValueLabel[gainSliderID].getText().getFloatValue();
			eqSlider[gainSliderID].setValue(value, sendNotification);

			eqSliderValueLabel[gainSliderID].setText(eqSliderValueLabel[gainSliderID].getText() + " dB", dontSendNotification);
		}
	}

	void AutoEQ::buttonClicked(Button* button) 
	{
		// Update bypass state to the toggle button state if pressed.
		if (button == &bypassButton)
		{
			bypassState = bypassButton.getToggleState();
		}
	}
	
	// ========================
	// Additional Functions
	// ========================

	void AutoEQ::updateBandData(int band, double frequency, double qFactor, double gain)
	{
		// Update filter parameters
		eqBand[band].peakingFilter[0].setParameters(frequency, qFactor, gain);
		eqBand[band].peakingFilter[1].setParameters(frequency, qFactor, gain);

		// Store the new slider values.
		eqBand[band].sliderValues.frequency = frequency;
		eqBand[band].sliderValues.qFactor = qFactor;
		eqBand[band].sliderValues.gain = gain;

		// Set the label text to the new values.
		eqSliderValueLabel[frequencySliderID].setText((String)(int)frequency + " Hz", dontSendNotification);
		eqSliderValueLabel[qFactorSliderID].setText((String)qFactor, dontSendNotification);
		eqSliderValueLabel[gainSliderID].setText((String)gain + " dB", dontSendNotification);

		// Scale UI modifiers based on new parameters.
		eqBand[band].uiCentreFreqModifier = eqBand[band].freqUIRange.convertTo0to1(frequency);
		eqBand[band].uiBandWidthModifier = (qFactor / 10) - 0.1;
		eqBand[band].uiGainModifierHandle = rangeRemapper.convert(gain, 18.0, -18.0, 0.95, 0.05);
		eqBand[band].uiGainModifierCurve = eqBand[band].uiGainModifierHandle;
	}

	void AutoEQ::selectEQBand(int bandID)
	{
		// Deselect all bands
		for (int band = 0; band < numOfBands; band++)
		{
			eqBand[band].selected = false;
		}

		// Set selected state to true.
		eqBand[bandID].selected = true;

		// Update the slider values to the value of the newly selected band.
		eqSlider[frequencySliderID].setValue(eqBand[bandID].sliderValues.frequency.getValue(), dontSendNotification);
		eqSlider[qFactorSliderID].setValue(eqBand[bandID].sliderValues.qFactor.getValue(), dontSendNotification);
		eqSlider[gainSliderID].setValue(eqBand[bandID].sliderValues.gain.getValue(), dontSendNotification);

		// Update the label text values to the value of the newly selected band.
		eqSliderValueLabel[frequencySliderID].setText((String)eqSlider[frequencySliderID].getValue() + " Hz", dontSendNotification);
		eqSliderValueLabel[qFactorSliderID].setText((String)eqSlider[qFactorSliderID].getValue(), dontSendNotification);
		eqSliderValueLabel[gainSliderID].setText((String)eqSlider[gainSliderID].getValue() + " dB", dontSendNotification);

		// Update the slider colours to the colour of the newly selected band.
		eqSlider[frequencySliderID].setColour(0x1001300, eqBand[bandID].colour);
		eqSlider[qFactorSliderID].setColour(0x1001300, eqBand[bandID].colour);
		eqSlider[gainSliderID].setColour(0x1001300, eqBand[bandID].colour);
	}

	void AutoEQ::snapshotFired()
	{
		// Update bypass state to the new state stored in the newly recalled snapshot.
		bypassState = bypassButton.getToggleState();

		// Update each bands data to the values stored in the recalled snapshot.
		for (int band = 0; band < numOfBands; band++)
		{
			double frequency = eqBand[band].sliderValues.frequency.getValue();
			double qFactor = eqBand[band].sliderValues.qFactor.getValue();
			double gain = eqBand[band].sliderValues.gain.getValue();

			updateBandData(band, frequency, qFactor, gain);
		}
		// By default select the first band.
		selectEQBand(0);
	}

	ValueTree AutoEQ::getValueTree()
	{
		return autoEQValueTree;
	}

	// ========================
	// Mouse Listener Functions
	// ========================
	
	void AutoEQ::mouseDown(const MouseEvent& event) 
	{
		for (int band = 0; band < numOfBands; band++)
		{
			if (event.x >= eqBand[band].handlePosition.x && event.x <= (eqBand[band].handlePosition.x + 20))
			{
				if (event.y >= eqBand[band].handlePosition.y && event.y <= (eqBand[band].handlePosition.y + 20))
				{
					// If the mouse has clicked on a handle, select the band which the handle belongs to.
					// Since the mouse as not been released the user must want to move it so set the
					// handleBeingMoved state to true.
					selectEQBand(band);
					eqBand[band].handleBeingMoved = true;
					break;
				}
			}
		}
	}

	void AutoEQ::mouseUp(const MouseEvent& event) 
	{
		// Mouse click has been released so handles are no longer being moved.
		for (int band = 0; band < numOfBands; band++)
		{
			eqBand[band].handleBeingMoved = false;
		}
	}

	void AutoEQ::mouseDrag(const MouseEvent& event) 
	{
		for (int band = 0; band < numOfBands; band++)
		{
			// If this band is being moved
			if (eqBand[band].handleBeingMoved == true)
			{
				// Set mousePosition to a value between 0.0 and 1.0 based on the eq height.
				float mousePosition = event.y / eqHeight;

				// Invert mouse position.
				mousePosition = 1.0 - mousePosition; 

				// Convert the mouse position to a usbale gain value and ensure it is between -18 and 18.
				int mouseToGain = rangeRemapper.convert(mousePosition, 1.0, -0.0, -18, 18);
				mouseToGain <= -18 ? mouseToGain = -18 : NULL;
				mouseToGain >= 18 ? mouseToGain = 18 : NULL;

				// Convert the mouse position to a usable frequency and ensure it is between -20.0 and 20000.
				float mouseToFreq = eqBand[band].freqUIRange.convertFrom0to1((event.x / eqWidth));
				mouseToFreq <= 20.0 ? mouseToFreq = 20.0 : mouseToFreq = mouseToFreq;
				mouseToFreq >= 20000.0 ? mouseToFreq = 20000.0 : mouseToFreq = mouseToFreq;

				// Update band data based on the posiiton of the mouse, (this will update audio and UI).
				updateBandData(band, mouseToFreq, eqBand[band].sliderValues.qFactor.getValue(), mouseToGain);

				// Set the frequency and gain sliders to the new values.
				eqSlider[frequencySliderID].setValue(mouseToFreq, dontSendNotification);
				eqSlider[gainSliderID].setValue(mouseToGain, dontSendNotification);
			}
		}
	}

	void AutoEQ::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) 
	{
		for (int band = 0; band < numOfBands; band++)
		{
			// If this band is selected
			if (eqBand[band].selected == true)
			{
				// Based on the direction of the wheel, determine the increment value.
				int qincrement = 0;
				wheel.deltaY < 0.0 ? qincrement = 1 : qincrement = -1;

				// Get all current parameter values
				float frequency = eqBand[band].sliderValues.frequency.getValue();
				float qFactor = eqBand[band].sliderValues.qFactor.getValue();
				float gain = eqBand[band].sliderValues.gain.getValue();

				// Increment the Q factor value and ensure it is between 1 and 10
				qFactor += qincrement;
				qFactor <= 1 ? qFactor = 1 : NULL;
				qFactor >= 10 ? qFactor = 10 : NULL;

				// Update band data based with the new Q factor value, (this will update audio and UI).
				updateBandData(band, frequency, qFactor, gain);
			}
		}
	}

	void AutoEQ::mouseDoubleClick(const MouseEvent& event)
	{
		for (int band = 0; band < numOfBands; band++)
		{
			if (event.x >= eqBand[band].handlePosition.x && event.x <= (eqBand[band].handlePosition.x + 20))
			{
				if (event.y >= eqBand[band].handlePosition.y && event.y <= (eqBand[band].handlePosition.y + 20))
				{
					// If the mouse has double clicked on a handle, reset the gain of the band that handle belongs to, to 0.0.
					updateBandData(band, eqBand[band].sliderValues.frequency.getValue(), eqBand[band].sliderValues.qFactor.getValue(), 0.0);
				}
			}
		}
	}
	
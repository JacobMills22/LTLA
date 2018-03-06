#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/AudioEngine/AudioPanelObject.h"
#include "FilterProcesses.h"
#include "../Utilities/GainToDecimalConverter.h"

// BW = f0/Q available from : http://www.sengpielaudio.com/calculator-cutoffFrequencies.htm
// Bandwidth = centre frequency / Qfactor

class AutoEQ : public AudioPanelObject,
	public Slider::Listener,
	public Button::Listener,
	public Label::Listener,
	public MouseListener

{

public:

	AutoEQ()
	{

		for (int parameter = 0; parameter < numOfSliders; parameter++)
		{
			addAndMakeVisible(eqSlider[parameter]);
			eqSlider[parameter].setSliderStyle(Slider::RotaryVerticalDrag);
			eqSlider[parameter].addListener(this);
			eqSlider[parameter].setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

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
		eqSliderValueLabel[frequencySliderID].setEditable(true, true, false);
		eqSliderNameLabel[frequencySliderID].setText("Frequency",dontSendNotification);
				
		eqSlider[qFactorSliderID].setRange(0.8, 10.0, 0.1);
		eqSlider[qFactorSliderID].setValue(1.0, dontSendNotification);
		eqSliderValueLabel[qFactorSliderID].setText("1.0", dontSendNotification);
		eqSliderValueLabel[qFactorSliderID].setEditable(true, true, false);
		eqSliderNameLabel[qFactorSliderID].setText("Q Factor", dontSendNotification);
						
		eqSlider[gainSliderID].setRange(-18.0, 18.0, 0.5);
		eqSlider[gainSliderID].setValue(0.0, dontSendNotification);
		eqSliderValueLabel[gainSliderID].setText("0.0 dB", dontSendNotification);
		eqSliderValueLabel[gainSliderID].setEditable(true, true, false);
		eqSliderNameLabel[gainSliderID].setText("Gain", dontSendNotification);
				
		addAndMakeVisible(bypassButton);
		bypassButton.addListener(this);
		bypassButton.setButtonText("Bypass");

		initialiseEQBands();
				
	}

		void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
		{
			eqSampleRate = sampleRate;

			for (int band = 0; band < numOfBands; band++)
			{
				eqBand[band].buffer.setSize(2, samplesPerBlockExpected, false, false, false);
			}
		}

		void process(AudioSampleBuffer &buffer)
		{
			if (bypassState == false)
			{
				float* outputL = buffer.getWritePointer(0);
				float* outputR = buffer.getWritePointer(1);

				for (int band = 0; band < numOfBands; band++)
				{
					eqBand[band].buffer.makeCopyOf(buffer, true);
				}

				for (int sample = 0; sample < buffer.getNumSamples(); sample++)
				{
					outputL[sample] = 0.0;
					outputR[sample] = 0.0;
				}

				for (int band = 0; band < 4; band++)
				{
					eqBand[band].peakingFilter.process(eqBand[band].buffer);

					float* processedSampleL = eqBand[band].buffer.getWritePointer(0);
					float* processedSampleR = eqBand[band].buffer.getWritePointer(1);

					for (int sample = 0; sample < buffer.getNumSamples(); sample++)
					{
						outputL[sample] += (processedSampleL[sample] * 0.25);
						outputR[sample] += (processedSampleR[sample] * 0.25);
					}
				}
			}
		}

		void resized() override
		{
			eqLeft = getWidth() * 0.01;
			eqRight = (getWidth() * 0.7) + eqLeft;

			bypassButton.setBounds(eqRight + 1, -5, 100, 30);

			eqSlider[frequencySliderID].setBounds(eqRight + 50, getHeight() * 0.08, 50, 50);
			eqSlider[qFactorSliderID].setBounds(eqRight + 50, getHeight() * 0.28, 50, 50);
			eqSlider[gainSliderID].setBounds(eqRight + 50, getHeight() * 0.48, 50, 50);

			eqSliderNameLabel[frequencySliderID].setBounds(eqSlider[frequencySliderID].getX() - 45, eqSlider[frequencySliderID].getY() + 15, 55, 20);
			eqSliderNameLabel[qFactorSliderID].setBounds(eqSlider[qFactorSliderID].getX() - 45, eqSlider[qFactorSliderID].getY() + 15, 50, 20);
			eqSliderNameLabel[gainSliderID].setBounds(eqSlider[gainSliderID].getX() - 45, eqSlider[gainSliderID].getY() + 15, 50, 20);

			eqSliderValueLabel[frequencySliderID].setBounds(eqSlider[frequencySliderID].getX() + 45, eqSlider[frequencySliderID].getY() + 15, 50, 20);
			eqSliderValueLabel[qFactorSliderID].setBounds(eqSlider[qFactorSliderID].getX() + 45, eqSlider[qFactorSliderID].getY() + 15, 50, 20);
			eqSliderValueLabel[gainSliderID].setBounds(eqSlider[gainSliderID].getX() + 45, eqSlider[gainSliderID].getY() + 15, 50, 20);
		}

		void paint(Graphics &g) override
		{
			eqLeft = getWidth() * 0.01;
			eqRight = (getWidth() * 0.7) + eqLeft;
			float eqCentreY = ((getHeight() * 0.65) * 0.5);
			float eqWidth = eqRight - eqLeft;
			float eqHeight = getHeight() * 0.65;

			Path eqCurve;
			juce::Rectangle<float> eqBackground;
			juce::Rectangle<float> eqInvisibleBorder[3];
			Point<float> mainEQLine[2];

			mainEQLine[eqLineStart].setXY(eqLeft, eqCentreY);

			for (int band = 0; band < numOfBands; band++)
			{
				eqBand[band].uiGraphPoints[eqLineBandCentre].setXY((eqWidth * eqBand[band].uiCentreFreqModifier) + eqLeft, eqHeight * eqBand[band].uiGainModifierCurve);
				eqBand[band].uiGraphPoints[eqLineBandStart].setXY(eqBand[band].uiGraphPoints[eqLineBandCentre].getX() - (eqWidth * eqBand[band].uiBandWidthModifier), eqCentreY);
				eqBand[band].uiGraphPoints[eqLineBandEnd].setXY(eqBand[band].uiGraphPoints[eqLineBandCentre].getX() + (eqWidth * eqBand[band].uiBandWidthModifier), eqCentreY);
			}
			mainEQLine[eqLineEnd].setXY(eqRight, eqCentreY);

			eqCurve.clear();

			g.setColour(Colours::dimgrey.darker(1.5));
			eqBackground.setBounds(eqLeft, 0, getWidth() * 0.7, getHeight() * 0.65);
			g.fillRect(eqBackground);
			g.setColour(Colours::black);
			g.drawRect(eqBackground);

			g.setColour(Colours::white);
			eqCurve.startNewSubPath(mainEQLine[eqLineStart]);
			eqCurve.lineTo(eqBand[0].uiGraphPoints[eqLineBandStart]);
			eqCurve.cubicTo(eqBand[0].uiGraphPoints[eqLineBandStart], eqBand[0].uiGraphPoints[eqLineBandCentre], eqBand[0].uiGraphPoints[eqLineBandEnd]);
			eqCurve.lineTo(eqBand[1].uiGraphPoints[eqLineBandStart]);
			eqCurve.cubicTo(eqBand[1].uiGraphPoints[eqLineBandStart], eqBand[1].uiGraphPoints[eqLineBandCentre], eqBand[1].uiGraphPoints[eqLineBandEnd]);
			eqCurve.lineTo(eqBand[2].uiGraphPoints[eqLineBandStart]);
			eqCurve.cubicTo(eqBand[2].uiGraphPoints[eqLineBandStart], eqBand[2].uiGraphPoints[eqLineBandCentre], eqBand[2].uiGraphPoints[eqLineBandEnd]);
			eqCurve.lineTo(eqBand[3].uiGraphPoints[eqLineBandStart]);
			eqCurve.cubicTo(eqBand[3].uiGraphPoints[eqLineBandStart], eqBand[3].uiGraphPoints[eqLineBandCentre], eqBand[3].uiGraphPoints[eqLineBandEnd]);

			eqCurve.lineTo(mainEQLine[eqLineEnd]);

			PathStrokeType strokeType(1.0, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded);
			g.setColour(Colours::lightblue);
			g.setOpacity(0.5);
			g.fillPath(eqCurve);
			g.setColour(Colours::white);
			g.setOpacity(1.0);
			g.strokePath(eqCurve, strokeType);

			eqInvisibleBorder[0].setBounds(eqRight, 0, 100, eqHeight * 1.2);
			eqInvisibleBorder[1].setBounds(0, 0, eqLeft, eqHeight * 1.2);
			eqInvisibleBorder[2].setBounds(0, eqHeight, eqWidth * 1.25, 20);
			
			for (int band = 0; band < numOfBands; band++)
			{
				eqBand[band].handlePosition.x = eqBand[band].uiGraphPoints[eqLineBandCentre].getX() - 10;
				eqBand[band].handlePosition.y = (eqHeight * eqBand[band].uiGainModifierHandle) - 10;

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

			g.setColour(Colours::darkgrey.darker());
			g.fillRect(eqInvisibleBorder[0]);
			g.fillRect(eqInvisibleBorder[1]);
			g.fillRect(eqInvisibleBorder[2]);
		}

		void sliderValueChanged(Slider* slider) override
		{
			for (int band = 0; band < numOfBands; band++)
			{
				if (eqBand[band].selected == true)
				{
					double centreFrequency = eqSlider[frequencySliderID].getValue();
					double qFactor = eqSlider[qFactorSliderID].getValue();
					double gainSliderValue = eqSlider[gainSliderID].getValue();

					if (isFilterSafe(centreFrequency, qFactor) == true)
					{
						if (slider == &eqSlider[frequencySliderID])
						{
							eqBand[band].peakingFilter.setCutoff(centreFrequency, eqSampleRate, qFactor, gainSliderValue);
							eqBand[band].sliderValues.frequency = centreFrequency;
							eqSliderValueLabel[frequencySliderID].setText((String)centreFrequency + " Hz", dontSendNotification);
						}
						else if (slider == &eqSlider[qFactorSliderID])
						{
							eqBand[band].peakingFilter.setCutoff(centreFrequency, eqSampleRate, qFactor, gainSliderValue);
							eqBand[band].sliderValues.qFactor = qFactor;
							eqSliderValueLabel[qFactorSliderID].setText((String)qFactor, dontSendNotification);
						}
						else if (slider == &eqSlider[gainSliderID])
						{
							eqBand[band].sliderValues.gain = gainSliderValue;
							eqBand[band].peakingFilter.setCutoff(centreFrequency, eqSampleRate, qFactor, gainSliderValue);
							eqSliderValueLabel[gainSliderID].setText((String)gainSliderValue + " dB", dontSendNotification);
							eqBand[band].audioGain = gainToDecConv.convertGainToDecimal(gainSliderValue);
						}

						//NewValue = (((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin
						eqBand[band].uiCentreFreqModifier = eqBand[band].freqUIRange.convertTo0to1(centreFrequency);
						eqBand[band].uiBandWidthModifier = (centreFrequency / qFactor) / 20000;

						eqBand[band].uiGainModifierCurve = (((gainSliderValue - -18.0) * (-0.5 - 1.5)) / (18.0 - -18.0)) + 1.5;
						eqBand[band].uiGainModifierHandle = (((gainSliderValue - -18.0) * (0.05 - 0.95)) / (18.0 - -18.0)) + 0.95;
					}
				}
			}
		}

		void labelTextChanged(Label* labelThatHasChanged) override
		{
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

		void buttonClicked(Button* button) override
		{
			if (button == &bypassButton)
			{
				bypassState = bypassButton.getToggleState();
			}
		}

		void mouseDown(const MouseEvent& event) override
		{
			for (int band = 0; band < numOfBands; band++)
			{
				if (event.x >= eqBand[band].handlePosition.x && event.x <= (eqBand[band].handlePosition.x + 20))
				{
					if (event.y >= eqBand[band].handlePosition.y && event.y <= (eqBand[band].handlePosition.y + 20))
					{
						selectEQBand(band);
					}
				}
			}
		}

		void selectEQBand(int bandID)
		{
			for (int band = 0; band < numOfBands; band++)
			{
				eqBand[band].selected = false;
			}

			eqBand[bandID].selected = true;
			eqSlider[frequencySliderID].setValue(eqBand[bandID].sliderValues.frequency, dontSendNotification);
			eqSlider[qFactorSliderID].setValue(eqBand[bandID].sliderValues.qFactor, dontSendNotification);
			eqSlider[gainSliderID].setValue(eqBand[bandID].sliderValues.gain, dontSendNotification);

			eqSliderValueLabel[frequencySliderID].setText((String)eqSlider[frequencySliderID].getValue() + " Hz", dontSendNotification);
			eqSliderValueLabel[qFactorSliderID].setText((String)eqSlider[qFactorSliderID].getValue(), dontSendNotification);
			eqSliderValueLabel[gainSliderID].setText((String)eqSlider[gainSliderID].getValue() + " dB", dontSendNotification);
			
			eqSlider[frequencySliderID].setColour(0x1001300, eqBand[bandID].colour);
			eqSlider[qFactorSliderID].setColour(0x1001300, eqBand[bandID].colour);
			eqSlider[gainSliderID].setColour(0x1001300, eqBand[bandID].colour);

		}

		bool isFilterSafe(double centreFreq, double qFactor)
		{
			double bandwidth = centreFreq / qFactor;
			double currentMaxWidth = centreFreq + (bandwidth * 0.5);
			double currentMinWidth = centreFreq - (bandwidth * 0.5);

			if (currentMaxWidth >= 10000)
			{
				DBG("Filter width exceeded max range");
				return false;
			}
			else
			{
				return true;
			}
		}

		void initialiseEQBands()
		{
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

			eqBand[0].peakingFilter.setCutoff(100, 48000, 2.0, 0.0);
			eqBand[1].peakingFilter.setCutoff(1000, 48000, 2.0, 0.0);
			eqBand[2].peakingFilter.setCutoff(2500, 48000, 2.0, 0.0);
			eqBand[3].peakingFilter.setCutoff(5000, 48000, 2.0, 0.0);

			for (int band = 0; band < numOfBands; band++)
			{
				eqBand[band].freqUIRange.start = 20.0;
				eqBand[band].freqUIRange.end = 20000.0;
				eqBand[band].freqUIRange.interval = 1.0;
				eqBand[band].freqUIRange.skew = 0.30;
			}
		}

	private:

		enum { numOfBands = 4 };
		enum { eqLineStart, eqLineEnd };
		enum { eqLineBandStart, eqLineBandCentre, eqLineBandEnd };
		
		struct UISliderValues
		{
			float frequency = 0.0;
			float qFactor = 2.0;
			float gain = 0.0;
		};

		struct EQBand
		{
			PeakingFilter peakingFilter;
			AudioSampleBuffer buffer;
			UISliderValues sliderValues;
			float uiBandWidthModifier = 0.125;
			float uiCentreFreqModifier = 0.5;
			float uiGainModifierCurve = 0.5;
			float uiGainModifierHandle = 0.5;
			Point<float> uiGraphPoints[3];
			Point<float> handlePosition;
			bool selected = false;
			Colour colour = Colours::blue;

			float audioGain = 0.0;
			bool isBoostingGain = true;
			NormalisableRange<float> freqUIRange;
		};
		
		EQBand eqBand[numOfBands];

		enum {frequencySliderID, qFactorSliderID, gainSliderID, numOfSliders};

		Slider eqSlider[numOfSliders];
		Label eqSliderValueLabel[numOfSliders];
		Label eqSliderNameLabel[numOfSliders];

		ToggleButton bypassButton;
		bool bypassState = false;


		float eqLeft = getWidth() * 0.1;
		float eqRight = (getWidth() * 0.7) + eqLeft;

		double eqSampleRate = 48000;

		GainToDecimalConverter gainToDecConv;

	};


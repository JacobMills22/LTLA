#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/AudioEngine/AudioPanelObject.h"
#include "FilterProcesses.h"


class AutoEQ : public AudioPanelObject,
	public Slider::Listener,
	public Button::Listener,
	public MouseListener

{

public:

	AutoEQ()
	{
		addAndMakeVisible(frequencySlider);
		frequencySlider.setSliderStyle(Slider::RotaryVerticalDrag);
		frequencySlider.setRange(20, 20000, 1);
		frequencySlider.addListener(this);
		frequencySlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
		frequencySlider.setValue(20, dontSendNotification);

		addAndMakeVisible(bandWidthSlider);
		bandWidthSlider.setSliderStyle(Slider::RotaryVerticalDrag);
		bandWidthSlider.setRange(0.0000001, 0.5, 0.000001);
		bandWidthSlider.addListener(this);
		bandWidthSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
		bandWidthSlider.setValue(0.0000001, dontSendNotification);
		
		addAndMakeVisible(gainSlider);
		gainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
		gainSlider.setRange(0.0, 1.0, 0.1);
		gainSlider.addListener(this);
		gainSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
		gainSlider.setValue(1.0);
		gainSlider.setValue(0.0, dontSendNotification);
		
		eqBand[0].uiCentreFreqModifier = 0.2;
		eqBand[1].uiCentreFreqModifier = 0.4;
		eqBand[2].uiCentreFreqModifier = 0.6;
		eqBand[3].uiCentreFreqModifier = 0.8;

		eqBand[0].sliderValues.frequency = 4000;
		eqBand[1].sliderValues.frequency = 8000;
		eqBand[2].sliderValues.frequency = 12000;
		eqBand[3].sliderValues.frequency = 16000;

		eqBand[0].colour = Colours::red;
		eqBand[1].colour = Colours::green;
		eqBand[2].colour = Colours::blue;
		eqBand[3].colour = Colours::yellow;


			
	}

		void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
		{
			for (int band = 0; band < numOfBands; band++)
			{
				eqBand[band].buffer.setSize(2, samplesPerBlockExpected, false, false, false);
			}
		}

		void process(AudioSampleBuffer &buffer)
		{
			for (int band = 0; band < numOfBands; band++)
			{
				eqBand[band].buffer.makeCopyOf(buffer, false);

				eqBand[band].bandPassFilter.process(eqBand[band].buffer);

				float* outputL = buffer.getWritePointer(0);
				float* outputR = buffer.getWritePointer(1);

				float* processedBand0L = eqBand[band].buffer.getWritePointer(0);
				float* processedBand0R = eqBand[band].buffer.getWritePointer(1);

				for (int sample = 0; sample < buffer.getNumSamples(); sample++)
				{
					outputL[sample] += (processedBand0L[sample] * eqBand[band].audioGain);
					outputR[sample] += (processedBand0R[sample] * eqBand[band].audioGain);
				}
			}
		}

		void resized() override
		{
			eqLeft = getWidth() * 0.1;
			eqRight = (getWidth() * 0.7) + eqLeft;

			frequencySlider.setBounds(eqRight + 10, 0, 50, 50);
			bandWidthSlider.setBounds(eqRight + 10, getHeight() * 0.2, 50, 50);
			gainSlider.setBounds(eqRight + 10, getHeight() * 0.4, 50, 50);

		}

		void paint(Graphics &g) override
		{
			eqLeft = getWidth() * 0.1;
			eqRight = (getWidth() * 0.7) + eqLeft;
			float eqCentreY = ((getHeight() * 0.65) * 0.5);
			float eqWidth = eqRight - eqLeft;
			float eqHeight = getHeight() * 0.65;

			mainEQLine[eqLineStart].setXY(eqLeft, eqCentreY);

			for (int band = 0; band < numOfBands; band++)
			{
				eqBand[band].uiGraphPoints[eqLineBandCentre].setXY((eqWidth * eqBand[band].uiCentreFreqModifier) + eqLeft, eqHeight * eqBand[band].uiGainModifierCurve);
				eqBand[band].uiGraphPoints[eqLineBandStart].setXY(eqBand[band].uiGraphPoints[eqLineBandCentre].getX() - (eqWidth * eqBand[band].uiBandWidthModifier), eqCentreY);
				eqBand[band].uiGraphPoints[eqLineBandEnd].setXY(eqBand[band].uiGraphPoints[eqLineBandCentre].getX() + (eqWidth * eqBand[band].uiBandWidthModifier), eqCentreY);
			}
			mainEQLine[eqLineEnd].setXY(eqRight, eqCentreY);

			eqCurve.clear();

			g.setColour(Colours::black);
			eqBackground.setBounds(eqLeft, 0, getWidth() * 0.7, getHeight() * 0.65);
			g.fillRect(eqBackground);

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
					g.setColour(Colours::grey);
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
					if (slider == &frequencySlider)
					{
						double cutoffValue = (frequencySlider.getValue() - 0.0) / (20000.0 - 0.0);
						eqBand[band].bandPassFilter.setFirstCoefficient(cutoffValue);
						eqBand[band].uiCentreFreqModifier = cutoffValue;
						eqBand[band].sliderValues.frequency = frequencySlider.getValue();
					}
					else if (slider == &bandWidthSlider)
					{
						eqBand[band].bandPassFilter.setBandWidth(bandWidthSlider.getValue());
						eqBand[band].uiBandWidthModifier = bandWidthSlider.getValue() - 0.0000001 / (0.5 - 0.0000001);
						eqBand[band].sliderValues.bandwidth = bandWidthSlider.getValue();

					}
					else if (slider == &gainSlider)
					{
						//NewValue = (((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin
						eqBand[band].uiGainModifierCurve = (((gainSlider.getValue() - 0.0) * (1.5 - -0.5)) / (1.0 - 0.0)) + -0.5;
						eqBand[band].uiGainModifierHandle = (((gainSlider.getValue() - 0.0) * (0.95 - 0.05)) / (1.0 - 0.0)) + 0.05;
						eqBand[band].sliderValues.gain = gainSlider.getValue();
						eqBand[band].audioGain = gainSlider.getValue() * 10.0;

					}
				}
			}
		}

		void buttonClicked(Button* button) override
		{

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
			frequencySlider.setValue(eqBand[bandID].sliderValues.frequency, dontSendNotification);
			bandWidthSlider.setValue(eqBand[bandID].sliderValues.bandwidth, dontSendNotification);
			gainSlider.setValue(eqBand[bandID].sliderValues.gain, dontSendNotification);

			frequencySlider.setColour(0x1001300, eqBand[bandID].colour);
			bandWidthSlider.setColour(0x1001300, eqBand[bandID].colour);
			gainSlider.setColour(0x1001300, eqBand[bandID].colour);

		}

	private:

		enum { numOfBands = 4 };
		enum { eqLineStart, eqLineEnd };
		enum { eqLineBandStart, eqLineBandCentre, eqLineBandEnd };
		
		struct UISliderValues
		{
			float frequency = 0.0;
			float bandwidth = 0.01;
			float gain = 0.5;
		};

		struct EQBand
		{
			BandPassFilter bandPassFilter;
			AudioSampleBuffer buffer;
			UISliderValues sliderValues;
			float uiBandWidthModifier = 0.01;
			float uiCentreFreqModifier = 0.5;
			float uiGainModifierCurve = 0.5;
			float uiGainModifierHandle = 0.5;
			Point<float> uiGraphPoints[3];
			Point<float> handlePosition;
			bool selected = false;
			Colour colour = Colours::blue;

			float audioGain = 0.0;
		};


		EQBand eqBand[numOfBands];

		Slider frequencySlider;
		Slider bandWidthSlider;
		Slider gainSlider;

		Rectangle<float> eqBackground;
		Rectangle<float> eqInvisibleBorder[3];

		Path eqCurve;

		float eqLeft = getWidth() * 0.1;
		float eqRight = (getWidth() * 0.7) + eqLeft;

		Point<float> mainEQLine[2];

	};


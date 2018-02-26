#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/AudioEngine/AudioPanelObject.h"
#include "FilterProcesses.h"

class AutoEQ : public AudioPanelObject,
	public Slider::Listener,
	public Button::Listener
{

public:

	AutoEQ()
	{
		for (int band = 0; band < numOfBands; band++)
		{
			addAndMakeVisible(frequencySlider[band]);
			frequencySlider[band].setSliderStyle(Slider::RotaryVerticalDrag);
			frequencySlider[band].setRange(20, 20000, 1);
			frequencySlider[band].addListener(this);
			frequencySlider[band].setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

			addAndMakeVisible(bandWidthSlider[band]);
			bandWidthSlider[band].setSliderStyle(Slider::RotaryVerticalDrag);
			bandWidthSlider[band].setRange(0.0000001, 0.5, 0.000001);
			bandWidthSlider[band].addListener(this);
			bandWidthSlider[band].setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

			addAndMakeVisible(gainSlider[band]);
			gainSlider[band].setSliderStyle(Slider::RotaryVerticalDrag);
			gainSlider[band].setRange(-1.0, 3.0, 0.1);
			gainSlider[band].addListener(this);
			gainSlider[band].setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

		}
	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		eqBand[0].buffer.setSize(2, samplesPerBlockExpected, false, false, false);
	}

	void process(AudioSampleBuffer &buffer)
	{
		eqBand[0].buffer.makeCopyOf(buffer, false);

		eqBand[0].bandPassFilter.process(eqBand[0].buffer);

		float* outputL = buffer.getWritePointer(0);
		float* outputR = buffer.getWritePointer(1);

		float* processedBand0L = eqBand[0].buffer.getWritePointer(0);
		float* processedBand0R = eqBand[0].buffer.getWritePointer(1);

		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			outputL[sample] += (processedBand0L[sample] * 10.0);
			outputR[sample] += (processedBand0R[sample] * 10.0);
		}
	}

	void resized() override
	{
		frequencySlider[0].setBounds(eqRight + 10, 0, 50, 50);
		bandWidthSlider[0].setBounds(eqRight + 10, getHeight() * 0.2, 50, 50);
		gainSlider[0].setBounds(eqRight + 10, getHeight() * 0.4, 50, 50);

	}

	void paint(Graphics &g) override
	{
		eqLeft = getWidth() * 0.1;
		eqRight = (getWidth() * 0.7) + eqLeft;
		float eqCentreY = ((getHeight() * 0.05) + (getHeight() * 0.59) * 0.5) + 1.0;
		float eqWidth = eqRight - eqLeft;

		/*
		eqLineStart.setXY(eqLeft, eqCentreY);
		
		eqLineBand1Start.setXY(eqWidth * 0.45, eqCentreY);
		eqLineBand1Centre.setXY(eqWidth * 0.5, eqCentreY * 0.1);
		eqLineBand1End.setXY(eqWidth * 0.55, eqCentreY);

		eqLineEnd.setXY(eqRight, eqCentreY);
		*/

		//eqBand[0].

		mainEQLine[eqLineStart].setXY(eqLeft, eqCentreY);

		
		eqBand[0].uiGraphPoints[eqLineBandCentre].setXY((eqWidth * eqBand[0].uiCentreFreqModifier) + eqLeft, eqCentreY * eqBand[0].uiGainModifier);
		eqBand[0].uiGraphPoints[eqLineBandStart].setXY(eqBand[0].uiGraphPoints[eqLineBandCentre].getX() - (eqWidth * eqBand[0].uiBandWidthModifier), eqCentreY);
		eqBand[0].uiGraphPoints[eqLineBandEnd].setXY(  eqBand[0].uiGraphPoints[eqLineBandCentre].getX() + (eqWidth * eqBand[0].uiBandWidthModifier), eqCentreY);

		mainEQLine[eqLineEnd].setXY(eqRight, eqCentreY);

		eqCurve.clear();

		g.fillAll(Colours::green);

		g.setColour(Colours::black);
		eqBackground.setBounds(eqLeft, getHeight() * 0.05, getWidth() * 0.7, getHeight() * 0.59);
		g.fillRect(eqBackground);

		g.setColour(Colours::white);
		eqCurve.startNewSubPath(mainEQLine[eqLineStart]);
		eqCurve.lineTo(eqBand[0].uiGraphPoints[eqLineBandStart]);
		eqCurve.cubicTo(eqBand[0].uiGraphPoints[eqLineBandStart], eqBand[0].uiGraphPoints[eqLineBandCentre], eqBand[0].uiGraphPoints[eqLineBandEnd]);
		eqCurve.lineTo(mainEQLine[eqLineEnd]);
				
		PathStrokeType strokeType(2.0, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded);
		g.strokePath(eqCurve, strokeType);

		eqInvisibleBorder[0].setBounds(eqRight, getHeight() * 0.05, 100, getHeight() * 0.59);
		eqInvisibleBorder[1].setBounds(0, getHeight() * 0.05, eqLeft, getHeight() * 0.59);

		g.setColour(Colours::darkgrey.darker());
		g.fillRect(eqInvisibleBorder[0]);
		g.fillRect(eqInvisibleBorder[1]);

		
	}

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &frequencySlider[0])
		{
			double cutoffValue = (frequencySlider[0].getValue() - 0.0) / (20000.0 - 0.0);
			eqBand[0].bandPassFilter.setFirstCoefficient(cutoffValue);
			eqBand[0].uiCentreFreqModifier = cutoffValue;
			eqBand[0].bandPassFilter.printCutOffFreqInHz(48000);
		}
		else if (slider == &bandWidthSlider[0])
		{
			eqBand[0].bandPassFilter.setBandWidth(bandWidthSlider[0].getValue());
			eqBand[0].bandPassFilter.printBandWidthInHz();
			eqBand[0].uiBandWidthModifier = bandWidthSlider[0].getValue() - 0.0000001 / (0.5 - 0.0000001);
		}
		else if (slider == &gainSlider[0])
		{
			eqBand[0].uiGainModifier = gainSlider[0].getValue();
			DBG((String)eqBand[0].uiGainModifier);
		}
	}

	void buttonClicked(Button* button) override
	{

	}

private:

	enum { numOfBands = 1 };
	enum { eqLineStart, eqLineEnd};
	enum { eqLineBandStart, eqLineBandCentre, eqLineBandEnd, };

	struct EQBand
	{
		BandPassFilter bandPassFilter;
		AudioSampleBuffer buffer;
		float uiBandWidthModifier = 0.1;
		float uiCentreFreqModifier = 0.5;
		float uiGainModifier = 0.0;
		Point<float> uiGraphPoints[3];
	};

	EQBand eqBand[4];

	Slider frequencySlider[numOfBands];
	Slider bandWidthSlider[numOfBands];
	Slider gainSlider[numOfBands];
	
	Rectangle<float> eqBackground;
	Rectangle<float> eqInvisibleBorder[2];
	
	Path eqCurve;

	float eqLeft = getWidth() * 0.1;
	float eqRight = (getWidth() * 0.7) + eqLeft;

	Point<float> mainEQLine[2];



};
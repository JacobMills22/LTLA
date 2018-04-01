#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/AudioEngine/AudioPanelObject.h"
#include "FilterProcesses.h"
#include "../Utilities/RangeRemapper.h"

/** This class represents the whole auto EQ module,
and contains four bands each containing one peaking filter 
each with variable cutoff, Q factor and gain. */

class AutoEQ : public AudioPanelObject,
	public Slider::Listener,
	public Button::Listener,
	public Label::Listener,
	public MouseListener

{

public:

	/** Constructor */
	AutoEQ();
	
	/** Initialises alll EQ bands (audio and UI) to default posiitons.*/
	void initialiseEQBands(float sampleRate);

	// ========================
	// Audio Functions
	// ========================

	/** Prepare for audio playback, set filters samplerate, initialise buffers.*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);

	/** Process each peaking filter. */
	void process(AudioSampleBuffer &buffer);

	// ========================
	// UI Functions
	// ========================

	/** Sets the bounds of various components */
	void resized() override;

	/** Draws the main EQ curve, background and handles*/
	void paint(Graphics &g) override;

	/** Called when a slider or EQ handle is moved, 
		used to update peaking filter parameters.*/
	void sliderValueChanged(Slider* slider) override;

	/** Called when the user changes a labels text,
	used to set slider values from their value labels.*/
	void labelTextChanged(Label* labelThatHasChanged) override;

	/** Called whena  user clicks a button, used to toggle bypass state*/
	void buttonClicked(Button* button) override;

	// ========================
	// Additional Functions
	// ========================

	/** Used to update a single EQ band, updates both audio and UI*/
	void updateBandData(int band, double frequency, double qFactor, double gain);

	/** Called when a user selects an EQ band handle,
	used to assign sliders to the selected band. */
	void selectEQBand(int bandID);

	/** Called when a snapshot is fired, 
		used to update bypass state and band data*/
	void snapshotFired();

	/** Returns the auto eq valuetree*/
	ValueTree getValueTree();

	// ========================
	// Mouse Listener Functions
	// ========================

	/** Called when the mouse is clicked, used to select EQ bands */
	void mouseDown(const MouseEvent& event) override;

	/** Called when the mouse is click is released, 
		used to reset handle being moved states */
	void mouseUp(const MouseEvent& event) override;

	/** Called when the mouse is being clicked and dragged
		Used to move the EQ handles, calls updateBandData() */
	void mouseDrag(const MouseEvent& event) override;

	/** Called when the mouse wheel is moved, 
		used to set the Q Factor of the selected band */
	void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;

	/** Called when the mouse is double clicked,
	used to reset the gain of the selected band to 0.0*/
	void mouseDoubleClick(const MouseEvent& event) override;

	private:

		enum { numOfBands = 4 };
		enum { eqLineStart, eqLineEnd };
		enum { eqLineBandStart, eqLineBandCentre, eqLineBandEnd, BezierHandleLeft, BezierHandleRight};
		
		struct UISliderValues
		{
			Value frequency;
			Value qFactor;
			Value gain;
		};

		struct EQBand
		{
			PeakingFilter peakingFilter[2];
			UISliderValues sliderValues;
			float uiBandWidthModifier = 0.125;
			float uiCentreFreqModifier = 0.5;
			float uiGainModifierCurve = 0.5;
			float uiGainModifierHandle = 0.5;
			Point<float> uiGraphPoints[5];
			Point<float> handlePosition;
			bool selected = false;
			bool handleBeingMoved = false;
			Colour colour = Colours::blue;
			NormalisableRange<float> freqUIRange;
		};
		
		EQBand eqBand[numOfBands];

		enum {frequencySliderID, qFactorSliderID, gainSliderID, numOfSliders};

		Slider eqSlider[numOfSliders];
		Label eqSliderValueLabel[numOfSliders];
		Label eqSliderNameLabel[numOfSliders];
		ToggleButton bypassButton;
		bool bypassState = false;
		
		RangeRemapper<double> rangeRemapper;

		float eqLeft = getWidth() * 0.1;
		float eqRight = (getWidth() * 0.7) + eqLeft;

		double eqSampleRate = 48000;

		ValueTree autoEQValueTree;

		float eqWidth = 100.0;
		float eqHeight = 100.0;

	};


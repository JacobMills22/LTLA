#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define SKELETON_COUNT 6

class LTLA_GUI : public Component
{

public:
	// Constructor
	LTLA_GUI();


	//==============================================================================	
								/** Main GUI Functions */

/** Overrides JUCE paint function called in MainComponent. 
	Handles all the GUI drawing code.*/
	void paint(Graphics& g) override;

/** Overrides JUCE resized function called in MainComponent.
	Handles all the GUI positioning code.*/
	void resized() override;

/** Draws an ellipse to represent performers position. 
	Uses coordinates provided by KinectTracker  */
	void PaintTrackedEllipse(Graphics& g, float PositionX, float PositionY, Colour colour);

/** Draws a stage outline once calibrated */
	void PaintStage(Graphics& g);

	//==============================================================================	
							/** Object Specific Functions */

/** Callibrates the stage for PaintStage. 
	Called in MainComponent - ButtonClicked. */
	void CalibrateStageButtonPressed(Button* button, int TimesPressed, float LocationDataX, float LocationDataY);

	//==============================================================================	
							 /** Getters and Setters */

/** Sets Ellipse Coordinates. */
	void SetEllipseCoordinates(float PositionX, float PositionY, int SkeltonNum);

/** Sets Stage Callibration State, used to determine whether the stage should be drawn. */
	void SetStageCalibrationState(bool State);

/** Sets the stage Coordinates. */
	void SetStageCoordinates(int StagePosition, float x, float y);

/** Sets the tracking state of each performer. */
	void SetKinectTrackingState(int PerformerNum, bool State);

	//==============================================================================	

private:

	// Stage Drawing Variables
	bool StageCalibrated = false;
	bool TrackingState[2];

	enum { FrontLeft, FrontRight, BackRight, BackLeft, NumOfStagePositions };
	struct LTLACoordinates
	{
		float x = 0.00;
		float y = 0.00;
	};
	LTLACoordinates StageCoordinates[NumOfStagePositions];
	LTLACoordinates EllipseCoordinates[SKELETON_COUNT];

};
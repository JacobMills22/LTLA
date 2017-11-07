#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define SKELETON_COUNT 2

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



	//==============================================================================	
							 /** Getters and Setters */

/** Sets Ellipse Coordinates. */
	void SetEllipseCoordinates(float PositionX, float PositionY, int SkeltonNum);

/** Sets Stage Callibration State, used to determine whether the stage should be drawn. */
	void SetStageCalibrationState(bool State);

/** Gets the Stage Callibration State, used to determine whether the stage should be drawn. */
	bool GetStageCalibrationState();

/** Sets the stage Coordinates. */
	void SetStageCoordinates(int StagePosition, float x, float y);

/** Sets the tracking state of each performer. */
	void SetKinectTrackingState(int PerformerNum, bool State);

	//==============================================================================	

	enum { FrontLeft, FrontRight, BackRight, BackLeft, NumOfStagePositions };
	int StageCalibrationCounter = 0;
	int StageCalibrationInterval = 5;

private:

	// Stage Drawing Variables
	bool StageCalibrated = true;
	bool TrackingState[2];

	struct LTLACoordinates
	{
		float x = 0.00;
		float y = 0.00;
	};
	 LTLACoordinates StageCoordinates[NumOfStagePositions];
	 LTLACoordinates EllipseCoordinates[SKELETON_COUNT];

};
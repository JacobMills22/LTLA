#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/StageAreas.h"

#define SKELETON_COUNT 2

class LTLA_GUI : public Component,
			     public MouseListener
{

public:
	// Constructor
	LTLA_GUI();

	//==============================================================================	
								/** Main GUI Functions */

/** Overrides JUCE paint function called in MainComponent. 
	Handles all the GUI drawing code.*/
	void paint(Graphics& g) override;

	void PaintGrid(Graphics& g);

/** Overrides JUCE resized function called in MainComponent.
	Handles all the GUI positioning code.*/
	void resized() override;

/** Draws an ellipse to represent performers position. 
	Uses coordinates provided by KinectTracker  */
	void PaintTrackedEllipse(Graphics& g, float PositionX, float PositionY, Colour colour);

/** Draws a stage outline once calibrated */
	void PaintStage(Graphics& g);

/** Paint Stage Areas */
	void PaintStageAreas(Graphics &g);

	//==============================================================================	
							/** Object Specific Functions */

/** Converts Stage Coordinates to be in-line with the Grid*/
	void SnapStageToGrid();

/** Called when a mouse is clicked */
	void mouseDown(const MouseEvent &event);

	void mouseDrag(const MouseEvent& event);

	void mouseUp(const MouseEvent& event);

	void mouseDoubleClick(const MouseEvent &event);

	
	//==============================================================================	
							 /** Getters & Setters */

/** Sets Ellipse Coordinates. */
	void SetEllipseCoordinates(float PositionX, float PositionY, int SkeltonNum);

	/** STAGE */ 

/** Sets Stage Callibration State, used to determine whether the stage should be drawn. */
	void SetStageDrawingState(bool State);

/** Gets the Stage Callibration State, used to determine whether the stage should be drawn. */
	bool GetStageDrawingState();

/** Sets the stage Coordinates. */
	void SetStageCoordinates(int StagePosition, float x, float y);

/** *Sets the Edit Stage State */
	void SetStageEditState(bool State);

/** *Gets the Edit Stage State */
	bool GetStageEditState();

	/** STAGE AREAS */
	
/** *Sets the Edit Stage Area State */
	void SetStageAreaEditState(bool State);

/** *Gets the Edit Stage Area State */
	bool GetStageAreaEditState();

/** Sets an Index for the Currently Selected Area*/
	void SetCurrentlySelectedArea(int Index);

/** Gets an Index for the Currently Selected Area*/
	int GetCurrentlySelectedArea();

	/** GRID */ 

/** Sets the state of Grid Drawing*/
	void SetGridDrawingState(bool state);

/** Gets the state of Grid Drawing*/
	bool GetGridDrawingState();

/** Sets the state of Grid Snapping*/
	void SetGridSnappingState(bool state);

/** Gets the state of Grid Snapping*/
	bool GetGridSnappingState();

/** Sets the Increment Value of Grid Snapping*/
	void SetGridIncrement(int Value);

/** Gets the Increment Value of Grid Snapping*/
	int GetGridIncrement();

	/** TRACKING */

/** Sets the tracking state of each performer. */
	void SetKinectTrackingState(int PerformerNum, bool State);

	//==============================================================================	

	enum { FrontLeft, FrontRight, BackRight, BackLeft, NumOfStagePositions };
	int StageCalibrationCounter = 0;
	int StageCalibrationInterval = 5;

	StageArea MainStageArea;
	OwnedArray<StageArea> StageAreas;

private:

	// Stage Drawing Variables
	bool StageDrawingState = false;
	bool GridDrawingState = false;
	bool TrackingState[2];
	bool GridSnappingState = false;
	bool StageCornerSelected[NumOfStagePositions];
	bool StageEditState = false;
	bool StageAreaEditState = false;

	struct LTLACoordinates
	{
		float x = 0.00;
		float y = 0.00;
	};
	 LTLACoordinates StageCoordinates[NumOfStagePositions];
	 LTLACoordinates EllipseCoordinates[SKELETON_COUNT];

	 enum { StageFront, StageRight, StageBack, StageLeft, NumOfStageSides };

	 int GridIncrement = 15;
	 int SelectedAreaIndex = 0;
};
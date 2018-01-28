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

	~LTLA_GUI();

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

/** Called when a mouse is clicked 
	If Area edit State is Enabled, this will find the corner of the area that's currently
	selected and nearest to the mouse click so that the areas corners position can be updated.
	Works for both the Stage and its Areas.*/
	void mouseDown(const MouseEvent &event);

/** Called when the mouse is couble clicked
	If Area Edit State is Enabled, this will find the StageArea that was double clicked
	and then deselect the CurrentlySelected Stage area, select the area that was double clicked
	and also update the Index of the CurrentlySelectedArea for use of TrackingGUI. */
	void mouseDoubleClick(const MouseEvent &event);

/** Called when a mouse click is held down and moved.
	This will find the stage/areas corner which is selected and update its coordinates based
	on the mouses new position.	*/
	void mouseDrag(const MouseEvent& event);

/** Called when a mouse click is released.
	This will simply ensure that all corners are deselected.*/
	void mouseUp(const MouseEvent& event);
		
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

	void setStageAreaHasChangedState(bool state);

	bool hasStageAreaChanged();
	
	void setAudioPanelState(bool state);

	bool getAudioPanelState();

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

	bool doesAreaIDContainPerfomer(int performerID, int areaID);

	float getPerformerXPosInsideArea(int areaID, int performerID)
	{
		float areaLeft = StageAreas[areaID]->GetAreaPath().getBounds().getX();
		float areaRight = StageAreas[areaID]->GetAreaPath().getBounds().getRight();
		float performerX = EllipseCoordinates[performerID].x;

	 // return (performerX - AreaMinimum) / (Max - Min)
		return (performerX - areaLeft) / (areaRight - areaLeft);
	}

	void setValueTree(ValueTree ValueTree)
	{
		valueTree = ValueTree;
	}

	//==============================================================================	

	enum { FrontLeft, FrontRight, BackRight, BackLeft, Centre, NumOfStagePositions };
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
	bool audioPanelState = false;
	bool selectedAreaHasChanged = false;

	int GridIncrement = 15;
	int SelectedAreaIndex = 0;

	struct LTLACoordinates
	{
		float x = 0.00;
		float y = 0.00;
	};
	 LTLACoordinates StageCoordinates[NumOfStagePositions];
	 LTLACoordinates EllipseCoordinates[SKELETON_COUNT];

	 enum { StageFront, StageRight, StageBack, StageLeft, NumOfStageSides };

	// ScopedPointer<ValueTree> valueTree;
	ValueTree valueTree;


};
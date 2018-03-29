#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/Components/StageAreas.h"

#define SKELETON_COUNT 2

class TrackingGUI : public Component,
			     public MouseListener
{

public:
	// Constructor
	TrackingGUI();

	~TrackingGUI();

	//==============================================================================	
								/** Main GUI Functions */

/** Overrides JUCE paint function called in MainComponent. 
	Handles all the GUI drawing code.*/
	void paint(Graphics& g) override;

	void paintGrid(Graphics& g);

/** Overrides JUCE resized function called in MainComponent.
	Handles all the GUI positioning code.*/
	void resized() override;

/** Draws an ellipse to represent performers position. 
	Uses coordinates provided by KinectTracker  */
	void paintTrackedEllipse(Graphics& g, float PositionX, float PositionY, Colour colour);

/** Draws a stage outline once calibrated */
	void paintStage(Graphics& g);

/** Paint Stage Areas */
	void paintStageAreas(Graphics &g);

	//==============================================================================	
							/** Object Specific Functions */

/** Converts Stage Coordinates to be in-line with the Grid*/
	void snapStageToGrid();

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

	void mouseMove(const MouseEvent& event);

		
	//==============================================================================	
							 /** Getters & Setters */

/** Sets Ellipse Coordinates. */
	void setEllipseCoordinates(float positionX, float positionY, int skeltonNum);

	/** STAGE */ 

/** Sets Stage Callibration State, used to determine whether the stage should be drawn. */
	void setStageDrawingState(bool state);

/** Gets the Stage Callibration State, used to determine whether the stage should be drawn. */
	bool getStageDrawingState();

/** Sets the stage Coordinates. */
	void setStageCoordinates(int stagePosition, float x, float y);

/** *Sets the Edit Stage State */
	void setStageEditState(bool state);

/** *Gets the Edit Stage State */
	bool getStageEditState();

	/** STAGE AREAS */
	
/** *Sets the Edit Stage Area State */
	void setStageAreaEditState(bool state);

/** *Gets the Edit Stage Area State */
	bool getStageAreaEditState();

/** Sets an Index for the Currently Selected Area*/
	void setCurrentlySelectedArea(int index);

/** Gets an Index for the Currently Selected Area*/
	int getCurrentlySelectedArea();

/** Sets the stage area has changed state */
	void setStageAreaHasChangedState(bool state);

/** Returns true if the stage area has been changed*/
	bool hasStageAreaChanged();
	
/** Sets whether the audio panel is currently displayed*/
	void setAudioPanelState(bool state);

/** Returns true if the audio panel is currently displayed*/
	bool getAudioPanelState();

	void deselectAllStageAreas()
	{
		for (int area = 0; area < stageAreas.size(); area++)
		{
			stageAreas[area]->setAreaSelectedState(false);
		}
	}

	/** GRID */ 

/** Sets the state of Grid Drawing*/
	void setGridDrawingState(bool state);

/** Gets the state of Grid Drawing*/
	bool getGridDrawingState();

/** Sets the state of Grid Snapping*/
	void setGridSnappingState(bool state);

/** Gets the state of Grid Snapping*/
	bool getGridSnappingState();

/** Sets the Increment Value of Grid Snapping*/
	void setGridIncrement(int Value);

/** Gets the Increment Value of Grid Snapping*/
	int getGridIncrement();

	/** TRACKING */

/** Sets the tracking state of each performer. */
	void setKinectTrackingState(int performerNum, bool state);

/** Retuns true if the performerID is currently inside the AreaID*/
	bool doesAreaIDContainPerfomer(int performerID, int areaID);

/** Returns the performers x position in relation to the AreaID, this is generally used for the autoPanner*/
	float getPerformerXPosInsideArea(int areaID, int performerID);

/** Sets the valuetree for this class to use */
	void setValueTree(ValueTree valueTree);

	//==============================================================================	

	enum { frontLeft, frontRight, backRight, backLeft, centre, numOfStagePositions };
	int stageCalibrationCounter = 0;
	int stageCalibrationInterval = 5;

	//ScopedPointer<StageArea> mainStageArea;
	StageArea mainStageArea;
	OwnedArray<StageArea> stageAreas;
	Value numOfStageAreas;

private:

	// Stage Drawing Variables
	bool stageDrawingState = false;
	bool gridDrawingState = false;
	bool trackingState[2];
	bool gridSnappingState = false;
	bool stageCornerSelected[numOfStagePositions];
	bool stageEditState = false;
	bool stageAreaEditState = false;
	bool audioPanelState = false;
	bool selectedAreaHasChanged = false;

	int gridIncrement = 15;
	int selectedAreaIndex = 0;

	struct LTLACoordinates
	{
		float x = 0.00;
		float y = 0.00;
	};
	 LTLACoordinates stageCoordinates[numOfStagePositions];
	 LTLACoordinates ellipseCoordinates[SKELETON_COUNT];

	 enum { stageFront, stageRight, stageBack, stageLeft, numOfStageSides };

	// ScopedPointer<ValueTree> valueTree;
	ValueTree valueTree;


};
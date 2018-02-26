#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioEngine\AudioPanel.h"

class StageArea : public Component
		
{
public:

/** Creates an Area on the Stage at Default Position */
	StageArea();

	~StageArea();

	//==============================================================================	
	/** Main Functions*/

/** Draws the Area in Either Normal or Edit Mode.
	Also Responds to the Performers Position Being Inside*/
	void drawArea(Graphics &g, bool Editmode);

/** Updates the Coordinates of one of the Areas Corners*/
	void updateArea( int Corner, float X, float Y);
	
	//==============================================================================	
	/** Getters & Setters*/
	
/** Gets the X Coordinate of one of the Areas Corners */
	float getX(int CornerID);

/** Gets the Y Coordinate of one of the Areas Corners */
	float getY(int CornerID);

/** Sets the X coordinate value of one of the areas corners */
	void setX(int CornerID, float value);

/** Sets the Y coordinate value of one of the areas corners */
	void setY(int CornerID, float value);

/** Gets the length difference between the centre X coordinate and the specified corner X coordinate */
	float getDifferenceX(int CornerID);

/** Gets the length difference between the centre Y coordinate and the specified corner Y coordinate */
	float getDifferenceY(int CornerID);

/** Sets the length difference between the centre X coordinate and the specified corner X coordinate */
	void setDifferenceX(int cornerID, float value);

/** Sets the length difference between the centre Y coordinate and the specified corner Y coordinate */
	void setDifferenceY(int cornerID, float value);

/** Updates the UI bounds for scaling */
	void updateTrackingGUIWidthAndHeight(float width, float height);
		
/** Gets the Path Used to Draw the Area*/
	Path getAreaPath();

/** Sets The Selection State of one of the Areas Corner*/
	void setCornerSelectedState(int Corner, bool State);

/** Gets The Selection State of one of the Areas Corner*/
	bool getCornerSelectedState(int Corner);

/** Sets the Selection State of the Area*/
	void setAreaSelectedState(bool State);

/** Gets the Selection State of the Area*/
	bool getAreaSelectedState();

/** Sets the colour of the area */
	void setAreaColour(Colour colour);

/** Gets the colour of the area */
	Colour getAreaColour();

/** Sets the name of the area */
	void setAreaName(String name);

/** Gets the name of the area */
	String getAreaName();

/** Sets the active state of the area*/
	void setActive(bool state);

/** Gets the active state of the area*/
	bool getActiveState();
	
/** returns this classes valuetree */
	ValueTree getValueTree();

/** Sets the index value of the valuetree */
	void setValueTreeChildIndex(int index);

/** Gets the index value of the valuetree */
	int getValueTreeChildIndex();

	

	//==============================================================================	

	enum { frontLeft, frontRight, backRight, backLeft, centre, numOfAreaCorners};

private:

    ValueTree stageAreaValueTree;

	struct AreaCorner
	{
		bool selected = false;
		float differenceX = 0.00;
		float differenceY = 0.00;
		Value x;
		Value y;
	};

	AreaCorner areaPosition[numOfAreaCorners];

	struct Coordinates
	{
		float x = 0.00;
		float y = 0.00;
	};
	Coordinates performerCoordinates;

	Path areaPath;
	Colour areaColour = Colours::floralwhite;

	float trackingGUIWidth = 900.0;
	float trackingGUIHeight = 500.0;
	bool areaSelectedState = false;
	int valueTreeChildIndex = 0;

	Value areaName;
	Value activeState;

}; 
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
	void DrawArea(Graphics &g, bool Editmode);

/** Updates the Coordinates of one of the Areas Corners*/
	void UpdateArea( int Corner, float X, float Y);
	
	//==============================================================================	
	/** Getters & Setters*/
	
/** Gets the X Coordinate of one of the Areas Corners */
	float GetX(int CornerID);

/** Gets the Y Coordinate of one of the Areas Corners */
	float GetY(int CornerID);

	void setX(int CornerID, float value);

	void setY(int CornerID, float value);

	float getDifferenceX(int CornerID);

	float getDifferenceY(int CornerID);

	void setDifferenceX(int cornerID, float value);

	void setDifferenceY(int cornerID, float value);

	void updateTrackingGUIWidthAndHeight(float width, float height);

	
/** Gets the Path Used to Draw the Area*/
	Path GetAreaPath();

/** Sets The Selection State of one of the Areas Corner*/
	void SetCornerSelectedState(int Corner, bool State);

/** Gets The Selection State of one of the Areas Corner*/
	bool GetCornerSelectedState(int Corner);

/** Sets the Selection State of the Area*/
	void SetAreaSelectedState(bool State);

/** Gets the Selection State of the Area*/
	bool GetAreaSelectedState();

	void SetAreaColour(Colour colour);

	Colour GetAreaColour();

	void setAreaName(String name);

	String getAreaName();

	ValueTree getValueTree()
	{
		return stageAreaValueTree;
	}

	//==============================================================================	

	enum { FrontLeft, FrontRight, BackRight, BackLeft, Centre, NumOfAreaCorners};

private:

    ValueTree stageAreaValueTree;

	struct AreaCorner
	{
		bool Selected = false;
		float differenceX = 0.00;
		float differenceY = 0.00;
	};

	AreaCorner AreaPosition[NumOfAreaCorners];

	struct Coordinates
	{
		float x = 0.00;
		float y = 0.00;
	};
	Coordinates PerformerCoordinates;

	Path AreaPath;
	Colour AreaColour = Colours::floralwhite;
	//String areaName;

	float trackingGUIWidth = 900.0;
	float trackingGUIHeight = 500.0;
	bool areaSelectedState = false;

}; 
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class StageArea : Component
{
public:

/** Creates an Area on the Stage at Default Position */
	StageArea();

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

	//==============================================================================	

	enum { FrontLeft, FrontRight, BackRight, BackLeft, NumOfAreaCorners};

private:

	struct AreaCorner
	{
		float x = 0.00;
		float y = 0.00;
		bool Selected = false;
	};
	AreaCorner AreaPosition[NumOfAreaCorners];

	struct Coordinates
	{
		float x = 0.00;
		float y = 0.00;
	};
	Coordinates PerformerCoordinates;
	
	Path AreaPath;
	
	bool AreaSelectedState = false;
};
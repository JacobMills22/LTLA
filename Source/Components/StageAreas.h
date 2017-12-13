#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioEngine\AudioPanel.h"

class StageArea : public Component,
				  public ValueTree::Listener
		
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

	 void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;

	 void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override
	 {

	 }

	 void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override
	 {

	 }

	 void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override
	 {

	 }

	 void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override
	 {

	 }
	
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

	void SetAreaColour(Colour colour);

	Colour GetAreaColour();

	//==============================================================================	

	enum { FrontLeft, FrontRight, BackRight, BackLeft, NumOfAreaCorners};

private:

ScopedPointer<ValueTree> valueTree;

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
	Colour AreaColour = Colours::floralwhite;
	
};
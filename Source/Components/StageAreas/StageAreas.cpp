
#include "StageAreas.h"

StageArea::StageArea() : stageAreaValueTree("StageArea")
{
	// Valuetree initialisation.

	areaColourString.setValue(areaColour.toString());

	stageAreaValueTree.setProperty("FrontLeftX", 50.0, nullptr);
	stageAreaValueTree.setProperty("FrontLeftY", 50.0, nullptr);
	stageAreaValueTree.setProperty("FrontRightX", 100.0, nullptr);
	stageAreaValueTree.setProperty("FrontRightY", 50.0, nullptr);
	stageAreaValueTree.setProperty("BackLeftX", 50.0, nullptr);
	stageAreaValueTree.setProperty("BackLeftY", 100.0, nullptr);
	stageAreaValueTree.setProperty("BackRightX", 100.0, nullptr);
	stageAreaValueTree.setProperty("BackRightY", 100.0, nullptr);
	stageAreaValueTree.setProperty("CentreX", 75.0, nullptr);
	stageAreaValueTree.setProperty("CentreY", 75.0, nullptr);

	stageAreaValueTree.setProperty("AreaName", "New Area", nullptr);
	stageAreaValueTree.setProperty("Colour", areaColourString, nullptr);
	stageAreaValueTree.setProperty("Active", true, nullptr);


	areaPosition[frontLeft].x.referTo(stageAreaValueTree.getPropertyAsValue("FrontLeftX", nullptr));
	areaPosition[frontLeft].y.referTo(stageAreaValueTree.getPropertyAsValue("FrontLeftY", nullptr));
	areaPosition[frontRight].x.referTo(stageAreaValueTree.getPropertyAsValue("FrontRightX", nullptr));
	areaPosition[frontRight].y.referTo(stageAreaValueTree.getPropertyAsValue("FrontRightY", nullptr));
	areaPosition[backLeft].x.referTo(stageAreaValueTree.getPropertyAsValue("BackLeftX", nullptr));
	areaPosition[backLeft].y.referTo(stageAreaValueTree.getPropertyAsValue("BackLeftY", nullptr));
	areaPosition[backRight].x.referTo(stageAreaValueTree.getPropertyAsValue("BackRightX", nullptr));
	areaPosition[backRight].y.referTo(stageAreaValueTree.getPropertyAsValue("BackRightY", nullptr));
	areaPosition[centre].x.referTo(stageAreaValueTree.getPropertyAsValue("CentreX", nullptr));
	areaPosition[centre].y.referTo(stageAreaValueTree.getPropertyAsValue("CentreY", nullptr));

	areaName.referTo(stageAreaValueTree.getPropertyAsValue("AreaName", nullptr));
	areaColourString.referTo(stageAreaValueTree.getPropertyAsValue("Colour", nullptr));
	activeState.referTo(stageAreaValueTree.getPropertyAsValue("Active", nullptr));

}

StageArea::~StageArea()
{
}

void StageArea::drawArea(Graphics &g, bool Editmode)
{
	// Clear the old path
	areaPath.clear();

	// If the stage area is active (hasn't been removed)
	if (activeState == true)
	{
		// Add a quadrilateral to the path using the coordinates.
		areaPath.addQuadrilateral(areaPosition[frontLeft].x.getValue(), areaPosition[frontLeft].y.getValue(),
			                      areaPosition[frontRight].x.getValue(), areaPosition[frontRight].y.getValue(),
			                      areaPosition[backRight].x.getValue(), areaPosition[backRight].y.getValue(),
			                      areaPosition[backLeft].x.getValue(), areaPosition[backLeft].y.getValue());
		
		// Fill the path with the areas colour.
		g.setColour(getAreaColour());
		g.fillPath(areaPath);

		// Draw a black outline around the area.
		g.setColour(Colours::black);
		g.setOpacity(1.0);
		g.strokePath(areaPath, PathStrokeType(1.0));

		// If edit mode is enabled
		if (Editmode == true)
		{
			// Draw a resize box around each corner and the cente of the area, 
			// set the colour of the box to white initially but update the colour to
			// blue if the corner is currently selected by the mouse.
			g.setColour(Colours::white);
			if (areaPosition[frontLeft].selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)areaPosition[frontLeft].x.getValue() - 5, (int)areaPosition[frontLeft].y.getValue() - 5, 10, 10, 1.0);

			g.setColour(Colours::white);
			if (areaPosition[frontRight].selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)areaPosition[frontRight].x.getValue() - 5, (int)areaPosition[frontRight].y.getValue() - 5, 10, 10, 1.0);

			g.setColour(Colours::white);
			if (areaPosition[backLeft].selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)areaPosition[backLeft].x.getValue() - 5, (int)areaPosition[backLeft].y.getValue() - 5, 10, 10, 1.0);

			g.setColour(Colours::white);
			if (areaPosition[backRight].selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)areaPosition[backRight].x.getValue() - 5, (int)areaPosition[backRight].y.getValue() - 5, 10, 10, 1.0);

			g.setColour(Colours::blue);
			if (areaPosition[centre].selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)areaPosition[centre].x.getValue() - 5, (int)areaPosition[centre].y.getValue() - 5, 10, 10, 1.0);

		}

		// If the area is selected draw a white outline around it.
		if (areaSelectedState == true)
		{
			g.setColour(Colours::white);
			g.strokePath(areaPath, PathStrokeType(1.0));
		}
	}
}

void StageArea::updateArea(int corner, float x, float y)
{
	if (activeState == true) // If the stage area is active (hasn't been removed)
	{
		if (corner == centre) // If the corner to update is the centre.
		{
			if (x >= 0 && x <= trackingGUIWidth) // If the x coordinate is inside the components bounds
			{
				areaPosition[centre].x = x; // Update the x coordinate of the centre box.

				for (int corner = 0; corner < numOfAreaCorners - 1; corner++)
				{
					// Also update the position of all corners so that the area moves as a whole.
					setX(corner, (x - areaPosition[corner].differenceX));
				}
			}
			else // If the x coordinate is not inside the components bounds and is either 
			{    // less than or equal to 0, set the x coordinate to 0 or
				 // greater than or equal to 0, set the x coordinate to the width of the component

				x <= 0 ? areaPosition[centre].x = 0.0 : areaPosition[centre].x = trackingGUIWidth;
			}

			if (y >= 0 && y <= trackingGUIHeight) // If the y coordinate is inside the components bounds
			{
				areaPosition[centre].y = y; // Update y coordinate of the centre box.
				
				for (int corner = 0; corner < numOfAreaCorners - 1; corner++)
				{
					// Also update the position of all corners so that the area moves as a whole.
					setY(corner, (y - areaPosition[corner].differenceY));
				}
			}
			else // If the y coordinate is not inside the components bounds and is either 
			{    // less than or equal to 0, set the y coordinate to 0 or
				 // greater than or equal to 0, set the y coordinate to the height of the component
				y <= 0 ? areaPosition[centre].y = 0.0 : areaPosition[centre].y = trackingGUIHeight;
			}
		}
		else // If the corner to update is not the centre.
		{
			if (x >= 0 && x <= trackingGUIWidth) // If the x coordinate is inside the components bounds
			{
				// Update x coordinate of the corner and centre box. 
				areaPosition[corner].x = x;
				areaPosition[centre].x = areaPath.getBounds().getCentreX();
			}
			else // If the x coordinate is not inside the components bounds
			{
				// Set the x coordinate to the far left or right of the components bounds.
				x <= 0 ? areaPosition[corner].x = 0.0 : areaPosition[corner].x = trackingGUIWidth;
			}

			if (y >= 0 && y <= trackingGUIHeight) // If the y coordinate is inside the components bounds
			{
				// Update y coordinate of the corner and centre box. 
				areaPosition[corner].y = y;
				areaPosition[centre].y = areaPath.getBounds().getCentreY();
			}
			else // If the y coordinate is not inside the components bounds
			{
				// Set the y coordinate to the far top or bottom of the components bounds.
				y <= 0 ? areaPosition[corner].y = 0.0 : areaPosition[corner].y = trackingGUIHeight;
			}
		}
	}
}

float StageArea::getX(int CornerID)
{
	return areaPosition[CornerID].x.getValue();
}
float StageArea::getY(int CornerID)
{
	return areaPosition[CornerID].y.getValue();
}

void StageArea::setX(int CornerID, float value)
{
	areaPosition[CornerID].x = value;
}

void StageArea::setY(int CornerID, float value)
{
	areaPosition[CornerID].y = value;
}

float StageArea::getDifferenceX(int CornerID)
{
	return areaPosition[CornerID].differenceX;
}

float StageArea::getDifferenceY(int CornerID)
{
	return areaPosition[CornerID].differenceY;
}

void StageArea::setDifferenceX(int cornerID, float value)
{
	areaPosition[cornerID].differenceX = value;
}

void StageArea::setDifferenceY(int cornerID, float value)
{
	areaPosition[cornerID].differenceY = value;
}

void StageArea::updateTrackingGUIWidthAndHeight(float width, float height)
{
	trackingGUIWidth = width;
	trackingGUIHeight = height;
}

Path StageArea::getAreaPath()
{
	return areaPath;
}

void StageArea::setCornerSelectedState(int Corner, bool State)
{
	areaPosition[Corner].selected = State;
}
bool StageArea::getCornerSelectedState(int Corner)
{
	return areaPosition[Corner].selected;
}

void StageArea::setAreaSelectedState(bool State)
{
	areaSelectedState = State;
}
bool StageArea::getAreaSelectedState()
{
	return areaSelectedState;
}

void StageArea::setAreaColour(Colour colour)
{
	areaColour = colour;
	areaColourString.setValue(colour.toString());
}

Colour StageArea::getAreaColour()
{
	return areaColour;
}

void StageArea::setAreaName(String name)
{
	areaName = name;
}

String StageArea::getAreaName()
{	
	return areaName.getValue();
}

ValueTree StageArea::getValueTree()
{
	return stageAreaValueTree;
}

void StageArea::setValueTreeChildIndex(int index)
{
	valueTreeChildIndex = index;
}

int StageArea::getValueTreeChildIndex()
{
	return valueTreeChildIndex;
}

void StageArea::setActive(bool state)
{
	stageAreaValueTree.setProperty("Active", state, nullptr);

	if (stageAreaValueTree.getPropertyAsValue("Active", nullptr) == true)
	{
		setVisible(false); 
	}
}

bool StageArea::getActiveState()
{
	return stageAreaValueTree.getPropertyAsValue("Active", nullptr).getValue();
}

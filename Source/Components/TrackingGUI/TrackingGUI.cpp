#include "../Source/Components/TrackingGUI/TrackingGUI.h"
#include <math.h>

// GUI Class, Handles GUI Related functions called from MainComponent.

// Constructor
TrackingGUI::TrackingGUI()
{	
	trackingState[0] = false;
	trackingState[1] = false;
}

TrackingGUI::~TrackingGUI()
{
	stageAreas.clear(true);
}


// Main GUI Functions

void TrackingGUI::paint(Graphics& g)
{
	// Colour Background. 
	g.fillAll(Colours::darkgrey); // Colour background.
	g.drawRect(getLocalBounds().expanded(1), 2);

	if(getGridDrawingState()) 
		paintGrid(g);

	paintStage(g); // Draw stage if calibrated.

	paintStageAreas(g);

	if (trackingState[0] == true) { paintTrackedEllipse(g, ellipseCoordinates[0].x, ellipseCoordinates[0].y, Colours::blue); }
	if (trackingState[1] == true) { paintTrackedEllipse(g, ellipseCoordinates[1].x, ellipseCoordinates[1].y, Colours::red); }
}

void TrackingGUI::paintTrackedEllipse(Graphics& g, float PositionX, float PositionY, Colour colour)
{	
	// Draw Circle to represent tracked position.
	float Diameter = 20.f;
	float Radius = Diameter * 0.5;
	g.setColour(colour);
	g.drawEllipse(PositionX - Radius, PositionY - Radius, Diameter, Diameter, 2);
}

void TrackingGUI::paintStage(Graphics& g)
{
	if (getGridSnappingState()) 
		snapStageToGrid();

		if (valueTree.getPropertyAsValue("StageDrawingState", nullptr) == true)
		{
			mainStageArea.setAreaColour(Colours::darkgrey.darker(0.5).withAlpha(0.4f));
			mainStageArea.drawArea(g, getStageEditState());
		}
}

void TrackingGUI::paintStageAreas(Graphics &g)
{
	for (int areaIndex = 0; areaIndex < stageAreas.size(); areaIndex++)
	{
		if (stageAreas[areaIndex]->getAreaPath().contains(ellipseCoordinates[0].x, ellipseCoordinates[0].y) == true)
			stageAreas[areaIndex]->setAreaColour(stageAreas[areaIndex]->getAreaColour().withAlpha(0.5f));
		else
			stageAreas[areaIndex]->setAreaColour(stageAreas[areaIndex]->getAreaColour().withAlpha(0.3f));

		if (stageAreas[areaIndex]->getAreaPath().contains(ellipseCoordinates[1].x, ellipseCoordinates[1].y) == true)
		g.setColour(Colours::salmon);

		stageAreas[areaIndex]->drawArea(g, stageAreas[areaIndex]->getAreaSelectedState());
	}
}

void TrackingGUI::paintGrid(Graphics& g)
{
	g.setColour(Colours::grey);

	int localWidth = getLocalBounds().getWidth();
	int localHeight = getLocalBounds().getHeight();
	
	int incrementedX = 0;
	int incremnetedY = 0;

	for (int lineNum = 0; lineNum < localWidth / gridIncrement; lineNum++)
	{
		g.drawLine(incrementedX, 0, incrementedX, localHeight, 0.2);
		incrementedX += gridIncrement;
	}

	for (int lineNum = 0; lineNum < localHeight / gridIncrement; lineNum++)
	{
		g.drawLine(0, incremnetedY, localWidth, incremnetedY, 0.2);
		incremnetedY += gridIncrement;
	}
}

void TrackingGUI::snapStageToGrid()
{
	for (int corner = 0; corner < mainStageArea.numOfAreaCorners - 1; corner++)
	{
		float snappedStageCornerX = round(mainStageArea.getX(corner) / gridIncrement) * gridIncrement;
		float snappedStageCornerY = round(mainStageArea.getY(corner) / gridIncrement) * gridIncrement;
		mainStageArea.updateArea(corner, snappedStageCornerX, snappedStageCornerY);
	}
	
	for (int areaIndex = 0; areaIndex < stageAreas.size(); areaIndex++)
	{
		for (int corner = 0; corner < stageAreas[areaIndex]->numOfAreaCorners - 1; corner++)
		{
			float snappedAreaCornerX = round(stageAreas[areaIndex]->getX(corner) / gridIncrement) * gridIncrement;
			float snappedAreaCornerY = round(stageAreas[areaIndex]->getY(corner) / gridIncrement) * gridIncrement;
			
			stageAreas[areaIndex]->updateArea(corner, snappedAreaCornerX, snappedAreaCornerY);
		}
	}
}

void TrackingGUI::resized()
{
	for (int areaIndex = 0; areaIndex < stageAreas.size(); areaIndex++)
	{
		stageAreas[areaIndex]->updateTrackingGUIWidthAndHeight(getWidth(), getHeight());
	}
}	

float TrackingGUI::getPerformerXPosInsideArea(int areaID, int performerID)
{
	float areaLeft = stageAreas[areaID]->getAreaPath().getBounds().getX();
	float areaRight = stageAreas[areaID]->getAreaPath().getBounds().getRight();
	float performerX = ellipseCoordinates[performerID].x;

	// return (performerX - AreaMinimum) / (Max - Min)
	return (performerX - areaLeft) / (areaRight - areaLeft);
}

void TrackingGUI::setValueTree(ValueTree valueTree)
{
	valueTree = valueTree;
}
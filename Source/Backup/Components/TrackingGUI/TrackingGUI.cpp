#include "../Source/Components/TrackingGUI/TrackingGUI.h"
#include <math.h>

// GUI Class, Handles GUI Related functions called from MainComponent.

// Constructor
LTLA_GUI::LTLA_GUI()
{	
	TrackingState[0] = false;
	TrackingState[1] = false;
}

// Main GUI Functions

void LTLA_GUI::paint(Graphics& g) 
{
	// Colour Background. 
	g.fillAll(Colours::darkgrey); // Colour background.
	g.drawRect(getLocalBounds().expanded(1), 2);

	if(GetGridDrawingState()) 
		PaintGrid(g);

	PaintStage(g); // Draw stage if calibrated.

	PaintStageAreas(g);

	if (TrackingState[0] == true) { PaintTrackedEllipse(g, EllipseCoordinates[0].x, EllipseCoordinates[0].y, Colours::blue); }
	if (TrackingState[1] == true) { PaintTrackedEllipse(g, EllipseCoordinates[1].x, EllipseCoordinates[1].y, Colours::red); }
}

void LTLA_GUI::PaintTrackedEllipse(Graphics& g, float PositionX, float PositionY, Colour colour)
{	
	// Draw Circle to represent tracked position.
	float Diameter = 20.f;
	float Radius = Diameter * 0.5;
	g.setColour(colour);
	g.drawEllipse(PositionX - Radius, PositionY - Radius, Diameter, Diameter, 2);
}

void LTLA_GUI::PaintStage(Graphics& g)
{
	if (GetGridSnappingState()) 
		SnapStageToGrid();

	if (StageDrawingState == true)
	{
		MainStageArea.SetAreaColour(Colours::darkgrey.darker(0.5).withAlpha(0.4f));
		MainStageArea.DrawArea(g, GetStageEditState());
	}
}

void LTLA_GUI::PaintStageAreas(Graphics &g)
{
	for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
	{
		if (StageAreas[AreaIndex]->GetAreaPath().contains(EllipseCoordinates[0].x, EllipseCoordinates[0].y) == true)
			StageAreas[AreaIndex]->SetAreaColour(StageAreas[AreaIndex]->GetAreaColour().withAlpha(0.5f));
		else
			StageAreas[AreaIndex]->SetAreaColour(StageAreas[AreaIndex]->GetAreaColour().withAlpha(0.3f));

		if (StageAreas[AreaIndex]->GetAreaPath().contains(EllipseCoordinates[1].x, EllipseCoordinates[1].y) == true)
		g.setColour(Colours::salmon);

		StageAreas[AreaIndex]->DrawArea(g, StageAreas[AreaIndex]->GetAreaSelectedState());
	}
}

void LTLA_GUI::PaintGrid(Graphics& g)
{
	g.setColour(Colours::grey);

	int LocalWidth = getLocalBounds().getWidth();
	int LocalHeight = getLocalBounds().getHeight();
	
	int IncrementedX = 0;
	int IncremnetedY = 0;

	for (int LineNum = 0; LineNum < LocalWidth / GridIncrement; LineNum++)
	{
		g.drawLine(IncrementedX, 0, IncrementedX, LocalHeight, 0.2);
		IncrementedX += GridIncrement;
	}

	for (int LineNum = 0; LineNum < LocalHeight / GridIncrement; LineNum++)
	{
		g.drawLine(0, IncremnetedY, LocalWidth, IncremnetedY, 0.2);
		IncremnetedY += GridIncrement;
	}
}

void LTLA_GUI::SnapStageToGrid()
{
	for (int Corner = 0; Corner < MainStageArea.NumOfAreaCorners; Corner++)
	{
		float SnappedStageCornerX = round(MainStageArea.GetX(Corner) / GridIncrement) * GridIncrement;
		float SnappedStageCornerY = round(MainStageArea.GetY(Corner) / GridIncrement) * GridIncrement;
		MainStageArea.UpdateArea(Corner, SnappedStageCornerX, SnappedStageCornerY);
	}
	
	for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
	{
		for (int Corner = 0; Corner < StageAreas[AreaIndex]->NumOfAreaCorners; Corner++)
		{
			float SnappedAreaCornerX = round(StageAreas[AreaIndex]->GetX(Corner) / GridIncrement) * GridIncrement;
			float SnappedAreaCornerY = round(StageAreas[AreaIndex]->GetY(Corner) / GridIncrement) * GridIncrement;
			
			StageAreas[AreaIndex]->UpdateArea(Corner, SnappedAreaCornerX, SnappedAreaCornerY);
		}
	}
}

void LTLA_GUI::resized()
{

}	
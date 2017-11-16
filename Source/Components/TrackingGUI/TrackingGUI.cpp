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
		MainStageArea.DrawArea(g, GetStageEditState());
}

void LTLA_GUI::PaintStageAreas(Graphics &g)
{
	for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
	{
		g.setColour(Colours::lightblue);

		if (StageAreas[AreaIndex]->GetAreaPath().contains(EllipseCoordinates[0].x, EllipseCoordinates[0].y) == true)
		g.setColour(Colours::lightcyan);
		if (StageAreas[AreaIndex]->GetAreaPath().contains(EllipseCoordinates[1].x, EllipseCoordinates[1].y) == true)
		g.setColour(Colours::lightcyan);

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
								// GETTERS & SETTERS.

void LTLA_GUI::SetEllipseCoordinates(float PositionX, float PositionY, int SkeltonNum)
{	// Coordinates are mapped based on the size of the window.
	EllipseCoordinates[SkeltonNum].x = getLocalBounds().getWidth() * 0.5 * PositionX;
	EllipseCoordinates[SkeltonNum].y = getLocalBounds().getHeight() * 0.5 * PositionY;	
}

	// STAGE GETTERS & SETTERS

void LTLA_GUI::SetStageCoordinates(int StagePosition, float x, float y)
{
	float ConvertedX = getLocalBounds().getWidth() * 0.5 * x;
	float ConvertedY = getLocalBounds().getHeight() * 0.5 * y;
	MainStageArea.UpdateArea(StagePosition, ConvertedX, ConvertedY);
}

void LTLA_GUI::SetStageDrawingState(bool State)
{
	StageDrawingState = State;
}

bool LTLA_GUI::GetStageDrawingState()
{
	return StageDrawingState;
}

void LTLA_GUI::SetStageEditState(bool State)
{
	StageEditState = State;
}

bool LTLA_GUI::GetStageEditState()
{
	return StageEditState;
}

	// STAGE AREA GETTERS & SETTERS

void LTLA_GUI::SetStageAreaEditState(bool State)
{
	StageAreaEditState = State;
}

bool LTLA_GUI::GetStageAreaEditState()
{
	return StageAreaEditState;
}

void LTLA_GUI::SetCurrentlySelectedArea(int Index)
{
	SelectedAreaIndex = Index;
}
int LTLA_GUI::GetCurrentlySelectedArea()
{
	return SelectedAreaIndex;
}

	// GRID GETTERS & SETTERS

void LTLA_GUI::SetGridDrawingState(bool state)
{
	GridDrawingState = state;
}

bool LTLA_GUI::GetGridDrawingState()
{
	return GridDrawingState;
}

void LTLA_GUI::SetGridSnappingState(bool state)
{
	GridSnappingState = state;
}

bool LTLA_GUI::GetGridSnappingState()
{
	return GridSnappingState;
}

void LTLA_GUI::SetGridIncrement(int Value)
{
	GridIncrement = Value;
}

int LTLA_GUI::GetGridIncrement()
{
	return GridIncrement;
}

	// TRACKING GETTERS & SETTERS

void LTLA_GUI::SetKinectTrackingState(int PerformerNum, bool State)
{
	TrackingState[PerformerNum] = State;
}


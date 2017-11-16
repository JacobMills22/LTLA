#include "../Source/Components/TrackingGUI.h"
#include <math.h>

// GUI Class, Handles GUI Related functions called from MainComponent.

// Constructor
LTLA_GUI::LTLA_GUI()
{
	// Set all Stage Positions to 0.0
	for (int StagePos = 0; StagePos < NumOfStagePositions; StagePos++)
	{
		StageCoordinates[StagePos].x = 0.00;
		StageCoordinates[StagePos].y = 0.00;
	}

	StageCoordinates[FrontLeft].x = 50.00;
	StageCoordinates[FrontLeft].y = 50.00;
	StageCoordinates[FrontRight].x = 200.00;
	StageCoordinates[FrontRight].y = 50.00;
	StageCoordinates[BackRight].x = 200.00;
	StageCoordinates[BackRight].y = 100.00;
	StageCoordinates[BackLeft].x = 50.00;
	StageCoordinates[BackLeft].y = 100.00;
	
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
	{
		Line<float> StageOutline[NumOfStageSides];
		Path StagePath;

		// Convert Stage Coordnates to Line Objects.
		StageOutline[StageFront].setStart(StageCoordinates[FrontLeft].x, StageCoordinates[FrontLeft].y);
		StageOutline[StageFront].setEnd(StageCoordinates[FrontRight].x, StageCoordinates[FrontRight].y);

		StageOutline[StageRight].setStart(StageCoordinates[FrontRight].x, StageCoordinates[FrontRight].y);
		StageOutline[StageRight].setEnd(StageCoordinates[BackRight].x, StageCoordinates[BackRight].y);

		StageOutline[StageBack].setStart(StageCoordinates[BackRight].x, StageCoordinates[BackRight].y);
		StageOutline[StageBack].setEnd(StageCoordinates[BackLeft].x, StageCoordinates[BackLeft].y);

		StageOutline[StageLeft].setStart(StageCoordinates[BackLeft].x, StageCoordinates[BackLeft].y);
		StageOutline[StageLeft].setEnd(StageCoordinates[FrontLeft].x, StageCoordinates[FrontLeft].y);

		// Add a Quadrilateral a path. (Graphics doesn;t support Quadrilaterals so path needs to be used.)
		StagePath.addQuadrilateral(StageOutline[StageFront].getStartX(), StageOutline[StageFront].getStartY(),
								   StageOutline[StageFront].getEndX(), StageOutline[StageFront].getEndY(),
								   StageOutline[StageBack].getStartX(), StageOutline[StageBack].getStartY(),
								   StageOutline[StageBack].getEndX(), StageOutline[StageBack].getEndY());

		// Set the Stage colour to Dimgrey.
		g.setColour(Colours::black); 
		g.setOpacity(0.25);

		// If either ellipses are inside the stage path then set the colour to light cyan
//		if (StagePath.contains(EllipseCoordinates[0].x, EllipseCoordinates[0].y) == true)
//			g.setColour(Colours::lightcyan);
		
//		else if (StagePath.contains(EllipseCoordinates[1].x, EllipseCoordinates[1].y) == true)
//			g.setColour(Colours::lightcyan);
	
		g.fillPath(StagePath);	// Fill the stage with desiered colour
		g.setOpacity(1.0);

		g.setColour(Colours::black);
		g.strokePath(StagePath, PathStrokeType(1.0));	// Draw black outline of stage.

		if (StageEditState == true)
		{
			g.setColour(Colours::white);
			if (StageCornerSelected[FrontLeft] == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)StageCoordinates[FrontLeft].x - 5, (int)StageCoordinates[FrontLeft].y - 5, 10, 10, 1.0);
			g.setColour(Colours::white);
			if (StageCornerSelected[FrontRight] == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)StageCoordinates[FrontRight].x - 5, (int)StageCoordinates[FrontRight].y - 5, 10, 10, 1.0);
			g.setColour(Colours::white);
			if (StageCornerSelected[BackRight] == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)StageCoordinates[BackRight].x - 5, (int)StageCoordinates[BackRight].y - 5, 10, 10, 1.0);
			g.setColour(Colours::white);
			if (StageCornerSelected[BackLeft] == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)StageCoordinates[BackLeft].x - 5, (int)StageCoordinates[BackLeft].y - 5, 10, 10, 1.0);
		}
	}
}

void LTLA_GUI::PaintStageAreas(Graphics &g)
{
	for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
	{
		g.setColour(Colours::lightblue);

		if (StageAreas.getUnchecked(AreaIndex)->GetAreaPath().contains(EllipseCoordinates[0].x, EllipseCoordinates[0].y) == true)
		g.setColour(Colours::lightcyan);
		if (StageAreas.getUnchecked(AreaIndex)->GetAreaPath().contains(EllipseCoordinates[1].x, EllipseCoordinates[1].y) == true)
		g.setColour(Colours::lightcyan);

		StageAreas.getUnchecked(AreaIndex)->DrawArea(g, GetStageAreaEditState());
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
	for (int StagePosition = 0; StagePosition < NumOfStagePositions; StagePosition++)
	{
		StageCoordinates[StagePosition].x = round(StageCoordinates[StagePosition].x / GridIncrement) * GridIncrement;
		StageCoordinates[StagePosition].y = round(StageCoordinates[StagePosition].y / GridIncrement) * GridIncrement;
	}

	for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
	{
		for (int Corner = 0; Corner < StageAreas.getUnchecked(AreaIndex)->NumOfAreaCorners; Corner++)
		{
			float SnappedAreaCornerX = round(StageAreas.getUnchecked(AreaIndex)->GetX(Corner) / GridIncrement) * GridIncrement;
			float SnappedAreaCornerY = round(StageAreas.getUnchecked(AreaIndex)->GetY(Corner) / GridIncrement) * GridIncrement;
			
			StageAreas.getUnchecked(AreaIndex)->UpdateArea(Corner, SnappedAreaCornerX, SnappedAreaCornerY);
		}
	}
}

void LTLA_GUI::mouseDown(const MouseEvent &event)
{
	if (GetStageEditState() == true)
	{
		for (int StagePos = 0; StagePos < NumOfStagePositions; StagePos++)
		{
			if (event.getMouseDownX() <= StageCoordinates[StagePos].x + 10 && event.getMouseDownX() >= StageCoordinates[StagePos].x - 10)
			{
				if (event.getMouseDownY() <= StageCoordinates[StagePos].y + 10 && event.getMouseDownY() >= StageCoordinates[StagePos].y - 10)
				{
					StageCornerSelected[StagePos] = true;
					break;
				}
			}
		}
	}
	else if (GetStageAreaEditState() == true)
	{
		for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
		{
			StageAreas.getUnchecked(AreaIndex)->SetAreaSelectedState(false);

			if (StageAreas.getUnchecked(AreaIndex)->GetAreaPath().contains(event.x, event.y))
			{
				StageAreas.getUnchecked(AreaIndex)->SetAreaSelectedState(true);
				SetCurrentlySelectedArea(AreaIndex);
			}

			for (int Corner = 0; Corner < StageAreas.getUnchecked(AreaIndex)->NumOfAreaCorners; Corner++)
			{
				if (event.getMouseDownX() <= StageAreas.getUnchecked(AreaIndex)->GetX(Corner) + 10 && event.getMouseDownX() >= StageAreas.getUnchecked(AreaIndex)->GetX(Corner) - 10)
				{
					if (event.getMouseDownY() <= StageAreas.getUnchecked(AreaIndex)->GetY(Corner) + 10 && event.getMouseDownY() >= StageAreas.getUnchecked(AreaIndex)->GetY(Corner) - 10)
					{
						StageAreas.getUnchecked(AreaIndex)->SetCornerSelectedState(Corner, true);
						break;
					}
				}

			}
		}
	}


}

void LTLA_GUI::mouseDrag(const MouseEvent& event)
{
	if (StageEditState == true)
	{

		for (int StagePos = 0; StagePos < NumOfStagePositions; StagePos++)
		{
			if (StageCornerSelected[StagePos] == true)
			{
				StageCoordinates[StagePos].x = event.x;
				StageCoordinates[StagePos].y = event.y;
			}
		}
	}
	else if (GetStageAreaEditState() == true)
	{
		for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
		{
			for (int Corner = 0; Corner < StageAreas.getUnchecked(AreaIndex)->NumOfAreaCorners; Corner++)
			{
				if (StageAreas.getUnchecked(AreaIndex)->GetCornerSelectedState(Corner) == true)
				{
					StageAreas.getUnchecked(AreaIndex)->UpdateArea(Corner, event.x, event.y);
					break;
				}
			}
		}
	}
}

void LTLA_GUI::mouseUp(const MouseEvent& event)
{
	if (StageEditState == true)
	{
		for (int StagePos = 0; StagePos < NumOfStagePositions; StagePos++)
		{
			StageCornerSelected[StagePos] = false;
		}
	}
	if (GetStageAreaEditState() == true)
	{
		for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
		{
			for (int Corner = 0; Corner < StageAreas.getUnchecked(AreaIndex)->NumOfAreaCorners; Corner++)
			{
				StageAreas.getUnchecked(AreaIndex)->SetCornerSelectedState(Corner, false);
			}
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

void LTLA_GUI::SetCurrentlySelectedArea(int Index)
{
	SelectedAreaIndex = Index;
}
int LTLA_GUI::GetCurrentlySelectedArea()
{
	return SelectedAreaIndex;
}

	// STAGE GETTERS & SETTERS

void LTLA_GUI::SetStageCoordinates(int StagePosition, float x, float y)
{
	StageCoordinates[StagePosition].x = getLocalBounds().getWidth() * 0.5 * x;
	StageCoordinates[StagePosition].y = getLocalBounds().getHeight() * 0.5 * y;
}

void LTLA_GUI::SetStageEditState(bool State)
{
	StageEditState = State;
}

bool LTLA_GUI::GetStageEditState()
{
	return StageEditState;
}

void LTLA_GUI::SetStageAreaEditState(bool State)
{
	StageAreaEditState = State;
}

bool LTLA_GUI::GetStageAreaEditState()
{
	return StageAreaEditState;
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

void LTLA_GUI::SetStageDrawingState(bool State)
{
	StageDrawingState = State;
}

bool LTLA_GUI::GetStageDrawingState()
{
	return StageDrawingState;
}

	// TRACKING GETTERS & SETTERS

void LTLA_GUI::SetKinectTrackingState(int PerformerNum, bool State)
{
	TrackingState[PerformerNum] = State;
}


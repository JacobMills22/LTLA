#include "LTLA_GUI.h"
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

	TrackingState[0] = false;
	TrackingState[1] = false;

	addAndMakeVisible(GUILabel[StageCalCountDownLabelID]);
	//GUILabel[StageCalCountDownLabelID].setText("Not Calibrating", dontSendNotification);
	Font LabelFont;
	LabelFont.setSizeAndStyle(20, 1, 1, 0.25);
	GUILabel[StageCalCountDownLabelID].setFont(LabelFont);
}

// Main GUI Functions

void LTLA_GUI::paint(Graphics& g) 
{
	// Colour Background. 
	g.fillAll(Colours::darkgrey); // Colour background.

	if(GetGridDrawingState()) 
		PaintGrid(g);

	PaintStage(g); // Draw stage if calibrated.

	if (TrackingState[0] == true) { PaintTrackedEllipse(g, EllipseCoordinates[0].x, EllipseCoordinates[0].y, Colours::blue); }
	if (TrackingState[1] == true) { PaintTrackedEllipse(g, EllipseCoordinates[1].x, EllipseCoordinates[1].y, Colours::red); }
}

void LTLA_GUI::PaintTrackedEllipse(Graphics& g, float PositionX, float PositionY, Colour colour)
{	
	// Draw Circle to represent tracked position.
	int Diameter = 20;
	int Radius = Diameter * 0.5;
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
		g.setColour(Colours::dimgrey); 

		// If either ellipses are inside the stage path then set the colour to light cyan
		if (StagePath.contains(EllipseCoordinates[0].x, EllipseCoordinates[0].y) == true)
			g.setColour(Colours::lightcyan);
		
		else if (StagePath.contains(EllipseCoordinates[1].x, EllipseCoordinates[1].y) == true)
			g.setColour(Colours::lightcyan);
	
		g.fillPath(StagePath);	// Fill the stage with desiered colour

		g.setColour(Colours::black);
		g.strokePath(StagePath, PathStrokeType(1.0));	// Draw black outline of stage.
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
		StageCoordinates[StagePosition].x = round((StageCoordinates[StagePosition].x / GridIncrement)) * GridIncrement;
		StageCoordinates[StagePosition].y = round((StageCoordinates[StagePosition].y / GridIncrement)) * GridIncrement;
	}
}

void LTLA_GUI::mouseDown(const MouseEvent &event)
{

	for (int StagePos = 0; StagePos < NumOfStagePositions; StagePos++)
	{
		if (GetMouseDrawingStageState(StagePos) == true)
		{
			StageCoordinates[StagePos].x = event.x;
			StageCoordinates[StagePos].y = event.y;
		}
	}
}


void LTLA_GUI::resized()
{
	GUILabel[StageCalCountDownLabelID].setBounds(getBounds().getWidth() - 200, getBounds().getHeight() - 100, 200, 100);
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
	StageCoordinates[StagePosition].x = getLocalBounds().getWidth() * 0.5 * x;
	StageCoordinates[StagePosition].y = getLocalBounds().getHeight() * 0.5 * y;
}

void LTLA_GUI::SetStageCalCountdownText(String Text)
{
	GUILabel[StageCalCountDownLabelID].setText(Text, dontSendNotification);
}

void LTLA_GUI::SetMouseDrawingStageState(int StagePosition, bool state)
{
	for (int StagePos = 0; StagePos < NumOfStagePositions; StagePos++)
	{
		MouseDrawingStageState[StagePos] = false;
	}
	
	MouseDrawingStageState[StagePosition] = state;
}

bool LTLA_GUI::GetMouseDrawingStageState(int StagePosition)
{
	return MouseDrawingStageState[StagePosition];
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


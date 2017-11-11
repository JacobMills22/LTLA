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
	if (TrackingState[0] == true) { PaintTrackedEllipse(g, EllipseCoordinates[0].x, EllipseCoordinates[0].y, Colours::blue); }
	if (TrackingState[1] == true) { PaintTrackedEllipse(g, EllipseCoordinates[1].x, EllipseCoordinates[1].y, Colours::red);  }

	PaintStage(g); // Draw stage if calibrated.

	if(GetGridDrawingState()) 
		PaintGrid(g);
}

void LTLA_GUI::PaintTrackedEllipse(Graphics& g, float PositionX, float PositionY, Colour colour)
{	
	// Draw Circle to represent tracked position.
	g.setColour(colour);
	g.drawEllipse(PositionX, PositionY, 20, 20, 2);
}

void LTLA_GUI::PaintStage(Graphics& g)
{
	if (GetGridSnappingState()) 
		SnapStageToGrid();

	// Draw stage if calibrated.
	if (StageDrawingState == true)
	{
		g.setColour(Colours::black);
		g.drawLine(StageCoordinates[FrontLeft].x, StageCoordinates[FrontLeft].y,
			StageCoordinates[FrontRight].x, StageCoordinates[FrontRight].y, 2);

		g.drawLine(StageCoordinates[FrontRight].x, StageCoordinates[FrontRight].y,
			StageCoordinates[BackRight].x, StageCoordinates[BackRight].y), 2;

		g.drawLine(StageCoordinates[BackRight].x, StageCoordinates[BackRight].y,
			StageCoordinates[BackLeft].x, StageCoordinates[BackLeft].y), 2;

		g.drawLine(StageCoordinates[BackLeft].x, StageCoordinates[BackLeft].y,
			StageCoordinates[FrontLeft].x, StageCoordinates[FrontLeft].y, 2);
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


void LTLA_GUI::resized()
{
	GUILabel[StageCalCountDownLabelID].setBounds(getBounds().getWidth() - 200, getBounds().getHeight() - 100, 200, 100);
}
// Getters and Setters.

void LTLA_GUI::SetEllipseCoordinates(float PositionX, float PositionY, int SkeltonNum)
{	// Coordinates are mapped based on the size of the window.
	EllipseCoordinates[SkeltonNum].x = getLocalBounds().getWidth() * 0.5 * PositionX;
	EllipseCoordinates[SkeltonNum].y = getLocalBounds().getHeight() * 0.5 * PositionY;	
}

void LTLA_GUI::SetKinectTrackingState(int PerformerNum, bool State)
{
	TrackingState[PerformerNum] = State;
}

void LTLA_GUI::SetStageCoordinates(int StagePosition, float x, float y)
{
	StageCoordinates[StagePosition].x = getLocalBounds().getWidth() * 0.5 * x;
	StageCoordinates[StagePosition].y = getLocalBounds().getHeight() * 0.5 * y;
}

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

void LTLA_GUI::SetStageCalCountdownText(String Text)
{
	GUILabel[StageCalCountDownLabelID].setText(Text, dontSendNotification);
}



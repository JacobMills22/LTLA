#include "LTLA_GUI.h"

// GUI Class, Handles GUI Related functions called from MainComponent.

// Constructor
LTLA_GUI::LTLA_GUI()
{
	setSize(800, 400);	// Set size of GUI Component

	// Set all Stage Positions to 0.0
	for (int StagePos = 0; StagePos < NumOfStagePositions; StagePos++)
	{
		//StageCoordinates[StagePos].x = 0.00;
		//StageCoordinates[StagePos].y = 0.00;
	}

	TrackingState[0] = false;
	TrackingState[1] = false;
}

// Main GUI Functions

void LTLA_GUI::paint(Graphics& g) 
{
	// Colour Background. 
	g.fillAll(Colours::darkgrey); // Colour background.
	if (TrackingState[0] == true) { PaintTrackedEllipse(g, EllipseCoordinates[0].x, EllipseCoordinates[0].y, Colours::blue); }
	if (TrackingState[1] == true) { PaintTrackedEllipse(g, EllipseCoordinates[1].x, EllipseCoordinates[1].y, Colours::red);  }

	PaintStage(g); // Draw stage if calibrated.
}

void LTLA_GUI::PaintTrackedEllipse(Graphics& g, float PositionX, float PositionY, Colour colour)
{	
	// Draw Circle to represent tracked position.
	g.setColour(colour);
	g.drawEllipse(PositionX, PositionY, 20, 20, 2);
}

void LTLA_GUI::PaintStage(Graphics& g)
{
	// Draw stage if calibrated.
	if (StageCalibrated == true)
	{
		// Set colour of stage outline.
		g.setColour(Colours::black);
		// Draw front of stage.
		g.drawLine(StageCoordinates[FrontLeft].x, StageCoordinates[FrontLeft].y,
			StageCoordinates[FrontRight].x, StageCoordinates[FrontRight].y);
		// Draw right side of stage.
		g.drawLine(StageCoordinates[FrontRight].x, StageCoordinates[FrontRight].y,
			StageCoordinates[BackRight].x, StageCoordinates[BackRight].y);
		// Draw back of stage
		g.drawLine(StageCoordinates[BackRight].x, StageCoordinates[BackRight].y,
			StageCoordinates[BackLeft].x, StageCoordinates[BackLeft].y);
		// Draw left side of stage.
		g.drawLine(StageCoordinates[BackLeft].x, StageCoordinates[BackLeft].y,
			StageCoordinates[FrontLeft].x, StageCoordinates[FrontLeft].y);
	}
}

void LTLA_GUI::resized() 
{

}
     // Called in Main Component when the calibration button is pressed
void LTLA_GUI::CalibrateStageButtonPressed(Button* button, int TimesPressed, float LocationDataX, float LocationDataY)
{
	switch (TimesPressed)
	{
	case 0:	// Set coordinates for front left of stage.
		SetStageCoordinates(FrontLeft, LocationDataX, LocationDataY);
		button->setButtonText("Calibrate Front Right");
		SetStageCalibrationState(false); // Stage is no longer calibrated.
		break;
	case 1: // Set coordinates for front right of stage.
		SetStageCoordinates(FrontRight, LocationDataX, LocationDataY);
		button->setButtonText("Calibrate Back Right");
		break;
	case 2: // Set coordinates for back left of stage.
		SetStageCoordinates(BackRight, LocationDataX, LocationDataY);
		button->setButtonText("Calibrate Back Left");
		break;
	case 3: // Set coordinates for back right of stage.
		SetStageCoordinates(BackLeft, LocationDataX, LocationDataY);
		button->setButtonText("Calibrate Front Left");
		SetStageCalibrationState(true);	// Stage is now calibrated and can be drawn.
	}
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

void LTLA_GUI::SetStageCalibrationState(bool State)
{
	StageCalibrated = State;
}

void LTLA_GUI::SetStageCoordinates(int StagePosition, float x, float y)
{// Coordinates are mapped based on the size of the window.
	StageCoordinates[StagePosition].x = getLocalBounds().getWidth() * 0.5 * x;
	StageCoordinates[StagePosition].y = getLocalBounds().getHeight() * 0.5 * y;
}

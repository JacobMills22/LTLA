
#include "TrackingGUI.h"

// ========================================================================================
// ELLIPSE GETTERS & SETTERS.
// ========================================================================================

void LTLA_GUI::SetEllipseCoordinates(float PositionX, float PositionY, int SkeltonNum)
{	// Coordinates are mapped based on the size of the window.
	EllipseCoordinates[SkeltonNum].x = getLocalBounds().getWidth() * 0.5 * PositionX;
	EllipseCoordinates[SkeltonNum].y = getLocalBounds().getHeight() * 0.5 * PositionY;
}

// ========================================================================================
// STAGE GETTERS & SETTERS
// ========================================================================================

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

// ========================================================================================
// STAGE AREA GETTERS & SETTERS
// ========================================================================================

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

// ========================================================================================
// GRID GETTERS & SETTERS
// ========================================================================================

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

// ========================================================================================
// TRACKING GETTERS & SETTERS
// ========================================================================================

void LTLA_GUI::SetKinectTrackingState(int PerformerNum, bool State)
{
	TrackingState[PerformerNum] = State;
}
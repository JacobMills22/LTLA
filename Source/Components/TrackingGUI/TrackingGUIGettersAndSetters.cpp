
#include "TrackingGUI.h"

// ========================================================================================
// ELLIPSE GETTERS & SETTERS.
// ========================================================================================

void TrackingGUI::setEllipseCoordinates(float positionX, float positionY, int skeltonNum)
{	// Coordinates are mapped based on the size of the window.
	ellipseCoordinates[skeltonNum].x = getLocalBounds().getWidth() * 0.5 * positionX;
	ellipseCoordinates[skeltonNum].y = getLocalBounds().getHeight() * 0.5 * positionY;
}

// ========================================================================================
// STAGE GETTERS & SETTERS
// ========================================================================================

void TrackingGUI::setStageCoordinates(int stagePosition, float x, float y)
{
	float convertedX = getLocalBounds().getWidth() * 0.5 * x;
	float convertedY = getLocalBounds().getHeight() * 0.5 * y;
	mainStageArea.updateArea(stagePosition, convertedX, convertedY);
}

void TrackingGUI::setStageDrawingState(bool state)
{
	valueTree.setProperty("StageDrawingState", state, nullptr);
}

bool TrackingGUI::getStageDrawingState()
{
	return valueTree.getPropertyAsValue("StageDrawingState", nullptr).getValue();
}

void TrackingGUI::setStageEditState(bool state)
{
	stageEditState = state;
}

bool TrackingGUI::getStageEditState()
{
	return stageEditState;
}

// ========================================================================================
// STAGE AREA GETTERS & SETTERS
// ========================================================================================

void TrackingGUI::setStageAreaEditState(bool state)
{
	stageAreaEditState = state;
}

bool TrackingGUI::getStageAreaEditState()
{
	return stageAreaEditState;
}

void TrackingGUI::setCurrentlySelectedArea(int index)
{
	for (int areaIndex = 0; areaIndex < stageAreas.size(); areaIndex++)
	{
		stageAreas[areaIndex]->setAreaSelectedState(false);
	}

	if (getCurrentlySelectedArea() != index)
		setStageAreaHasChangedState(true);

	selectedAreaIndex = index;
	stageAreas[index]->setAreaSelectedState(true);
}
int TrackingGUI::getCurrentlySelectedArea()
{
	return selectedAreaIndex;
}

void TrackingGUI::setStageAreaHasChangedState(bool state)
{
	selectedAreaHasChanged = state;
}

bool TrackingGUI::hasStageAreaChanged()
{
	return selectedAreaHasChanged;
}

void TrackingGUI::setAudioPanelState(bool state)
{
	audioPanelState = state;
}

bool TrackingGUI::getAudioPanelState()
{
	return audioPanelState;
}

// ========================================================================================
// GRID GETTERS & SETTERS
// ========================================================================================

void TrackingGUI::setGridDrawingState(bool state)
{
	gridDrawingState = state;
}

bool TrackingGUI::getGridDrawingState()
{
	return gridDrawingState;
}

void TrackingGUI::setGridSnappingState(bool state)
{
	gridSnappingState = state;
}

bool TrackingGUI::getGridSnappingState()
{
	return gridSnappingState;
}

void TrackingGUI::setGridIncrement(int Value)
{
	gridIncrement = Value;
}

int TrackingGUI::getGridIncrement()
{
	return gridIncrement;
}

// ========================================================================================
// TRACKING GETTERS & SETTERS
// ========================================================================================

void TrackingGUI::setKinectTrackingState(int performerNum, bool state)
{
	trackingState[performerNum] = state;
}

bool TrackingGUI::doesAreaIDContainPerfomer(int performerID, int areaID)
{
	if (stageAreas[areaID]->getAreaPath().contains(ellipseCoordinates[performerID].x, ellipseCoordinates[performerID].y) == true)
		return true;
	else
		return false;
}

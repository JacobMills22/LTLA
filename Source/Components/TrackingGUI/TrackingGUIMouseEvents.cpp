#include "../Source/Components/TrackingGUI/TrackingGUI.h"
#include "TrackingGUI.h"


void TrackingGUI::mouseDown(const MouseEvent &event)
{
	if (getStageEditState() == true) // If edit state is enabled. (Following code only refers to the calibratable single main stage area)
	{
		for (int corner = 0; corner < mainStageArea.numOfAreaCorners; corner++) // For each of the MainStages corners
		{
			// if the mouse position is in a resize box area
			if (event.getMouseDownX() <= mainStageArea.getX(corner) + 10 && event.getMouseDownX() >= mainStageArea.getX(corner) - 10)
			{	
				if (event.getMouseDownY() <= mainStageArea.getY(corner) + 10 && event.getMouseDownY() >= mainStageArea.getY(corner) - 10)
				{
					// tell the mainStageArea that this corner has been selected, (this will update the colour of the white resize box)
					mainStageArea.setCornerSelectedState(corner, true);

					// Calculate the position of each corner (except for the centre) relative to the centre of the area.
					// This allows the area to moved around as a whole from the centre since the bounds of each corner are updated
					// each time a resize box is selected
					for (int corner = 0; corner < numOfStagePositions - 1; corner++)
					{
						mainStageArea.setDifferenceX(corner, mainStageArea.getX(centre) - mainStageArea.getX(corner));
						mainStageArea.setDifferenceY(corner, mainStageArea.getY(centre) - mainStageArea.getY(corner));
					}
					break;
				}
			}
		}
	}
	else if (getStageAreaEditState() == true) // If edit state is enabled. (Following code refers to stage areas the user can add and remove)
	{
		for (int corner = 0; corner < stageAreas[getCurrentlySelectedArea()]->numOfAreaCorners; corner++) // For each corner
		{
			// if the mouse position is in a resize box area
			if (event.getMouseDownX() <= stageAreas[selectedAreaIndex]->getX(corner) + 10 && event.getMouseDownX() >= stageAreas[selectedAreaIndex]->getX(corner) - 10 &&
				event.getMouseDownY() <= stageAreas[selectedAreaIndex]->getY(corner) + 10 && event.getMouseDownY() >= stageAreas[selectedAreaIndex]->getY(corner) - 10)
			{
				// tell the selected area that this corner has been selected, (this will update the colour of the white resize box)
				stageAreas[selectedAreaIndex]->setCornerSelectedState(corner, true);

				// Calculate the position of each corner (except for the centre) relative to the centre of the area.
				// This allows the area to moved around as a whole from the centre since the bounds of each corner are updated
				// each time a resize box is selected
				for (int corner = 0; corner < numOfStagePositions - 1; corner++)
				{
					stageAreas[selectedAreaIndex]->setDifferenceX(corner, stageAreas[selectedAreaIndex]->getX(centre) - stageAreas[selectedAreaIndex]->getX(corner));
					stageAreas[selectedAreaIndex]->setDifferenceY(corner, stageAreas[selectedAreaIndex]->getY(centre) - stageAreas[selectedAreaIndex]->getY(corner));
				}
				break;
			}
		}
	}

	// If the mouse was clicked on performer1s ellipse and performer simulation is enabled, select it.
	if (shouldSimulatePerformer1 == true)
	{
		if (event.getMouseDownX() <= ellipseCoordinates[0].x + 10 && event.getMouseDownX() >= ellipseCoordinates[0].x - 10 &&
			event.getMouseDownY() <= ellipseCoordinates[0].y + 10 && event.getMouseDownY() >= ellipseCoordinates[0].y - 10)
		{
			performer1EllipseSelected = true;
		}
	}
}

void TrackingGUI::mouseDoubleClick(const MouseEvent & event)
{
	if (getStageAreaEditState() == true) // If edit mode is enabled
	{
		for (int areaIndex = 0; areaIndex < stageAreas.size(); areaIndex++) // for each area
		{
			// If the mouse was double clicked inside the current stage area
			if (stageAreas[areaIndex]->getAreaPath().contains(event.x, event.y) == true)
			{
				if (stageAreas[areaIndex]->getAreaPath().contains(event.x, event.y) == true)
				{
					// Select the stage area 
					setCurrentlySelectedArea(areaIndex);
				}
			}
		}
	}

	// If performer 1 should be simulated, check the coordinates are valid then update
	// the posiiton of the ellipse.
	if (shouldSimulatePerformer1 == true)
	{
		if (event.x >= 0 && event.x <= getWidth())
			ellipseCoordinates[0].x = event.x;

		if (event.y >= 0 && event.y <= getHeight())
			ellipseCoordinates[0].y = event.y;
	}
}

void TrackingGUI::mouseDrag(const MouseEvent& event)
{
	if (stageEditState == true) //if edit mode is enabled (Following code only refers to the calibratable single main stage area)
	{
		for (int corner = 0; corner < mainStageArea.numOfAreaCorners; corner++)
		{
			// Find the selected corner
			if (mainStageArea.getCornerSelectedState(corner) == true)
			{
				// Update the selected corners position if edit mode is enabled.
				mainStageArea.updateArea(corner, event.x, event.y);
			}
		}
	}
	else if (getStageAreaEditState() == true) // if edit mode is enabled (Following code refers to stage areas the user can add and remove)
	{
		for (int corner = 0; corner < stageAreas[selectedAreaIndex]->numOfAreaCorners; corner++)
		{
			// Find the selected corner
			if (stageAreas[selectedAreaIndex]->getCornerSelectedState(corner) == true)
			{
				// Update the selected corners position.
				stageAreas[selectedAreaIndex]->updateArea(corner, event.x, event.y);
				break;
			}
		}
	}

	// If performer 1 should be simulated and is selected, 
	// check the coordinates are valid then update the posiiton of the ellipse.
	if (shouldSimulatePerformer1 == true && performer1EllipseSelected == true)
	{
		if (event.x >= 0 && event.x <= getWidth())
			ellipseCoordinates[0].x = event.x;

		if (event.y >= 0 && event.y <= getHeight())
			ellipseCoordinates[0].y = event.y;
	}
}

void TrackingGUI::mouseUp(const MouseEvent& event)
{
	if (stageEditState == true) //if edit mode is enabled (Following code only refers to the calibratable single main stage area)
	{
		for (int corner = 0; corner < mainStageArea.numOfAreaCorners; corner++)
		{
			// Deselect all corners since the mouse is no longer held down.
			mainStageArea.setCornerSelectedState(corner, false);
		}
	}

	if (getStageAreaEditState() == true) // if edit mode is enabled (Following code refers to stage areas the user can add and remove)
	{
		for (int areaIndex = 0; areaIndex < stageAreas.size(); areaIndex++)
		{
			for (int corner = 0; corner < stageAreas[areaIndex]->numOfAreaCorners; corner++)
			{
				// Deselect all corners of all stage areas since the mouse is no longer held down.
				stageAreas[areaIndex]->setCornerSelectedState(corner, false);
			}
		}
	}

	// Reset the ellipse selected state false (Performer simulation)
	performer1EllipseSelected = false;
}

void TrackingGUI::mouseMove(const MouseEvent& event)
{
	if (stageEditState == true) //if edit mode is enabled (Following code only refers to the calibratable single main stage area)
	{
		for (int corner = 0; corner < mainStageArea.numOfAreaCorners; corner++)
		{
			if (event.getPosition().getX() <= mainStageArea.getX(corner) + 10 && event.getPosition().getX() >= mainStageArea.getX(corner) - 10 &&
				event.getPosition().getY() <= mainStageArea.getY(corner) + 10 && event.getPosition().getY() >= mainStageArea.getY(corner) - 10)
			{
				// Set the mouse cursor to a dragging hand if the mouse position is inside one of the resize boxes.
				setMouseCursor(MouseCursor::DraggingHandCursor);
				break;
			}
			else
				setMouseCursor(MouseCursor::NormalCursor); // Else set it the cursor to the normal cursor.
		}
	}

	if (getStageAreaEditState() == true && stageAreas.size() > 0) // If Edit mode is enabled 
	{
		for (int corner = 0; corner < stageAreas[getCurrentlySelectedArea()]->numOfAreaCorners; corner++)
		{
			if (event.getPosition().getX() <= stageAreas[selectedAreaIndex]->getX(corner) + 10 && event.getPosition().getX() >= stageAreas[selectedAreaIndex]->getX(corner) - 10 &&
				event.getPosition().getY() <= stageAreas[selectedAreaIndex]->getY(corner) + 10 && event.getPosition().getY() >= stageAreas[selectedAreaIndex]->getY(corner) - 10)
			{
				// Set the mouse cursor to a dragging hand if the mouse position is inside one of the resize boxes.
				setMouseCursor(MouseCursor::DraggingHandCursor);
				break;
			}
			else
			{
				setMouseCursor(MouseCursor::NormalCursor); // Else set it the cursor to the normal cursor.
			}
		}
	}
}
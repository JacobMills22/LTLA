#include "../Source/Components/TrackingGUI/TrackingGUI.h"
#include "TrackingGUI.h"


void TrackingGUI::mouseDown(const MouseEvent &event)
{
	if (getStageEditState() == true)
	{
		for (int corner = 0; corner < mainStageArea.numOfAreaCorners; corner++)
		{
			if (event.getMouseDownX() <= mainStageArea.getX(corner) + 10 && event.getMouseDownX() >= mainStageArea.getX(corner) - 10)
			{
				if (event.getMouseDownY() <= mainStageArea.getY(corner) + 10 && event.getMouseDownY() >= mainStageArea.getY(corner) - 10)
				{
					mainStageArea.setCornerSelectedState(corner, true);
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
	else if (getStageAreaEditState() == true)
	{
		for (int corner = 0; corner < stageAreas[getCurrentlySelectedArea()]->numOfAreaCorners; corner++)
		{
			if (event.getMouseDownX() <= stageAreas[selectedAreaIndex]->getX(corner) + 10 && event.getMouseDownX() >= stageAreas[selectedAreaIndex]->getX(corner) - 10 &&
				event.getMouseDownY() <= stageAreas[selectedAreaIndex]->getY(corner) + 10 && event.getMouseDownY() >= stageAreas[selectedAreaIndex]->getY(corner) - 10)
			{
				stageAreas[selectedAreaIndex]->setCornerSelectedState(corner, true);

				for (int corner = 0; corner < numOfStagePositions - 1; corner++)
				{
					stageAreas[selectedAreaIndex]->setDifferenceX(corner, stageAreas[selectedAreaIndex]->getX(centre) - stageAreas[selectedAreaIndex]->getX(corner));
					stageAreas[selectedAreaIndex]->setDifferenceY(corner, stageAreas[selectedAreaIndex]->getY(centre) - stageAreas[selectedAreaIndex]->getY(corner));
				}
				break;
			}
		}
	}

}

void TrackingGUI::mouseDoubleClick(const MouseEvent & event)
{
	if (getStageAreaEditState() == true)
	{
		for (int areaIndex = 0; areaIndex < stageAreas.size(); areaIndex++)
		{
			if (stageAreas[areaIndex]->getAreaPath().contains(event.x, event.y) == true)
			{
				if (stageAreas[areaIndex]->getAreaPath().contains(event.x, event.y) == true)
				{
					setCurrentlySelectedArea(areaIndex);
				}
			}
		}
	}	
}

void TrackingGUI::mouseDrag(const MouseEvent& event)
{
	if (stageEditState == true)
	{
		for (int corner = 0; corner < mainStageArea.numOfAreaCorners; corner++)
		{
			if (mainStageArea.getCornerSelectedState(corner) == true)
			{
				mainStageArea.updateArea(corner, event.x, event.y);
			}
		}
	}
	else if (getStageAreaEditState() == true)
	{
		for (int corner = 0; corner < stageAreas[selectedAreaIndex]->numOfAreaCorners; corner++)
		{
			if (stageAreas[selectedAreaIndex]->getCornerSelectedState(corner) == true)
			{
				stageAreas[selectedAreaIndex]->updateArea(corner, event.x, event.y);
				break;
			}
		}
	}
}

void TrackingGUI::mouseUp(const MouseEvent& event)
{
	if (stageEditState == true)
	{
		for (int corner = 0; corner < mainStageArea.numOfAreaCorners; corner++)
		{
			mainStageArea.setCornerSelectedState(corner, false);
		}
	}

	if (getStageAreaEditState() == true)
	{
		for (int areaIndex = 0; areaIndex < stageAreas.size(); areaIndex++)
		{
			for (int corner = 0; corner < stageAreas[areaIndex]->numOfAreaCorners; corner++)
			{
				stageAreas[areaIndex]->setCornerSelectedState(corner, false);
			}
		}
	}
}

void TrackingGUI::mouseMove(const MouseEvent& event)
{
	if (getStageAreaEditState() == true && stageAreas.size() > 0)
	{
		for (int corner = 0; corner < stageAreas[getCurrentlySelectedArea()]->numOfAreaCorners; corner++)
		{
			if (event.getPosition().getX() <= stageAreas[selectedAreaIndex]->getX(corner) + 10 && event.getPosition().getX() >= stageAreas[selectedAreaIndex]->getX(corner) - 10 &&
				event.getPosition().getY() <= stageAreas[selectedAreaIndex]->getY(corner) + 10 && event.getPosition().getY() >= stageAreas[selectedAreaIndex]->getY(corner) - 10)
			{
				setMouseCursor(MouseCursor::DraggingHandCursor);
				break;
			}
			else
			{
				setMouseCursor(MouseCursor::NormalCursor);
			}
		}
	}
}
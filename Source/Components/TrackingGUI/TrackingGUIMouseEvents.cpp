#include "../Source/Components/TrackingGUI/TrackingGUI.h"
#include "TrackingGUI.h"


void LTLA_GUI::mouseDown(const MouseEvent &event)
{
	if (GetStageEditState() == true)
	{
		for (int Corner = 0; Corner < MainStageArea.NumOfAreaCorners; Corner++)
		{
			if (event.getMouseDownX() <= MainStageArea.GetX(Corner) + 10 && event.getMouseDownX() >= MainStageArea.GetX(Corner) - 10)
			{
				if (event.getMouseDownY() <= MainStageArea.GetY(Corner) + 10 && event.getMouseDownY() >= MainStageArea.GetY(Corner) - 10)
				{
					MainStageArea.SetCornerSelectedState(Corner, true);
					break;
				}
			}
		}
	}
	
	else if (GetStageAreaEditState() == true)
	{
		for (int Corner = 0; Corner < StageAreas[GetCurrentlySelectedArea()]->NumOfAreaCorners; Corner++)
		{
			if (event.getMouseDownX() <= StageAreas[SelectedAreaIndex]->GetX(Corner) + 10 && event.getMouseDownX() >= StageAreas[SelectedAreaIndex]->GetX(Corner) - 10 &&
				event.getMouseDownY() <= StageAreas[SelectedAreaIndex]->GetY(Corner) + 10 && event.getMouseDownY() >= StageAreas[SelectedAreaIndex]->GetY(Corner) - 10)
			{
				StageAreas[SelectedAreaIndex]->SetCornerSelectedState(Corner, true);
				break;
			}
		}
	}
}

void LTLA_GUI::mouseDoubleClick(const MouseEvent & event)
{
	if (GetStageAreaEditState() == true)
	{
		for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
		{
			StageAreas[AreaIndex]->SetAreaSelectedState(false);

			if (StageAreas[AreaIndex]->GetAreaPath().contains(event.x, event.y) == true)
			{
				if (StageAreas[AreaIndex]->GetAreaPath().contains(event.x, event.y) == true)
				{
					StageAreas[GetCurrentlySelectedArea()]->SetAreaSelectedState(false);
					StageAreas[AreaIndex]->SetAreaSelectedState(true);
					SetCurrentlySelectedArea(AreaIndex);
				}
			}
		}
	}
}

void LTLA_GUI::mouseDrag(const MouseEvent& event)
{
	if (StageEditState == true)
	{
		for (int Corner = 0; Corner < MainStageArea.NumOfAreaCorners; Corner++)
		{
			if (MainStageArea.GetCornerSelectedState(Corner) == true)
			{
				MainStageArea.UpdateArea(Corner, event.x, event.y);
			}
		}
	}
	else if (GetStageAreaEditState() == true)
	{
		for (int Corner = 0; Corner < StageAreas[SelectedAreaIndex]->NumOfAreaCorners; Corner++)
		{
			if (StageAreas[SelectedAreaIndex]->GetCornerSelectedState(Corner) == true)
			{
				StageAreas[SelectedAreaIndex]->UpdateArea(Corner, event.x, event.y);
				break;
			}
		}
	}
}

void LTLA_GUI::mouseUp(const MouseEvent& event)
{
	if (StageEditState == true)
	{
		for (int Corner = 0; Corner < MainStageArea.NumOfAreaCorners; Corner++)
		{
			MainStageArea.SetCornerSelectedState(Corner, false);
		}
	}

	if (GetStageAreaEditState() == true)
	{
		for (int AreaIndex = 0; AreaIndex < StageAreas.size(); AreaIndex++)
		{
			for (int Corner = 0; Corner < StageAreas[AreaIndex]->NumOfAreaCorners; Corner++)
			{
				StageAreas[AreaIndex]->SetCornerSelectedState(Corner, false);
			}
		}
	}
}


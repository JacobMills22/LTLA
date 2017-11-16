
#pragma once

#include "../Source/MainComponent.h"

/** Function declarations are located in Source/MainComponent.h*/

ApplicationCommandTarget* MainContentComponent::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void MainContentComponent::getAllCommands(Array<CommandID>& commands)
{
	const CommandID ids[] = { MenuBar.Interval5SecondsID, MenuBar.Interval10SecondsID, MenuBar.Interval20SecondsID,
		MenuBar.CalibrationStartID, MenuBar.DrawStageID, MenuBar.EditStageID, MenuBar.DrawGridID, MenuBar.SnaptoGridID, MenuBar.GridSize10ID,
		MenuBar.GridSize15ID, MenuBar.GridSize20ID, MenuBar.AddStageAreaID, MenuBar.EditStageAreasID, MenuBar.RemoveStageAreaID };
	commands.addArray(ids, numElementsInArray(ids));
}

void MainContentComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	switch (commandID)
	{
	case LTLAMenuBar::Interval5SecondsID:
		result.setInfo("5 Seconds", "Set the Interval to 5 seconds", "Calibration", 0);
		if (GUI.StageCalibrationInterval == 5) result.setTicked(true);
		break;
	case LTLAMenuBar::Interval10SecondsID:
		result.setInfo("10 Seconds", "Set the Interval to 10 seconds", "Calibration", 0);
		if (GUI.StageCalibrationInterval == 10) result.setTicked(true);
		break;
	case LTLAMenuBar::Interval20SecondsID:
		result.setInfo("20 Seconds", "Set the Interval to 20 seconds", "Calibration", 0);
		if (GUI.StageCalibrationInterval == 20) result.setTicked(true);
		break;
	case LTLAMenuBar::CalibrationStartID:
		result.setInfo("Start Calibration", "Starts the calibration sequence.", "Calibration", 0);
		break;
	case LTLAMenuBar::DrawStageID:
		result.setInfo("Draw Stage", "Draws the stage once calibrated.", "Calibration", 0);
		result.setTicked(GUI.GetStageDrawingState());
		break;
	case LTLAMenuBar::EditStageID:
		result.setInfo("Edit Stage", "Edits the stage shape.", "Calibration", 0);
		result.setTicked(GUI.GetStageEditState());
		break;
	case LTLAMenuBar::DrawGridID:
		result.setInfo("Draw Grid", "Draws a Grid.", "Grid", 0);
		result.setTicked(GUI.GetGridDrawingState());
		break;
	case LTLAMenuBar::SnaptoGridID:
		result.setInfo("Enable Grid Snapping", "", "Grid", 0);
		result.setTicked(GUI.GetGridSnappingState());
		break;
	case LTLAMenuBar::GridSize10ID:
		result.setInfo("Grid Size 10", "", "Grid", 0);
		break;
	case LTLAMenuBar::GridSize15ID:
		result.setInfo("Grid Size 15", "", "Grid", 0);
		break;
	case LTLAMenuBar::GridSize20ID:
		result.setInfo("Grid Size 20", "", "Grid", 0);
		break;
	case LTLAMenuBar::AddStageAreaID:
		result.setInfo("Add Area", "Adds a resizeable area to the stage which can be assigned for different audio processing", "Stage Areas", 0);
		break;
	case LTLAMenuBar::EditStageAreasID:
		result.setInfo("Edit Stage Areas", "Resize current stage areas", "Stage Areas", 0);
		break;
	case LTLAMenuBar::RemoveStageAreaID:
		result.setInfo("Remove Selected Area", "Removes the currently selected shape.", "Stage Areas", 0);
		break;
		
	}
}

bool MainContentComponent::perform(const InvocationInfo& info)
{
	switch (info.commandID)
	{
	case LTLAMenuBar::Interval5SecondsID: GUI.StageCalibrationInterval = 5;
		break;
	case LTLAMenuBar::Interval10SecondsID: GUI.StageCalibrationInterval = 10;
		break;
	case LTLAMenuBar::Interval20SecondsID: GUI.StageCalibrationInterval = 20;
		break;
	case LTLAMenuBar::CalibrationStartID:
	{
		GUI.StageCalibrationCounter = 0;
		startTimer(CalibrationIntervalTimer, GUI.StageCalibrationInterval * 1000);
		CalibrationCountDownLabel.setText("Calibrating Front Left", dontSendNotification);
	}
	break;
	case LTLAMenuBar::DrawStageID: GUI.GetStageDrawingState() == true ? GUI.SetStageDrawingState(false) : GUI.SetStageDrawingState(true);
		break;
	case LTLAMenuBar::EditStageID: 
		GUI.GetStageEditState() == true ? GUI.SetStageEditState(false) : GUI.SetStageEditState(true);
		GUI.SetStageAreaEditState(false);
		break;
	case LTLAMenuBar::DrawGridID: GUI.GetGridDrawingState() == true ? GUI.SetGridDrawingState(false) : GUI.SetGridDrawingState(true);
		break;
	case LTLAMenuBar::SnaptoGridID: GUI.GetGridSnappingState() == true ? GUI.SetGridSnappingState(false) : GUI.SetGridSnappingState(true);
		break;
	case LTLAMenuBar::GridSize10ID: GUI.SetGridIncrement(10);
		break;
	case LTLAMenuBar::GridSize15ID: GUI.SetGridIncrement(15);
		break;
	case LTLAMenuBar::GridSize20ID: GUI.SetGridIncrement(20);
		break;
	case LTLAMenuBar::AddStageAreaID: GUI.StageAreas.add(new StageArea); DBG("Adding new Stage Area");
		break;
	case LTLAMenuBar::EditStageAreasID: 
		GUI.GetStageAreaEditState() == true ? GUI.SetStageAreaEditState(false) : GUI.SetStageAreaEditState(true);
		GUI.SetStageEditState(false);
		break;
	case LTLAMenuBar::RemoveStageAreaID: GUI.StageAreas.remove(GUI.GetCurrentlySelectedArea(), true);
		break;
	default: return false;
	}
	return true;

}
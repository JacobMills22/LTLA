
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
		MenuBar.GridSize15ID, MenuBar.GridSize20ID, MenuBar.AddStageAreaID, MenuBar.EditStageAreasID, MenuBar.RemoveStageAreaID, MenuBar.AudioParametersID,
		MenuBar.AudioDeviceSettingsID };
	
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
		GUI.StageAreas.size() > 0 ? result.setActive(true) : result.setActive(false);
		GUI.GetStageAreaEditState() == true ? result.setTicked(true) : result.setTicked(false);
		break;
	case LTLAMenuBar::RemoveStageAreaID:
		result.setInfo("Remove Selected Area", "Removes the currently selected shape.", "Stage Areas", 0);
		GUI.GetStageAreaEditState() == true ? result.setActive(true) : result.setActive(false);
		break;
	case LTLAMenuBar::AudioParametersID:
		result.setInfo("Edit Area Parameters", "Open a panel to edit audio parameters", "Stage Areas", 0);
		GUI.GetStageAreaEditState() == true ? result.setActive(true) : result.setActive(false);
		GUI.getAudioPanelState() == true ? result.setTicked(true) : result.setTicked(false);
		break;
	case LTLAMenuBar::AudioDeviceSettingsID:
		result.setInfo("Audio Device Settings", "Open a panel to edit audio device settings", "Preferences", 0);
		audioSettingsWindow.isVisible() == true ? result.setTicked(true) : result.setTicked(false);
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
		if (GUI.getAudioPanelState() == true)
		{
			editAudioParametersPressed();
		}
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
	case LTLAMenuBar::AddStageAreaID: AddStageAreaIDPressed();
		break;
	case LTLAMenuBar::EditStageAreasID: EditStageAreasIDPressed();
		break;
	case LTLAMenuBar::RemoveStageAreaID: RemoveStageAreaIDPressed();
		break;
	case LTLAMenuBar::AudioParametersID: editAudioParametersPressed();
		break;
	case LTLAMenuBar::AudioDeviceSettingsID: audioSettingsWindow.isVisible() == true ? audioSettingsWindow.setVisible(false) : audioSettingsWindow.setVisible(true);
		break;
	default: return false; 
	}
	return true;
}

void MainContentComponent::AddStageAreaIDPressed()
{
	GUI.StageAreas.add(new StageArea); // Add a new area.
	GUI.SetStageEditState(false);
	GUI.SetStageAreaEditState(true); // Enable menuBars edit area button.
	GUI.StageAreas[GUI.GetCurrentlySelectedArea()]->SetAreaSelectedState(false); // Deselect currently selected area.
	GUI.StageAreas.getLast()->SetAreaSelectedState(true); // Select the newly created area.
	GUI.SetCurrentlySelectedArea(GUI.StageAreas.size() - 1); // Set Selected Area Index to the newly created area.
	GUI.StageAreas.getLast()->updateTrackingGUIWidthAndHeight(GUI.getWidth(), GUI.getHeight());

	valueTree.addChild(GUI.StageAreas.getLast()->getValueTree(), 0, nullptr);

	GUI.StageAreas.getLast()->setValueTreeChildIndex(valueTree.getNumChildren());

	audioEngine.addNewStageAreaAudioPanel();

	valueTree.addChild(audioEngine.audioPanel.getLast()->getValueTree(), 0, nullptr);
	//DBG((String)valueTree.getNumChildren());
}


void MainContentComponent::EditStageAreasIDPressed()
{
	if (GUI.GetStageAreaEditState() == false)
	{
		GUI.SetStageAreaEditState(true);
		GUI.StageAreas[0]->SetAreaSelectedState(true);
		GUI.SetCurrentlySelectedArea(0);
	}
	else if (GUI.GetStageAreaEditState() == true)
	{
		GUI.SetStageAreaEditState(false);
		for (int AreaIndex = 0; AreaIndex < GUI.StageAreas.size(); AreaIndex++)
		{
			GUI.StageAreas[AreaIndex]->SetAreaSelectedState(false);
		}
	}
	GUI.SetStageEditState(false);
}

void MainContentComponent::RemoveStageAreaIDPressed()
{
	//valueTree.removeChild(GUI.StageAreas[GUI.GetCurrentlySelectedArea()]->getValueTreeChildIndex(), nullptr);
	//DBG( "Index beign removed is  " + (String)GUI.StageAreas[GUI.GetCurrentlySelectedArea()]->getValueTreeChildIndex());
	//GUI.StageAreas.remove(GUI.GetCurrentlySelectedArea(), true);

	GUI.StageAreas[GUI.GetCurrentlySelectedArea()]->setActive(false);

	if (GUI.StageAreas.size() < 1) 
	{ 
		GUI.SetStageAreaEditState(false); 
		AreaColourSelector.setVisible(false);
	}
	else
	{
		GUI.SetCurrentlySelectedArea(0);
		GUI.StageAreas[0]->SetAreaSelectedState(true);
	}
}

void MainContentComponent::editAudioParametersPressed()
{
	GUI.setAudioPanelState(!GUI.getAudioPanelState());

	if (GUI.getAudioPanelState() == true)
	{
		AreaColourSelector.setVisible(true);
		audioEngine.setVisible(true);
	}
	else
	{
		AreaColourSelector.setVisible(false);
		audioEngine.setVisible(false);

		for (int AreaIndex = 0; AreaIndex < GUI.StageAreas.size(); AreaIndex++)
		{
			GUI.StageAreas[AreaIndex]->SetAreaSelectedState(false);
		}
	}
	resized();
}
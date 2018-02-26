
#pragma once

#include "../Source/MainComponent.h"

/** Function declarations are located in Source/MainComponent.h*/

ApplicationCommandTarget* MainContentComponent::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void MainContentComponent::getAllCommands(Array<CommandID>& commands)
{
	const CommandID ids[] = { menuBar.interval5SecondsID, menuBar.interval10SecondsID, menuBar.interval20SecondsID,
		menuBar.calibrationStartID, menuBar.drawStageID, menuBar.editStageID, menuBar.drawGridID, menuBar.snaptoGridID, menuBar.gridSize10ID,
		menuBar.gridSize15ID, menuBar.gridSize20ID, menuBar.addStageAreaID, menuBar.editStageAreasID, menuBar.removeStageAreaID, menuBar.audioParametersID,
		menuBar.audioDeviceSettingsID };
	
	commands.addArray(ids, numElementsInArray(ids));
}

void MainContentComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	switch (commandID)
	{
	case LTLAMenuBar::interval5SecondsID:
		result.setInfo("5 Seconds", "Set the Interval to 5 seconds", "Calibration", 0);
		if (trackingGUI.stageCalibrationInterval == 5) result.setTicked(true);
		break;
	case LTLAMenuBar::interval10SecondsID:
		result.setInfo("10 Seconds", "Set the Interval to 10 seconds", "Calibration", 0);
		if (trackingGUI.stageCalibrationInterval == 10) result.setTicked(true);
		break;
	case LTLAMenuBar::interval20SecondsID:
		result.setInfo("20 Seconds", "Set the Interval to 20 seconds", "Calibration", 0);
		if (trackingGUI.stageCalibrationInterval == 20) result.setTicked(true);
		break;
	case LTLAMenuBar::calibrationStartID:
		result.setInfo("Start Calibration", "Starts the calibration sequence.", "Calibration", 0);
		break;
	case LTLAMenuBar::drawStageID:
		result.setInfo("Draw Stage", "Draws the stage once calibrated.", "Calibration", 0);
		result.setTicked(trackingGUI.getStageDrawingState());
		break;
	case LTLAMenuBar::editStageID:
		result.setInfo("Edit Stage", "Edits the stage shape.", "Calibration", 0);
		result.setTicked(trackingGUI.getStageEditState());
		break;
	case LTLAMenuBar::drawGridID:
		result.setInfo("Draw Grid", "Draws a Grid.", "Grid", 0);
		result.setTicked(trackingGUI.getGridDrawingState());
		break;
	case LTLAMenuBar::snaptoGridID:
		result.setInfo("Enable Grid Snapping", "", "Grid", 0);
		result.setTicked(trackingGUI.getGridSnappingState());
		break;
	case LTLAMenuBar::gridSize10ID:
		result.setInfo("Grid Size 10", "", "Grid", 0);
		break;
	case LTLAMenuBar::gridSize15ID:
		result.setInfo("Grid Size 15", "", "Grid", 0);
		break;
	case LTLAMenuBar::gridSize20ID:
		result.setInfo("Grid Size 20", "", "Grid", 0);
		break;
	case LTLAMenuBar::addStageAreaID:
		result.setInfo("Add Area", "Adds a resizeable area to the stage which can be assigned for different audio processing", "Stage Areas", 0);
		break;
	case LTLAMenuBar::editStageAreasID:
		result.setInfo("Edit Stage Areas", "Resize current stage areas", "Stage Areas", 0);
		trackingGUI.stageAreas.size() > 0 ? result.setActive(true) : result.setActive(false);
		trackingGUI.getStageAreaEditState() == true ? result.setTicked(true) : result.setTicked(false);
		break;
	case LTLAMenuBar::removeStageAreaID:
		result.setInfo("Remove Selected Area", "Removes the currently selected shape.", "Stage Areas", 0);
		trackingGUI.getStageAreaEditState() == true ? result.setActive(true) : result.setActive(false);
		break;
	case LTLAMenuBar::audioParametersID:
		result.setInfo("Edit Area Parameters", "Open a panel to edit audio parameters", "Stage Areas", 0);
		trackingGUI.getStageAreaEditState() == true ? result.setActive(true) : result.setActive(false);
		trackingGUI.getAudioPanelState() == true ? result.setTicked(true) : result.setTicked(false);
		break;
	case LTLAMenuBar::audioDeviceSettingsID:
		result.setInfo("Audio Device Settings", "Open a panel to edit audio device settings", "Preferences", 0);
		audioSettingsWindow.isVisible() == true ? result.setTicked(true) : result.setTicked(false);
	}
}

bool MainContentComponent::perform(const InvocationInfo& info)
{
	switch (info.commandID)
	{
	case LTLAMenuBar::interval5SecondsID: trackingGUI.stageCalibrationInterval = 5;
		break;
	case LTLAMenuBar::interval10SecondsID: trackingGUI.stageCalibrationInterval = 10;
		break;
	case LTLAMenuBar::interval20SecondsID: trackingGUI.stageCalibrationInterval = 20;
		break;
	case LTLAMenuBar::calibrationStartID:
	{
		trackingGUI.stageCalibrationCounter = 0;
		startTimer(calibrationIntervalTimer, trackingGUI.stageCalibrationInterval * 1000);
		calibrationCountDownLabel.setText("Calibrating Front Left", dontSendNotification);
	}
	break;
	case LTLAMenuBar::drawStageID: trackingGUI.getStageDrawingState() == true ? trackingGUI.setStageDrawingState(false) : trackingGUI.setStageDrawingState(true);
		break;
	case LTLAMenuBar::editStageID: 
		trackingGUI.getStageEditState() == true ? trackingGUI.setStageEditState(false) : trackingGUI.setStageEditState(true);
		trackingGUI.setStageAreaEditState(false);
		if (trackingGUI.getAudioPanelState() == true)
		{
			editAudioParametersPressed();
		}
		break;
	case LTLAMenuBar::drawGridID: trackingGUI.getGridDrawingState() == true ? trackingGUI.setGridDrawingState(false) : trackingGUI.setGridDrawingState(true);
		break;
	case LTLAMenuBar::snaptoGridID: trackingGUI.getGridSnappingState() == true ? trackingGUI.setGridSnappingState(false) : trackingGUI.setGridSnappingState(true);
		break;
	case LTLAMenuBar::gridSize10ID: trackingGUI.setGridIncrement(10);
		break;
	case LTLAMenuBar::gridSize15ID: trackingGUI.setGridIncrement(15);
		break;
	case LTLAMenuBar::gridSize20ID: trackingGUI.setGridIncrement(20);
		break;
	case LTLAMenuBar::addStageAreaID: addStageAreaIDPressed();
		break;
	case LTLAMenuBar::editStageAreasID: editStageAreasIDPressed();
		break;
	case LTLAMenuBar::removeStageAreaID: removeStageAreaIDPressed();
		break;
	case LTLAMenuBar::audioParametersID: editAudioParametersPressed();
		break;
	case LTLAMenuBar::audioDeviceSettingsID: audioSettingsWindow.isVisible() == true ? audioSettingsWindow.setVisible(false) : audioSettingsWindow.setVisible(true);
		break;
	default: return false; 
	}
	return true;
}

void MainContentComponent::addStageAreaIDPressed()
{
	trackingGUI.stageAreas.add(new StageArea); // Add a new area.
	trackingGUI.setStageEditState(false);
	trackingGUI.setStageAreaEditState(true); // Enable menuBars edit area button.
	trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->setAreaSelectedState(false); // Deselect currently selected area.
	trackingGUI.stageAreas.getLast()->setAreaSelectedState(true); // Select the newly created area.
	trackingGUI.setCurrentlySelectedArea(trackingGUI.stageAreas.size() - 1); // Set Selected Area Index to the newly created area.
	trackingGUI.stageAreas.getLast()->updateTrackingGUIWidthAndHeight(trackingGUI.getWidth(), trackingGUI.getHeight());

	valueTree.addChild(trackingGUI.stageAreas.getLast()->getValueTree(), 0, nullptr);

	trackingGUI.stageAreas.getLast()->setValueTreeChildIndex(valueTree.getNumChildren());

	audioEngine.addNewStageAreaAudioPanel();

	valueTree.addChild(audioEngine.audioPanel.getLast()->getValueTree(), 0, nullptr);
	//DBG((String)valueTree.getNumChildren());
}


void MainContentComponent::editStageAreasIDPressed()
{
	if (trackingGUI.getStageAreaEditState() == false)
	{
		trackingGUI.setStageAreaEditState(true);
		trackingGUI.stageAreas[0]->setAreaSelectedState(true);
		trackingGUI.setCurrentlySelectedArea(0);
	}
	else if (trackingGUI.getStageAreaEditState() == true)
	{
		trackingGUI.setStageAreaEditState(false);
		for (int areaIndex = 0; areaIndex < trackingGUI.stageAreas.size(); areaIndex++)
		{
			trackingGUI.stageAreas[areaIndex]->setAreaSelectedState(false);
		}
	}
	trackingGUI.setStageEditState(false);
}

void MainContentComponent::removeStageAreaIDPressed()
{
	trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->setActive(false);

	if (trackingGUI.stageAreas.size() < 1)
	{ 
		trackingGUI.setStageAreaEditState(false);
		areaColourSelector.setVisible(false);
	}
	else
	{
		trackingGUI.setCurrentlySelectedArea(0);
		trackingGUI.stageAreas[0]->setAreaSelectedState(true);
	}
}

void MainContentComponent::editAudioParametersPressed()
{
	trackingGUI.setAudioPanelState(!trackingGUI.getAudioPanelState());

	if (trackingGUI.getAudioPanelState() == true)
	{
		areaColourSelector.setVisible(true);
		audioEngine.setVisible(true);
	}
	else
	{
		areaColourSelector.setVisible(false);
		audioEngine.setVisible(false);

		for (int areaIndex = 0; areaIndex < trackingGUI.stageAreas.size(); areaIndex++)
		{
			trackingGUI.stageAreas[areaIndex]->setAreaSelectedState(false);
		}
	}
	resized();
}
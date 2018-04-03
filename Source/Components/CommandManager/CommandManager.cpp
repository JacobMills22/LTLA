
#pragma once

#include "../Source/MainComponent.h"

/** Function declarations are located in Source/MainComponent.h*/

ApplicationCommandTarget* MainContentComponent::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void MainContentComponent::getAllCommands(Array<CommandID>& commands)
{
	
	const CommandID ids[] = {  menuBar.saveID, menuBar.saveAsID, menuBar.loadID, menuBar.interval5SecondsID, menuBar.interval10SecondsID, menuBar.interval20SecondsID,
		menuBar.calibrationStartID, menuBar.drawStageID, menuBar.editStageID, menuBar.drawGridID, menuBar.snaptoGridID, menuBar.gridSize10ID,
		menuBar.gridSize15ID, menuBar.gridSize20ID, menuBar.addStageAreaID, menuBar.editStageAreasID, menuBar.removeStageAreaID, menuBar.audioParametersID,
		menuBar.audioDeviceSettingsID, menuBar.performerSimulation };
	
	commands.addArray(ids, numElementsInArray(ids));
	
}

void MainContentComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	switch (commandID)
	{
	case LTLAMenuBar::saveID:
		result.setInfo("Save Project", "Saves the whole project", "File", 0);
		break;
	case LTLAMenuBar::saveAsID:
		result.setInfo("Save Project As", "Saves the whole project as a new file", "File", 0);
		break;
	case LTLAMenuBar::loadID:
		result.setInfo("Load Project", "Loads a project", "File", 0);
		break;
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
		break;
	case LTLAMenuBar::performerSimulation:
		result.setInfo("Simulate Performer 1", "Allows performer 1s position to be manipulated by mouse", "Preferences", 0);
		result.setTicked(trackingGUI.isPerformerBeingSimulated());
	}
}

bool MainContentComponent::perform(const InvocationInfo& info)
{
	switch (info.commandID)
	{
	case LTLAMenuBar::saveAsID: saveProjectAs();
		break;
	case LTLAMenuBar::saveID: saveProject();
		break;
	case LTLAMenuBar::loadID: loadProjectFile(valueTree);
		break;
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
	case LTLAMenuBar::performerSimulation: trackingGUI.isPerformerBeingSimulated() == true ? trackingGUI.simulatePerformer1(false) : trackingGUI.simulatePerformer1(true);
		break;
	default: return false; 
	}
	return true;
}

void MainContentComponent::addStageAreaIDPressed()
{
	trackingGUI.stageAreas.add(new StageArea); // Add a new area.
	trackingGUI.deselectAllStageAreas();	  // Desleect all areas.

	trackingGUI.setStageEditState(false);
	trackingGUI.setStageAreaEditState(true); // Enable menuBars edit area button.
	trackingGUI.stageAreas.getLast()->setAreaSelectedState(true); // Select the newly created area.
	trackingGUI.setCurrentlySelectedArea(trackingGUI.stageAreas.size() - 1); // Set Selected Area Index to the newly created area.
	trackingGUI.stageAreas.getLast()->updateTrackingGUIWidthAndHeight(trackingGUI.getWidth(), trackingGUI.getHeight());

	valueTree.addChild(trackingGUI.stageAreas.getLast()->getValueTree(), 0, nullptr);
	trackingGUI.stageAreas.getLast()->setValueTreeChildIndex(valueTree.getNumChildren());

	audioEngine.addNewStageAreaAudioPanel(); // Add a new audio panel to the engine for the newly created area.

	valueTree.addChild(audioEngine.audioPanel.getLast()->getValueTree(), 0, nullptr);

	trackingGUI.numOfStageAreas = trackingGUI.stageAreas.size(); // Update the number of stage areas
}


void MainContentComponent::editStageAreasIDPressed()
{
	if (trackingGUI.getStageAreaEditState() == false) // If Stage area edit mode was disabled
	{
		trackingGUI.setStageAreaEditState(true); // Enable stage area edit mode
		trackingGUI.stageAreas[0]->setAreaSelectedState(true); // Select the first stage area
		trackingGUI.setCurrentlySelectedArea(0); // Set the id of the newly selected area.
	}
	else if (trackingGUI.getStageAreaEditState() == true) // If Stage area edit mode was enabled
	{
		trackingGUI.setStageAreaEditState(false); // Disable stage area edit mode
		trackingGUI.deselectAllStageAreas(); // Deslect all stage areas
	}
	trackingGUI.setStageEditState(false); // Set the main stage area edit state to false.
}

void MainContentComponent::removeStageAreaIDPressed()
{
	// Note: To allow for snapshots to have a different number of stage areas in each snapshot,
	// stage areas a never fully deleted until the app closes, instead they are set to be "inactive"
	// The inactive states essentially just means they afe invisible and in no way respond to the performers.

	// Set the curently selected areas state to inactive. (Completely hides it from the user)
	trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->setActive(false);

	// If no more stage areas are active disable edit mode and hide the colour picker.
	if (trackingGUI.stageAreas.size() < 1)
	{ 
		trackingGUI.setStageAreaEditState(false);
		areaColourSelector.setVisible(false);
	}
	else // If some stage areas are still active, select the first area.
	{
		trackingGUI.setCurrentlySelectedArea(0);
		trackingGUI.stageAreas[0]->setAreaSelectedState(true);
	}

	// Update the number of stage areas.
	trackingGUI.numOfStageAreas = trackingGUI.stageAreas.size();
}

void MainContentComponent::editAudioParametersPressed()
{
	// Enable or disable the audio panel view
	trackingGUI.setAudioPanelState(!trackingGUI.getAudioPanelState());

	// if the audio panel is enabled
	if (trackingGUI.getAudioPanelState() == true)
	{
		// Display the colour picker and the audio engine.
		areaColourSelector.setVisible(true);
		audioEngine.setVisible(true);
	}
	else // if the audio panel is disabled
	{
		// Hide the colour picker and the audio engine.
		areaColourSelector.setVisible(false);
		audioEngine.setVisible(false);

		// Deselect all stage areas.
		trackingGUI.deselectAllStageAreas();
	}
	// Update components sizes.
	resized();
}
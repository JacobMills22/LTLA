
#include "../Source/MainComponent.h"

/** These functions are declared in MainContentComponent
	and are used to save and load XML projects. */

void MainContentComponent::askUserIfSnapshotShouldBeUpdated()
{
	// If there is only one snapshot simply update it.
	if (snapshotManager.getNumberOfSnapshots() <= 1)
	{
		snapshotManager.updateCurrentSnapshot();
	}
	else 
	{
		// else display an alert window asking the user if the current snapshot needs updating.
		int updateSnapshot = messageBox.showYesNoBox(AlertWindow::QuestionIcon, "Update Snapshot?", "Update Current Snapshot Before Saving?", nullptr);

		// If the user selected yes, update the current snapshot.
		if (updateSnapshot == true)
			snapshotManager.updateCurrentSnapshot();
	}
}


void MainContentComponent::saveProjectAs()
{
	// Update the stored number od snapshots in the project which is being saved.
	valueTree.setProperty("NumberOfSnapshots", snapshotManager.getNumberOfSnapshots(), nullptr);

	// Ask user if the current snapshot should be updated before saving.
	askUserIfSnapshotShouldBeUpdated();

	// Browse for a save file location and name.
	FileChooser fileChooser("Select a location to save", File::getSpecialLocation(File::userHomeDirectory), "", true, false);
	fileChooser.browseForFileToSave(true);

	// If the file path is valid.
	if (fileChooser.getResult().getFullPathName().isEmpty() == false)
	{
		// Store the number of snapshots.
		int numOfSnapshots = snapshotManager.getNumberOfSnapshots();

		// Create a file at the file path selected by the user.
		currentProjectFullPath = fileChooser.getResult().getFullPathName();
		File fileToSave(currentProjectFullPath);

		// Create a temporary value tree which is a complete copy of the current tree.
		ValueTree treeToSave = valueTree.createCopy();

		// Get the number of children the current tree has.
		int childrenNum = treeToSave.getNumChildren();

		// Remove all children (snapshots) from the temporary tree.
		for (int id = 0; id < childrenNum; id++)
			treeToSave.removeChild(id, nullptr);

		// Add all children (snapshots) to the the temporary tree.
		for (int s = 0; s < numOfSnapshots; s++)
			treeToSave.addChild(snapshotManager.getSnapshotValueTree(s), -1, nullptr);

		// Create an XML Element from the temporary tree and write the element to the new file.
		XmlElement xmlElement("SavedProject");
		xmlElement = *treeToSave.createXml();
		xmlElement.writeToFile(fileToSave, "", "UTF-8", 60);
	}
}

void MainContentComponent::saveProject()
{
	if (currentProjectFullPath == "")
	{
		// If the project has not been saved before "save project as" instead
		saveProjectAs();
	}
	else
	{
		// Ask user if the current snapshot should be updated before saving.
		askUserIfSnapshotShouldBeUpdated();

		// Store the number of snapshots.
		int numOfSnapshots = snapshotManager.getNumberOfSnapshots();

		// Create a new file with the same path as the old save file. 
		File fileToSave(currentProjectFullPath);

		// Create a temporary value tree which is a complete copy of the current tree.
		ValueTree treeToSave = valueTree.createCopy();

		// Get the number of children the current tree has.
		int childrenNum = treeToSave.getNumChildren();

		// Remove all children (snapshots) from the temporary tree.
		for (int id = 0; id < childrenNum; id++)
			treeToSave.removeChild(id, nullptr);
		
		// Add all children (snapshots) to the the temporary tree.
		for (int s = 0; s < numOfSnapshots; s++)
			treeToSave.addChild(snapshotManager.getSnapshotValueTree(s), -1, nullptr);
		
		// Create an XML Element from the temporary tree and write the element to the new file.
		XmlElement xmlElement("SavedProject");
		xmlElement = *treeToSave.createXml();
		xmlElement.writeToFile(fileToSave, "", "UTF-8", 60);
	}
}

void MainContentComponent::loadProjectFile(ValueTree valueTreeToLoadInto)
{
	// Prompt user to look for a file to load.
	FileChooser fileChooser("Select a project file to load", File::getSpecialLocation(File::userHomeDirectory), "", true, false);
	fileChooser.browseForFileToOpen(nullptr);
	
	// If the file is valid.
	if (fileChooser.getResult().existsAsFile() == true)
	{
		// Create an XML document containing the file.
		ScopedPointer<XmlElement> xml(XmlDocument::parse(fileChooser.getResult()));

		// If the converted tree is valid
		if (ValueTree::fromXml(*xml).isValid() == true)
		{
			trackingGUI.stageAreas.clear(true); // Clear all old stage areas.
			snapshotManager.clearAllSnapshots(true); // Clear all old snapshots.
			// Store the number of stage areas and snapshots that need to be loaded.
			int numOfStageAreas = ValueTree::fromXml(*xml).getPropertyAsValue("NumberOfStageAreas", nullptr).getValue();
			int numOfSnapshots = ValueTree::fromXml(*xml).getPropertyAsValue("NumberOfSnapshots", nullptr).getValue();

			// Add needed number of stage areas.
			for (int c = 0; c < numOfStageAreas; c++)
				addStageAreaIDPressed();

			// Add needed number of snapshots.
			for (int c = 0; c < numOfSnapshots; c++)
				snapshotManager.addNewBlankSnapshot(ValueTree::fromXml(*xml), c + numOfStageAreas);

			// Load the tree into the current tree
			valueTreeToLoadInto = ValueTree::fromXml(*xml).createCopy();
			
			// Reset the current snapshot to the start.
			snapshotManager.setCurrentSnapshot(0, true);

			// Tell the audio engine to update.
			audioEngine.snapshotFired();
		}
	}
}
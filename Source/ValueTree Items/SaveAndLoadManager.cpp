
#include "../Source/MainComponent.h"

void MainContentComponent::saveProjectAs()
{
	valueTree.setProperty("NumberOfSnapshots", snapshotManager.getNumberOfSnapshots(), nullptr);

	FileChooser fileChooser("Select a location to save", File::getSpecialLocation(File::userHomeDirectory), "", true, false);
	fileChooser.browseForFileToSave(true);

	if (fileChooser.getResult().getFullPathName().isEmpty() == false)
	{
		int numOfSnapshots = snapshotManager.getNumberOfSnapshots();
		snapshotManager.setCurrentSnapshot(numOfSnapshots - 1, true);

		currentProjectFullPath = fileChooser.getResult().getFullPathName();
		File fileToSave(currentProjectFullPath);

		XmlElement xmlElement("SavedProject");
		ValueTree treeToSave = valueTree.createCopy();
		int childrenNum = treeToSave.getNumChildren();

		for (int id = 0; id < childrenNum; id++)
		{
			treeToSave.removeChild(id, nullptr);
		}

		for (int s = 0; s < numOfSnapshots; s++)
		{
			treeToSave.addChild(snapshotManager.getSnapshotValueTree(s), -1, nullptr);
		}

		xmlElement = *treeToSave.createXml();
		xmlElement.writeToFile(fileToSave, "", "UTF-8", 60);
	}	

}

void MainContentComponent::saveProject()
{
	if (currentProjectFullPath == "")
	{
		saveProjectAs();
	}
	else
	{
		valueTree.setProperty("NumberOfSnapshots", snapshotManager.getNumberOfSnapshots() - 1, nullptr);

		File fileToSave(currentProjectFullPath);

		XmlElement xmlElement("SavedProject");
		xmlElement = *valueTree.createXml();
		xmlElement.writeToFile(fileToSave, "", "UTF-8", 60);

	}
}

void MainContentComponent::loadProjectFile(ValueTree valueTreeToLoadInto)
{
	FileChooser fileChooser("Select a project file to load", File::getSpecialLocation(File::userHomeDirectory), "", true, false);
	fileChooser.browseForFileToOpen(nullptr);

	XmlDocument xmlDocument(fileChooser.getResult());

	if (fileChooser.getResult().existsAsFile() == true)
	{
		ScopedPointer<XmlElement> xml(XmlDocument::parse(fileChooser.getResult()));

		if (ValueTree::fromXml(*xml).isValid() == true)
		{

			trackingGUI.stageAreas.clear(true);
			snapshotManager.clearAllSnapshots(true);
			int numOfStageAreas = ValueTree::fromXml(*xml).getPropertyAsValue("NumberOfStageAreas", nullptr).getValue();
			int numOfSnapshots = ValueTree::fromXml(*xml).getPropertyAsValue("NumberOfSnapshots", nullptr).getValue();

			for (int c = 0; c < numOfStageAreas; c++)
			{
				addStageAreaIDPressed();
			}

			for (int c = 0; c < numOfSnapshots; c++)
			{
				snapshotManager.addNewBlankSnapshot(ValueTree::fromXml(*xml), c + numOfStageAreas);
			}

			valueTreeToLoadInto = ValueTree::fromXml(*xml).createCopy();
			
			snapshotManager.setCurrentSnapshot(0, true);
			audioEngine.snapshotFired();
		}
	}
}

#include "../Source/MainComponent.h"

void MainContentComponent::saveProjectAs()
{
	valueTree.setProperty("NumberOfSnapshots", snapshotManager.getNumberOfSnapshots(), nullptr);

	FileChooser fileChooser("Select a location to save", File::getSpecialLocation(File::userHomeDirectory), "", true, false);
	fileChooser.browseForFileToSave(true);

	if (fileChooser.getResult().getFullPathName().isEmpty() == false)
	{
		currentProjectFullPath = fileChooser.getResult().getFullPathName();
		File fileToSave(currentProjectFullPath);

		XmlElement xmlElement("SavedProject");
		xmlElement = *valueTree.createXml();
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

			for (int c = 0; c < numOfSnapshots; c++)
			{
				snapshotManager.addNewBlankSnapshot(ValueTree::fromXml(*xml), numOfStageAreas * 2);
			}

			for (int c = 0; c < numOfStageAreas; c++)
			{
				addStageAreaIDPressed();
			}

			valueTreeToLoadInto.copyPropertiesFrom(ValueTree::fromXml(*xml), nullptr);

			for (int childTree = 0; childTree < numOfStageAreas * 2; childTree++)
			{
				valueTreeToLoadInto.getChild(childTree).copyPropertiesFrom(ValueTree::fromXml(*xml).getChild(childTree), nullptr);

				for (int subChildTree = 0; subChildTree < ValueTree::fromXml(*xml).getChild(childTree).getNumChildren(); subChildTree++)
				{
					valueTreeToLoadInto.getChild(childTree).getChild(subChildTree).copyPropertiesFrom(ValueTree::fromXml(*xml).getChild(childTree).getChild(subChildTree), nullptr);
				}
			}

			snapshotManager.setCurrentSnapshot(0, false);
			audioEngine.snapshotFired();
		}
	}
}
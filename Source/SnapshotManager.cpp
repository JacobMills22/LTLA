
#include "SnapshotManager.h"

	// Single Snapshot
	//====================================================
	void Snapshot::recallSnapshot(ValueTree valueTree, int index)
	{
		
		// Main Tree
		valueTree.copyPropertiesFrom(snapshotValueTree, nullptr);

		int numOfStageAreas = valueTree.getPropertyAsValue("NumberOfStageAreas", nullptr).getValue();
		
		for (int childTree = 0; childTree < valueTree.getNumChildren(); childTree++)
		{
			// Childtree such as StageAreas and Audio Panels
			//DBG("Number of trees at Level 2 is " + (String)valueTree.getNumChildren());
			
			valueTree.getChild(childTree).copyPropertiesFrom(snapshotValueTree.getChild(childTree), nullptr);
			

			for (int subChildTree = 0; subChildTree < valueTree.getChild(childTree).getNumChildren(); subChildTree++)
			{
				// Child Tree such as Fileplayer
				//DBG("Number of trees at Level 3 is " + (String)valueTree.getChild(childTree).getNumChildren());
				valueTree.getChild(childTree).getChild(subChildTree).copyPropertiesFrom(snapshotValueTree.getChild(childTree).getChild(subChildTree), nullptr);
			}
		}		
	}

	void Snapshot::updateSnapshot(ValueTree valueTree)
	{
		snapshotValueTree = valueTree.createCopy();
	}

	void Snapshot::setSnapshotName(String name)
	{
		snapshotName = name;
	}

	String Snapshot::getSnapshotName()
	{
		return snapshotName;
	}

	ValueTree Snapshot::getValueTree()
	{
		return snapshotValueTree;
	}

	void Snapshot::setValueTree(ValueTree tree)
	{
	}

	// Snapshot Manager
	//====================================================

	SnapshotManager::SnapshotManager()
	{
		for (int butttonID = 0; butttonID < numOfButtons; butttonID++)
		{
			addAndMakeVisible(snapshotButtons[butttonID]);
			snapshotButtons[butttonID].addListener(this);
		}
		snapshotButtons[firePreviousID].setButtonText("<");
		snapshotButtons[fireNextID].setButtonText(">");
		snapshotButtons[updateSnapshotID].setButtonText("Update");
		snapshotButtons[addNewSnapshotID].setButtonText("Add");

		addAndMakeVisible(currentSnapshotLabel);
		currentSnapshotLabel.setText("No Snapshot", dontSendNotification);
		currentSnapshotLabel.setJustificationType(Justification::centred);
		currentSnapshotLabel.addListener(this);
		currentSnapshotLabel.setEditable(false, true, false);

		//createNewSnapshot();
	}

	void SnapshotManager::createNewSnapshot()
	{
		snapshots.add(new Snapshot);
		valueTree.addChild(snapshots.getLast()->getValueTree(), -1, nullptr);
		if (valueTree.getNumChildren() <= 3)
		{
			snapshots.getLast()->valueTreeChildIndex = 2;
		}
		else
		{
			snapshots.getLast()->valueTreeChildIndex = valueTree.getNumChildren() - 1;
		}

		snapshots.getLast()->updateSnapshot(valueTree);
		snapshots.getLast()->setSnapshotName("Snapshot " + (String)(snapshots.size()));
		setCurrentSnapshot(snapshots.size() - 1, false);

		valueTree.setProperty("NumberOfSnapshots", snapshots.size(), nullptr);

	}

	void SnapshotManager::setValueTree(ValueTree ValueTree)
	{
		valueTree = ValueTree;

		valueTree.setProperty("NumberOfSnapshots", 1, nullptr);
		numberOfSnapshots.referTo(valueTree.getPropertyAsValue("NumberOfSnapshots", nullptr));
	}

	void SnapshotManager::setCurrentSnapshot(int ID, bool alsoFireSnapshot)
	{
		if (snapshots[currentSnapshotID] != nullptr)
		{
			currentSnapshotID = ID;
			currentSnapshotLabel.setText(snapshots[currentSnapshotID]->getSnapshotName(), dontSendNotification);
		}

		if (alsoFireSnapshot == true)
		{
			snapshots[currentSnapshotID]->recallSnapshot(valueTree, currentSnapshotID + 1);
			snapshotHasbeenFired = true;
		}
	}

	void SnapshotManager::buttonClicked(Button* button) 
	{
		if (button == &snapshotButtons[firePreviousID])
		{
			if (currentSnapshotID > 0)
			{
				currentSnapshotID -= 1;
				setCurrentSnapshot(currentSnapshotID, true);
			}
		}
		else if (button == &snapshotButtons[fireNextID])
		{
			if (currentSnapshotID < snapshots.size() - 1)
			{
				currentSnapshotID += 1;
				setCurrentSnapshot(currentSnapshotID, true);
			}
		}
		else if (button == &snapshotButtons[updateSnapshotID])
		{
			snapshots[currentSnapshotID]->updateSnapshot(valueTree);

		//	valueTree.getChild(snapshots[currentSnapshotID]->valueTreeChildIndex) = valueTree.createCopy();

			ValueTree currentTree = valueTree.getChild(snapshots[currentSnapshotID]->valueTreeChildIndex);

			int totalSnapshots = numberOfSnapshots.getValue();

			for (int childTree = 0; childTree < valueTree.getNumChildren() - totalSnapshots; childTree++)
			{
				if (currentTree.getChild(childTree).isValid())
				{
					currentTree.removeChild(childTree, nullptr);
					currentTree.addChild(valueTree.getChild(childTree).createCopy(), childTree, nullptr);

					//currentTree.getChild(childTree) = valueTree.getChild(childTree).createCopy();
				}
				else
				{
					currentTree.addChild(valueTree.getChild(childTree).createCopy(), -1, nullptr);
				}
			}
		//	currentTree.addChild(valueTree.getChild(0).createCopy(), -1, nullptr);
		//	currentTree.addChild(valueTree.getChild(1).createCopy(), -1, nullptr);

		//	currentTree.setProperty("Test", "testproeprty", nullptr);
		}
		else if (button == &snapshotButtons[addNewSnapshotID])
		{
			createNewSnapshot();
		}
	}

	void SnapshotManager::labelTextChanged(Label* labelThatHasChanged)
	{
		if (labelThatHasChanged == &currentSnapshotLabel)
		{
			snapshots[currentSnapshotID]->setSnapshotName(currentSnapshotLabel.getText());
		}
	}


	void SnapshotManager::resized() 
	{
		snapshotButtons[firePreviousID].setBounds(0, 0, getWidth() * 0.1, getHeight());
		currentSnapshotLabel.setBounds(getWidth() * 0.11, 0, getWidth() * 0.35, getHeight());
		snapshotButtons[updateSnapshotID].setBounds(getWidth() * 0.48, 0, getWidth() * 0.18, getHeight());
		snapshotButtons[addNewSnapshotID].setBounds(getWidth() * 0.68, 0, getWidth() * 0.15, getHeight());
		snapshotButtons[fireNextID].setBounds(getWidth() * 0.85, 0, getWidth() * 0.1, getHeight());
	}

	bool SnapshotManager::hasSnapshotBeenFired()
	{
		return snapshotHasbeenFired;
	}

	void SnapshotManager::setHasbeenFiredState(bool state)
	{
		snapshotHasbeenFired = state;
	}

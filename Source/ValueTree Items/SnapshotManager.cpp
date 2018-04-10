
#include "SnapshotManager.h"

    //====================================================
	// Single Snapshot
	//====================================================

	Snapshot::Snapshot() : snapshotValueTree("Snapshot")
	{
		snapshotValueTree.setProperty("SnapshotName", "New Snapshot", nullptr);
		snapshotName = "New Snapshot";
		snapshotName.referTo(snapshotValueTree.getPropertyAsValue("SnapshotName", nullptr));
	}

	void Snapshot::recallSnapshot(ValueTree valueTree)
	{
		int numOfStageAreas = valueTree.getPropertyAsValue("NumberOfStageAreas", nullptr).getValue();

		// Copy properties from the main tree.
		valueTree.copyPropertiesFrom(snapshotValueTree, nullptr);
		
		// For each child tree (StageAreas and Audio Panels)
		for (int childTree = 0; childTree < valueTree.getNumChildren(); childTree++)
		{
			// Copy properties from each child tree.
			valueTree.getChild(childTree).copyPropertiesFrom(snapshotValueTree.getChild(childTree), nullptr);
			
			// For each Sub child tree (FilePlayer, Filters etc)
			for (int subChildTree = 0; subChildTree < valueTree.getChild(childTree).getNumChildren(); subChildTree++)
			{
				// Copy properties from each sub child tree.
				valueTree.getChild(childTree).getChild(subChildTree).copyPropertiesFrom(snapshotValueTree.getChild(childTree).getChild(subChildTree), nullptr);
			}
		}		
	}

	void Snapshot::updateSnapshot(ValueTree valueTree)
	{
		// Store current settings into this snapshots own ValueTree.
		snapshotValueTree = valueTree.createCopy();
	}

	void Snapshot::updateSnapshotAtProjectLoad(ValueTree valueTree, int index)
	{
		// Store settings from XML project file into this snapshots own ValueTree.
		snapshotValueTree = valueTree.getChild(index).createCopy();
	}

	void Snapshot::setSnapshotName(String name)
	{
		snapshotName = name;
	}

	void Snapshot::setValueTreeChildIndex(int index)
	{
		valueTreeChildIndex = index;
	}

	int Snapshot::getValueTreeChildIndex()
	{
		return valueTreeChildIndex;
	}

	String Snapshot::getSnapshotName()
	{
		return snapshotName.getValue();
	}

	ValueTree Snapshot::getValueTree()
	{
		return snapshotValueTree;
	}

	//====================================================
	// Snapshot Manager
	//====================================================

	SnapshotManager::SnapshotManager()
	{
		// UI Initialisation

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
	}

	void SnapshotManager::createNewSnapshot()
	{
		// Add a new snapshot to the array.
		snapshots.add(new Snapshot);

		// (Save and Loading Manager)
		if (valueTree.getNumChildren() <= 3)
		{	// Tree index less than 2 are always occupied so the snapshot trees must start at an index of 2 
			snapshots.getLast()->setValueTreeChildIndex(2);
		}
		else
		{
			// Set the child tree index of the new snapshot
			snapshots.getLast()->setValueTreeChildIndex(valueTree.getNumChildren() - 1);
		}
		
		// Automatically update the newly created snapshot to the current settings, set the name of the new snapshot.
		snapshots.getLast()->updateSnapshot(valueTree);
		snapshots.getLast()->setSnapshotName("Snapshot " + (String)(snapshots.size()));
		setCurrentSnapshot(snapshots.size() - 1, false); // Refire the new snapshot for consistancy.

		// Update the number of snapshots for save and loading manager.
		valueTree.setProperty("NumberOfSnapshots", snapshots.size(), nullptr);

	}

	void SnapshotManager::updateCurrentSnapshot()
	{
		snapshots[currentSnapshotID]->updateSnapshot(valueTree);
	}


	void SnapshotManager::setValueTree(ValueTree ValueTree)
	{
		valueTree = ValueTree;

		valueTree.setProperty("NumberOfSnapshots", 1, nullptr);
		numberOfSnapshots.referTo(valueTree.getPropertyAsValue("NumberOfSnapshots", nullptr));
	}

	void SnapshotManager::setCurrentSnapshot(int ID, bool alsoFireSnapshot)
	{
		// Allows snapshots to be selected but not recalled

		if (snapshots[currentSnapshotID] != nullptr)
		{
			// Update cuurent snapshot ID and the current snapshot display label. 
			currentSnapshotID = ID;
			currentSnapshotLabel.setText(snapshots[currentSnapshotID]->getSnapshotName(), dontSendNotification);
		}

		if (alsoFireSnapshot == true)
		{
			// Recall ths specified snapshot if required, and set a flag to say that a snapshot has just been recalled.
			snapshots[currentSnapshotID]->recallSnapshot(valueTree);
			snapshotHasbeenFired = true;
		}
	}

	void SnapshotManager::buttonClicked(Button* button) 
	{
		if (button == &snapshotButtons[firePreviousID])
		{
			if (currentSnapshotID > 0)
			{
				// If the fire previous button has been pressed and the currently loaded snapshot isn't the first one
				// decrement the current snapshot ID and recall it.
				currentSnapshotID -= 1;
				setCurrentSnapshot(currentSnapshotID, true);
			}
		}
		else if (button == &snapshotButtons[fireNextID])
		{
			if (currentSnapshotID < snapshots.size() - 1)
			{
				// If the fire next button has been pressed and the currently loaded snapshot isn't the last one
				// increment the current snapshot ID and recall it.
				currentSnapshotID += 1;
				setCurrentSnapshot(currentSnapshotID, true);
			}
		}
		else if (button == &snapshotButtons[updateSnapshotID])
		{
			// if the update snapshot button is pressed, update the snapshot.
			updateCurrentSnapshot();
		}
		else if (button == &snapshotButtons[addNewSnapshotID])
		{
			// Add a new snapshot if the add new snapshot bytton was pressed
			createNewSnapshot();
		}
	}

	void SnapshotManager::labelTextChanged(Label* labelThatHasChanged)
	{
		// Update the current snapshots name if the name label has been changed.
		if (labelThatHasChanged == &currentSnapshotLabel)
		{
			snapshots[currentSnapshotID]->setSnapshotName(currentSnapshotLabel.getText());
		}
	}

	void SnapshotManager::addNewBlankSnapshot(ValueTree valueTreeFromXml, int index)
	{
		//(Save and Loading Manager)
		//Adds a snapshot ready for update from a loaded XML project file (Used for the save and loading manager)
		snapshots.add(new Snapshot);
		snapshots.getLast()->setSnapshotName("Snapshot " + (String)(snapshots.size()));

		//valueTree.addChild(snapshots.getLast()->getValueTree(), -1, nullptr);
		snapshots.getLast()->updateSnapshotAtProjectLoad(valueTreeFromXml, index);
		snapshots.getLast()->setSnapshotName(snapshots.getLast()->getSnapshotName());

	}

	void SnapshotManager::clearAllSnapshots(bool deleteObjects)
	{
		snapshots.clear(deleteObjects);
		currentSnapshotID = 0;
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

	int SnapshotManager::getNumberOfSnapshots()
	{
		return snapshots.size();
	}

	ValueTree SnapshotManager::getSnapshotValueTree(int ID)
	{
		return snapshots[ID]->getValueTree();
	}

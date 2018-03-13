#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Snapshot
{

public:

	Snapshot() : snapshotValueTree("Snapshot"), snapshotValueTreeStore("StoredSnapshotTree")
	{
		snapshotValueTree.setProperty("snapshotName", "SnapshotNameProperty", nullptr);
	}

	/** Copys all properties from this snapshot to the main value tree */
	void recallSnapshot(ValueTree valueTree, int index);

	/** Copys currently assigned properties and stores them in a seperate valuetree*/
	void updateSnapshot(ValueTree valueTree);

	/** Sets the name of this snapshot to be displayed */
	void setSnapshotName(String name);

	/** Returns the name of this snapshot*/
	String getSnapshotName();

	ValueTree getValueTree();

	void setValueTree(ValueTree tree);

	int valueTreeChildIndex = 3;

	void updateSnapshotAtProjectLoad(ValueTree valueTree, int index)
	{
		//snapshotValueTree.addChild(valueTree.getChild(index).createCopy(), -1, nullptr);

		snapshotValueTree = valueTree.getChild(index).createCopy();

		for (int childTree = 0; childTree < valueTree.getChild(index).getNumChildren() + 1; childTree++)
		{
			//snapshotValueTree.addChild(valueTree.getChild(index).getChild(childTree).createCopy(), -1, nullptr);
		}

		/*
		for (int childTree = 0; childTree < valueTree.getChild(index).getNumChildren(); childTree++)
		{
			if (snapshotValueTree.getChild(childTree).isValid())
			{
				snapshotValueTree.addChild(valueTree.getChild(index).createCopy(), childTree, nullptr);
				snapshotValueTree.removeChild(childTree, nullptr);
				DBG("CHILD WAS NOT VALID");

			//	snapshotValueTree.getChild(childTree) = valueTree.getChild(index).getChild(childTree).createCopy();
			}
			else
			{
				snapshotValueTree.addChild(valueTree.getChild(index).getChild(childTree).createCopy(), -1, nullptr);
				DBG("CHILD WAS VALID");

			}
		}
		*/

		//snapshotValueTree.addChild(valueTree.getChild(index).getChild(0).createCopy(), -1, nullptr);
		//snapshotValueTree.addChild(valueTree.getChild(index).getChild(1).createCopy(), -1, nullptr);

	}

private:

	ValueTree snapshotValueTree;
	ValueTree snapshotValueTreeStore;
	String snapshotName = "New Snapshot";

};

class SnapshotManager : public Component,
					    public Button::Listener,
						public Label::Listener
{
public: 

	/** Constructor, initialises GUI variables and creates one snapshot */
	SnapshotManager();

	/** Adds a new snapshot to the list */
	void createNewSnapshot();

	/** Sets the valuetree for use*/
	void setValueTree(ValueTree ValueTree);

	/** Sets the current snapshot which is in use, can also be used to recall a snapshot when needed*/
	void setCurrentSnapshot(int ID, bool alsoFireSnapshot);

	/** Responds to button presses such as fire next/previous*/
	void buttonClicked(Button* button) override;

	/** Responds to label text change messages, used to store the name of a snapshot*/
	void labelTextChanged(Label* labelThatHasChanged) override;
	
	/** Sets the bounds of components*/
	void resized() override;

	/** Returns true if a snapshot has been fired recently, used to tell the audio engine that it needs to update its parameters*/
	bool hasSnapshotBeenFired();

	/** Sets the has snapshot been fired state*/
	void setHasbeenFiredState(bool state);
	
	void addNewBlankSnapshot(ValueTree valueTreeFromXml, int numOfStageAreas)
	{
		snapshots.add(new Snapshot);
		snapshots.getLast()->setSnapshotName("Snapshot " + (String)(snapshots.size()));

		valueTree.addChild(snapshots.getLast()->getValueTree(), -1, nullptr);
		snapshots.getLast()->updateSnapshotAtProjectLoad(valueTreeFromXml, numOfStageAreas + snapshots.size() - 1);
		//snapshots.getLast()->updateSnapshot(valueTreeFromXml);

	}

	int getNumberOfSnapshots()
	{
		return snapshots.size();
	}

	void clearAllSnapshots(bool deleteObjects)
	{
		snapshots.clear(deleteObjects);
		currentSnapshotID = 0;
		
	}

private:

	ValueTree valueTree;
	OwnedArray<Snapshot> snapshots;

	enum {firePreviousID, fireNextID, updateSnapshotID, addNewSnapshotID, numOfButtons};
	TextButton snapshotButtons[numOfButtons];
	Label currentSnapshotLabel;

	int currentSnapshotID = 0;
	Value numberOfSnapshots;
	bool snapshotHasbeenFired = false;

};


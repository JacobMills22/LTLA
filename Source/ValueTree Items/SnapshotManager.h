#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/** This class represents a single snapshot and is used
	to store and recall the various valuetree parameters. */

class Snapshot
{

public:

	Snapshot();

	/** Copys all properties from this snapshot to the main value tree */
	void recallSnapshot(ValueTree valueTree);

	/** Copys currently assigned properties and stores them in a seperate valuetree*/
	void updateSnapshot(ValueTree valueTree);

	/** updates this snapshot from the loaded XML project file*/
	void updateSnapshotAtProjectLoad(ValueTree valueTree, int index);

	/** Sets the name of this snapshot to be displayed */
	void setSnapshotName(String name);

	/** Returns the name of this snapshot*/
	String getSnapshotName();
	
	/** Returns this snapshots ValueTree */
	ValueTree getValueTree();

	/** Sets the ValueTree child index of this snapshot (Used for project saving and loading)*/
	void setValueTreeChildIndex(int index);

	/** Gets the ValueTree child index of this snapshot (Used for project saving and loading)*/
	int getValueTreeChildIndex();

private:

	ValueTree snapshotValueTree;
	String snapshotName = "New Snapshot";
	int valueTreeChildIndex = 3;

};

/** This class represents the majority of the snapshot management system, 
	which contains a dynamic array of snapshots which contain various 
	parameter values. Each snapshot can be updated or recalled allowing
	a user to store a series of parameter values which can be instantly
	restored.*/

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
	
	/** Adds a snapshot ready for update from a loaded XML project file (Used for the save and loading manager)*/
	void addNewBlankSnapshot(ValueTree valueTreeFromXml, int index);

	/** Clears all snapshots with the option to delete them aswell */
	void clearAllSnapshots(bool deleteObjects);

	/** Sets the bounds of components*/
	void resized() override;

	/** Returns true if a snapshot has been fired recently, used to tell the audio engine that it needs to update its parameters*/
	bool hasSnapshotBeenFired();

	/** Sets the has snapshot been fired state*/
	void setHasbeenFiredState(bool state);
	
	/** Returns the number of snapshots currently created */
	int getNumberOfSnapshots();

	ValueTree getSnapshotValueTree(int ID)
	{
		return snapshots[ID]->getValueTree();
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


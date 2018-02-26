#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Snapshot
{

public:

	/** Copys all properties from this snapshot to the main value tree */
	void recallSnapshot(ValueTree valueTree);

	/** Copys currently assigned properties and stores them in a seperate valuetree*/
	void updateSnapshot(ValueTree valueTree);

	/** Sets the name of this snapshot to be displayed */
	void setSnapshotName(String name);

	/** Returns the name of this snapshot*/
	String getSnapshotName();

private:

	ValueTree snapshotValueTree;
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

private:

	ValueTree valueTree;
	OwnedArray<Snapshot> snapshots;

	enum {firePreviousID, fireNextID, updateSnapshotID, addNewSnapshotID, numOfButtons};
	TextButton snapshotButtons[numOfButtons];
	Label currentSnapshotLabel;

	int currentSnapshotID = 0;
	bool snapshotHasbeenFired = false;

};


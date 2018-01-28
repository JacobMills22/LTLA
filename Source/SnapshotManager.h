#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Snapshot
{

public:

	void recallSnapshot(ValueTree valueTree)
	{
		valueTree.copyPropertiesFrom(snapshotValueTree, nullptr);
	}

	void updateSnapshot(ValueTree valueTree)
	{
		snapshotValueTree = valueTree.createCopy();
	}

	void setSnapshotName(String name)
	{
		snapshotName = name;
	}

	String getSnapshotName()
	{
		return snapshotName;
	}

private:

	ValueTree snapshotValueTree;
	String snapshotName = "New Snapshot";

};

class SnapshotManager : public Component,
					    public Button::Listener,
						public Label::Listener
{
public: 

	SnapshotManager()
	{
		for (int butttonID = 0; butttonID < numOfButtons; butttonID++)
		{
			addAndMakeVisible(snapshotButtons[butttonID]);
			snapshotButtons[butttonID].addListener(this);
		}
		snapshotButtons[firePreviousID].setButtonText("<");
		snapshotButtons[fireNextID].setButtonText(">");
		snapshotButtons[updateSnapshotID].setButtonText("Update");
		snapshotButtons[addNewSnapshotID].setButtonText("Add New");

		addAndMakeVisible(currentSnapshotLabel);
		currentSnapshotLabel.setText("No Snapshot", dontSendNotification);
		currentSnapshotLabel.setJustificationType(Justification::centred);
		currentSnapshotLabel.addListener(this);
		currentSnapshotLabel.setEditable(false, true, false);
	}

	void createNewSnapshot()
	{
		snapshots.add(new Snapshot);
		snapshots.getLast()->updateSnapshot(valueTree);
		snapshots.getLast()->setSnapshotName("New Snappy");
		setCurrentSnapshot(snapshots.size() - 1, false);
	}

	void setValueTree(ValueTree ValueTree)
	{
		valueTree = ValueTree;
	}

	void setCurrentSnapshot(int ID, bool alsoFireSnapshot)
	{
		if (snapshots[currentSnapshotID] != nullptr)
		{
			currentSnapshotID = ID;
			currentSnapshotLabel.setText(snapshots[currentSnapshotID]->getSnapshotName(), dontSendNotification);
		}

		if (alsoFireSnapshot == true)
		{
			snapshots[currentSnapshotID]->recallSnapshot(valueTree);
		}
	}

	void buttonClicked(Button* button) override
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
		}
		else if (button == &snapshotButtons[addNewSnapshotID])
		{
			createNewSnapshot();
		}
	}

	void labelTextChanged(Label* labelThatHasChanged) override
	{
		if (labelThatHasChanged == &currentSnapshotLabel)
		{
			snapshots[currentSnapshotID]->setSnapshotName(currentSnapshotLabel.getText());
		}
	}

	
	void resized() override
	{
		snapshotButtons[firePreviousID].setBounds(0, 0, getWidth() * 0.1, getHeight());
		currentSnapshotLabel.setBounds(getWidth() * 0.11, 0, getWidth() * 0.4, getHeight());
		snapshotButtons[updateSnapshotID].setBounds(getWidth() * 0.51, 0, getWidth() * 0.15, getHeight());
		snapshotButtons[addNewSnapshotID].setBounds(getWidth() * 0.66, 0, getWidth() * 0.15, getHeight());
		snapshotButtons[fireNextID].setBounds(getWidth() * 0.91, 0, getWidth() * 0.1, getHeight());

	}

private:

	ValueTree valueTree;
	OwnedArray<Snapshot> snapshots;

	enum {firePreviousID, fireNextID, updateSnapshotID, addNewSnapshotID, numOfButtons};
	TextButton snapshotButtons[numOfButtons];
	Label currentSnapshotLabel;

	int currentSnapshotID = 0;

};


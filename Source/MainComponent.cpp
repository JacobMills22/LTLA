/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#define SKELETON_COUNT 2

	//==============================================================================
	/*
		This component lives inside our window, and this is where you should put all
		your controls and content.
	*/
	//==============================================================================
MainContentComponent::MainContentComponent() : audioDeviceSelector(deviceManager, 0, 256, 0, 256, false, false, true, true),
	                                           audioSettingsWindow("Audio Device Settings", false), valueTree("LTLAValueTree")            
	{
		// Initialise Components

		setSize(1280, 720);
		setAudioChannels(4, 2);
		kinectSensor.startKinectST();
		startTimer(kinectUpdateTimer, 40);
		startTimer(guiTimer, 15);

		addAndMakeVisible(trackingGUI);
		addAndMakeVisible(menuBar);

		addAndMakeVisible(audioEngine);
		audioEngine.setVisible(false);

		addAndMakeVisible(areaColourSelector);
		areaColourSelector.addChangeListener(this);
		areaColourSelector.setVisible(false);

		addAndMakeVisible(calibrationCountDownLabel);
		Font labelFont;
		labelFont.setSizeAndStyle(20, 1, 1, 0.25);
		calibrationCountDownLabel.setFont(labelFont);

		addAndMakeVisible(areaNameLabel);
		areaNameLabel.addListener(this);
		areaNameLabel.setEditable(true, true, false);
		areaNameLabel.setText("Enter Area Name", dontSendNotification);
		areaNameLabel.setJustificationType(Justification::centred);

		addAndMakeVisible(stereoAudioMeter);
						
		for (int buttonNum = 0; buttonNum < numOfButtons; buttonNum++)
		{
			addAndMakeVisible(globalButton[buttonNum]);
			globalButton[buttonNum].addListener(this);
		}

		globalButton[selectNextAreaButtonID].setButtonText("Next Area");
		globalButton[selectPreviousAreaButtonID].setButtonText("Previous Area");
				
		addAndMakeVisible(audioDeviceSelector);
		addAndMakeVisible(audioSettingsWindow);
		audioSettingsWindow.setVisible(false);
		audioSettingsWindow.setContentNonOwned(&audioDeviceSelector, true);
		
		auto& commandManager = LTLAcmd.getApplicationCommandManager();
		commandManager.registerAllCommandsForTarget(this);

		addAndMakeVisible(snapshotManager);

		valueTree.addListener(this);
		trackingGUI.setValueTree(valueTree);
		snapshotManager.setValueTree(valueTree);

		// Create one snapshot to begin with.
		snapshotManager.createNewSnapshot();
	}

	MainContentComponent::~MainContentComponent()
	{
		LTLAcmd.makeNullptr();
		shutdownAudio();
	}

	// MAIN AUDIO FUNCTIONS
	//==============================================================================
	void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		// Initialise audio parameters and engine.
		samplerate = sampleRate;
		samplesPerBlock = samplesPerBlockExpected;
		audioEngine.initialiseEngine(samplesPerBlockExpected, sampleRate);
		audioEngine.prepareToPlay(samplesPerBlockExpected, sampleRate);
		audioEngine.setDeviceManagerToUse(&deviceManager);
	}

	void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
	{
		// All audio processing is handled by the audioEngine.
		audioEngine.getNextAudioBlock(bufferToFill);
	}

	void MainContentComponent::releaseResources()
	{
		audioEngine.releaseResources();
		stopTimer(kinectUpdateTimer);
		stopTimer(guiTimer);
	}

	// GUI FUNCTION CALLERS
	//==============================================================================
	void MainContentComponent::paint(Graphics& g)
	{
		g.fillAll(Colours::darkgrey.darker());
	}

	void MainContentComponent::resized()
	{
		trackingGUI.resized();
		menuBar.setBounds(10, 10, 500, 30);
	
		if (trackingGUI.getAudioPanelState() == false)
			trackingGUI.setBounds(getBounds().reduced(50).getX(), getBounds().reduced(50).getY(), getBounds().reduced(50).getWidth(), getBounds().reduced(50).getHeight());
		else
			trackingGUI.setBounds(getBounds().reduced(50).getX(), getBounds().reduced(50).getY(), getBounds().reduced(50).getWidth(), getHeight() * 0.5);

		audioSettingsWindow.setBounds(trackingGUI.getX() + 1, trackingGUI.getY() + 1, 500, 600);
		audioDeviceSelector.setBounds(trackingGUI.getX() + 1, trackingGUI.getY() + 1, 500, 600);

		calibrationCountDownLabel.setBounds(getBounds().getWidth() - 150, getBounds().getHeight() - 50, 150, 50);
		areaColourSelector.setBounds(trackingGUI.getX() + trackingGUI.getWidth() * 0.9, trackingGUI.getHeight() + 55, trackingGUI.getWidth() * 0.1, (getHeight() - trackingGUI.getHeight()) * 0.3);

		audioEngine.setBounds(trackingGUI.getX(), trackingGUI.getHeight() + 50, trackingGUI.getWidth() - areaColourSelector.getWidth(), getHeight() - trackingGUI.getHeight());

		float MaxGuiHeight = getBounds().reduced(50).getHeight();
		int GlobalPanelHeight = getHeight() - ((getHeight() - MaxGuiHeight) * 0.4);

		globalButton[selectPreviousAreaButtonID].setBounds(trackingGUI.getX(), GlobalPanelHeight, getWidth() * 0.1, 25);
		areaNameLabel.setBounds(globalButton[selectPreviousAreaButtonID].getRight() + (getWidth() * 0.01), GlobalPanelHeight, getWidth() * 0.1, 25);
		globalButton[selectNextAreaButtonID].setBounds(areaNameLabel.getRight() + (getWidth() * 0.01), GlobalPanelHeight, getWidth() * 0.1, 25);

		stereoAudioMeter.setBounds(getBounds().reduced(50).getX() + trackingGUI.getWidth() + 5, trackingGUI.getBottom() - getHeight() * 0.2, 40, getHeight() * 0.2);

		snapshotManager.setBounds(globalButton[selectNextAreaButtonID].getRight() + getWidth() * 0.1, GlobalPanelHeight, getWidth() * 0.3, 25);

	}

	void MainContentComponent::timerCallback(int timerID)
	{
		if (timerID == kinectUpdateTimer)
		{
			// Update Kinect sensor
			kinectSensor.updateKinectST();
		}
		if (timerID == guiTimer)
		{
			// For each tracked skeleton (performer)
			for (int CurrentSkel = 0; CurrentSkel < SKELETON_COUNT; ++CurrentSkel)
			{
				if (kinectSensor.getKinectTrackingState(CurrentSkel) == true)
				{
					// Send positional data to the trackingGUi if they are being tracked.
					trackingGUI.setEllipseCoordinates(kinectSensor.getX(CurrentSkel), kinectSensor.getY(CurrentSkel), CurrentSkel);
				}

				// Send the tracking state to the trackingGUI
				trackingGUI.setKinectTrackingState(CurrentSkel, kinectSensor.getKinectTrackingState(CurrentSkel));
			}

			// If user has selected a new stage area
			if (trackingGUI.hasNewStageAreaBeenSelected() == true)
			{
				// Reset state to false, update the area name label and open the audio panel which relates to the newly selected stage area.
				trackingGUI.newStageAreaIsSelected(false);
				areaNameLabel.setText(trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->getAreaName(), dontSendNotification);
				audioEngine.reopenAudioPanel(trackingGUI.getCurrentlySelectedArea());
			}

			// If user has recalled a snapshot
			if (snapshotManager.hasSnapshotBeenFired() == true)
			{
				// Reset hasbeenFiredState to false, update the area name label, and tell the audioEngine that it needs to respond to a snapshot recall.  
				snapshotManager.setHasbeenFiredState(false);
				areaNameLabel.setText(trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->getAreaName(), dontSendNotification);
				trackingGUI.snapshotFired();
				audioEngine.snapshotFired();
			}

			// Update meter data.
			stereoAudioMeter.setMeterData(0, audioEngine.getMeterData(0));
			stereoAudioMeter.setMeterData(1, audioEngine.getMeterData(1));

			// Send various required data to the audioEngine.
			setAudioEngineData();

			repaint();
		}

		if (timerID == calibrationIntervalTimer)
		{
			/* Switch case called each time the calibration countdown as reached 0, each case updates the coordinates of one of the
			main stage areas corners and sets the text telling the user to prepare for the next update. Timer is stopped once calibrated. */
			switch (trackingGUI.stageCalibrationCounter)
			{
			case 0: trackingGUI.setStageCoordinates(trackingGUI.frontLeft, kinectSensor.getX(0), kinectSensor.getY(0));
				calibrationCountDownLabel.setText("Calibrating Front Right", dontSendNotification);
					break;
			case 1: trackingGUI.setStageCoordinates(trackingGUI.frontRight, kinectSensor.getX(0), kinectSensor.getY(0));
				calibrationCountDownLabel.setText("Calibrating Back Right", dontSendNotification);
					break;
			case 2: trackingGUI.setStageCoordinates(trackingGUI.backRight, kinectSensor.getX(0), kinectSensor.getY(0));
				calibrationCountDownLabel.setText("Calibrating Back Left", dontSendNotification);
					break;
			case 3:	trackingGUI.setStageCoordinates(trackingGUI.backLeft, kinectSensor.getX(0), kinectSensor.getY(0));
					stopTimer(calibrationIntervalTimer);
					calibrationCountDownLabel.setText("", dontSendNotification);
					break;
			}
			trackingGUI.stageCalibrationCounter++;
		}
	}

	void MainContentComponent::setAudioEngineData()
	{
		// For each perfomer
		for (int performerNum = 0; performerNum < 2; performerNum++)
		{	
			// For each stage area
			for (int areaID = 0; areaID < trackingGUI.stageAreas.size(); areaID++)
			{	
				// If the current performer is inside the current area
				if (trackingGUI.doesAreaIDContainPerfomer(performerNum, areaID))
				{	
					// If the current performer has already exited this area
					if (audioEngine.getPerformerExitedAreaState(performerNum, areaID) == true)
					{
						// Tell the audio engine which performer has just entered which area.
						audioEngine.setPerformerEnteredAreaState(performerNum, true, areaID);
					}
					else
					{
						// Tell the audio engine that this performer has not just entered this area.
						audioEngine.setPerformerEnteredAreaState(performerNum, false, areaID);
					}
						// If the current performer is stood inside the current area.
					if (trackingGUI.doesAreaIDContainPerfomer(performerNum, areaID) == true)
					{
						// Tell the audio engine that this perfomer is stood inside this area.
						audioEngine.setAreaIDContainingPerformerState(performerNum, areaID, true);
					}
					else
					{
						// Tell the audio engine that this perfomer is not stood inside this area.
						audioEngine.setAreaIDContainingPerformerState(performerNum, areaID, false);
					}

					// Since the performer is stood in the area they cannot of exited it.
					audioEngine.setPerformerExitedAreaState(performerNum, false, areaID);

					// Update the audio engines panning amount based on the performers position relative the the areas bounds.
					audioEngine.setAutoPannerAmount(trackingGUI.getPerformerXPosInsideArea(areaID, performerNum), areaID);
				}
				else // else if the current performer is not stood inside the current area.
				{
						// Esnure that the audioEngines states area correct
					if (audioEngine.getPerformerEnteredAreaState(performerNum, areaID) == false)
					{
						audioEngine.setPerformerExitedAreaState(performerNum, true, areaID);
						audioEngine.setPerformerEnteredAreaState(performerNum, false, areaID);
						audioEngine.setAreaIDContainingPerformerState(performerNum, areaID, false);
					}
				}
			}
		}
	}

	void MainContentComponent::buttonClicked(Button* button)
	{
		// Simple logic to select either the next or previous stage area in the array when 
		// either the next or previous buttons are pressed.
		if (button == &globalButton[selectPreviousAreaButtonID] && trackingGUI.getCurrentlySelectedArea() > 0)
		{
			trackingGUI.setCurrentlySelectedArea(trackingGUI.getCurrentlySelectedArea() - 1);
		}
		else if (button == &globalButton[selectNextAreaButtonID] && trackingGUI.getCurrentlySelectedArea() < trackingGUI.stageAreas.size() - 1)
		{
			trackingGUI.setCurrentlySelectedArea(trackingGUI.getCurrentlySelectedArea() + 1);
		}
	}

	void MainContentComponent::changeListenerCallback(ChangeBroadcaster* source)
	{
		// If the Colour selector has been changed and a stage area exists
		if (source == &areaColourSelector && trackingGUI.stageAreas.size() > 0)
		{
			// Set the colour of the selected stage area to the current colour of the colour picker.
			trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->setAreaColour(areaColourSelector.getCurrentColour());
		}
	}

	void MainContentComponent::labelTextChanged(Label* labelThatHasChanged) 
	{
		// If the area name label has been changed and a stage area exists
		if (labelThatHasChanged == &areaNameLabel && trackingGUI.stageAreas.size() >= 1)
		{
			// Update and store the stage areas name.
			trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->setAreaName(areaNameLabel.getText());
		}
	}

	
	// (This function is called by the app startup code to create our main component)
	Component* createMainContentComponent() { return new MainContentComponent(); }
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
								             //  audioSettingsWindow("Audio Settings", Colours::black, 4, false)
	{
		
		setSize(1000, 600);
	//	deviceManager.initialise(2, 2, nullptr, true, String(), nullptr);
		setAudioChannels(2, 2);
		kinectSensor.startKinectST();
		startTimer(kinectUpdateTimer, 40);
		startTimer(guiTimer, 30);

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
						
		// Global Parameters Initialisation

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
		audioSettingsWindow.setContentOwned(&audioDeviceSelector, true);

		auto& commandManager = LTLACommandManager::getApplicationCommandManager();
		commandManager.registerAllCommandsForTarget(this);

		addAndMakeVisible(snapshotManager);

		valueTree.addListener(this);

		valueTree.setProperty("StageDrawingState", false, nullptr);

		trackingGUI.setValueTree(valueTree);
		snapshotManager.setValueTree(valueTree);
	}

	MainContentComponent::~MainContentComponent()
	{
		shutdownAudio();
	}

	// MAIN AUDIO FUNCTIONS
	//==============================================================================
	void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		samplerate = sampleRate;
		samplesPerBlock = samplesPerBlockExpected;
		audioEngine.initialiseEngine(samplesPerBlockExpected, sampleRate);
		audioEngine.prepareToPlay(samplesPerBlockExpected, sampleRate);
	}

	void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
	{			
		audioEngine.setDeviceManagerToUse(&deviceManager);
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
			kinectSensor.updateKinectST();
		}
		if (timerID == guiTimer)
		{
			for (int CurrentSkel = 0; CurrentSkel < SKELETON_COUNT; ++CurrentSkel)
			{
				if (kinectSensor.getKinectTrackingState(CurrentSkel) == true)
				{
					trackingGUI.setEllipseCoordinates(kinectSensor.getX(CurrentSkel), kinectSensor.getY(CurrentSkel), CurrentSkel);
				}

				trackingGUI.setKinectTrackingState(CurrentSkel, kinectSensor.getKinectTrackingState(CurrentSkel));
			}

			if (trackingGUI.hasStageAreaChanged() == true)
			{
				trackingGUI.setStageAreaHasChangedState(false);
				areaNameLabel.setText(trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->getAreaName(), dontSendNotification);
				audioEngine.reopenAudioPanel(trackingGUI.getCurrentlySelectedArea());
			}

			if (snapshotManager.hasSnapshotBeenFired() == true)
			{
				areaNameLabel.setText(trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->getAreaName(), dontSendNotification);
				audioEngine.snapshotFired();
				snapshotManager.setHasbeenFiredState(false);
			}

			stereoAudioMeter.setMeterData(0, audioEngine.getMeterData(0));
			stereoAudioMeter.setMeterData(1, audioEngine.getMeterData(1));

			setAudioEngineData();
			repaint();
		}

		if (timerID == calibrationIntervalTimer)
		{
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
		for (int performerNum = 0; performerNum < 2; performerNum++)
		{
			for (int areaID = 0; areaID < trackingGUI.stageAreas.size(); areaID++)
			{
				if (trackingGUI.doesAreaIDContainPerfomer(performerNum, areaID))
				{
					if (audioEngine.getPerformerExitedAreaState(performerNum, areaID) == true)
					{
						audioEngine.setPerformerEnteredAreaState(performerNum, true, areaID);
					}
					else
					{
						audioEngine.setPerformerEnteredAreaState(performerNum, false, areaID);
					}

					if (trackingGUI.doesAreaIDContainPerfomer(performerNum, areaID) == true)
					{
						audioEngine.setAreaIDContainingPerformerState(performerNum, areaID, true);
					}
					else
					{
						audioEngine.setAreaIDContainingPerformerState(performerNum, areaID, false);
					}

					audioEngine.setPerformerExitedAreaState(performerNum, false, areaID);
					audioEngine.setAutoPannerAmount(trackingGUI.getPerformerXPosInsideArea(areaID, performerNum), areaID);
				}
				else
				{
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
		if (button == &globalButton[selectPreviousAreaButtonID] && trackingGUI.getCurrentlySelectedArea() > 0)
		{
			trackingGUI.setCurrentlySelectedArea(trackingGUI.getCurrentlySelectedArea() - 1);
		}
		else if (button == &globalButton[selectNextAreaButtonID] && trackingGUI.getCurrentlySelectedArea() < trackingGUI.stageAreas.size() - 1)
		{
			trackingGUI.setCurrentlySelectedArea(trackingGUI.getCurrentlySelectedArea() + 1);
		}
	}

	void MainContentComponent::sliderValueChanged(Slider* slider)
	{

	}

	void MainContentComponent::changeListenerCallback(ChangeBroadcaster* source)
	{
		if (source == &areaColourSelector && trackingGUI.stageAreas.size() > 0)
		{
			trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->setAreaColour(areaColourSelector.getCurrentColour());
		}
	}

	void MainContentComponent::labelTextChanged(Label* labelThatHasChanged) 
	{
		if (labelThatHasChanged == &areaNameLabel && trackingGUI.stageAreas.size() >= 1)
		{
			trackingGUI.stageAreas[trackingGUI.getCurrentlySelectedArea()]->setAreaName(areaNameLabel.getText());
		}
	}

	
	// (This function is called by the app startup code to create our main component)
	Component* createMainContentComponent() { return new MainContentComponent(); }
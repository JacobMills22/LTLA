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
	MainContentComponent::MainContentComponent()
	{
		setSize(1000, 600);
		setAudioChannels(0, 2);
		KinectSensor.StartKinectST();
		startTimer(KinectUpdateTimer, 40);
		startTimer(GUITimer, 30);

		addAndMakeVisible(GUI);
		addAndMakeVisible(MenuBar);

		addAndMakeVisible(audioEngine);

		addAndMakeVisible(AreaColourSelector);
		AreaColourSelector.addChangeListener(this);
		AreaColourSelector.setVisible(false);

		addAndMakeVisible(CalibrationCountDownLabel);
		Font LabelFont;
		LabelFont.setSizeAndStyle(20, 1, 1, 0.25);
		CalibrationCountDownLabel.setFont(LabelFont);

		addAndMakeVisible(areaNameLabel);
		areaNameLabel.addListener(this);
		areaNameLabel.setEditable(true, true, false);
		areaNameLabel.setText("Enter Area Name", dontSendNotification);
		areaNameLabel.setJustificationType(Justification::centred);

		// Global Parameters Initialisation

		for (int buttonNum = 0; buttonNum < numOfButtons; buttonNum++)
		{
			addAndMakeVisible(GlobalButton[buttonNum]);
			GlobalButton[buttonNum].addListener(this);
		}

		GlobalButton[SelectNextAreaButtonID].setButtonText("Next Area");
		GlobalButton[SelectPreviousAreaButtonID].setButtonText("Previous Area");
				
		auto& commandManager = LTLACommandManager::getApplicationCommandManager();
		commandManager.registerAllCommandsForTarget(this);

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
		bufferToFill.clearActiveBufferRegion();	
		audioEngine.getNextAudioBlock(bufferToFill);
	}

	void MainContentComponent::releaseResources()
	{
		audioEngine.releaseResources();
		stopTimer(KinectUpdateTimer);
		stopTimer(GUITimer);
	}

	// GUI FUNCTION CALLERS
	//==============================================================================
	void MainContentComponent::paint(Graphics& g)
	{
		g.fillAll(Colours::darkgrey.darker());
	}

	void MainContentComponent::resized()
	{
		GUI.resized();
		MenuBar.setBounds(10, 10, 300, 30);


		if (GUI.getAudioPanelState() == false)
			GUI.setBounds(getBounds().reduced(50).getX(), getBounds().reduced(50).getY(), getBounds().reduced(50).getWidth(), getBounds().reduced(50).getHeight());
		else
			GUI.setBounds(getBounds().reduced(50).getX(), getBounds().reduced(50).getY(), getBounds().reduced(50).getWidth(), getHeight() * 0.5);

		CalibrationCountDownLabel.setBounds(getBounds().getWidth() - 150, getBounds().getHeight() - 50, 150, 50);
		AreaColourSelector.setBounds(GUI.getX() + GUI.getWidth() * 0.9, GUI.getHeight() + 55, GUI.getWidth() * 0.1, (getHeight() - GUI.getHeight()) * 0.3);

		audioEngine.setBounds(GUI.getX(), GUI.getHeight() + 50, GUI.getWidth() - AreaColourSelector.getWidth(), getHeight() - GUI.getHeight());

		float MaxGuiHeight = getBounds().reduced(50).getHeight();
		int GlobalPanelHeight = getHeight() - ((getHeight() - MaxGuiHeight) * 0.4);

		GlobalButton[SelectPreviousAreaButtonID].setBounds(GUI.getX(), GlobalPanelHeight, getWidth() * 0.1, 25);
		areaNameLabel.setBounds(GlobalButton[SelectPreviousAreaButtonID].getRight() + (getWidth() * 0.01), GlobalPanelHeight, getWidth() * 0.1, 25);
		GlobalButton[SelectNextAreaButtonID].setBounds(areaNameLabel.getRight() + (getWidth() * 0.01), GlobalPanelHeight, getWidth() * 0.1, 25);

	}

	void MainContentComponent::timerCallback(int timerID)
	{
		if (timerID == KinectUpdateTimer)
		{
			KinectSensor.UpdateKinectST();
		}
		if (timerID == GUITimer)
		{
			for (int CurrentSkel = 0; CurrentSkel < SKELETON_COUNT; ++CurrentSkel)
			{
				if (KinectSensor.GetKinectTrackingState(CurrentSkel) == true)
				{
					GUI.SetEllipseCoordinates(KinectSensor.GetX(CurrentSkel), KinectSensor.GetY(CurrentSkel), CurrentSkel);
				}

				GUI.SetKinectTrackingState(CurrentSkel, KinectSensor.GetKinectTrackingState(CurrentSkel));
			}

			if (GUI.hasStageAreaChanged() == true)
			{
				GUI.setStageAreaHasChangedState(false);
				areaNameLabel.setText(GUI.StageAreas[GUI.GetCurrentlySelectedArea()]->getAreaName(), dontSendNotification);
				audioEngine.reopenAudioPanel(GUI.GetCurrentlySelectedArea());
			}

			for (int performerNum = 0; performerNum < 2; performerNum++)
			{
				if (GUI.doesAnyAreaContainPerfomer(performerNum) == true)
				{
					if (oldAreaIDContainingPerfromer[performerNum] != GUI.getAreaIDContainingPerformer(performerNum))
					{
						audioEngine.setPerformerExitedAreaState(performerNum, true);
						audioEngine.setPerformerEnteredAreaState(performerNum, false);
					}

					if (audioEngine.getPerformerExitedAreaState(performerNum) == true)
					{
						audioEngine.setPerformerEnteredAreaState(performerNum, true);
					}
					else
					{
						audioEngine.setPerformerEnteredAreaState(performerNum, false);
					}

					audioEngine.setAreaIDWhichContainsPerformer(performerNum, GUI.getAreaIDContainingPerformer(performerNum));
					oldAreaIDContainingPerfromer[performerNum] = GUI.getAreaIDContainingPerformer(performerNum);
					audioEngine.setPerformerExitedAreaState(performerNum, false);

				}
				else
				{
					if (audioEngine.getPerformerEnteredAreaState(performerNum) == false)
					{
						audioEngine.setPerformerExitedAreaState(performerNum, true);
						audioEngine.setPerformerEnteredAreaState(performerNum, false);
					}
				}
			}

			repaint();
		}
		if (timerID == CalibrationIntervalTimer)
		{
			switch (GUI.StageCalibrationCounter)
			{
			case 0: GUI.SetStageCoordinates(GUI.FrontLeft, KinectSensor.GetX(0), KinectSensor.GetY(0)); 
					CalibrationCountDownLabel.setText("Calibrating Front Right", dontSendNotification);
					break;
			case 1: GUI.SetStageCoordinates(GUI.FrontRight, KinectSensor.GetX(0), KinectSensor.GetY(0));
					CalibrationCountDownLabel.setText("Calibrating Back Right", dontSendNotification);
					break;
			case 2: GUI.SetStageCoordinates(GUI.BackRight, KinectSensor.GetX(0), KinectSensor.GetY(0));
					CalibrationCountDownLabel.setText("Calibrating Back Left", dontSendNotification);
					break;
			case 3:	GUI.SetStageCoordinates(GUI.BackLeft, KinectSensor.GetX(0), KinectSensor.GetY(0));
					stopTimer(CalibrationIntervalTimer);
					CalibrationCountDownLabel.setText("", dontSendNotification);
					break;
			}
			GUI.StageCalibrationCounter++;
		}
	}

	void MainContentComponent::buttonClicked(Button* button)
	{
		if (button == &GlobalButton[SelectPreviousAreaButtonID] && GUI.GetCurrentlySelectedArea() > 0)
		{
			GUI.SetCurrentlySelectedArea(GUI.GetCurrentlySelectedArea() - 1);
		}
		else if (button == &GlobalButton[SelectNextAreaButtonID] && GUI.GetCurrentlySelectedArea() < GUI.StageAreas.size() - 1)
		{
			GUI.SetCurrentlySelectedArea(GUI.GetCurrentlySelectedArea() + 1);
		}
	}

	void MainContentComponent::sliderValueChanged(Slider* slider)
	{

	}

	void MainContentComponent::changeListenerCallback(ChangeBroadcaster* source)
	{
		if (source == &AreaColourSelector && GUI.StageAreas.size() > 0)
		{
			GUI.StageAreas[GUI.GetCurrentlySelectedArea()]->SetAreaColour(AreaColourSelector.getCurrentColour());
		}
	}

	void MainContentComponent::labelTextChanged(Label* labelThatHasChanged) 
	{
		if (labelThatHasChanged == &areaNameLabel && GUI.StageAreas.size() >= 1)
		{
			GUI.StageAreas[GUI.GetCurrentlySelectedArea()]->setAreaName(areaNameLabel.getText());
		}
	}

	
	// (This function is called by the app startup code to create our main component)
	Component* createMainContentComponent() { return new MainContentComponent(); }
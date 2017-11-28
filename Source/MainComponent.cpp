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

		addAndMakeVisible(AreaColourSelector);
		AreaColourSelector.addChangeListener(this);
		AreaColourSelector.setVisible(false);

		addAndMakeVisible(CalibrationCountDownLabel);
		Font LabelFont;
		LabelFont.setSizeAndStyle(20, 1, 1, 0.25);
		CalibrationCountDownLabel.setFont(LabelFont);
		
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
	}

	void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
	{
		bufferToFill.clearActiveBufferRegion();
	}

	void MainContentComponent::releaseResources()
	{
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
		GUI.setBounds(getBounds().reduced(50).getX(), getBounds().reduced(50).getY(), getBounds().reduced(50).getWidth(), getBounds().reduced(80).getHeight());
		CalibrationCountDownLabel.setBounds(getBounds().getWidth() - 150, getBounds().getHeight() - 50, 150, 50);
		AreaColourSelector.setBounds(GUI.getX(), GUI.getHeight() + 55, 150, getHeight() - GUI.getHeight() - 60);

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
	}

	void MainContentComponent::changeListenerCallback(ChangeBroadcaster* source)
	{
		if (source == &AreaColourSelector && GUI.StageAreas.size() > 0)
		{
			GUI.StageAreas[GUI.GetCurrentlySelectedArea()]->SetAreaColour(AreaColourSelector.getCurrentColour());
		}
	}
	
	// (This function is called by the app startup code to create our main component)
	Component* createMainContentComponent() { return new MainContentComponent(); }
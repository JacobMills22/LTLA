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

		addAndMakeVisible(stageAreaAudioPanel[0]);

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
		samplerate = sampleRate;
		samplesPerBlock = samplesPerBlockExpected;

		mixerAudioSource.addInputSource(stageAreaAudioPanel.getLast(), true);
		mixerAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	}

	void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
	{
		bufferToFill.clearActiveBufferRegion();	
		mixerAudioSource.getNextAudioBlock(bufferToFill);
	}

	void MainContentComponent::releaseResources()
	{
		mixerAudioSource.releaseResources();
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

		for (int stageAreaAudioID = 0; stageAreaAudioID < stageAreaAudioPanel.size(); stageAreaAudioID++)
		{
			stageAreaAudioPanel[stageAreaAudioID]->setBounds(GUI.getX(), GUI.getHeight() + 50, GUI.getWidth() - AreaColourSelector.getWidth(), getHeight() - GUI.getHeight());
		}
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
				reopenAudioPanel();
			
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

	void MainContentComponent::reopenAudioPanel()
	{
		GUI.setStageAreaHasChangedState(false);

		for (int stageAreaAudioID = 0; stageAreaAudioID < stageAreaAudioPanel.size(); stageAreaAudioID++)
		{
			stageAreaAudioPanel[stageAreaAudioID]->setVisible(false);
		}
		stageAreaAudioPanel[GUI.GetCurrentlySelectedArea()]->setVisible(true);
		stageAreaAudioPanel[GUI.GetCurrentlySelectedArea()]->toFront(false);
	}


	
	// (This function is called by the app startup code to create our main component)
	Component* createMainContentComponent() { return new MainContentComponent(); }
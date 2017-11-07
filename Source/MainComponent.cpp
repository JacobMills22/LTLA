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
		setSize(800, 600);
		setAudioChannels(0, 2);
		KinectSensor.StartKinectST();
		startTimer(KinectUpdateTimer, 40);
		startTimer(GUITimer, 30);

		addAndMakeVisible(MenuBar);
		
		auto& commandManager = LTLAApplication::MainWindow::getApplicationCommandManager();
		commandManager.registerAllCommandsForTarget(this);
		
	}

	MainContentComponent::~MainContentComponent()
	{
		shutdownAudio();
	}

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

	//==============================================================================
	void MainContentComponent::paint(Graphics& g)
	{
		GUI.paint(g);
	}

	void MainContentComponent::resized()
	{
		GUI.resized();
		MenuBar.setBounds(10, 10, 140, 40);
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
			case 0: GUI.SetStageCoordinates(GUI.FrontLeft, KinectSensor.GetX(0), KinectSensor.GetY(0)); DBG("!!!!FRONT LEFT SET!!!!");
				break;
			case 1: GUI.SetStageCoordinates(GUI.FrontRight, KinectSensor.GetX(0), KinectSensor.GetY(0)); DBG("!!!!FRONT RIGHT SET!!!!");
				break;
			case 2: GUI.SetStageCoordinates(GUI.BackRight, KinectSensor.GetX(0), KinectSensor.GetY(0)); DBG("!!!!BACK LEFT SET!!!!");
				break;
			case 3:
			{
				GUI.SetStageCoordinates(GUI.BackLeft, KinectSensor.GetX(0), KinectSensor.GetY(0));
				stopTimer(CalibrationIntervalTimer);
				DBG("!!!!BACK RIGHT SET!!!!");
			}
			break;
			}

			GUI.StageCalibrationCounter++;
		}
	}

	void MainContentComponent::buttonClicked(Button* button)
	{
	}
	
	//==============================================================================
	//============================ COMMAND MANAGER =================================
	//==============================================================================

	ApplicationCommandTarget* MainContentComponent::getNextCommandTarget()
	{
		return findFirstTargetParentComponent();
	}

	void MainContentComponent::getAllCommands(Array<CommandID>& commands)
	{
		const CommandID ids[] = { MenuBar.FrontLeftID, MenuBar.FrontRightID, MenuBar.BackRightID, MenuBar.BackLeftID, 
								  MenuBar.Interval5Seconds, MenuBar.Interval10Seconds, MenuBar.Interval20Seconds, MenuBar.CalibrationStartID, MenuBar.DrawStageID };
		commands.addArray(ids, numElementsInArray(ids));
	}

	void MainContentComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
	{
		switch (commandID)
		{
		case LTLAMenuBar::FrontLeftID:
			result.setInfo("Calibrate Front Left", "Instantly Calibrate the Front Left corner of the Stage", "Calibration", 0);
			break;
		case LTLAMenuBar::FrontRightID:
			result.setInfo("Calibrate Front Right", "Instantly Calibrate the Front Right corner of the Stage", "Calibration", 0);
			break;
		case LTLAMenuBar::BackRightID:
			result.setInfo("Calibrate Back Right", "Instantly Calibrate the Back Right corner of the Stage", "Calibration", 0);
			break;
		case LTLAMenuBar::BackLeftID:
			result.setInfo("Calibrate Back Left", "Instantly Calibrate the Back Left corner of the Stage", "Calibration", 0);
			break;
		case LTLAMenuBar::Interval5Seconds:
			result.setInfo("5 Seconds", "Set the Interval to 5 seconds", "Calibration", 0);
			break;
		case LTLAMenuBar::Interval10Seconds:
			result.setInfo("10 Seconds", "Set the Interval to 10 seconds", "Calibration", 0);
			break;
		case LTLAMenuBar::Interval20Seconds:
			result.setInfo("20 Seconds", "Set the Interval to 20 seconds", "Calibration", 0);
			break;
		case LTLAMenuBar::CalibrationStartID:
			result.setInfo("Start Calibration", "Starts the calibration sequence.", "Calibration", 0);
			break;
		case LTLAMenuBar::DrawStageID: 
			result.setInfo("Draw Stage", "Draws the stage once calibrated.", "Calibration", 0);
			break;
		}
	}

	bool MainContentComponent::perform(const InvocationInfo& info)
	{
		switch (info.commandID)
		{
		case LTLAMenuBar::FrontLeftID: GUI.SetStageCoordinates(GUI.FrontLeft, KinectSensor.GetX(0), KinectSensor.GetY(0)); DBG("FL SET");
			break;
		case LTLAMenuBar::FrontRightID: GUI.SetStageCoordinates(GUI.FrontRight, KinectSensor.GetX(0), KinectSensor.GetY(0)); DBG("FR SET");
			break;
		case LTLAMenuBar::BackRightID: GUI.SetStageCoordinates(GUI.BackRight, KinectSensor.GetX(0), KinectSensor.GetY(0)); DBG("BL SET");
			break;
		case LTLAMenuBar::BackLeftID: GUI.SetStageCoordinates(GUI.BackLeft , KinectSensor.GetX(0), KinectSensor.GetY(0)); DBG("BR SET");
			break;
		case LTLAMenuBar::Interval5Seconds: GUI.StageCalibrationInterval = 5;
			break;
		case LTLAMenuBar::Interval10Seconds: GUI.StageCalibrationInterval = 10;
			break;
		case LTLAMenuBar::Interval20Seconds: GUI.StageCalibrationInterval = 20;
			break;
		case LTLAMenuBar::CalibrationStartID:
		{
			GUI.StageCalibrationCounter = 0; 
			startTimer(CalibrationIntervalTimer, GUI.StageCalibrationInterval * 1000);
		}
			break;
		case LTLAMenuBar::DrawStageID: GUI.GetStageCalibrationState() == true ? GUI.SetStageCalibrationState(false) : GUI.SetStageCalibrationState(true); DBG("Draw stage is " + (String)GUI.GetStageCalibrationState());
			break;
		default: return false;
		}
		return true;

	}
	
	//==============================================================================
	
	// (This function is called by the app startup code to create our main component)
	Component* createMainContentComponent() { return new MainContentComponent(); }
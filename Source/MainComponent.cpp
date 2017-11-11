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

		addAndMakeVisible(GUI);
		addAndMakeVisible(MenuBar);
		
		auto& commandManager = LTLAApplication::MainWindow::getApplicationCommandManager();
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
	}

	void MainContentComponent::resized()
	{
		GUI.resized();
		MenuBar.setBounds(10, 10, 200, 40);
		GUI.setBounds(getBounds());
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
					GUI.SetStageCalCountdownText("Calibrating Front Right");
					break;
			case 1: GUI.SetStageCoordinates(GUI.FrontRight, KinectSensor.GetX(0), KinectSensor.GetY(0));
					GUI.SetStageCalCountdownText("Calibrating Back Right");
					break;
			case 2: GUI.SetStageCoordinates(GUI.BackRight, KinectSensor.GetX(0), KinectSensor.GetY(0));
					GUI.SetStageCalCountdownText("Calibrating Back Left");
					break;
			case 3:	GUI.SetStageCoordinates(GUI.BackLeft, KinectSensor.GetX(0), KinectSensor.GetY(0));
					stopTimer(CalibrationIntervalTimer);
					GUI.SetStageCalCountdownText("");
					break;
			}
			GUI.StageCalibrationCounter++;
		}
	}

	void MainContentComponent::buttonClicked(Button* button)
	{
	}

	// COMMAND MANAGER FUNCTIONS
	//==============================================================================

	ApplicationCommandTarget* MainContentComponent::getNextCommandTarget()
	{
		return findFirstTargetParentComponent();
	}

	void MainContentComponent::getAllCommands(Array<CommandID>& commands)
	{
		const CommandID ids[] = { MenuBar.FrontLeftID, MenuBar.FrontRightID, MenuBar.BackRightID, MenuBar.BackLeftID, 
								  MenuBar.Interval5SecondsID, MenuBar.Interval10SecondsID, MenuBar.Interval20SecondsID, 
								  MenuBar.CalibrationStartID, MenuBar.DrawStageID, MenuBar.DrawGridID, MenuBar.SnaptoGridID, MenuBar.GridSize10ID, 
								  MenuBar.GridSize15ID, MenuBar.GridSize20ID };
		commands.addArray(ids, numElementsInArray(ids));
	}

	void MainContentComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
	{
		switch (commandID)
		{
		case LTLAMenuBar::FrontLeftID:
			result.setInfo("Draw Front Left", "Instantly Calibrate the Front Left corner of the Stage", "Calibration", 0);
			break;
		case LTLAMenuBar::FrontRightID:
			result.setInfo("Draw Front Right", "Instantly Calibrate the Front Right corner of the Stage", "Calibration", 0);
			break;
		case LTLAMenuBar::BackRightID:
			result.setInfo("Draw Back Right", "Instantly Calibrate the Back Right corner of the Stage", "Calibration", 0);
			break;
		case LTLAMenuBar::BackLeftID:
			result.setInfo("Draw Back Left", "Instantly Calibrate the Back Left corner of the Stage", "Calibration", 0);
			break;
		case LTLAMenuBar::Interval5SecondsID:
			result.setInfo("5 Seconds", "Set the Interval to 5 seconds", "Calibration", 0);
			if (GUI.StageCalibrationInterval == 5 ) result.setTicked(true);
			break;
		case LTLAMenuBar::Interval10SecondsID:
			result.setInfo("10 Seconds", "Set the Interval to 10 seconds", "Calibration", 0);
			if (GUI.StageCalibrationInterval == 10) result.setTicked(true);
			break;
		case LTLAMenuBar::Interval20SecondsID:
			result.setInfo("20 Seconds", "Set the Interval to 20 seconds", "Calibration", 0);
			if (GUI.StageCalibrationInterval == 20) result.setTicked(true);
			break;
		case LTLAMenuBar::CalibrationStartID:
			result.setInfo("Start Calibration", "Starts the calibration sequence.", "Calibration", 0);
			break;
		case LTLAMenuBar::DrawStageID: 
			result.setInfo("Draw Stage", "Draws the stage once calibrated.", "Calibration", 0);
			result.setTicked(GUI.GetStageDrawingState());
			break;
		case LTLAMenuBar::DrawGridID:
			result.setInfo("Draw Grid", "Draws a Grid.", "Grid", 0);
			result.setTicked(GUI.GetGridDrawingState());
			break;
		case LTLAMenuBar::SnaptoGridID:
			result.setInfo("Enable Grid Snapping", "", "Grid", 0);
			result.setTicked(GUI.GetGridSnappingState());
			break;
		case LTLAMenuBar::GridSize10ID:
			result.setInfo("Grid Size 10", "", "Grid", 0);
			break;
		case LTLAMenuBar::GridSize15ID:
			result.setInfo("Grid Size 15", "", "Grid", 0);
			break;
		case LTLAMenuBar::GridSize20ID:
			result.setInfo("Grid Size 20", "", "Grid", 0);
			break;
		}
	}

	bool MainContentComponent::perform(const InvocationInfo& info)
	{
		switch (info.commandID)
		{
		case LTLAMenuBar::FrontLeftID: //GUI.SetStageCoordinates(GUI.FrontLeft, KinectSensor.GetX(0), KinectSensor.GetY(0));
			GUI.SetMouseDrawingStageState(GUI.FrontLeft, true);
			break;
		case LTLAMenuBar::FrontRightID: //GUI.SetStageCoordinates(GUI.FrontRight, KinectSensor.GetX(0), KinectSensor.GetY(0));
			GUI.SetMouseDrawingStageState(GUI.FrontRight, true);
			break;
		case LTLAMenuBar::BackRightID:// GUI.SetStageCoordinates(GUI.BackRight, KinectSensor.GetX(0), KinectSensor.GetY(0));
			GUI.SetMouseDrawingStageState(GUI.BackRight, true);
			break;
		case LTLAMenuBar::BackLeftID: //GUI.SetStageCoordinates(GUI.BackLeft , KinectSensor.GetX(0), KinectSensor.GetY(0));
			GUI.SetMouseDrawingStageState(GUI.BackLeft, true);
			break;
		case LTLAMenuBar::Interval5SecondsID: GUI.StageCalibrationInterval = 5;
			break;
		case LTLAMenuBar::Interval10SecondsID: GUI.StageCalibrationInterval = 10;
			break;
		case LTLAMenuBar::Interval20SecondsID: GUI.StageCalibrationInterval = 20;
			break;
		case LTLAMenuBar::CalibrationStartID:
		{
			GUI.StageCalibrationCounter = 0; 
			startTimer(CalibrationIntervalTimer, GUI.StageCalibrationInterval * 1000);
			GUI.SetStageCalCountdownText("Calibrating Front Left");
		}
			break;
		case LTLAMenuBar::DrawStageID: GUI.GetStageDrawingState() == true ? GUI.SetStageDrawingState(false) : GUI.SetStageDrawingState(true);
			break;
		case LTLAMenuBar::DrawGridID: GUI.GetGridDrawingState() == true ? GUI.SetGridDrawingState(false) : GUI.SetGridDrawingState(true);
			break;
		case LTLAMenuBar::SnaptoGridID: GUI.GetGridSnappingState() == true ? GUI.SetGridSnappingState(false) : GUI.SetGridSnappingState(true);
			break;
		case LTLAMenuBar::GridSize10ID: GUI.SetGridIncrement(10);
			break;
		case LTLAMenuBar::GridSize15ID: GUI.SetGridIncrement(15);
			break;
		case LTLAMenuBar::GridSize20ID: GUI.SetGridIncrement(20);
			break;
		default: return false;
		}
		return true;

	}
	
	//==============================================================================
	
	// (This function is called by the app startup code to create our main component)
	Component* createMainContentComponent() { return new MainContentComponent(); }
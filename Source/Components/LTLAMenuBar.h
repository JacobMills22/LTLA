#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/LTLA_GUI.h"
#include "../Source/Main.h"

class LTLAMenuBar : public Component,
					public MenuBarModel//,
					//public ApplicationCommandTarget
{
public:

	LTLAMenuBar()
	{
		addAndMakeVisible(menuBar = new MenuBarComponent(this));

	//	auto& commandManager = LTLAApplication::MainWindow::getApplicationCommandManager();
	//	commandManager.registerAllCommandsForTarget(this);
	//	commandManager.registerAllCommandsForTarget(JUCEApplication::getInstance());
	}

	~LTLAMenuBar()
	{
		PopupMenu::dismissAllActiveMenus();
		delete menuBar;
	}

	void resized() override
	{
		menuBar->setBounds(getLocalBounds().removeFromTop(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));
	}

	StringArray getMenuBarNames() override
	{
		const char* const names[] = { "Stage Calibration", nullptr };
		return StringArray(names);
	}

	PopupMenu getMenuForIndex(int menuIndex, const String& menuBar) override
	{
		ApplicationCommandManager* commandManager = &LTLAApplication::MainWindow::getApplicationCommandManager();
		PopupMenu menu;
		
		if (menuIndex == CalibrationIndexID)
		{
			PopupMenu CalibrationIntervalSubMenu;
					
				menu.addCommandItem(commandManager, 1);
				menu.addCommandItem(commandManager, 2);
			


			/*
			menu.addItem(CalibrateFrontLeftID, "Calibrate Front Left", true, false);
			menu.addItem(CalibrateFrontRightID, "Calibrate Front Right", true, false);
			menu.addItem(CalibrateBackLeftID, "Calibrate Back Left", true, false);
			menu.addItem(CalibrateBackRightID, "Calibrate Back Right", true, false);
			menu.addSeparator();

			CalibrationIntervalSubMenu.addItem(IntervalID_5seconds,   "5 Seconds", true, false);
			CalibrationIntervalSubMenu.addItem(IntervalID_10seconds, "10 Seconds", true, false);
			CalibrationIntervalSubMenu.addItem(IntervalID_20seconds, "20 Seconds", true, false);
			menu.addSubMenu("Calibration Interval", CalibrationIntervalSubMenu, true);

			menu.addItem(StartCalibrationID, "Start Calibration (" + (String)CalibrationInterval + " Seconds)", true, false);
			*/
			
		}
		return menu;
	}
	
	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override
	{
		
	}

	/*
	ApplicationCommandTarget* getNextCommandTarget()
	{
		return findFirstTargetParentComponent();
	}

	void getAllCommands(Array<CommandID>& commands)
	{
		const CommandID ids[] = { 1, 2 };
		commands.addArray(ids, numElementsInArray(ids));
	}

	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
	{
		switch (commandID)
		{
		case 1:
			result.setInfo("Test1ShortName", "TestDesvription", "Test1 Category", 0);
			break;
		case 2:
			result.setInfo("Test2ShortName", "Test2Desvription", "Test2 Category", 0);
			break;
		}
	}

	bool perform(const InvocationInfo& info)
	{
		switch (info.commandID)
		{
		case 1: DBG("Test1 Prssed");
			break;
		case 2: DBG("Test2 Prssed");
			break;
		default: return false;
		}

		return true;

	}
	/*
	/** Getters and Setters */

	int GetStageCalibrationInterval()
	{
		return CalibrationInterval;
	}

private:

	ScopedPointer<MenuBarComponent> menuBar;
	enum { CalibrationIndexID };
	enum { CalibrateFrontLeftID = 1, CalibrateFrontRightID, CalibrateBackLeftID, CalibrateBackRightID,
		   CalibrationIntervalID, StartCalibrationID, IntervalID_5seconds, 
		   IntervalID_10seconds, IntervalID_20seconds};

	int CalibrationInterval = 5;

	enum { FrontLeft, FrontRight, BackRight, BackLeft, NumOfStagePositions };
	struct LTLACoordinates
	{
		float x = 0.00;
		float y = 0.00;
	};
		LTLACoordinates StageCoordinates[NumOfStagePositions];



};
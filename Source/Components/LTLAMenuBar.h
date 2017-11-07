#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/LTLA_GUI.h"

class LTLAMenuBar : public Component,
	public MenuBarModel
{
public:

	LTLAMenuBar()
	{
		addAndMakeVisible(menuBar = new MenuBarComponent(this));
	}

	~LTLAMenuBar()
	{
		PopupMenu::dismissAllActiveMenus();
		delete menuBar;	// Is this needed?
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
		PopupMenu menu;



		if (menuIndex == CalibrationIndexID)
		{
			PopupMenu CalibrationIntervalSubMenu;

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
			
		}
		return menu;
	}
	
	virtual void menuItemSelected(int menuItemID, int topLevelMenuIndex) override
	{

		switch (menuItemID)
		{
			case CalibrateFrontLeftID:
			{
				
			}
			break;
			case CalibrateFrontRightID:
			{

			}
			break;
			case CalibrateBackLeftID:
			{

			}
			break;
			case CalibrateBackRightID:
			{

			}
			break;
			case CalibrationIntervalID:
			{

			}
			break;
			case StartCalibrationID:
			{

			}
			break;
			case IntervalID_5seconds:
			{
				CalibrationInterval = 5;
			}
			break;
			case IntervalID_10seconds:
			{
				CalibrationInterval = 10;
			}
			break;
			case IntervalID_20seconds:
			{
				CalibrationInterval = 20;

			}
			break;
			default: break;

		}
		
	}
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
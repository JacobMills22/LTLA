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
				menu.addCommandItem(commandManager, FrontLeftID);
				menu.addCommandItem(commandManager, FrontRightID);
				menu.addCommandItem(commandManager, BackRightID);
				menu.addCommandItem(commandManager, BackLeftID);
				menu.addSeparator();
				PopupMenu CalibrationIntervalSubMenu;
				CalibrationIntervalSubMenu.addCommandItem(commandManager, Interval5Seconds);
				CalibrationIntervalSubMenu.addCommandItem(commandManager, Interval10Seconds);
				CalibrationIntervalSubMenu.addCommandItem(commandManager, Interval20Seconds);
				menu.addSubMenu("Calibration Interval", CalibrationIntervalSubMenu, true);
				menu.addCommandItem(commandManager, CalibrationStartID);
				menu.addSeparator();
				menu.addCommandItem(commandManager, DrawStageID);
		}
		return menu;
	}
	
	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override
	{
	}

	/** Getters and Setters */


	enum MainCommandManagerIDs {
		FrontLeftID = 1, FrontRightID, BackRightID, BackLeftID, CalibrationStartID, 
		Interval5Seconds, Interval10Seconds, Interval20Seconds, DrawStageID, NumOfCommandIDs
	};

private:

	ScopedPointer<MenuBarComponent> menuBar;
	enum { CalibrationIndexID };

};
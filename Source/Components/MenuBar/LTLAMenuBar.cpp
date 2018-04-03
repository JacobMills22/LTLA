
#include "LTLAMenuBar.h"

	LTLAMenuBar::LTLAMenuBar()
	{
		addAndMakeVisible(menuBar = new MenuBarComponent(this));
	}

	LTLAMenuBar::~LTLAMenuBar()
	{
		PopupMenu::dismissAllActiveMenus();
	}

	void LTLAMenuBar::resized() 
	{
		menuBar->setBounds(getLocalBounds().removeFromTop(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));
	}

	StringArray LTLAMenuBar::getMenuBarNames() 
	{
		const char* const names[] = { "File", "Stage Calibration", "Grid", "Stage Areas", "Preferences", nullptr };
		return StringArray(names);
	}

	PopupMenu LTLAMenuBar::getMenuForIndex(int menuIndex, const String& menuBar) 
	{
		auto& commandManager = LTLAcmd.getApplicationCommandManager();

		PopupMenu menu;

		if (menuIndex == fileIndexID)
		{
			menu.addCommandItem(&commandManager, saveID); 
			menu.addCommandItem(&commandManager, saveAsID);
			menu.addSeparator();
			menu.addCommandItem(&commandManager, loadID);
		} 
		else if (menuIndex == calibrationIndexID)
		{
			menu.addSeparator();
			PopupMenu CalibrationIntervalSubMenu;
			CalibrationIntervalSubMenu.addCommandItem(&commandManager, interval5SecondsID);
			CalibrationIntervalSubMenu.addCommandItem(&commandManager, interval10SecondsID);
			CalibrationIntervalSubMenu.addCommandItem(&commandManager, interval20SecondsID);
			menu.addSubMenu("Calibration Interval", CalibrationIntervalSubMenu, true);
			menu.addCommandItem(&commandManager, calibrationStartID);
			menu.addSeparator();
			menu.addCommandItem(&commandManager, drawStageID);
			menu.addCommandItem(&commandManager, editStageID);
		}
		else if (menuIndex == gridIndexID)
		{
			menu.addCommandItem(&commandManager, drawGridID);
			menu.addCommandItem(&commandManager, snaptoGridID);
			PopupMenu GridSizeSubMenu;
			GridSizeSubMenu.addCommandItem(&commandManager, gridSize10ID);
			GridSizeSubMenu.addCommandItem(&commandManager, gridSize15ID);
			GridSizeSubMenu.addCommandItem(&commandManager, gridSize20ID);
			menu.addSubMenu("Grid Size", GridSizeSubMenu, true);
		}
		else if (menuIndex == stageAreasID)
		{
			menu.addCommandItem(&commandManager, addStageAreaID);
			menu.addCommandItem(&commandManager, editStageAreasID);
			menu.addCommandItem(&commandManager, removeStageAreaID);
			menu.addCommandItem(&commandManager, audioParametersID);
		}
		else if (menuIndex == preferencesID)
		{
			menu.addCommandItem(&commandManager, audioDeviceSettingsID);
			menu.addCommandItem(&commandManager, performerSimulation);
		}
		return menu;
	}

	void LTLAMenuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex)
	{
	}



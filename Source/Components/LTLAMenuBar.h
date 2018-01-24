#pragma once

#include "../Source/Components/TrackingGUI/TrackingGUI.h"
#include "../Source/Components/CommandManager.h"

class LTLAMenuBar : public Component,
					public MenuBarModel
{
public:
/** Creates new MenuBar Component.*/
	LTLAMenuBar()
	{
		addAndMakeVisible(menuBar = new MenuBarComponent(this));
	}
	
/** Destroys MenuBar Component.*/
	~LTLAMenuBar()
	{
		PopupMenu::dismissAllActiveMenus();	
		menuBar.release();
	}

/** Called when component is resized, used to set bounds of component.*/
	void resized() override
	{
		menuBar->setBounds(getLocalBounds().removeFromTop(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));
	}

/** Gets the main menu bar heading names. */
	StringArray getMenuBarNames() override
	{
		const char* const names[] = { "Stage Calibration", "Grid", "Stage Areas", "Preferences", nullptr };
		return StringArray(names);
	}

/** Called when a heading is opened.
	Gets the Sub headings and adds them to the list.
	Adds items to the command manager ready to be called.*/
	PopupMenu getMenuForIndex(int menuIndex, const String& menuBar) override
	{
		ApplicationCommandManager* commandManager = &LTLACommandManager::getApplicationCommandManager();
		PopupMenu menu;

		if (menuIndex == CalibrationIndexID)
		{
			menu.addSeparator();
			PopupMenu CalibrationIntervalSubMenu;
			CalibrationIntervalSubMenu.addCommandItem(commandManager, Interval5SecondsID);
			CalibrationIntervalSubMenu.addCommandItem(commandManager, Interval10SecondsID);
			CalibrationIntervalSubMenu.addCommandItem(commandManager, Interval20SecondsID);
			menu.addSubMenu("Calibration Interval", CalibrationIntervalSubMenu, true);
			menu.addCommandItem(commandManager, CalibrationStartID);
			menu.addSeparator();
			menu.addCommandItem(commandManager, DrawStageID);
			menu.addCommandItem(commandManager, EditStageID);
		}
		else if (menuIndex == GridIndexID)
		{
			menu.addCommandItem(commandManager, DrawGridID);
			menu.addCommandItem(commandManager, SnaptoGridID);
			PopupMenu GridSizeSubMenu;
			GridSizeSubMenu.addCommandItem(commandManager, GridSize10ID);
			GridSizeSubMenu.addCommandItem(commandManager, GridSize15ID);
			GridSizeSubMenu.addCommandItem(commandManager, GridSize20ID);
			menu.addSubMenu("Grid Size", GridSizeSubMenu, true);
		}
		else if (menuIndex == StageAreasID)
		{
			menu.addCommandItem(commandManager, AddStageAreaID);
			menu.addCommandItem(commandManager, EditStageAreasID);
			menu.addCommandItem(commandManager, RemoveStageAreaID);
			menu.addCommandItem(commandManager, AudioParametersID);
		}
		else if (menuIndex == PreferencesID)
		{
			menu.addCommandItem(commandManager, AudioDeviceSettingsID);
		}
		return menu;
	}
	
/**  Called when a Sub Heading is selected.
	Used to call any special cases that cannot be provided through the
	command manager. */
	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override
	{

	}
	
	enum MainCommandManagerIDs {CalibrationStartID = 1, Interval5SecondsID, Interval10SecondsID, 
		Interval20SecondsID, DrawStageID, EditStageID, DrawGridID, SnaptoGridID, GridSize10ID, 
		GridSize15ID, GridSize20ID, AddStageAreaID, EditStageAreasID, RemoveStageAreaID, 
		AudioParametersID, AudioDeviceSettingsID, NumOfCommandIDs};

private:

	ScopedPointer<MenuBarComponent> menuBar;
	enum { CalibrationIndexID, GridIndexID, StageAreasID, PreferencesID };

};



#include "LTLAMenuBar.h"

LTLAMenuBar::LTLAMenuBar()
{
	addAndMakeVisible(menuBar = new MenuBarComponent(this));
}

LTLAMenuBar::~LTLAMenuBar()
{
	PopupMenu::dismissAllActiveMenus();
	delete menuBar;
}

void LTLAMenuBar::resized() 
{
	menuBar->setBounds(getLocalBounds().removeFromTop(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));
}

StringArray LTLAMenuBar::getMenuBarNames() 
{
	const char* const names[] = { "Stage Calibration", nullptr };
	return StringArray(names);
}

PopupMenu LTLAMenuBar::getMenuForIndex(int menuIndex, const String& menuBar) 
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

void LTLAMenuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex) 
{
}
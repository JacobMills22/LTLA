#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/LTLA_GUI.h"
#include "../Source/Main.h"

class LTLAMenuBar : public Component,
					public MenuBarModel
{
public:
/** Creates new MenuBar Component.*/
	LTLAMenuBar();
	
/** Destroys MenuBar Component.*/
	~LTLAMenuBar();

/** Called when component is resized, used to set bounds of component.*/
	void resized() override;

/** Gets the main menu bar heading names. */
	StringArray getMenuBarNames() override;

/** Called when a heading is opened.
	Gets the Sub headings and adds them to the list.
	Adds items to the command manager ready to be called.*/
	PopupMenu getMenuForIndex(int menuIndex, const String& menuBar) override;
	
/**  Called when a Sub Heading is selected.
	Used to call any special cases that cannot be provided through the
	command manager. */
	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
	
	enum MainCommandManagerIDs {FrontLeftID = 1, FrontRightID, BackRightID, 
		BackLeftID, CalibrationStartID, Interval5Seconds, Interval10Seconds, 
		Interval20Seconds, DrawStageID, NumOfCommandIDs};

private:

	ScopedPointer<MenuBarComponent> menuBar;
	enum { CalibrationIndexID };

};
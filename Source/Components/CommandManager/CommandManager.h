#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

/** This class is used to set and get an application Command Manager
	for the menu bar to use. */

class LTLACommandManager
{
public:

	~LTLACommandManager()
	{
		// Memory leak fix, forcing nullptr assign when removed.
		applicationCommandManager = nullptr;
	}

	void makeNullptr()
	{
		// Memory leak fix, forcing nullptr assign when removed.
		applicationCommandManager = nullptr;
	}

	// Creates the Application Command Manager.
	static ApplicationCommandManager& getApplicationCommandManager()
	{
		if (applicationCommandManager == nullptr)
			applicationCommandManager = new ApplicationCommandManager();

		return *applicationCommandManager;
	}
};



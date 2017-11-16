#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class LTLACommandManager
{
public:

	// Creates the Application Command Manager.
	static ApplicationCommandManager& getApplicationCommandManager()
	{
		static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

		if (applicationCommandManager == nullptr)
			applicationCommandManager = new ApplicationCommandManager();

		return *applicationCommandManager;
	}
};


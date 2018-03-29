#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

class LTLACommandManager
{
public:

	~LTLACommandManager()
	{
		applicationCommandManager = nullptr;
	}

	void makeNullptr()
	{
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



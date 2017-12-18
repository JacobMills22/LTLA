#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioPanelObject : public Component
{

public:

	void openPanel()
	{
		setVisible(true);
	}

	void closePanel()
	{
		setVisible(false);
	}

private:

};
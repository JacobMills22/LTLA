#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Class for all Audio Panel DSP Modules which will share simple functions
// such as becoming visible.

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
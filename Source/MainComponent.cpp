/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LTLA_GUI.h"
#include "KinectTracking.h"
#include "Components\LTLAMenuBar.h"

#define SKELETON_COUNT 2

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
							   private MultiTimer,
							   public Button::Listener
							
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);
        setAudioChannels (0, 2);
		KinectSensor.StartKinectST();
		startTimer(KinectUpdateTimer, 40);
		startTimer(GUITimer, 30);
	
		addAndMakeVisible(MenuBar);

		
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {

    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
		stopTimer(KinectUpdateTimer);
		stopTimer(GUITimer);
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
		GUI.paint(g);
    }

    void resized() override
    {
		GUI.resized();
		MenuBar.setBounds(10, 10, 140, 40);
    }

	void MainContentComponent::timerCallback(int timerID) override
	{
		if (timerID == KinectUpdateTimer)
		{
			KinectSensor.UpdateKinectST();
		}
		if (timerID == GUITimer)
		{
			for (int CurrentSkel = 0; CurrentSkel < SKELETON_COUNT; ++CurrentSkel)
			{
				if (KinectSensor.GetKinectTrackingState(CurrentSkel) == true)
				{
					GUI.SetEllipseCoordinates(KinectSensor.GetX(CurrentSkel), KinectSensor.GetY(CurrentSkel), CurrentSkel);
				}

				GUI.SetKinectTrackingState(CurrentSkel, KinectSensor.GetKinectTrackingState(CurrentSkel));
			}
			repaint();	
		}
	}
	
	void MainContentComponent::buttonClicked(Button* button) override
	{
		
	}


private:
    //==============================================================================

    // Your private member variables go here...
	LTLA_GUI GUI;
	KinectTracker KinectSensor;
	LTLAMenuBar MenuBar;

	enum TimerID {KinectUpdateTimer, GUITimer, NumOfTimerIDs };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};



// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }

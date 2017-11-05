/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LTLA_GUI.h"
#include "KinectTracking.h"

#define SKELETON_COUNT 6

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

		CalibrationButton.setButtonText("Calibrate Front Left");
		CalibrationButton.addListener(this);
		addAndMakeVisible(&CalibrationButton);
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
		CalibrationButton.setBounds(100, 500, 300, 100);
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
		if (button == &CalibrationButton)
		{
			if (KinectSensor.GetKinectTrackingState(0) == true)
			{			
				GUI.CalibrateStageButtonPressed(button, CalibrationTimesPressed, KinectSensor.GetX(0), KinectSensor.GetY(0));	// CHANGE 0? 
				CalibrationTimesPressed < 3 ? CalibrationTimesPressed++ : CalibrationTimesPressed = 0;
			}	
		}
	}

private:
    //==============================================================================

    // Your private member variables go here...
	LTLA_GUI GUI;
	KinectTracker KinectSensor;

	enum TimerID {KinectUpdateTimer, GUITimer, NumOfTimerIDs };

	TextButton CalibrationButton;
	int CalibrationTimesPressed = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }

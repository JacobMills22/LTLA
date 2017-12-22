#pragma once

#include "../Source/Components/TrackingGUI/TrackingGUI.h"
#include "KinectTracking.h"
#include "Components\LTLAMenuBar.h"
#include "Components\AudioEngine\AudioPanel.h"
#include "Components\AudioEngine\AudioEngine.h"

class MainContentComponent : public AudioAppComponent,
	private MultiTimer,
	public Button::Listener,
	public ApplicationCommandTarget,
	public ChangeListener,
	public Slider::Listener,
	public Label::Listener

{
public:

	MainContentComponent();
	~MainContentComponent();

	//==============================================================================
	/** Main Audio Functions */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	
	//==============================================================================
	/** GUI Function Callers*/
	void paint(Graphics& g) override;
	void resized() override;

	void timerCallback(int timerID) override;
	void buttonClicked(Button* button) override;
	void sliderValueChanged(Slider* slider) override;
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void labelTextChanged(Label* labelThatHasChanged) override;


	//==============================================================================
	/** Command Manager Functions. Implemented in Source/Components/CommandManager.cpp */

	/** Returns a Command Target to Use.*/
	ApplicationCommandTarget* getNextCommandTarget() override;

	/** Gets an array of commandIDs for use by the command manager.*/
	void getAllCommands(Array<CommandID>& commands) override;

	/** Gets Command Information such as the commands name for use by the command manager.*/
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;

	/** Called when the command is invoked, command function code goes here.*/
	bool perform(const InvocationInfo& info) override;

	/** Split Perform Functions, called when specific toolbar item is pressed.*/
	void AddStageAreaIDPressed();
	void EditStageAreasIDPressed();
	void RemoveStageAreaIDPressed();
	void editAudioParametersPressed();

	void setAudioEngineData();

private:
	//==============================================================================

	LTLA_GUI GUI;
	KinectTracker KinectSensor;
	LTLAMenuBar MenuBar;
	LTLAAudioEngine audioEngine;

	ColourSelector AreaColourSelector{(ColourSelector::showColourspace), 4, 7 };
	Label areaNameLabel;
	Label CalibrationCountDownLabel;

	enum TimerID { KinectUpdateTimer, GUITimer, CalibrationIntervalTimer, NumOfTimerIDs };

	double samplerate = 0;
	int samplesPerBlock = 0;

	int oldAreaIDContainingPerfromer[2];

	// Global Parameters
	enum { SelectNextAreaButtonID, SelectPreviousAreaButtonID, numOfButtons};
	TextButton GlobalButton[numOfButtons];
	enum {StagePanningSliderID, numOfSliders};
	Slider GlobalSliders[numOfSliders];

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)


};
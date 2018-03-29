#pragma once

#include "../Source/Components/TrackingGUI/TrackingGUI.h"
#include "KinectTracking.h"
#include "Components\LTLAMenuBar.h"
#include "Components\AudioEngine\AudioPanel.h"
#include "Components\AudioEngine\AudioEngine.h"
#include "Components\AudioEngine\Meter.h"
#include "SnapshotManager.h"


class MainContentComponent : public AudioAppComponent,
	private MultiTimer,
	public Button::Listener,
	public ApplicationCommandTarget,
	public ChangeListener,
	public Slider::Listener,
	public Label::Listener,
	public ValueTree::Listener

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
	void addStageAreaIDPressed();
	void editStageAreasIDPressed();
	void removeStageAreaIDPressed();
	void editAudioParametersPressed();

	//==============================================================================

	/** Sends performer data to the audio engine*/
	void setAudioEngineData();

	//==============================================================================

	void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;

	void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;

	void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;

	void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;

	void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;


	//==============================================================================
	// Save and Loading Manager
	
	void saveProjectAs();

	void saveProject();
	
	void loadProjectFile(ValueTree valueTreeToLoadInto);


	//==============================================================================

private:
	//==============================================================================

	TrackingGUI trackingGUI;
	KinectTracker kinectSensor;
	LTLAMenuBar menuBar;
	LTLAAudioEngine audioEngine;

	ColourSelector areaColourSelector{(ColourSelector::showColourspace), 4, 7 };
	AudioDeviceSelectorComponent audioDeviceSelector;
	Label areaNameLabel;
	Label calibrationCountDownLabel;

	enum TimerID { kinectUpdateTimer, guiTimer, calibrationIntervalTimer, numOfTimerIDs };

	double samplerate = 0;
	int samplesPerBlock = 0;

	int oldAreaIDContainingPerfromer[2]; 

	// Global Parameters
	enum { selectNextAreaButtonID, selectPreviousAreaButtonID, numOfButtons};
	TextButton globalButton[numOfButtons];

	AudioMeter stereoAudioMeter;

	ResizableWindow audioSettingsWindow;

	ValueTree valueTree;
	SnapshotManager snapshotManager;

	String currentProjectFullPath = "";

	LTLACommandManager LTLAcmd;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


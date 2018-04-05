#pragma once

#include "../Source/Components/TrackingGUI/TrackingGUI.h"
#include "KinectTracking.h"
#include "Components/MenuBar/LTLAMenuBar.h"
#include "Components\AudioEngine\AudioPanel.h"
#include "Components\AudioEngine\AudioEngine.h"
#include "Components\AudioEngine\Metering\Meter.h"
#include "ValueTree Items\SnapshotManager.h"

/** The MainContentComponent is where all components come together. 
	This class runs various timers in order to transport data to the
	various classes it uses. */

class MainContentComponent : public AudioAppComponent,
	private MultiTimer,
	public Button::Listener,
	public ApplicationCommandTarget,
	public ChangeListener,
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
	/** GUI Function Callers */
	void paint(Graphics& g) override;
	void resized() override;

	void timerCallback(int timerID) override; // Timer used to send various data between classes.
	void buttonClicked(Button* button) override;
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void labelTextChanged(Label* labelThatHasChanged) override;
	
	//==============================================================================
	/** Command Manager Functions. Implemented in Source/Components/CommandManager/CommandManager.cpp */

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

	/** ValueTree functions need overriding but are currently unused. */
	void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
	void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
	void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
	void valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;
	void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;
	
	//==============================================================================

	/** Saves project as a new XML file.*/
	void saveProjectAs();

	/** Saves over the existing XML file. */
	void saveProject();
	
	/** Loads a saved XML ile into the current valuetree. */
	void loadProjectFile(ValueTree valueTreeToLoadInto);

	/** Displays an alert window asking the user if the current snapshot needs updating.*/
	void askUserIfSnapshotShouldBeUpdated();
	
	//==============================================================================

private:
	//==============================================================================

	enum TimerID { kinectUpdateTimer, guiTimer, calibrationIntervalTimer, numOfTimerIDs };

	TrackingGUI trackingGUI;
	KinectTracker kinectSensor;
	LTLAAudioEngine audioEngine;

	LTLAMenuBar menuBar;
	LTLACommandManager LTLAcmd;
	AudioMeter stereoAudioMeter;

	ResizableWindow audioSettingsWindow;
	AudioDeviceSelectorComponent audioDeviceSelector;

	ValueTree valueTree;
	SnapshotManager snapshotManager;

	String currentProjectFullPath = "";
	double samplerate = 48000;
	int samplesPerBlock = 480;
	int oldAreaIDContainingPerfromer[2];

	// Global Parameters
	enum { selectNextAreaButtonID, selectPreviousAreaButtonID, numOfButtons };
	TextButton globalButton[numOfButtons];
	//ColourSelector areaColourSelector{ (ColourSelector::showColourspace), 4, 7 };
	ColourSelector areaColourSelector{ (ColourSelector::showColourspace + ColourSelector::showColourAtTop), 4, 7 };

	Label areaNameLabel;
	Label calibrationCountDownLabel;
	Label waterMark;

	static NativeMessageBox messageBox;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


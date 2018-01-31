
#include "MainComponent.h"

/*
PARAMETERS CURENTLY ADDED TO VALUETREE FOR SNAPSHOTS.

- Draw Stage State
- Stage Areas Positions
- Stage Areas Names
- FilePlayer: Level
- FilePlayer: Triggered By
- FilePlayer: Continue/Stop Playback When Performer Exits
- FilePlayer: Retrigger
- AutoPan: Enabled/Disabled (Resets pan to 0.5 if auto pan is disabled in the snapshot that is being recalled.)
- Filters: Low/High Pass Enable/Disable
- Filters: Low/High Pass Cutoff 


*/

void MainContentComponent::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) 
{

}

void MainContentComponent::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
{

}

void MainContentComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{

}

void MainContentComponent::valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{

}

void MainContentComponent::valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged)
{

}
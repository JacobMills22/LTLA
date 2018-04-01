
#include "../MainComponent.h"

/*
PARAMETERS CURENTLY ADDED TO VALUETREE FOR SNAPSHOTS.

- Draw Stage State
- Stage Areas: Positions
- Stage Areas: Names
- FilePlayer: Level
- FilePlayer: Triggered By
- FilePlayer: Continue/Stop playback when performer exits
- FilePlayer: Retrigger
- AutoPan: Enabled/Disabled (resets pan to 0.5 if auto pan is disabled in the snapshot that is being recalled.)
- Filters: Low/high pass enable/disable
- Filters: Low/high pass cutoff 
- EQ: Bypass
- EQ: Each bands frequency
- EQ: Each bands Q factor
- EQ: Each bands gain
- Reverb: Bypass
- Reverb: LowPassCutoff
- Reverb: HighPassCutoff
- Reverb: DirectGain
- Reverb: Early Reflections Gain
- Reverb: Late Reflections 1 Gain
- Reverb: Late Reflections 2 Gain
- Reverb: FeedbackAmount
- Reverb: Pre-Delay
- Reverb: DelayModifier

*/

void MainContentComponent::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) 
{}
void MainContentComponent::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
{}
void MainContentComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{}
void MainContentComponent::valueTreeChildOrderChanged(ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{}
void MainContentComponent::valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged)
{}
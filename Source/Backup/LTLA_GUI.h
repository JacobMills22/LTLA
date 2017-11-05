#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define SKELETON_COUNT 6

class LTLA_GUI : public Component
{

public:
	// Constructor
	LTLA_GUI();

	// Main GUI Functions
	void paint(Graphics& g) override;
	void resized() override;
	void PaintTrackedEllipse(Graphics& g, float PositionX, float PositionY, Colour colour);
	void PaintStage(Graphics& g);

	// Object Specific Functions
	void CalibrateStageButtonPressed(Button* button, int TimesPressed, float LocationDataX, float LocationDataY);

	// Getters and Setters
	void SetEllipseCoordinates(float PositionX, float PositionY, int SkeltonNum);
	void SetStageCalibrationState(bool State);
	void SetStageCoordinates(int StagePosition, float x, float y);


private:

	// Stage Drawing Variables
	bool StageCalibrated = false;
	enum { FrontLeft, FrontRight, BackLeft, BackRight, NumOfStagePositions };
	struct LTLACoordinates
	{
		float x = 0.0;
		float y = 0.0;
	};
	LTLACoordinates StageCoordinates[NumOfStagePositions];
	LTLACoordinates EllipseCoordinates[SKELETON_COUNT];
};
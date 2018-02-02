
#include "StageAreas.h"

#define FrontLeftXValue stageAreaValueTree.getPropertyAsValue("FrontLeftX", nullptr).getValue()
#define FrontLeftYValue stageAreaValueTree.getPropertyAsValue("FrontLeftY", nullptr).getValue()

#define FrontRightXValue stageAreaValueTree.getPropertyAsValue("FrontRightX", nullptr).getValue()
#define FrontRightYValue stageAreaValueTree.getPropertyAsValue("FrontRightY", nullptr).getValue()

#define BackleftXValue stageAreaValueTree.getPropertyAsValue("BackLeftX", nullptr).getValue()
#define BackLeftYValue stageAreaValueTree.getPropertyAsValue("BackLeftY", nullptr).getValue()

#define BackRightXValue stageAreaValueTree.getPropertyAsValue("BackRightX", nullptr).getValue()
#define BackRightYValue stageAreaValueTree.getPropertyAsValue("BackRightY", nullptr).getValue()

#define CentreXValue stageAreaValueTree.getPropertyAsValue("CentreX", nullptr).getValue()
#define CentreYValue stageAreaValueTree.getPropertyAsValue("CentreY", nullptr).getValue()


StageArea::StageArea() : stageAreaValueTree("LTLAValueTree")
{
	//areaName = "New Area";

	stageAreaValueTree.setProperty("FrontLeftX", 50.0, nullptr);
	stageAreaValueTree.setProperty("FrontLeftY", 50.0, nullptr);

	stageAreaValueTree.setProperty("FrontRightX", 100.0, nullptr);
	stageAreaValueTree.setProperty("FrontRightY", 50.0, nullptr);

	stageAreaValueTree.setProperty("BackLeftX", 50.0, nullptr);
	stageAreaValueTree.setProperty("BackLeftY", 100.0, nullptr);

	stageAreaValueTree.setProperty("BackRightX", 100.0, nullptr);
	stageAreaValueTree.setProperty("BackRightY", 100.0, nullptr);

	stageAreaValueTree.setProperty("CentreX", 75.0, nullptr);
	stageAreaValueTree.setProperty("CentreY", 75.0, nullptr);

	stageAreaValueTree.setProperty("AreaName", "New Area", nullptr);

	stageAreaValueTree.setProperty("Active", true, nullptr);

}

StageArea::~StageArea()
{
}

void StageArea::DrawArea(Graphics &g, bool Editmode)
{
	AreaPath.clear();

	if (stageAreaValueTree.getPropertyAsValue("Active", nullptr) == true)
	{

		AreaPath.addQuadrilateral(FrontLeftXValue, FrontLeftYValue, FrontRightXValue, FrontRightYValue,
			BackRightXValue, BackRightYValue, BackleftXValue, BackLeftYValue);

		g.setColour(GetAreaColour());
		g.fillPath(AreaPath);

		g.setColour(Colours::black);
		g.setOpacity(1.0);
		g.strokePath(AreaPath, PathStrokeType(1.0));


		if (Editmode == true)
		{
			g.setColour(Colours::white);
			if (AreaPosition[FrontLeft].Selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)FrontLeftXValue - 5, (int)FrontLeftYValue - 5, 10, 10, 1.0);

			g.setColour(Colours::white);
			if (AreaPosition[FrontRight].Selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)FrontRightXValue - 5, (int)FrontRightYValue - 5, 10, 10, 1.0);

			g.setColour(Colours::white);
			if (AreaPosition[BackLeft].Selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)BackleftXValue - 5, (int)BackLeftYValue - 5, 10, 10, 1.0);

			g.setColour(Colours::white);
			if (AreaPosition[BackRight].Selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)BackRightXValue - 5, (int)BackRightYValue - 5, 10, 10, 1.0);

			g.setColour(Colours::blue);
			if (AreaPosition[Centre].Selected == true) { g.setColour(Colours::cyan); }
			g.drawRect((int)CentreXValue - 5, (int)CentreYValue - 5, 10, 10, 1.0);

		}

		if (areaSelectedState == true)
		{
			g.setColour(Colours::white);
			g.strokePath(AreaPath, PathStrokeType(1.0));
		}
	}
}

void StageArea::UpdateArea(int Corner, float X, float Y)
{
	if (stageAreaValueTree.getPropertyAsValue("Active", nullptr) == true)
	{
		if (Corner == Centre)
		{
			if (X >= 0 && X <= trackingGUIWidth)
			{
				stageAreaValueTree.setProperty("CentreX", X, nullptr);

				for (int corner = 0; corner < NumOfAreaCorners - 1; corner++)
				{
					setX(corner, (X - AreaPosition[corner].differenceX));
				}
			}
			else
			{
				X <= 0 ? stageAreaValueTree.setProperty("CentreX", 0.0, nullptr) : stageAreaValueTree.setProperty("CentreX", trackingGUIWidth, nullptr);
			}

			if (Y >= 0 && Y <= trackingGUIHeight)
			{
				stageAreaValueTree.setProperty("CentreY", Y, nullptr);

				for (int corner = 0; corner < NumOfAreaCorners - 1; corner++)
				{
					setY(corner, (Y - AreaPosition[corner].differenceY));
				}
			}
			else
			{
				Y <= 0 ? stageAreaValueTree.setProperty("CentreY", 0.0, nullptr) : stageAreaValueTree.setProperty("CentreY", trackingGUIHeight, nullptr);
			}
		}
		else
		{
			if (X >= 0 && X <= trackingGUIWidth)
			{
				setX(Corner, X);
				stageAreaValueTree.setProperty("CentreX", AreaPath.getBounds().getCentreX(), nullptr);
			}
			else
			{
				X <= 0 ? setX(Corner, 0.0) : setX(Corner, trackingGUIWidth);
			}

			if (Y >= 0 && Y <= trackingGUIHeight)
			{
				setY(Corner, Y);
				stageAreaValueTree.setProperty("CentreY", AreaPath.getBounds().getCentreY(), nullptr);
			}
			else
			{
				Y <= 0 ? setY(Corner, 0.0) : setY(Corner, trackingGUIHeight);
			}
		}
	}
}

float StageArea::GetX(int CornerID)
{
	if (CornerID == FrontLeft)
		return FrontLeftXValue;
	else if (CornerID == FrontRight)
		return FrontRightXValue;
	else if (CornerID == BackRight)
		return BackRightXValue;
	else if (CornerID == BackLeft)
		return BackleftXValue;
	else if (CornerID == Centre)
		return CentreXValue;
}
float StageArea::GetY(int CornerID)
{
	if (CornerID == FrontLeft)
		return FrontLeftYValue;
	else if (CornerID == FrontRight)
		return FrontRightYValue;
	else if (CornerID == BackRight)
		return BackRightYValue;
	else if (CornerID == BackLeft)
		return BackLeftYValue;
	else if (CornerID == Centre)
		return CentreYValue;
}

void StageArea::setX(int CornerID, float value)
{
	if (CornerID == FrontLeft)
		stageAreaValueTree.setProperty("FrontLeftX", value, nullptr);
	else if (CornerID == FrontRight)
		stageAreaValueTree.setProperty("FrontRightX", value, nullptr);
	else if (CornerID == BackRight)
		stageAreaValueTree.setProperty("BackRightX", value, nullptr);
	else if (CornerID == BackLeft)
		stageAreaValueTree.setProperty("BackLeftX", value, nullptr);
	else if (CornerID == Centre)
		stageAreaValueTree.setProperty("CentreX", value, nullptr);
}

void StageArea::setY(int CornerID, float value)
{
	if (CornerID == FrontLeft)
		stageAreaValueTree.setProperty("FrontLeftY", value, nullptr);
	else if (CornerID == FrontRight)
		stageAreaValueTree.setProperty("FrontRightY", value, nullptr);
	else if (CornerID == BackRight)
		stageAreaValueTree.setProperty("BackRightY", value, nullptr);
	else if (CornerID == BackLeft)
		stageAreaValueTree.setProperty("BackLeftY", value, nullptr);
	else if (CornerID == Centre)
		stageAreaValueTree.setProperty("CentreY", value, nullptr);
}

float StageArea::getDifferenceX(int CornerID)
{
	return AreaPosition[CornerID].differenceX;
}

float StageArea::getDifferenceY(int CornerID)
{
	return AreaPosition[CornerID].differenceY;
}

void StageArea::setDifferenceX(int cornerID, float value)
{
	AreaPosition[cornerID].differenceX = value;
}

void StageArea::setDifferenceY(int cornerID, float value)
{
	AreaPosition[cornerID].differenceY = value;
}

void StageArea::updateTrackingGUIWidthAndHeight(float width, float height)
{
	trackingGUIWidth = width;
	trackingGUIHeight = height;
}

Path StageArea::GetAreaPath()
{
	return AreaPath;
}

void StageArea::SetCornerSelectedState(int Corner, bool State)
{
	AreaPosition[Corner].Selected = State;
}
bool StageArea::GetCornerSelectedState(int Corner)
{
	return AreaPosition[Corner].Selected;
}

void StageArea::SetAreaSelectedState(bool State)
{
	areaSelectedState = State;
}
bool StageArea::GetAreaSelectedState()
{
	return areaSelectedState;
}

void StageArea::SetAreaColour(Colour colour)
{
	AreaColour = colour;
}

Colour StageArea::GetAreaColour()
{
	return AreaColour;
}

void StageArea::setAreaName(String name)
{
	stageAreaValueTree.setProperty("AreaName", name, nullptr);
}

String StageArea::getAreaName()
{	
	return stageAreaValueTree.getPropertyAsValue("AreaName", nullptr).getValue();
}


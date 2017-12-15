
#include "StageAreas.h"



StageArea::StageArea()
{
	AreaPosition[FrontLeft].x = 50.0;       
	AreaPosition[FrontLeft].y = 50.0;
	AreaPosition[FrontRight].x = 100.0;
	AreaPosition[FrontRight].y = 50.0;
	AreaPosition[BackRight].x = 100.0;
	AreaPosition[BackRight].y = 100.0;
	AreaPosition[BackLeft].x = 50.0;
	AreaPosition[BackLeft].y = 100.0;

	areaName = "New Area";

	valueTree = new ValueTree("Stage Area Value Tree");
	valueTree->addListener(this);

	valueTree->setProperty("AreaSelectedState", false, nullptr);	
}

void StageArea::DrawArea(Graphics &g, bool Editmode)
{
	AreaPath.clear();

	AreaPath.addQuadrilateral(AreaPosition[FrontLeft].x, AreaPosition[FrontLeft].y,
							  AreaPosition[FrontRight].x, AreaPosition[FrontRight].y,
							  AreaPosition[BackRight].x, AreaPosition[BackRight].y,
							  AreaPosition[BackLeft].x, AreaPosition[BackLeft].y);

	g.setColour(GetAreaColour());
	g.fillPath(AreaPath);

	g.setColour(Colours::black);
	g.setOpacity(1.0);
	g.strokePath(AreaPath, PathStrokeType(1.0));

	if (Editmode == true)
	{
		g.setColour(Colours::white);
		if (AreaPosition[FrontLeft].Selected == true) { g.setColour(Colours::cyan); }
		g.drawRect((int)AreaPosition[FrontLeft].x - 5, (int)AreaPosition[FrontLeft].y - 5, 10, 10, 1.0);

		g.setColour(Colours::white);
		if (AreaPosition[FrontRight].Selected == true) { g.setColour(Colours::cyan); }
		g.drawRect((int)AreaPosition[FrontRight].x - 5, (int)AreaPosition[FrontRight].y - 5, 10, 10, 1.0);

		g.setColour(Colours::white);
		if (AreaPosition[BackLeft].Selected == true) { g.setColour(Colours::cyan); }
		g.drawRect((int)AreaPosition[BackLeft].x - 5, (int)AreaPosition[BackLeft].y - 5, 10, 10, 1.0);

		g.setColour(Colours::white);
		if (AreaPosition[BackRight].Selected == true) { g.setColour(Colours::cyan); }
		g.drawRect((int)AreaPosition[BackRight].x - 5, (int)AreaPosition[BackRight].y - 5, 10, 10, 1.0);
	}

	if (valueTree->getPropertyAsValue("AreaSelectedState", nullptr) == true)
	{
		g.setColour(Colours::white);
		g.strokePath(AreaPath, PathStrokeType(1.0));
	}
}

void StageArea::UpdateArea(int Corner, float X, float Y)
{
	AreaPosition[Corner].x = X;
	AreaPosition[Corner].y = Y;
}

void StageArea::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
}

float StageArea::GetX(int CornerID)
{
	return AreaPosition[CornerID].x;
}
float StageArea::GetY(int CornerID)
{
	return AreaPosition[CornerID].y;
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
	valueTree->setProperty("AreaSelectedState", State, nullptr);
//	AreaSelectedState = State;
}
bool StageArea::GetAreaSelectedState()
{
//	return AreaSelectedState;
	return valueTree->getPropertyAsValue("AreaSelectedState", nullptr).getValue();
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
	areaName = name;
}

String StageArea::getAreaName()
{
	return areaName;
}


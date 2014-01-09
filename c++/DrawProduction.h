#pragma once
#include "DrawBuilding.h"
namespace GUI{
class DrawProduction :
	public DrawBuilding
{
public:
	DrawProduction(CEGUI::Window* mainWindow);
	~DrawProduction(void);
	void DoDraw(Logic::ShowProduction *evnt);

	bool SetProduction(const CEGUI::EventArgs &e);
};
}


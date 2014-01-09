#pragma once
#include "DrawBuilding.h"
namespace GUI{
class DrawMilitary :
	public DrawBuilding
{
public:
	DrawMilitary(CEGUI::Window* mainWindow);
	~DrawMilitary(void);
	bool AddUnitToQueue(const CEGUI::EventArgs &e);
	bool UnitClick(const CEGUI::EventArgs &e);
	bool CountryClick(const CEGUI::EventArgs &e);
	void DoDrawUnit(Logic::ShowUnitInBase *evnt);
	void IsNeedRefresh();
	void DoDraw(Logic::ShowMilitary *evnt);
	
protected:
	CEGUI::Window* mDialog;
	Logic::BaseUnit* mCacheUnit;
};
}


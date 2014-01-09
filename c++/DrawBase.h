#pragma once
#include "BaseDraw.h"
namespace GUI{
class DrawBase : public BaseDraw
{
public:
	DrawBase(CEGUI::Window* mainWindow);
	~DrawBase(void);
	void SetBase(Logic::BaseCore* building);
	void DoDraw(Logic::SendCoreToGui *evnt);
	void DrawBlueprint(Logic::SendBlueToGui *evnt);
	void IsNeedRefresh();
	bool Close(const CEGUI::EventArgs &e);
	bool SlotClick(const CEGUI::EventArgs &e);
	bool BuildOrder(const CEGUI::EventArgs &e);
protected:
	Logic::BaseBaseCore* mBuilding;
	CEGUI::Window* mBlueSheet;
	CEGUI::Window* mBlueSheetClose;
};
}


#pragma once
#include "basedraw.h"
namespace GUI {
class DrawWarfare :
	public BaseDraw
{
public:
	DrawWarfare(CEGUI::Window* mainWindow);
	~DrawWarfare(void);
	bool SetAction(const CEGUI::EventArgs &e);
	void  DoDraw(Logic::ShowWarfare* evnt);
	void IsNeedRefresh();
	void Close(Logic::CloseWarfare *evnt);
protected:
	Logic::BaseCountry* mCountry;
};
}


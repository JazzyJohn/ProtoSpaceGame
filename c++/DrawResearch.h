#pragma once
#include "basedraw.h"
namespace GUI{
class DrawResearch :
	public BaseDraw
{
public:
	DrawResearch(CEGUI::Window* mainWindow);
	~DrawResearch(void);
	bool AskForAllResourse(const CEGUI::EventArgs &e);
	bool SetResearch(const CEGUI::EventArgs &e);
	bool UseResource(const CEGUI::EventArgs &e);
	void DoDraw(Logic::ShowResearchCenter *evnt);
	void DrawResourceDialog(Logic::ShowAllResourse *resevnt);
	void IsNeedRefresh();

protected:
	CEGUI::Window * mDialog;
};
}


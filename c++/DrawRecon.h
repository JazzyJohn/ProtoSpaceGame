#pragma once
#include "basedraw.h"
namespace GUI{
class DrawRecon :
	public BaseDraw
{
public:
	DrawRecon(CEGUI::Window* mainWindow);
	~DrawRecon(void);
	void DoDraw(Logic::ReconStateEvnt *evnt);
	bool Launch(const CEGUI::EventArgs &e);
	void IsNeedRefresh();
};
};


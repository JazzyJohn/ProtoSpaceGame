#pragma once
#include "basedraw.h"
namespace GUI{
class DrawResource :
	public BaseDraw
{
public:
	DrawResource(CEGUI::Window* mainWindow);
	~DrawResource(void);
	void IsNeedRefresh();
	void DoDraw(Logic::ShowResourceEvnt *resevnt);
};
}


#pragma once
#include "basedraw.h"
namespace GUI{
class DrawBuilding :
	public BaseDraw
{
public:
	DrawBuilding(CEGUI::Window* mainWindow);
	~DrawBuilding(void);
	virtual void IsNeedRefresh();
protected:
	Logic::BaseBuilding* mBuilding;
};
};


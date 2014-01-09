#pragma once
#include "DrawBuilding.h"
namespace GUI{
class DrawWarehouse :
	public DrawBuilding
{
public:
	DrawWarehouse(CEGUI::Window* mainWindow);
	~DrawWarehouse(void);
	void DoDraw(Logic::ShowWarehouse *evnt);
	
	bool Delete(const CEGUI::EventArgs &e);
protected:
	
};
}


#pragma once
#include "DrawBuilding.h"
namespace GUI{
class DrawMining :
	public DrawBuilding
{
public:
	DrawMining(CEGUI::Window* mainWindow);
	~DrawMining(void);
	bool SetDeposit(const CEGUI::EventArgs &e);

	void DoDraw(Logic::ShowMining *evnt);
protected:
	
};
}

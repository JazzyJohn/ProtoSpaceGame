#include "DrawBuilding.h"
using namespace GUI;

DrawBuilding::DrawBuilding(CEGUI::Window* mainWindow):BaseDraw(mainWindow)
{
}


DrawBuilding::~DrawBuilding(void)
{
}

void DrawBuilding::IsNeedRefresh(){
	if(isOpen&&!mIsResend){
			Logic::ShowBuilding *evnt = new Logic::ShowBuilding();
			evnt->building =mBuilding;
			evnt->isReSend = true;
			Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
			mIsResend= true;
			PriorIdDraw= false;
	}
}
#include "BaseResearchBuilding.h"
using namespace Logic;
ShowResearchCenter::ShowResearchCenter(){
	type =ET_CONTROL;
	gameType =GUIET_SHOWRESEARCHCENTER;
}
		
BaseResearchBuilding::BaseResearchBuilding()
{

	mProductionPower=BaseAsset::GetSigleton("RESEARCH_PRODUCTION_POWER");
		mType = TB_RESEARCH;
}

void BaseResearchBuilding::SetManager (BaseReasearchManager *resMan){
		mResearchManager = resMan;

}
BaseResearchBuilding::~BaseResearchBuilding(void)
{
}
void BaseResearchBuilding::PassDay(std::queue<NotificationEvent*> &notificationEvent){
		
	mIdle =!mResearchManager->addProgress(GetProductionPower());
	BaseBuilding::PassDay(notificationEvent);	
}
GameEvent *BaseResearchBuilding::ReturnBuildingInfo(){

	
	return mResearchManager-> ReturnResearchInfo();
	
}

#include "DrawResource.h"
using namespace GUI;

DrawResource::DrawResource(CEGUI::Window* mainWindow):BaseDraw(mainWindow)
{
	isOpen = true;
}


DrawResource::~DrawResource(void)
{
}
void DrawResource::IsNeedRefresh(){
	if(!mIsResend){
		mIsResend=true;
		Logic::ControlEvent *evnt = new Logic::ControlEvent();
		evnt->controlType =Logic::CTE_SHOWRESOURCE;
		Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	}

}
void DrawResource::DoDraw(Logic::ShowResourceEvnt *resevnt){
	mIsResend = false;
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	std::map<Ogre::String,int>::iterator the_iterator;
	int i=0;
	Ogre::String name;
	Ogre::String textamount;
	CEGUI::Window *res;

	for(the_iterator = resevnt->mResorseCnt.begin();the_iterator != resevnt->mResorseCnt.end();++the_iterator){
		name=the_iterator->first;
		if(wmgr.isWindowPresent(name+"_Rees")){
			res= wmgr.getWindow(name+"_Rees");
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name+"_Rees");
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.12*i, 0), CEGUI::UDim(0, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.12, 0), CEGUI::UDim(0.05, 0)));
			mMainWindow->addChildWindow(res);
		}
		Logic::LogicStd::strToInt(&textamount,the_iterator->second);
		res->setText(name+":"+textamount);
		i++;

	}



}
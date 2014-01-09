#include "DrawWarehouse.h"

using namespace GUI;
DrawWarehouse::DrawWarehouse(CEGUI::Window* mainWindow):DrawBuilding(mainWindow)
{
}


DrawWarehouse::~DrawWarehouse(void)
{
}

bool DrawWarehouse::Delete(const CEGUI::EventArgs &e){
	Logic::ShowBuilding *evnt = new Logic::ShowBuilding();
			evnt->building =mBuilding;
			
			evnt->controlType = Logic::CTE_DELETEBUILDING;
			Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	return true;
}
void DrawWarehouse::DoDraw(Logic::ShowWarehouse *evnt){
	if(!isOpen&&evnt->isReSend){
	return;
	}
	if(evnt->isReSend&& PriorIdDraw){
		return;
	}
	
	isOpen = true;
	mIsResend= false;
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window *res;
	mBuilding= evnt->target;
	Ogre::String name ="WareHouseSheet/";
	Ogre::String text;

	if(!wmgr.isWindowPresent("WareHouseSheet")){
		mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "WareHouseSheet");
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.3, 0)));
		mFrameWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.5, 0)));
		mCloseBtn= wmgr.createWindow("TaharezLook/Button",  "WareHouseSheet/Close");
		mCloseBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		mCloseBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.15, 0)));
		mCloseBtn->setText("Close");
		mCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close, (BaseDraw*)this));
		mFrameWindow->addChildWindow(mCloseBtn); 
			res= wmgr.createWindow("TaharezLook/Button",  "WareHouseSheet/Delete");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.15, 0)));
		res->setText("Delete");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&DrawWarehouse::Delete, this));
		mFrameWindow->addChildWindow(res); 
		
	}
	int x=0,y=0;
	for(int i=0; i<evnt->SlotNum;i++){

		name="WareHouseSheet/slot"+Logic::LogicStd::IntToString(i);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);


		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25*x,0), CEGUI::UDim(0.15*(y+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.15, 0)));
			mFrameWindow->addChildWindow(res);


		}
		res->setText(evnt->allSlot[i]);
		if(++x>3){
			y++;
			x=0;
		}

	}
	if(!evnt->isReSend){
	mMainWindow->addChildWindow(mFrameWindow);
	PriorIdDraw= true;
	}else{
	PriorIdDraw= false;
	}
}
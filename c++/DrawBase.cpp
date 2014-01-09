#include "DrawBase.h"
#include "DrawAbility.h"
using namespace GUI;
DrawBase::DrawBase(CEGUI::Window* mainWindow):BaseDraw(mainWindow)
{
	

}



DrawBase::~DrawBase(void)
{
}
bool DrawBase::Close(const CEGUI::EventArgs &e){
		const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	if(mMainWindow->isChild(args.window->getParent()->getName())){
		if(args.window->getParent()==mFrameWindow){
			isOpen = false;
			Logic::GameEvent *evnt = new Logic::GameEvent();
			evnt->gameType = Logic::GUIET_RESETCAM;
			Logic::BaseListener::GetSigleton()->fireGameEvent(evnt);
			
		}	
		mMainWindow->removeChildWindow(args.window->getParent()->getName());
	}
	if(DrawAbility::GetSigleton()->Close(this)){
	ClosePopUp();
	}
	return true;
}
bool DrawBase::SlotClick(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::BuildingForSent *build = static_cast<Logic::BuildingForSent *>(args.window->getUserData());
	if(build->empty){
		Logic::ShowBlueprint *evnt = new Logic::ShowBlueprint();
		evnt->core =build->Core;
		
		Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	}else{
		Logic::ShowBuilding *showEvnt = new Logic::ShowBuilding();
		showEvnt->building =build->building;
		
		Logic::BaseLogic::GetSigleton()->fireGameEvent(showEvnt);

	}
	return true;
}

bool DrawBase::BuildOrder(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::BuildingBlueprint *blueprint = static_cast<Logic::BuildingBlueprint *>(args.window->getUserData());
	Logic::BuildOrder *buildOrder= new   Logic::BuildOrder();
	buildOrder->blueprint =blueprint;
	buildOrder->core = mBuilding;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(buildOrder);
	ClearFromAllChild(mBlueSheet);
	mMainWindow->removeChildWindow( mBlueSheet);
	return true;
}
void DrawBase::SetBase(Logic::BaseCore* building){
	mBuilding= static_cast<Logic::BaseBaseCore *>(building);
}
void DrawBase::IsNeedRefresh(){
	if(isOpen&&!mIsResend){
			Logic::ShowBuilding *evnt = new Logic::ShowBuilding();
			evnt->building =mBuilding;
			evnt->isReSend = true;
			Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
			mIsResend = true;
			PriorIdDraw= false;
	}
}

void DrawBase::DoDraw(Logic::SendCoreToGui *evnt){
	if(!isOpen&&evnt->isReSend){
	return;
	}
	if(evnt->isReSend&& PriorIdDraw){
		return;
	}
	
	mIsResend= false;
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *res;
		isOpen = true;
	Ogre::String name;

	int x=0,y=0;
	name= "BaseDraw";
	if(wmgr.isWindowPresent(name)){
		mFrameWindow= wmgr.getWindow(name);

	}else{
		mFrameWindow= wmgr.createWindow("DefaultWindow", name);
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
		mFrameWindow->setMousePassThroughEnabled(true);
		mCloseBtn= wmgr.createWindow("TaharezLook/Button",  "BaseDraw/Close");
		mCloseBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		mCloseBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
		mCloseBtn->setText("Close");
		mCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close,(BaseDraw*)this));
		mFrameWindow->addChildWindow(mCloseBtn); 
	}

	for(int i=0; i < evnt->SlotCnt;++i){
		name= "BaseDraw/BuildNum_"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			delete static_cast<Logic::BuildingForSent *>( res->getUserData());

			res->setUserData(evnt->allSlots[i]);
		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.33*x,0), CEGUI::UDim(0.1*(y+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.33, 0), CEGUI::UDim(0.1, 0)));
			res->setUserData(evnt->allSlots[i]);
			res->subscribeEvent(CEGUI::Window::EventDestructionStarted,
				CEGUI::Event::Subscriber(&BaseDraw::ClearUserData,(BaseDraw*)this));
			mFrameWindow->addChildWindow(res);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawBase::SlotClick, this));

		}
		if(evnt->allSlots[i]->empty){
			res->setText("EMPTY");
		}else{

			res->setText(evnt->allSlots[i]->name);
		}
		if(++x>2){
			x=0;
			y++;
		}
	}
	SetBase(evnt->target);
	if(!evnt->isReSend){
	mMainWindow->addChildWindow(mFrameWindow);
	PriorIdDraw= true;
	}else{
	PriorIdDraw= false;
	}
	


}
void DrawBase::DrawBlueprint(Logic::SendBlueToGui *evnt){
	
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *res;
	Ogre::String name;
	Ogre::String text;
	int x=0,y=0,i=0;
	std::vector<Logic::BuildingBlueprint *>::iterator the_iterator;
	the_iterator = evnt->allBlue.begin();
	
	if(wmgr.isWindowPresent( "Blueprint/sheet")){
		mBlueSheet= wmgr.getWindow( "Blueprint/sheet");
		ClearFromAllChild(mBlueSheet);
		mBlueSheet->addChildWindow(mBlueSheetClose); 

	}else{

		mBlueSheet= wmgr.createWindow("TaharezLook/StaticImage", "Blueprint/sheet");
		mBlueSheet->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
		mBlueSheet->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0, 0)));
		mBlueSheetClose= wmgr.createWindow("TaharezLook/Button",   "Blueprint/close");
		mBlueSheetClose->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		mBlueSheetClose->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
		mBlueSheetClose->setText("Close");
		mBlueSheetClose->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close,(BaseDraw*) this));
		mBlueSheet->addChildWindow(mBlueSheetClose); 
	}
	while (the_iterator !=  evnt->allBlue.end()) {
		name= "Blueprint/BlueNum"+Logic::LogicStd::IntToString(i++);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			res->setUserData((*the_iterator));
			mBlueSheet->addChildWindow(res);
		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5*x,0), CEGUI::UDim(0.2*y+0.1, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
			res->setUserData((*the_iterator));

			mBlueSheet->addChildWindow(res);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawBase::BuildOrder, this));


		}
		text=(*the_iterator)->mName;
		text+="_"+Logic::LogicStd::IntToString((*the_iterator)->mCost);
		res->setText(text);
		if(++x>=2){
			x=0;
			y++;
		}
		++the_iterator;
	}
	mMainWindow->addChildWindow(mBlueSheet);


}

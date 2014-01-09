#include "DrawMilitary.h"
#include "DrawAbility.h"
using namespace GUI;

DrawMilitary::DrawMilitary(CEGUI::Window* mainWindow):DrawBuilding(mainWindow)
{
}


DrawMilitary::~DrawMilitary(void)
{
}
bool DrawMilitary::AddUnitToQueue(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::AddUnittoQueue *sendEvnt = new Logic::AddUnittoQueue();
	sendEvnt->building =mBuilding;
	sendEvnt->blueprint =	static_cast <Logic::BaseUnitBlueprint*>(args.window->getUserData());
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawMilitary::UnitClick(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
	sendEvnt->controlType =Logic::CTE_UNITCLICK;
	sendEvnt->target =	args.window->getUserData();
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawMilitary::CountryClick(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SendUnitToCountry *sendEvnt = new Logic::SendUnitToCountry();
	sendEvnt->country =static_cast<Logic::BaseCountry*>(args.window->getUserData());
	sendEvnt->base =mBuilding;
	sendEvnt->unit = mCacheUnit;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
void DrawMilitary::DoDraw(Logic::ShowMilitary *evnt){
	if(!isOpen&&evnt->isReSend){
		return;
	}
		if(evnt->isReSend&& PriorIdDraw){
		return;
	}
	
	isOpen =true;
	mIsResend = false;
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window *res;
	CEGUI::ProgressBar *prog;
	Ogre::String name ="MilitarySheet/";
	Ogre::String text;
	mBuilding = evnt->target;
	if(wmgr.isWindowPresent("MilitarySheet")){
		if(!evnt->isReSend){
			ClearFromAllChild(mFrameWindow);
		}
	
		prog= static_cast<CEGUI::ProgressBar *>(wmgr.getWindow("MilitarySheet/Progress"));
		if(!evnt->isReSend){
				mFrameWindow->addChildWindow(mCloseBtn); 
				mFrameWindow->addChildWindow(prog); 	
		}
	}else{
		mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "MilitarySheet");
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
		mCloseBtn= wmgr.createWindow("TaharezLook/Button",  "MilitarySheet/Close");
		mCloseBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		mCloseBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		mCloseBtn->setText("Close");
		mCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close,(BaseDraw*) this));
		prog=  static_cast<CEGUI::ProgressBar *>(wmgr.createWindow("TaharezLook/ProgressBar", "MilitarySheet/Progress"));
		prog->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2,0), CEGUI::UDim(0, 0)));
		prog->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
		mFrameWindow->addChildWindow(mCloseBtn);
		mFrameWindow->addChildWindow(prog); 	
	}
	prog->setProgress(evnt->procent);

	int i=0;
	while(i<5&&!(evnt->unitQueue.empty())){
			name="MilitarySheet/quere"+Logic::LogicStd::IntToString(i);
			if(wmgr.isWindowPresent(name)){
				res= wmgr.getWindow(name);
				if(!evnt->isReSend){
				mFrameWindow->addChildWindow(res);
				}
			}else{
				res= wmgr.createWindow("TaharezLook/StaticText", name);
				res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1*i,0), CEGUI::UDim(0.05, 0)));
				res->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
				mFrameWindow->addChildWindow(res);		
			}
			res->setText(evnt->unitQueue.front());
			evnt->unitQueue.pop();
			i++;
	}
	i=0;
	std::map<Ogre::String,Logic::BaseUnitBlueprint*>::iterator iterAvUn;
	for(iterAvUn = evnt->avaibleUnits.begin();iterAvUn!=evnt->avaibleUnits.end();++iterAvUn){
		name="MilitarySheet/availebtobuild"+Logic::LogicStd::IntToString(i);
			if(wmgr.isWindowPresent(name)){
				res= wmgr.getWindow(name);
				if(!evnt->isReSend){
				mFrameWindow->addChildWindow(res);
				}
			}else{
				res= wmgr.createWindow("TaharezLook/Button", name);
				res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.9,0), CEGUI::UDim(0.1*i+0.05, 0)));
				res->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
					res->subscribeEvent(CEGUI::PushButton::EventClicked,
						CEGUI::Event::Subscriber(&DrawMilitary::AddUnitToQueue, this));
				mFrameWindow->addChildWindow(res);		
			}
			res->setUserData(iterAvUn->second);
			res->setText(iterAvUn->first);
		
	}
	i=0;
	std::vector<Logic::UnitToSend>::iterator iterUnOnBase;
	int x=0,y=0;
	for(iterUnOnBase = evnt->unitOnBase.begin();iterUnOnBase!=evnt->unitOnBase.end();++iterUnOnBase){
		name="MilitarySheet/onBase"+Logic::LogicStd::IntToString(i);
			if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			if(!evnt->isReSend){
			mFrameWindow->addChildWindow(res);
			}
		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15*x,0), CEGUI::UDim(0.05*(y+1)+0.5, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
			mFrameWindow->addChildWindow(res);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
						CEGUI::Event::Subscriber(&DrawMilitary::UnitClick, this));
		
		


		}
			res->setText(iterUnOnBase->name);
			res->setUserData(iterUnOnBase->unit);
		if(++x>3){
			y++;
			x=0;
		}
		i++;
	}
	if(!evnt->isReSend){
	mMainWindow->addChildWindow(mFrameWindow);
	PriorIdDraw= true;
	}else{
	PriorIdDraw= false;
	}
}
void DrawMilitary::IsNeedRefresh(){
	if(isOpen&&!mIsResend){
			Logic::ShowBuilding *evnt = new Logic::ShowBuilding();
			evnt->building =mBuilding;
			evnt->isReSend = true;
			Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
			mIsResend = true;
			PriorIdDraw= false;	
			
	}
	if(isPopMenu){
		isPopMenu= true;
		DrawAbility::GetSigleton()->DoDraw(mCacheUnit,mAbilitys,this);
	}

}
void DrawMilitary::DoDrawUnit(Logic::ShowUnitInBase *evnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *res,*ability;
	mAbilitys= evnt->abilitys;
	Ogre::String name;
	mCacheUnit = evnt->unit;
	int x=0,y=0;
	name= "UnitDialog";
	if(wmgr.isWindowPresent(name)){

		ClearFromAllChild(mDialog);


	}else{
		mDialog= wmgr.createWindow("TaharezLook/StaticImage", name);
		mDialog->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

	}
	if(wmgr.isWindowPresent("UnitDialog/CloseDialog")){

		res= wmgr.getWindow("UnitDialog/CloseDialog");
		mDialog->addChildWindow(res);
		
		res= wmgr.getWindow("UnitDialog/UnitName");
		mDialog->addChildWindow(res);
		ability= wmgr.getWindow("UnitDialog/Ability");
		mDialog->addChildWindow(ability); 
	}else{
		res= wmgr.createWindow("TaharezLook/Button",  "UnitDialog/CloseDialog");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Close");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close,(BaseDraw*) this));
		mDialog->addChildWindow(res); 
		res= wmgr.createWindow("TaharezLook/StaticText",  "UnitDialog/UnitName");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.1, 0)));
		
		mDialog->addChildWindow(res);
		ability= wmgr.createWindow("TaharezLook/Button",  "UnitDialog/Ability");
		ability->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7,0), CEGUI::UDim(0.15, 0)));
		ability->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		ability->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown,
				CEGUI::Event::Subscriber(&BaseDraw::PopUp,(BaseDraw*) this));
		ability->setText("Ability");
		mDialog->addChildWindow(ability);
	}
	res->setText(evnt->name);
	ability->setUserData(mCacheUnit);
	int i=0;
	std::map<Ogre::String,int>::iterator charItr;
	float size= evnt->character.size();
	for(charItr=evnt->character.begin();charItr!= evnt->character.end();++charItr){
		name="UnitDialog/targetchar"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			mDialog->addChildWindow(res);
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(1/size*i,0), CEGUI::UDim(0.3, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(1/size, 0), CEGUI::UDim(0.15, 0)));

			mDialog->addChildWindow(res);
		}
		i++;
		res->setText(charItr->first+" :\n "+Logic::LogicStd::IntToString(charItr->second));
	}
	i=0;
	size= evnt->allCountry.size();
	std::map<Ogre::String,Logic::BaseCountry *>::iterator cntriter;
	for(cntriter=evnt->allCountry.begin();cntriter!= evnt->allCountry.end();++cntriter){
		name="UnitDialog/targetcntry"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);

			mDialog->addChildWindow(res);
		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(1/size*i,0), CEGUI::UDim(0.5, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(1/size, 0), CEGUI::UDim(0.15, 0)));

			mDialog->addChildWindow(res);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawMilitary::CountryClick, this));

		}
		res->setUserData(cntriter->second);
		res->setText(cntriter->first);
		i++;
	}
	if(isPopMenu){
		isPopMenu= true;
		DrawAbility::GetSigleton()->DoDraw(mCacheUnit,mAbilitys,this);
	}
	
	mMainWindow->addChildWindow(mDialog);
}
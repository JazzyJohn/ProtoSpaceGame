#include "DrawWarfare.h"
using namespace GUI;

DrawWarfare::DrawWarfare(CEGUI::Window* mainWindow):BaseDraw(mainWindow)
{
}


DrawWarfare::~DrawWarfare(void)
{
}
void  DrawWarfare::Close(Logic::CloseWarfare *evnt){
	if(mMainWindow->isChild("BattlifieldSheet")&&evnt->contry==mCountry){
		mMainWindow->removeChildWindow("BattlifieldSheet");
		isOpen = false;
	}
}
bool DrawWarfare::SetAction(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::ActionWarfareSet *sendEvnt = new Logic::ActionWarfareSet();
	sendEvnt->country = mCountry;
	std::stringstream convert(args.window->getUserString("type").c_str());
	int i;
	convert>>i;
	sendEvnt->action = (Logic::WarfareAction)i;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
void DrawWarfare::IsNeedRefresh(){
	if(isOpen&&!mIsResend){
			Logic::SomethingSimple *evnt = new Logic::SomethingSimple();
			evnt->target =mCountry;
			evnt->isReSend = true;
			evnt->controlType =Logic::CTE_SHOWWARAFERE;
			Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
			mIsResend = true;
			PriorIdDraw= false;
	}
}

void  DrawWarfare::DoDraw(Logic::ShowWarfare* evnt){
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
	Ogre::String name ="BattlifieldSheet/";
	Ogre::String text;
	mCountry = evnt->contry;
	if(wmgr.isWindowPresent("BattlifieldSheet")){
		if(!evnt->isReSend){
			ClearFromAllChild(mFrameWindow);
		}

		if(!evnt->isReSend){
			mFrameWindow->addChildWindow(mCloseBtn); 

		}
	}else{
		mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "BattlifieldSheet");
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
		mCloseBtn= wmgr.createWindow("TaharezLook/Button",  "BattlifieldSheet/Close");
		mCloseBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		mCloseBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		mCloseBtn->setText("Close");
		mCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close,(BaseDraw*) this));
		mFrameWindow->addChildWindow(mCloseBtn);

	}
	int i=0,j=0;
	std::map<Ogre::String,int>::iterator charItr;
	std::map<Ogre::String,Ogre::String>::iterator charContItr;
	float size= evnt->countryChar.size();
	for(charContItr=evnt->countryChar.begin();charContItr!= evnt->countryChar.end();++charContItr){

		name="BattlifieldSheet/countrychar"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			if(!evnt->isReSend){
			mFrameWindow->addChildWindow(res);
			}
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(1/size*i,0), CEGUI::UDim(0.1, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(1/size, 0), CEGUI::UDim(0.15, 0)));

			mFrameWindow->addChildWindow(res);
		}
		i++;
		res->setText(charContItr->first+" :\n "+charContItr->second);
	}
	std::vector<Logic::UnitForSendWar>::iterator unitItr;
	i=0;
	for(unitItr=evnt->playerUnit.begin();unitItr!= evnt->playerUnit.end();++unitItr){
		name="BattlifieldSheet/unitName"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			if(!evnt->isReSend){
				mFrameWindow->addChildWindow(res);
			}
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1*i,0), CEGUI::UDim(0.25, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.3, 0)));

			mFrameWindow->addChildWindow(res);
		}
		
		res->setText(unitItr->name);
		
		j=0;
		name="";
		
		for(charItr=unitItr->character.begin();charItr!= unitItr->character.end();++charItr){
			if(charItr->first=="Manpower"){
				text="\n HP:"+Logic::LogicStd::IntToString(charItr->second);
			}
			name +=charItr->first+" : "+Logic::LogicStd::IntToString(charItr->second)+"\n";
		}
		res->setText(unitItr->name+text);
		res->setTooltipText(name);
		i++;
	}
	i=0;
	for(unitItr=evnt->NPCUnit.begin();unitItr!= evnt->NPCUnit.end();++unitItr){
		name="BattlifieldSheet/npcunitName"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			if(!evnt->isReSend){
				mFrameWindow->addChildWindow(res);
			}
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1*i,0), CEGUI::UDim(0.65, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.3, 0)));

			mFrameWindow->addChildWindow(res);
		}
		
	
		
		
		name="";
		
		for(charItr=unitItr->character.begin();charItr!= unitItr->character.end();++charItr){
			if(charItr->first=="Manpower"){
				text="\n HP:"+Logic::LogicStd::IntToString(charItr->second);
			}
			name +=charItr->first+" : "+Logic::LogicStd::IntToString(charItr->second)+"\n";
		}
		res->setText(unitItr->name+text);
		res->setTooltipText(name);
		i++;
	}

	for(i=0;i<4;i++){
		name = "BattlifieldSheet/action"+Logic::LogicStd::IntToString(i);
		if(!wmgr.isWindowPresent(name)){

			res= wmgr.createWindow("TaharezLook/Button",name );
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(1-0.15*(i+1),0), CEGUI::UDim(0, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
			
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawWarfare::SetAction,this));
			res->setUserString("type" ,Logic::LogicStd::IntToString(i));
		}else{
			res= wmgr.getWindow(name);
			if(!evnt->isReSend){
				mFrameWindow->addChildWindow(res);
			}
		}
		name = "";
		
		if(evnt->curAction == (Logic::WarfareAction)i){
			name="[colour='FFFF0000']";
		}
		switch(i){
			case 0:
				res->setText(name+"Stand");	
				break;
			case 1:
				res->setText(name+"Occupied");	
				break;
			case 2:
				res->setText(name+"Pillage");	
				break;	
			case 3:
				res->setText(name+"Kill with Fire");	
				break;
			}
		if(!evnt->isReSend){
			mFrameWindow->addChildWindow(res);
		}
	} 
	switch(evnt->status){
	case  Logic::WARSTS_AFTERMATCH:

		break;
	}
	if(!evnt->isReSend){
	mMainWindow->addChildWindow(mFrameWindow);
	PriorIdDraw= true;
	}else{
	PriorIdDraw= false;
	}
}
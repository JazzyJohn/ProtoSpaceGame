#include "DrawIntelegence.h"

using namespace GUI;
DrawIntelegence::DrawIntelegence(CEGUI::Window* mainWindow):DrawBuilding(mainWindow)
{
}


DrawIntelegence::~DrawIntelegence(void)
{
}
void DrawIntelegence::IsNeedRefresh(){
	if(isOpen&&!mIsResend){
		Logic::ShowBuilding *evnt = new Logic::ShowBuilding();
		evnt->building =mBuilding;
		evnt->isReSend = true;
		Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
		mIsResend= true;
		PriorIdDraw= false;
	}
}
bool DrawIntelegence::OpenIntActionDialog(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::AskIntActionDialog *sendEvnt = new Logic::AskIntActionDialog();
	sendEvnt->target =mBuilding;
	sendEvnt->action = static_cast<Logic::BaseAction*>(args.window->getUserData());
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawIntelegence::SendIntActionDialog(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::ControlEvent *sendEvnt = new Logic::ControlEvent();
	sendEvnt->controlType = Logic::CTE_DIALOGFORACTIONGSEND;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawIntelegence::DeclineIntActionDialog(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::ControlEvent *sendEvnt = new Logic::ControlEvent();	
	sendEvnt->controlType = Logic::CTE_DIALOGFORACTIONDECLINE;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawIntelegence::SetTargetIntActionDialog(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SetSomtingIntAction *sendEvnt = new Logic::SetSomtingIntAction();
	sendEvnt->target =static_cast <Logic::BaseCountry*>(args.window->getUserData());
	sendEvnt->isTarget = true;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawIntelegence::SetReasonIntActionDialog(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SetSomtingIntAction *sendEvnt = new Logic::SetSomtingIntAction();
	sendEvnt->target =static_cast <Logic::BaseCountry*>(args.window->getUserData());
	sendEvnt->isTarget = false;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawIntelegence::AskForTrade(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);

	Logic::TradeRequest *sendEvnt = new Logic::TradeRequest();
	sendEvnt->demand =static_cast <Logic::BaseDemand*>(args.window->getUserData());
	sendEvnt->building =mBuilding;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	

	return true;
}
void DrawIntelegence::DoDraw(Logic::ShowIntelegence *evnt){
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
	CEGUI::ProgressBar *prog;
	Ogre::String name ="IntelegenceSheet/";
	mBuilding = evnt->target;
	if(wmgr.isWindowPresent("IntelegenceSheet")){

		if(!evnt->isReSend){
			ClearFromAllChild(mFrameWindow);
			mFrameWindow->addChildWindow(mCloseBtn); 
		}
	}else{
		mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "IntelegenceSheet");
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
		mCloseBtn= wmgr.createWindow("TaharezLook/Button",  "IntelegenceSheet/Close");
		mCloseBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		mCloseBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		mCloseBtn->setText("Close");
		mCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close,(BaseDraw*) this));
		mFrameWindow->addChildWindow(mCloseBtn); 		
	}
	name ="IntelegenceSheet/CurMission";

	if(wmgr.isWindowPresent(name)){
		res= wmgr.getWindow(name);
		prog= static_cast<CEGUI::ProgressBar *>(wmgr.getWindow("IntelegenceSheet/Progress"));
		if(!evnt->isReSend){
			mFrameWindow->addChildWindow(res); 
			mFrameWindow->addChildWindow(prog); 	
		}
	}else{
		res= wmgr.createWindow("TaharezLook/StaticText", name);
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.15, 0)));
		mFrameWindow->addChildWindow(res); 	
		prog=  static_cast<CEGUI::ProgressBar *>(wmgr.createWindow("TaharezLook/ProgressBar", "IntelegenceSheet/Progress"));
		prog->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2,0), CEGUI::UDim(0.05, 0)));
		prog->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
		mFrameWindow->addChildWindow(prog); 	
	}
	res->setText(evnt->curAction);
	if(evnt->needProduction==0){
		prog->setProgress(0);
	}else{
		prog->setProgress((double)evnt->progress/(double)evnt->needProduction);
	}
	name ="IntelegenceSheet/CurCountry";
	if(wmgr.isWindowPresent(name)){
		res= wmgr.getWindow(name);

		if(!evnt->isReSend){
			mFrameWindow->addChildWindow(res); 

		}
	}else{
		res= wmgr.createWindow("TaharezLook/StaticText", name);
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7,0), CEGUI::UDim(0.0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.15, 0)));
		mFrameWindow->addChildWindow(res); 	
	}
	res->setText(evnt->name);
	std::list<Logic::SendBaseDemand>::iterator iterDemand;
	int i=0;
	for(iterDemand = evnt->demand.begin();iterDemand!= evnt->demand.end();++iterDemand){

		name ="IntelegenceSheet/Demand"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);

			if(!evnt->isReSend){
				mFrameWindow->addChildWindow(res); 

			}
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7,0), CEGUI::UDim(0.15*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.15, 0)));
			mFrameWindow->addChildWindow(res);
			res->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick,
			CEGUI::Event::Subscriber(&DrawIntelegence::AskForTrade, this));
			mFrameWindow->addChildWindow(mCloseBtn); 	
		}
		res->setUserData(iterDemand->demandPnt);
		if(iterDemand->isTrade){
			res->setText("[colour='FF00FF00']" +iterDemand->demand+" for \n"+Logic::LogicStd::IntToString(iterDemand->amount)+" "+iterDemand->offer );
		
		}else{
			res->setText(iterDemand->demand+" for \n"+Logic::LogicStd::IntToString(iterDemand->amount)+" "+iterDemand->offer );
		
		}
		i++;
	}



	std::map<Ogre::String , Logic::BaseAction*>::iterator iterAct;
	i=0;
	for(iterAct=evnt->actions.begin();iterAct!= evnt->actions.end();++iterAct){
		name="IntelegenceSheet/actions"+Logic::LogicStd::IntToString(i);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			if(!evnt->isReSend){
				mFrameWindow->addChildWindow(res);
			}
		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2*i,0), CEGUI::UDim(0.5, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.15, 0)));

			mFrameWindow->addChildWindow(res);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawIntelegence::OpenIntActionDialog, this));

		}
		res->setText(iterAct->first);
		res->setUserData(iterAct->second);
		i++;

	}
	if(!evnt->isReSend){
		mMainWindow->addChildWindow(mFrameWindow);
		PriorIdDraw= true;
	}else{
		PriorIdDraw= false;
	}
}
void DrawIntelegence::DoDrawDialog(Logic::ActionIntDialog *evnt ){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *res;

	Ogre::String name;

	int x=0,y=0;
	name= "IntelegenceActionDialog";
	if(wmgr.isWindowPresent(name)){

		ClearFromAllChild(mDialog);


	}else{
		mDialog= wmgr.createWindow("TaharezLook/StaticImage", name);
		mDialog->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

	}
	if(wmgr.isWindowPresent("IntelegenceActionDialog/CloseDialog")){

		res= wmgr.getWindow("IntelegenceActionDialog/CloseDialog");
		mDialog->addChildWindow(res);
		res= wmgr.getWindow("IntelegenceActionDialog/Send");
		mDialog->addChildWindow(res);
	}else{
		res= wmgr.createWindow("TaharezLook/Button",  "IntelegenceActionDialog/CloseDialog");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Close");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&DrawIntelegence::DeclineIntActionDialog, this));
		mDialog->addChildWindow(res); 
		res= wmgr.createWindow("TaharezLook/Button",  "IntelegenceActionDialog/Send");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Send");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&DrawIntelegence::SendIntActionDialog, this));
		mDialog->addChildWindow(res); 
	}

	int i=0;
	std::map<Ogre::String,Logic::BaseCountry *>::iterator cntriter;
	for(cntriter=evnt->allCountry.begin();cntriter!= evnt->allCountry.end();++cntriter){
		name="IntelegenceActionDialog/targetcntry"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);

			mDialog->addChildWindow(res);
		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2*i,0), CEGUI::UDim(0.5, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.15, 0)));

			mDialog->addChildWindow(res);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawIntelegence::SetTargetIntActionDialog, this));

		}
		res->setUserData(cntriter->second);
		if(cntriter->second==evnt->selected){
			res->setText("[colour='FFFF0000']"+cntriter->first);
		}else{
			res->setText(cntriter->first);
		}

		i++;
	}

	if(evnt->canBeReason){
		i=0;
		for(cntriter=evnt->allCountry.begin();cntriter!= evnt->allCountry.end();++cntriter){
			name="IntelegenceActionDialog/reasoncntry"+Logic::LogicStd::IntToString(i);
			if(wmgr.isWindowPresent(name)){
				res= wmgr.getWindow(name);

				mDialog->addChildWindow(res);
			}else{
				res= wmgr.createWindow("TaharezLook/Button", name);
				res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2*i,0), CEGUI::UDim(0.8, 0)));
				res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.15, 0)));

				mDialog->addChildWindow(res);
				res->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&DrawIntelegence::SetReasonIntActionDialog, this));

			}
			res->setUserData(cntriter->second);
			if(cntriter->second==evnt->reason){
				res->setText("[colour='FFFF0000']"+cntriter->first);
			}else{
				res->setText(cntriter->first);
			}

			i++;
		}
	}
	mMainWindow->addChildWindow(mDialog);

}
void DrawIntelegence::CloseDialog(){
	if(mMainWindow->isChild("IntelegenceActionDialog")){
		mMainWindow->removeChildWindow("IntelegenceActionDialog");
	}
}
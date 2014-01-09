#include "DrawRecon.h"
using namespace GUI;

DrawRecon::DrawRecon(CEGUI::Window* mainWindow):BaseDraw(mainWindow)
{
}


DrawRecon::~DrawRecon(void)
{
}
void DrawRecon::IsNeedRefresh(){
	if(isOpen&&!mIsResend){
		Logic::ControlEvent *evnt = new Logic::ControlEvent();
		evnt->controlType	= Logic::CTE_SHOWRECON;
		evnt->isReSend = true;
		Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
		mIsResend= true;
		PriorIdDraw= false;
	}
}

bool DrawRecon::Launch(const CEGUI::EventArgs &e){
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType	= Logic::CTE_SETRECON;
	evnt->isReSend = true;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	return true;
}
void DrawRecon::DoDraw(Logic::ReconStateEvnt *evnt){

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
	Ogre::String name ="Recon/";
	Ogre::String text;

	if(wmgr.isWindowPresent("Recon")){
		if(!evnt->isReSend){
			ClearFromAllChild(mFrameWindow);
		}
		mCloseBtn= wmgr.getWindow( "Recon/Close");
		
		res= wmgr.getWindow( "Recon/LaunchRecon");



		if(!evnt->isReSend){
			mFrameWindow->addChildWindow(mCloseBtn); 
			mFrameWindow->addChildWindow(res); 

		}

	}else{
		mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "Recon");
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
		mFrameWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res= wmgr.createWindow("TaharezLook/Button",  "Recon/Close");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
		res->setText("Close");

		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close, (BaseDraw*)this));
		mFrameWindow->addChildWindow(res); 
		res= wmgr.createWindow("TaharezLook/Button",  "Recon/LaunchRecon");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));

		res->setText("Launch Recon");
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawRecon::Launch,this));
		mFrameWindow->addChildWindow(res); 
	}
	std::list<Logic::ReconToSend>::iterator productionIt;
	int i =0;
		for(productionIt = evnt->mAllRecon.begin();productionIt != evnt->mAllRecon.end();++productionIt){
		name="Recon/recon"+Logic::LogicStd::IntToString(i);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			
			if(!evnt->isReSend){
				mFrameWindow->addChildWindow(res);
			}

		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.1*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0.1, 0)));
			mFrameWindow->addChildWindow(res);
			

		}
		i++;
		res->setText(productionIt->name+" : "+productionIt->country);
	}

	if(!evnt->isReSend){
		mMainWindow->addChildWindow(mFrameWindow);
		PriorIdDraw= true;
	}else{
		PriorIdDraw= false;
	}

}
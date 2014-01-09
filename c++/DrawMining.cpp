#include "DrawMining.h"
using namespace GUI;

DrawMining::DrawMining(CEGUI::Window* mainWindow):DrawBuilding(mainWindow)
{
}


DrawMining::~DrawMining(void)
{
}
bool DrawMining::SetDeposit(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::ResourseDeposits *build = static_cast<Logic::ResourseDeposits *>(args.window->getUserData());
	Logic::SetDeposit *evnt = new Logic::SetDeposit();
	evnt->building =mBuilding;
	evnt->deposit =build;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	mMainWindow->removeChildWindow( "MiningSheet");
	isOpen = false;
	return true;
}

void DrawMining::DoDraw(Logic::ShowMining *evnt){
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
	Ogre::String name ="MiningSheet/";
	Ogre::String text;
	mBuilding = evnt->target;
	if(wmgr.isWindowPresent("MiningSheet")){
		ClearFromAllChild(mFrameWindow);
		mCloseBtn= wmgr.getWindow( "MiningSheet/Close");
		mFrameWindow->addChildWindow(mCloseBtn); 
		res= wmgr.getWindow( "MiningSheet/DepositMain");
		mFrameWindow->addChildWindow(res); 
		
		res->setText(evnt->curDeposit);
		prog =static_cast<CEGUI::ProgressBar *>( wmgr.getWindow("MiningSheet/Progress"));

		mFrameWindow->addChildWindow(prog); 

	}else{
		mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "MiningSheet");
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
		mFrameWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0, 0)));
		res= wmgr.createWindow("TaharezLook/Button",  "MiningSheet/Close");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
		res->setText("Close");
		
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close, (BaseDraw*)this));
		mFrameWindow->addChildWindow(res); 
		res= wmgr.createWindow("TaharezLook/StaticText",  "MiningSheet/DepositMain");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
		
		res->setText(evnt->curDeposit);
		prog=  static_cast<CEGUI::ProgressBar *>(wmgr.createWindow("TaharezLook/ProgressBar", "MiningSheet/Progress"));
		prog->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6,0), CEGUI::UDim(0.0, 0)));
		prog->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
		mFrameWindow->addChildWindow(prog); 
		mFrameWindow->addChildWindow(res); 
	}
	prog->setProgress(evnt->procent);
	std::map<Ogre::String,Logic::ResourseDeposits*>::iterator depIt;


	int x=0,y=0,i=0;
	for(depIt = evnt->allDeposit.begin();depIt != evnt->allDeposit.end();++depIt){
		name="MiningSheet/depos"+Logic::LogicStd::IntToString(i);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			res->setUserData(depIt->second);
			mFrameWindow->addChildWindow(res);

		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3*x,0), CEGUI::UDim(0.1*(y+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
			res->setUserData(depIt->second);
			mFrameWindow->addChildWindow(res);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawMining::SetDeposit, this));

		}
		if(++x>2){
			y++;
			x=0;
		}
		i++;
		res->setText(depIt->first);
	}	
	if(!evnt->isReSend){
	mMainWindow->addChildWindow(mFrameWindow);
	PriorIdDraw= true;
	}else{
	PriorIdDraw= false;
	}
}
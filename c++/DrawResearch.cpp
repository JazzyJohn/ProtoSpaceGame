#include "DrawResearch.h"

using namespace GUI;
DrawResearch::DrawResearch(CEGUI::Window* mainWindow):BaseDraw(mainWindow)
{
}


DrawResearch::~DrawResearch(void)
{
}
bool DrawResearch::SetResearch(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);

	Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
	sendEvnt->target =args.window->getUserData();
	sendEvnt->controlType = Logic::CTE_SETRESEARCH;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawResearch::AskForAllResourse(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::ControlEvent *evnt= new   Logic::ControlEvent();
	evnt->controlType = Logic::CTE_ASKDETAILEDRESOURSERESEARCH;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	return true;
}
bool DrawResearch::UseResource(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
			Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
			sendEvnt->target =args.window->getUserData();
			sendEvnt->controlType = Logic::CTE_USERESOURCEONRESEARCH;
			Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	

	mMainWindow->removeChildWindow( "DetailedRes");
	return true;
}
void DrawResearch::IsNeedRefresh(){
	if(isOpen&&!mIsResend){
		Logic::ControlEvent *evnt = new Logic::ControlEvent();
		evnt->controlType =Logic::CTE_ASKFORRESEARCH;
		evnt->isReSend = true;
			Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
			mIsResend= true;
			PriorIdDraw= false;
	}
}
void DrawResearch::DoDraw(Logic::ShowResearchCenter *evnt){
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

	Ogre::String name ="ResearchScreen/";
	Ogre::String text;


	if(wmgr.isWindowPresent("ResearchScreen")){
	

		res= wmgr.getWindow("ResearchScreen/CurResearch");

	}else{
		mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "ResearchScreen");
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0.6, 0)));
		mCloseBtn= wmgr.createWindow("TaharezLook/Button",  "ResearchScreen/Close");
		mCloseBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		mCloseBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		mCloseBtn->setText("Close");
		mCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close, (BaseDraw*)this));
		mFrameWindow->addChildWindow(mCloseBtn); 
		res= wmgr.createWindow("TaharezLook/Button",  "ResearchScreen/AddResToResearch");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2,0), CEGUI::UDim(0.05, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
		res->setText("Use Resourse");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&DrawResearch::AskForAllResourse, this));
		mFrameWindow->addChildWindow(res);
		res= wmgr.createWindow("TaharezLook/Button",  "ResearchScreen/CurResearch");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));

		mFrameWindow->addChildWindow(res);

	}
	res->setText(evnt->curResearch);

	std::map<Ogre::String, int>::iterator itrProg;
	int i=0;
	for(itrProg = evnt->progress.begin();itrProg!= evnt->progress.end();++itrProg){
		name="ResearchScreen/Progress"+Logic::LogicStd::IntToString(i);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);


		}else{
			res= wmgr.createWindow("TaharezLook/Titlebar", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05*(i+3), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.05, 0)));

			mFrameWindow->addChildWindow(res);


		}	
		res->setText(Logic::LogicStd::IntToString(itrProg->second)+" / "+Logic::LogicStd::IntToString(evnt->doneProgress[itrProg->first]));
		++i;
	}

	std::map<Ogre::String, Logic::BaseResearch*>::iterator nextItr;
	int j= 0;
	for(nextItr = evnt->nextLvl.begin();nextItr!= evnt->nextLvl.end();++nextItr){
		name="ResearchScreen/nextLvl"+Logic::LogicStd::IntToString(j);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05*j,0), CEGUI::UDim(0.05*(i+4), 0)));

		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05*j,0), CEGUI::UDim(0.05*(i+4), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.2, 0)));
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawResearch::SetResearch, this));

			mFrameWindow->addChildWindow(res);
		}
		res->setUserData(nextItr->second);
		res->setText(nextItr->first);
		++j;
	}
	if(!evnt->isReSend){
	mMainWindow->addChildWindow(mFrameWindow);
	PriorIdDraw= true;
	}else{
	PriorIdDraw= false;
	}

}
void DrawResearch::DrawResourceDialog(Logic::ShowAllResourse *resevnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *res;
	
	Ogre::String name= "DetailedRes";

	int x=0,y=0;

	if(wmgr.isWindowPresent(name)){
		
		ClearFromAllChild(mDialog);

	}else{
		mDialog= wmgr.createWindow("TaharezLook/StaticImage", name);
		mDialog->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
			mDialog->setAlwaysOnTop(true);
	}

	if(wmgr.isWindowPresent("DetailedRes/Close")){
		res= wmgr.getWindow("DetailedRes/Close");
		mDialog->addChildWindow(res);
	}else{
		res= wmgr.createWindow("TaharezLook/Button",  "DetailedRes/Close");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Close");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close,(BaseDraw*) this));
		mDialog->addChildWindow(res); 
	}
	int i=0;
	std::map<Ogre::String,int>::iterator itr;	
	for(itr = resevnt->amount.begin(); itr != resevnt->amount.end();++itr){
		name= "DetailedRes/NameAnd_Amount"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			mDialog->addChildWindow(res); 
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.05, 0)));
			mDialog->addChildWindow(res);


		}
		res->setText(itr->first +":" +Logic::LogicStd::IntToString(itr->second));

		
			name= "DetailedRes/UseBtn"+Logic::LogicStd::IntToString(i);
			if(wmgr.isWindowPresent(name)){
				res= wmgr.getWindow(name);
				mDialog->addChildWindow(res);
			}else{
				res= wmgr.createWindow("TaharezLook/Button", name);
				res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0.05*(i+1), 0)));
				res->setSize(CEGUI::UVector2(CEGUI::UDim(0.16, 0), CEGUI::UDim(0.05, 0)));
				mDialog->addChildWindow(res);
				res->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&DrawResearch::UseResource, this));
				res->setText("use");
			}
			res->setUserData(resevnt->links[itr->first]);
		
		i++;
	}
	
	for(itr = resevnt->intAmount.begin(); itr != resevnt->intAmount.end();++itr){
		name= "DetailedRes/NameAnd_Amount"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			mDialog->addChildWindow(res); 
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.05, 0)));
			mDialog->addChildWindow(res);


		}
		res->setText(itr->first +":" +Logic::LogicStd::IntToString(itr->second));

		
			name= "DetailedRes/UseBtn"+Logic::LogicStd::IntToString(i);
			if(wmgr.isWindowPresent(name)){
				res= wmgr.getWindow(name);
				mDialog->addChildWindow(res);
			}else{
				res= wmgr.createWindow("TaharezLook/Button", name);
				res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0.05*(i+1), 0)));
				res->setSize(CEGUI::UVector2(CEGUI::UDim(0.16, 0), CEGUI::UDim(0.05, 0)));
				mDialog->addChildWindow(res);
				res->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&DrawResearch::UseResource, this));
				res->setText("use");
			}
			res->setUserData(resevnt->intLinks[itr->first]);
		
		i++;
	}
	mMainWindow->addChildWindow(mDialog);


}
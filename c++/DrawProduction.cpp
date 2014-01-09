#include "DrawProduction.h"
using namespace GUI;

DrawProduction::DrawProduction(CEGUI::Window* mainWindow):DrawBuilding(mainWindow)
{
}


DrawProduction::~DrawProduction(void)
{
}
bool DrawProduction::SetProduction(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);

	Logic::ProductSet *sendEvnt = new Logic::ProductSet();
	sendEvnt->production =static_cast <Logic::Production*>(args.window->getUserData());
	sendEvnt->building =mBuilding;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	
	return true;
}
void DrawProduction::DoDraw(Logic::ShowProduction *evnt){

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
	Ogre::String name ="Productoion/";
	Ogre::String text;
	mBuilding = evnt->target;
	if(wmgr.isWindowPresent("Productoion")){
		if(!evnt->isReSend){
		ClearFromAllChild(mFrameWindow);
		}
		mCloseBtn= wmgr.getWindow( "Productoion/Close");
		mFrameWindow->addChildWindow(mCloseBtn); 
		res= wmgr.getWindow( "Productoion/DepositMain");
		
		
		res->setText(evnt->curProduction);
		prog =static_cast<CEGUI::ProgressBar *>( wmgr.getWindow("Productoion/Progress"));
if(!evnt->isReSend){
		mFrameWindow->addChildWindow(res); 
		mFrameWindow->addChildWindow(prog); 
}

	}else{
		mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "Productoion");
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
		mFrameWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0.5, 0)));
		res= wmgr.createWindow("TaharezLook/Button",  "Productoion/Close");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
		res->setText("Close");
		
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close, (BaseDraw*)this));
		mFrameWindow->addChildWindow(res); 
		res= wmgr.createWindow("TaharezLook/StaticText",  "Productoion/DepositMain");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
		
		res->setText(evnt->curProduction);
		prog=  static_cast<CEGUI::ProgressBar *>(wmgr.createWindow("TaharezLook/ProgressBar", "Productoion/Progress"));
		prog->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6,0), CEGUI::UDim(0.0, 0)));
		prog->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
		mFrameWindow->addChildWindow(prog); 
		mFrameWindow->addChildWindow(res); 
	}
	prog->setProgress(evnt->procent);
	std::list<Logic::SendProductionVar>::iterator productionIt;


	int i=0;
	for(productionIt = evnt->mProductionVar.begin();productionIt != evnt->mProductionVar.end();++productionIt){
		name="Productoion/depos"+Logic::LogicStd::IntToString(i);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			res->setUserData(productionIt->fromPtn);
			if(!evnt->isReSend){
				mFrameWindow->addChildWindow(res);
			}

		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.1*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0.1, 0)));
			res->setUserData(productionIt->fromPtn);
			mFrameWindow->addChildWindow(res);
			res->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick,
				CEGUI::Event::Subscriber(&DrawProduction::SetProduction, this));

		}
		
		i++;
		if(productionIt->refine){
		name = "refine: " + productionIt->from;
	
		}else{
			std::map<Ogre::String,int>::iterator recpt;
				name = "product: " +productionIt->to +" from ";
			for(recpt= productionIt->recept.begin();recpt!= productionIt->recept.end();++recpt){
					name+=Logic::LogicStd::IntToString(recpt->second) +" "+ recpt->first +" ";
			}
		}
		if(productionIt->active){
			name = "[colour='FF00FF00']"  +name;
		}
		res->setText(name);
	}	
	if(!evnt->isReSend){
	mMainWindow->addChildWindow(mFrameWindow);
	PriorIdDraw= true;
	}else{
	PriorIdDraw= false;
	}

}

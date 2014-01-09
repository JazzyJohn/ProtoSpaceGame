#include "DrawAbility.h"

using namespace GUI;
DrawAbility *DrawAbility::sigleton=0;
DrawAbility::DrawAbility(CEGUI::Window* mainWindow):BaseDraw(mainWindow){
	mOpenBy=0;
	mTarget=0;
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "PopUpAbility");
	mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0, 0)));
	mFrameWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
	mFrameWindow->setAlwaysOnTop(true);
	sigleton= this;
}
DrawAbility::~DrawAbility(void){

}
DrawAbility *DrawAbility::GetSigleton(){
	return sigleton;
}
void DrawAbility::DoDraw(void* target,std::map<Ogre::String,Logic::BaseAbility*> abilitys,void* open){
	CEGUI::UVector2 point =CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0));
	DoDraw(target,abilitys,point,open);
}
void DrawAbility::DoDraw(void* target,std::map<Ogre::String,Logic::BaseAbility*> abilitys,CEGUI::UVector2 popup_pos,void* open){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	mTarget= target;
	
	bool totalReDraw= false;
	CEGUI::Window *res;
	Ogre::String name ="PopUpAbility/";
	if(mOpenBy!=open){
		totalReDraw = true;
		if(mOpenBy!=0){
			static_cast <BaseDraw*>(mOpenBy)->ClosePopUp();
		}
		
	}
	mOpenBy=open;
	std::map<Ogre::String,Logic::BaseAbility*>::iterator abItr;
	if(totalReDraw){
		ClearFromAllChild(mFrameWindow);

		mFrameWindow->setPosition(popup_pos);
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.05*abilitys.size(), 0)));
	}
	int i=0;
	float size =1;
	if(abilitys.size()!=0){
		 size =1/ abilitys.size();

	}
	for(abItr =abilitys.begin();abItr!=abilitys.end();++abItr ){
		name= "PopUpAbility/Ability#"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			if(totalReDraw){
				mFrameWindow->addChildWindow(res);
			}
		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(size*(i), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(size, 0)));
			
			mFrameWindow->addChildWindow(res);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawAbility::AbilityClick, this));

		}
			res->setUserData(abItr->second);
			res->setText(abItr->first);
		i++;
		
	}
	if(totalReDraw){
		mMainWindow->addChildWindow(mFrameWindow);
	}
}
bool DrawAbility::AbilityClick(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);

	Logic::ActivateAbility *sendEvnt = new Logic::ActivateAbility();
	sendEvnt->ability =static_cast <Logic::BaseAbility*>(args.window->getUserData());
	sendEvnt->target=mTarget;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	
	return true;
}
bool DrawAbility::Close(void* open){
	if(open==mOpenBy){
		mOpenBy=0;
		if(mMainWindow->isChild(mFrameWindow)){

		mMainWindow->removeChildWindow(mFrameWindow);

	}
		return true;
	}
	return false;
}
	
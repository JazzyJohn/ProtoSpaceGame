#include "BaseGUI.h"
#include "DrawMining.h"
#include "DrawBase.h"
#include "DrawWarehouse.h"
#include "DrawMilitary.h"
#include "DrawResource.h"
#include "DrawIntelegence.h"
#include "DrawResearch.h"
#include "DrawRequests.h"
#include "DrawProduction.h"
#include "DrawWarfare.h"
#include "DrawRecon.h"
#include "DrawAbility.h"
using namespace GUI;
void GUI_cache::makeMeForm(GUI_cache*cache){
	mTarget= cache->mTarget;
	mType= cache->mType;

}
BaseGUI::BaseGUI(void)
{

}


BaseGUI::~BaseGUI(void)
{
}
bool BaseGUI::ClearUserData(const CEGUI::EventArgs &e){
	return true;
}
bool BaseGUI::Ignore(const CEGUI::EventArgs &e){
	return false;
}
void BaseGUI::ShowTitle(Ogre::String trg){
	mSheet->getChild("CEGUIDemo/Tooltip")->setText(trg);
}
void BaseGUI::Init(Logic::BaseLogic *logic){
	mLogic = logic;
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");

	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	mSheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
//	mSheet->subscribeEvent(CEGUI::DefaultWindow::EventMouseButtonDown,
//			CEGUI::Event::Subscriber(&BaseGUI::Ignore,this));
	mSheet->setMousePassThroughEnabled(true);
	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");

	CEGUI::System::getSingleton().setGUISheet(mSheet);
	mDateSheet = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/DateTime");
	mDateSheet->setText(mDate);
	mDateSheet->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0, 0)));

	mDateSheet->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
		quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/ResInfo");
	mSheet->addChildWindow(mDateSheet);

	quit->setText("ResourseInfo");
	quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.05, 0)));

	quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
	GUI_cache *ch = new GUI_cache();
	ch->mType= Logic::ROF_SIMPLESHOW;
	quit->setUserData(ch);
	quit->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&BaseGUI::AskForAllResourse, this));
	quit->subscribeEvent(CEGUI::Window::EventDestructionStarted,
		CEGUI::Event::Subscriber(&BaseGUI::ClearUserData, this));
	mSheet->addChildWindow(quit);
	CEGUI::System::getSingleton().setGUISheet(mSheet);

	quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/RequestInfo");
	quit->setText("RequestInfo");
	quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.1, 0)));

	quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
	quit->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&BaseGUI::AskForRequests, this));
	mSheet->addChildWindow(quit);
	quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/ReconInfo");
	quit->setText("Recon Info");
	quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.15, 0)));

	quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
	quit->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&BaseGUI::ShowRecon, this));
	mSheet->addChildWindow(quit);
		quit = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/Tooltip");
	quit->setText("");
	quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0.2, 0)));

	quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.05, 0)));
	
	mSheet->addChildWindow(quit);
	InitAllDraw();
	 CEGUI::System::getSingleton().setDefaultTooltip("TaharezLook/Tooltip" );
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType =Logic::CTE_SHOWRESOURCE;
	mLogic->fireGameEvent(evnt);
	

	
}
void BaseGUI::InitAllDraw(){
	mAllDraws["DrawBase"] = new GUI::DrawBase(mSheet);
	mAllDraws["DrawMining"] = new GUI::DrawMining(mSheet);
	mAllDraws["DrawWarehouse"] = new GUI::DrawWarehouse(mSheet);
	mAllDraws["DrawMilitary"] = new GUI::DrawMilitary(mSheet);
	mAllDraws["DrawResource"] = new GUI::DrawResource(mSheet);
	mAllDraws["DrawIntelegence"] = new GUI::DrawIntelegence(mSheet);
	mAllDraws["DrawResearch"] = new GUI::DrawResearch(mSheet);
	mAllDraws["DrawRequests"] = new GUI::DrawRequests(mSheet);
	mAllDraws["DrawProduction"] = new GUI::DrawProduction(mSheet);
	mAllDraws["DrawWarfare"] = new GUI::DrawWarfare(mSheet);
	mAllDraws["DrawRecon"] = new GUI::DrawRecon(mSheet);
	mAllDraws["DrawAbility"] = new GUI::DrawAbility(mSheet);
}

bool BaseGUI::AskForRequests(const CEGUI::EventArgs &e){
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType =Logic::CTE_SHOWREQUESTS;
	mLogic->fireGameEvent(evnt);
	return true;
}
bool BaseGUI::Close(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	if(mSheet->isChild(args.window->getParent()->getName())){

		mSheet->removeChildWindow(args.window->getParent()->getName());
	}
	return true;
}
bool BaseGUI::AskCountry(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);

	Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
	sendEvnt->target =args.window->getUserData();
	sendEvnt->controlType = Logic::CTE_ASKFORCOUNTRY;
	mLogic->fireGameEvent(sendEvnt);
	return true;
}

bool BaseGUI::BuildCore(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
	sendEvnt->target =args.window->getUserData();
	sendEvnt->controlType = Logic::CTE_BUILDCOREINCOUNTRY;
	mLogic->fireGameEvent(sendEvnt);
	return true;
}
void  BaseGUI::RefreshOpenWindow(){
	std::map<Ogre::String,BaseDraw *>::iterator iter;
	for(iter = mAllDraws.begin();iter!= mAllDraws.end();++iter){
		iter->second->IsNeedRefresh();
	}
}
bool BaseGUI::AskBuilding(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::BaseBuilding* build = static_cast<Logic::BaseBuilding*>(args.window->getUserData());
	Logic::ShowBuilding *evnt = new Logic::ShowBuilding();
	evnt->building =build;
	mLogic->fireGameEvent(evnt);
	//delete build;
	return true;
}
bool BaseGUI::MenuResume(const CEGUI::EventArgs &e){
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType =Logic::CTE_RESUMEGAME;
	mLogic->fireGameEvent(evnt);
	if(mSheet->isChild("PauseMenu")){
		mSheet->removeChildWindow("PauseMenu");
	}
	return true;
}
bool BaseGUI::MenuExit(const CEGUI::EventArgs &e){
	Logic::BaseListener::GetSigleton()->Exit();
	return true;
}
bool  BaseGUI::MenuSave(const CEGUI::EventArgs &e){
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType =Logic::CTE_SAVEGAME;
	mLogic->fireGameEvent(evnt);
	return true;
}
bool BaseGUI::AskForAllResourse(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	GUI_cache *cahce= static_cast<GUI_cache*>(args.window->getUserData());
	mSimpleCache.makeMeForm(cahce);
	Logic::ControlEvent *evnt= new   Logic::ControlEvent();
	evnt->controlType = Logic::CTE_ASKDETAILEDRESOURSE;
	mLogic->fireGameEvent(evnt);
	return true;
}
bool BaseGUI::ShowRecon(const CEGUI::EventArgs &e){
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType	= Logic::CTE_SHOWRECON;
	
	Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	return true;
}

void BaseGUI::ClearFromAllChild(CEGUI::Window *win){

	while(win->getChildCount()>0){
		win->removeChildWindow(win->getChildAtIdx(0)->getName());

	}

}

void BaseGUI::DrawBuilding(Logic::BuildingBuiltEvnt *resevnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	static int i=0;
	Ogre::String name = "buildng_"+Logic::LogicStd::IntToString(resevnt->buildingType)+"_"+Logic::LogicStd::IntToString(i);
	CEGUI::Window *res;
	if(wmgr.isWindowPresent(name)){
		res= wmgr.getWindow(name);
	}else{
		res= wmgr.createWindow("TaharezLook/Button",name);
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2*(i), 0), CEGUI::UDim(0.5, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
		mSheet->addChildWindow(res);
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseGUI::AskBuilding, this));
	}
	;
	res->setText(name);

	res->setUserData(resevnt->building);
	i++;

}

void  BaseGUI::DrawAllResourse(Logic::ShowAllResourse *resevnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *res;
	CEGUI::Window *allresSheet;
	Ogre::String name;

	int x=0,y=0;

	if(wmgr.isWindowPresent(name)){
		allresSheet= wmgr.getWindow(name);
		ClearFromAllChild(allresSheet);

	}else{
		allresSheet= wmgr.createWindow("TaharezLook/StaticImage", name);
		allresSheet->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
			allresSheet->setAlwaysOnTop(true);
	}

	if(wmgr.isWindowPresent("DetailedRes/Close")){
		res= wmgr.getWindow("DetailedRes/Close");
		allresSheet->addChildWindow(res);
	}else{
		res= wmgr.createWindow("TaharezLook/Button",  "DetailedRes/Close");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Close");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseGUI::Close, this));
		allresSheet->addChildWindow(res); 
	}
	int i=0;
	std::map<Ogre::String,int>::iterator itr;	
	for(itr = resevnt->amount.begin(); itr != resevnt->amount.end();++itr){
		name= "DetailedRes/NameAnd_Amount"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			allresSheet->addChildWindow(res); 
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.05, 0)));
			allresSheet->addChildWindow(res);


		}
		res->setText(itr->first +":" +Logic::LogicStd::IntToString(itr->second));
		res->setTooltipText(resevnt->links[itr->first]->GetTooltip());
		
		i++;
	}
	for(itr = resevnt->intAmount.begin(); itr != resevnt->intAmount.end();++itr){
		name= "DetailedRes/NameAnd_Amount"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			allresSheet->addChildWindow(res); 
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.05, 0)));
			allresSheet->addChildWindow(res);


		}
		res->setText(itr->first +":" +Logic::LogicStd::IntToString(itr->second));
		res->setTooltipText(resevnt->intLinks[itr->first]->GetTooltip());
		
		i++;
	}
	mSheet->addChildWindow(allresSheet);


}

void BaseGUI::DrawResource(Logic::ShowResourceEvnt *resevnt){
	GUI::DrawResource * temp = static_cast<GUI::DrawResource*>(	mAllDraws["DrawResource"]);
	temp->DoDraw(resevnt);
}
void  BaseGUI::DrawBase(Logic::SendCoreToGui *evnt){
	GUI::DrawBase * temp = static_cast<GUI::DrawBase*>(	mAllDraws["DrawBase"]);
	temp->DoDraw(evnt);
}
void  BaseGUI::DrawBlueprint(Logic::SendBlueToGui *evnt){
	GUI::DrawBase * temp = static_cast<GUI::DrawBase*>(	mAllDraws["DrawBase"]);
	temp->DrawBlueprint(evnt);

}
void BaseGUI::DrawMining(Logic::ShowMining *evnt){
		
	GUI::DrawMining * temp = static_cast<GUI::DrawMining*>(	mAllDraws["DrawMining"]);
	temp->DoDraw(evnt);
}
void BaseGUI::DrawProduction(Logic::ShowProduction *evnt){
	GUI::DrawProduction * temp = static_cast<GUI::DrawProduction*>(	mAllDraws["DrawProduction"]);
	temp->DoDraw(evnt);
}
void BaseGUI::DrawWarehouse(Logic::ShowWarehouse *evnt){
	GUI::DrawWarehouse * temp = static_cast<GUI::DrawWarehouse*>(	mAllDraws["DrawWarehouse"]);
	temp->DoDraw(evnt);

}
void BaseGUI::DrawMilitaryBase(Logic::ShowMilitary *evnt){
	GUI::DrawMilitary * temp = static_cast<GUI::DrawMilitary*>(	mAllDraws["DrawMilitary"]);
	temp->DoDraw(evnt);
}
void  BaseGUI::DrawIntelegence(Logic::ShowIntelegence *evnt){
		GUI::DrawIntelegence * temp = static_cast<GUI::DrawIntelegence*>(	mAllDraws["DrawIntelegence"]);
	temp->DoDraw(evnt);
	
}
void  BaseGUI::DrawIntelegenceDialog(Logic::ActionIntDialog *evnt ){
		GUI::DrawIntelegence * temp = static_cast<GUI::DrawIntelegence*>(	mAllDraws["DrawIntelegence"]);
		temp->DoDrawDialog(evnt);

}
void  BaseGUI::CloseIntelegenceDialog(){
		GUI::DrawIntelegence * temp = static_cast<GUI::DrawIntelegence*>(	mAllDraws["DrawIntelegence"]);
		temp->CloseDialog();
}

void BaseGUI::DrawResearchScreen(Logic::ShowResearchCenter *evnt){
	GUI::DrawResearch * temp = static_cast<GUI::DrawResearch*>(	mAllDraws["DrawResearch"]);
		temp->DoDraw(evnt);
}
void BaseGUI::DrawAllResourseResearch(Logic::ShowAllResourse  *resevnt){

	GUI::DrawResearch * temp = static_cast<GUI::DrawResearch*>(	mAllDraws["DrawResearch"]);
		temp->DrawResourceDialog(resevnt);
}

void  BaseGUI::DrawAllRequest(Logic::ShowAllRequest *evnt){
	GUI::DrawRequests * temp = static_cast<GUI::DrawRequests*>(	mAllDraws["DrawRequests"]);
		temp->DoDraw(evnt);
}
void  BaseGUI::DrawAllResourse(Logic::ShowSendStatus *resevnt){
	GUI::DrawRequests * temp = static_cast<GUI::DrawRequests*>(	mAllDraws["DrawRequests"]);
	temp->DoDialogDraw(resevnt);

}
void  BaseGUI::DrawAllResourse(Logic::ShowProjectSendStatus *resevnt){
	GUI::DrawRequests * temp = static_cast<GUI::DrawRequests*>(	mAllDraws["DrawRequests"]);
	temp->DoDialogDraw(resevnt);

}
void BaseGUI::DrawUnitInBase(Logic::ShowUnitInBase *evnt){
	GUI::DrawMilitary * temp = static_cast<GUI::DrawMilitary*>(	mAllDraws["DrawMilitary"]);
	temp->DoDrawUnit(evnt);
}
void  BaseGUI::DrawBattlifield(Logic::ShowWarfare *evnt){
	GUI::DrawWarfare * temp = static_cast<GUI::DrawWarfare*>(	mAllDraws["DrawWarfare"]);
	temp->DoDraw(evnt);
}
void BaseGUI::CloseBattlifield(Logic::CloseWarfare *evnt){
	GUI::DrawWarfare * temp = static_cast<GUI::DrawWarfare*>(	mAllDraws["DrawWarfare"]);
	temp->Close(evnt);
}
void BaseGUI::DrawRecon(Logic::ReconStateEvnt *evnt){
	GUI::DrawRecon * temp = static_cast<GUI::DrawRecon*>(	mAllDraws["DrawRecon"]);
	temp->DoDraw(evnt);
}

void  BaseGUI::ClearUnitDialog(){
	if(mSheet->isChild("UnitDialog")){
		mSheet->removeChildWindow("UnitDialog");
	}
}
void BaseGUI::ClearResourseDialog(){
	if(mSheet->isChild("DetailedRes")){
		mSheet->removeChildWindow("DetailedRes");
	}
	if(mSheet->isChild("DetailedResReq")){
		mSheet->removeChildWindow("DetailedResReq");
	}
	if(mSheet->isChild("DetailedResProj")){
		mSheet->removeChildWindow("DetailedResProj");
	}
}
void BaseGUI::ClearDialog(Ogre::String Name){
	if(mSheet->isChild(Name)){
		mSheet->removeChildWindow(Name);
	}
	
}
void BaseGUI::DrawPlanet(Logic::PlanetJustGenerateEvent *evnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	std::map<Ogre::String,Logic::BaseCountry*>::iterator the_iterator;
	CEGUI::Window *res;
	Ogre::String name ="CountryBtn/";
	int i=0;
	int x =0,y=0;
	for(the_iterator = evnt->country.begin();the_iterator != evnt->country.end();++the_iterator){
		name="CountryBtn/";
		name +=the_iterator->second->mName;
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			res->setUserData(the_iterator->second);

		}else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8+0.1*x,0), CEGUI::UDim(0.5+0.05*(y+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
			res->setUserData(the_iterator->second);
			mSheet->addChildWindow(res);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&BaseGUI::AskCountry, this));

		}
		i++;
		if(++x>=2){
		y++;
		x=0;
		}
		
		res->setText("[colour='"+Logic::LogicStd::IntToString(the_iterator->second->mColor.getAsARGB())+"']"+ the_iterator->second->mName);
	
	}

}
void  BaseGUI::DrawCountry(Logic::SendCountry *evnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	std::map<Logic::CountryCharacteristic *,Ogre::String>::iterator charIt;
	CEGUI::Window *res;
	Ogre::String name ="CountrySheet/";
	Ogre::String text;
	CEGUI::Window *countSheet;
	if(wmgr.isWindowPresent( "CountrySheet")){
		countSheet= wmgr.getWindow( "CountrySheet");
		res= wmgr.getWindow("CountrySheet/BuildCore");
		res->setUserData(evnt->contry);
	}else{
		countSheet= wmgr.createWindow("TaharezLook/StaticImage", "CountrySheet");
		countSheet->setSize(CEGUI::UVector2(CEGUI::UDim(1,0), CEGUI::UDim(1, 0)));
		countSheet->setAlwaysOnTop(true);
		res= wmgr.createWindow("TaharezLook/Button",  "CountrySheet/Close");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Close");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseGUI::Close, this));
		countSheet->addChildWindow(res); 
		res= wmgr.createWindow("TaharezLook/Button",  "CountrySheet/BuildCore");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.2, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseGUI::BuildCore, this));
		countSheet->addChildWindow(res); 
		res->setText("BuildCore");
		res->setUserData(evnt->contry);
	}
	int i=0;
	for(charIt = evnt->allChar.begin();charIt != evnt->allChar.end();++charIt){
	
		name = "CountryBtn/Char_" ;name +=charIt->first->name;
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2,0), CEGUI::UDim(0.05*i+0.2, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.05, 0)));
			countSheet->addChildWindow(res);
		}
		text= charIt->first->name;
		if(evnt->deltaChar.count(charIt->first)>0){
			if(evnt->deltaChar[charIt->first]){
				text ="[colour='FF00FF00']" +text;
			}else{
				text ="[colour='FFFF0000']" +text;
			}
		}
		if(charIt->first->type==Logic::CT_DESCMANY){
			res->setText(text +":" + charIt->second+"K");
		}else{
			res->setText(text +":" + charIt->second);
		}
		i++;
	}
	std::map<Ogre::String,int>::iterator relIt;
	std::map<Ogre::String,Logic::CountryRelationStatus>::iterator relStIt;
	i=0;
	relStIt= evnt->RealtionStatus.begin();
	for(relIt = evnt->Realtion.begin();relIt != evnt->Realtion.end();++relIt){
		name = "CountryBtn/Rel_" +  Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6,0), CEGUI::UDim(0.05*i+0.2, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
			countSheet->addChildWindow(res);
		}
		text= relIt->first;
		res->setText(text +":" + Logic::LogicStd::IntToString(relIt->second));

		name = "CountryBtn/RelStat_"  +  Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8,0), CEGUI::UDim(0.05*i+0.2, 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
			countSheet->addChildWindow(res);
		}
		text= relIt->first;
		switch(relStIt->second){
		case Logic::CRS_NEUTRAL:
			res->setText("neutral");
			break;
		case Logic::CRS_TRADE:
			res->setText("trade");
			break;
		case Logic::CRS_WAR:
			res->setText("war");
			break;
		case Logic::CRS_ALLIANCE:
			res->setText("alliance");
			break;
		case Logic::CRS_COLDWAR:
			res->setText("cold war");
			break;

		}

		i++;
		++relStIt;
	}
	mSheet->addChildWindow(countSheet);

}


void BaseGUI::AdNotify(Logic::GameEvent *evnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	Logic::NotificationEvent *notevnt= static_cast<Logic::NotificationEvent*>(evnt);
	static int i =0;
	CEGUI::Listbox *listbox; 
	CEGUI::ListboxTextItem *temp; 
	static  CEGUI::ListboxTextItem *lisitem; 
	if(wmgr.isWindowPresent("NotifySheet")){
		listbox= static_cast<CEGUI::Listbox *>(wmgr.getWindow("NotifySheet"));
	}else{	
		listbox= static_cast<CEGUI::Listbox *>( wmgr.createWindow("TaharezLook/Listbox", "NotifySheet"));
		listbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.8, 0)));
		listbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
		mSheet->addChildWindow(listbox);
	}
	Ogre::String name = "NotifySheet/Item"+Logic::LogicStd::IntToString( i);
	temp= new CEGUI::ListboxTextItem(name);

	name="";
	switch(notevnt->notType){
	case	Logic::NTE_BUILDINGIDLE:
		name = "Building "+notevnt->name+" is idle ";
		break;
	case	Logic::NTE_REQUESTEEXPIRATE:
		name = "Request "+notevnt->name+" as expiret";
		break;
	case	Logic::NTE_RESEARCHCOMPLITE:
		name = "Research "+notevnt->name+" complited";
		break;
	case Logic::NTE_EMBASSYCOMPLITE:
		name = notevnt->name;
		break;
	case Logic::NTE_BATTLEOVER:
		name = "Battle for country " +notevnt->name+"over";
		break;

	}
	temp->setText(name);
	if(i==0){
		listbox->addItem(temp);
	}else{
		listbox->insertItem(temp,lisitem);

	}
	lisitem= temp;
	i++;
}
void BaseGUI::AddNewNews(Logic::NewNews *evnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	static int i =0;
	CEGUI::Listbox *listbox; 
	CEGUI::ListboxTextItem *temp; 
	static  CEGUI::ListboxTextItem *lisitem; 
	if(wmgr.isWindowPresent("NewsSheet")){
		listbox= static_cast<CEGUI::Listbox *>(wmgr.getWindow("NewsSheet"));
	}else{	
		listbox= static_cast<CEGUI::Listbox *>( wmgr.createWindow("TaharezLook/Listbox", "NewsSheet"));
		listbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0.8, 0)));
		listbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
		mSheet->addChildWindow(listbox);
	}
	Ogre::String name = "NewsSheet/Item"+Logic::LogicStd::IntToString( i);
	temp= new CEGUI::ListboxTextItem(name);
	temp->setText(evnt->depname+":"+evnt->descr);
	if(i==0){
		listbox->addItem(temp);
	}else{
		listbox->insertItem(temp,lisitem);

	}
	lisitem= temp;
	i++;
}
void BaseGUI::ShowMenu(){
		Logic::ControlEvent *evnt= new   Logic::ControlEvent();
		evnt->controlType = Logic::CTE_PAUSEGAME;
		mLogic->fireGameEvent(evnt);
		CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
		CEGUI::Window *res, *parent;
		Ogre::String name ="PauseMenu";
		if(wmgr.isWindowPresent(name)){
			mSheet->addChildWindow(wmgr.getWindow(name));
		}else{
			parent= wmgr.createWindow("DefaultWindow", name);
			parent->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
			res= wmgr.createWindow("TaharezLook/StaticText", name+"/back");
			res->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
			res->setAlpha(0.5);

			parent->addChildWindow(res);
			res= wmgr.createWindow("TaharezLook/Button", name+ "Resume");
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
			res->setText("Resume");
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&BaseGUI::MenuResume, this));
			res->setAlwaysOnTop(true);
			parent->addChildWindow(res);
			res= wmgr.createWindow("TaharezLook/Button", name+ "Save");
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&BaseGUI::MenuSave, this));
				res->setAlwaysOnTop(true);
			res->setText("Save");
			parent->addChildWindow(res);
			res= wmgr.createWindow("TaharezLook/Button", name+ "Exit");
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.4, 0)));
			res->setText("Exit");
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&BaseGUI::MenuExit, this));
				res->setAlwaysOnTop(true);
			parent->addChildWindow(res);
			res= wmgr.createWindow("TaharezLook/Button", name+ "Load");
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.3, 0)));
			res->setText("Load");
				res->setAlwaysOnTop(true);
			parent->addChildWindow(res);
		}
}
void BaseGUI::Rendering(float deltaTime){
	CEGUI::System::getSingleton().injectTimePulse(deltaTime);
	mDateSheet->setText(mDate);
}
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
		break;

	case OIS::MB_Right:
		return CEGUI::RightButton;
		break;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
		break;

	default:
		return CEGUI::LeftButton;
		break;
	}
}

bool BaseGUI::mouseMoved( const OIS::MouseEvent &arg )
{
	if(CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel)) return true;
	return false;
}

bool BaseGUI::mousePressed(  OIS::MouseButtonID id )
{
	if(CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id))) return true;
	return false;
}

bool BaseGUI::mouseReleased( OIS::MouseButtonID id )
{
	if(CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id))) return true;
	return false;
}

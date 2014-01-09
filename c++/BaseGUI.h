#pragma once
#include <CEGUI.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "BaseLogic.h"
#include "BaseDraw.h"
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
namespace GUI{
	class GUI_cache{
	public:
		GUI_cache(){};
		~GUI_cache(){};
		void makeMeForm(GUI_cache*cache);
		Logic::ReseourseOpenFrom mType;
		void *mTarget;
	};
class BaseGUI 
{
public:
	BaseGUI(void);
	~BaseGUI(void);
	void Init(Logic::BaseLogic *logic);
	CEGUI::OgreRenderer* mRenderer;
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed(  OIS::MouseButtonID id );
 	bool mouseReleased( OIS::MouseButtonID id );
	void Rendering(float deltaTime);
	
	bool AskBuilding(const CEGUI::EventArgs &e);
	
	bool ClearUserData(const CEGUI::EventArgs &e);
	bool Close(const CEGUI::EventArgs &e);
	bool AskCountry(const CEGUI::EventArgs &e);
	bool BuildCore(const CEGUI::EventArgs &e);
	bool ShowRecon(const CEGUI::EventArgs &e);
	bool AskForAllResourse(const CEGUI::EventArgs &e);

	bool AskForRequests(const CEGUI::EventArgs &e);
	bool MenuResume(const CEGUI::EventArgs &e);
	bool MenuExit(const CEGUI::EventArgs &e);
	bool MenuSave(const CEGUI::EventArgs &e);
	bool Ignore(const CEGUI::EventArgs &e);
	void ShowMenu();
	void DrawResource(Logic::ShowResourceEvnt *resevnt);
	void DrawAllResourse(Logic::ShowAllResourse *resevnt);
	void DrawAllResourseResearch(Logic::ShowAllResourse *resevnt);
	void DrawAllResourse(Logic::ShowSendStatus *resevnt);
	void DrawAllResourse(Logic::ShowProjectSendStatus *resevnt);
	void ClearResourseDialog();
	void AdNotify(Logic::GameEvent *evnt);
	void DrawBuilding(Logic::BuildingBuiltEvnt *resevnt);
	void DrawBase(Logic::SendCoreToGui *evnt);
	void DrawPlanet(Logic::PlanetJustGenerateEvent *evnt);
	void ClearFromAllChild(CEGUI::Window *win);
	void DrawBlueprint(Logic::SendBlueToGui *evnt);
	void DrawCountry(Logic::SendCountry *evnt);
	void DrawMining(Logic::ShowMining *evnt);
	void DrawProduction(Logic::ShowProduction *evnt);
	void DrawWarehouse(Logic::ShowWarehouse *evnt);
	void DrawResearchScreen(Logic::ShowResearchCenter *evnt);
	void DrawMilitaryBase(Logic::ShowMilitary *evnt);
	void DrawAllRequest(Logic::ShowAllRequest *evnt);
	void DrawIntelegence(Logic::ShowIntelegence *evnt);
	void DrawIntelegenceDialog(Logic::ActionIntDialog *evnt );
	void DrawUnitInBase(Logic::ShowUnitInBase *evnt);
	void DrawBattlifield(Logic::ShowWarfare *evnt);
	void DrawRecon(Logic::ReconStateEvnt *evnt);
	void ShowTitle(Ogre::String);
	void CloseBattlifield(Logic::CloseWarfare *evnt);
	void ClearDialog(Ogre::String Name);
	void CloseIntelegenceDialog();
	void ClearUnitDialog();
	void AddNewNews(Logic::NewNews *evnt);
	Logic::BaseLogic *mLogic;
	void InitAllDraw();
	void RefreshOpenWindow();
	Ogre::String mDate;
protected:
	
	GUI_cache mSimpleCache;
	CEGUI::Window *mSheet;
	CEGUI::Window *mDateSheet;
	std::map<Ogre::String,BaseDraw *> mAllDraws;
		
	Logic::BuildingForSent *mBuildCache;
	Logic::BaseBuilding *mBuildingCache;
};
}


#include "TestGame.h" 
#include "ClickHelper.h"
#include <OgreRoot.h>
#include <OgreManualObject.h>
void TestGame::MoveCamTo(Ogre::String name){
	
	if(!mSceneMgr-> 	hasSceneNode("CamPoint_"+name)){
		return;
	}
	mCamera->getParentSceneNode()->detachObject(mCamera);
    Ogre::SceneNode *camNode = mSceneMgr->getSceneNode("CamPoint_"+name);
	camNode->attachObject(mCamera);

}
void TestGame::ObjectClick(Ogre::RaySceneQueryResult &result,int mouse){
		Ogre::String name;
				Ogre::Any any;
				for(Ogre::RaySceneQueryResult::iterator itr = result.begin( );itr!=result.end();itr++){
					if(itr->movable!=0){
						any = itr->movable->getUserAny();
						if(any.getType()==typeid(Control::ClickHelper*)){
								Control::ClickHelper* helper   = Ogre::any_cast<Control::ClickHelper*>(itr->movable->getUserAny());
								switch(helper->mType){
								case Logic::CHT_BUILDING:
									{
										Logic::ShowBuilding *evnt = new Logic::ShowBuilding();
										evnt->building = static_cast<Logic::BaseBuilding*>(helper->target);
										if(mouse==OIS::MB_Left){
										mGameLogic->fireGameEvent(evnt);
										}
										return;
									}
									break;
								case Logic::CHT_COUNTRY:
									{
										Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
										sendEvnt->target = helper->target;
										sendEvnt->controlType = Logic::CTE_ASKFORCOUNTRY;
										if(mouse==OIS::MB_Left){
											mGameLogic->fireGameEvent(sendEvnt);
										}
										if(mouse==OIS::MB_Right){
											
											mGui.ShowTitle(static_cast<Logic::BaseCountry*>(helper->target)->mName);
										}
										return;
									}
									break;
								case Logic::CHT_EMPTYSPACE:
									{
										Logic::ShowBlueprint *evnt = new Logic::ShowBlueprint();
										evnt->core = static_cast<Logic::BaseCore*>(helper->target);
										if(mouse==OIS::MB_Left){
										Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
										}
									}
									break;
								
								}
								
								return;
						}
						
					}
				
				}
 


}
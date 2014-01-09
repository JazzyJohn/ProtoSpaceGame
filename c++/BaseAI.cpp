#include "BaseAI.h"
using namespace Logic;
 BaseAIManager* BaseAIManager::sigleton=0;

 BaseAIManager::BaseAIManager(void){
	sigleton =this;
 }
 BaseAIManager::~BaseAIManager(void){
	 std::map<Ogre::String,BaseAINode*>::iterator aiItr;
	 for(aiItr = mAllAi.begin();aiItr!=mAllAi.end();aiItr++){
		 delete aiItr->second;
	 
	 }
 }
void BaseAIManager::SetMap(std::map<Ogre::String,BaseAINode*> allAi){
	mAllAi = allAi;

}
BaseAINode* BaseAIManager::GetAINode(Ogre::String name){
	
	return mAllAi[name];
}
void BaseAIManager::SetPlanet(BasePlanet* Planet){
mPlanet = Planet;
}
BaseAIManager* BaseAIManager::GetSigleton(){
	return sigleton;
}
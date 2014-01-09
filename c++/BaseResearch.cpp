#include "BaseResearch.h"
#include "BaseLogic.h"
using namespace Logic;
int ResearchDepartment::mLvlNum =0;
BaseResearch::BaseResearch(void){
	mName="NullName";
}
BaseResearch::BaseResearch(Ogre::String name,Ogre::String disname){
	mIsDone= false;
	mDisplayName= disname;
	mName = name;

}
BaseResearch::~BaseResearch(void)
{

}
void BaseResearch::CreateMap(std::map<BaseResource*,int> donePogress){
	std::map<BaseResource*,int>::const_iterator itr;
	for(itr = donePogress.begin(); itr != donePogress.end(); ++itr){
		mDonePogress[itr->first] =itr->second;	
		mPogress[itr->first] =0;	
	}

}

bool BaseResearch::addProgress(BaseResourceObject* nextObject){
	std::map<BaseResource*,int>::const_iterator itr;
	if(mIsDone)return mIsDone;
	for(itr = mDonePogress.begin(); itr != mDonePogress.end(); ++itr){
		mPogress[itr->first] +=nextObject->mResource[itr->first];	
	}
	IsDone();
	return mIsDone;
}
bool BaseResearch::addProgress(std::map<BaseResource*,int> AddingProgress){
	std::map<BaseResource*,int>::const_iterator itr;
	if(mIsDone)return mIsDone;
	for(itr = mDonePogress.begin(); itr != mDonePogress.end(); ++itr){
		mPogress[itr->first] +=AddingProgress[itr->first];	
	}
	IsDone();
	return mIsDone;
}
bool BaseResearch::addProgress(int progress){
	std::map<BaseResource*,int>::const_iterator itr;
	if(mIsDone)return mIsDone;
	for(itr = mDonePogress.begin(); itr != mDonePogress.end(); ++itr){
		mPogress[itr->first] +=progress;	
	}
	IsDone();
	return mIsDone;
}
void   BaseResearch::GetMap(std::map<Ogre::String, int> &progress,std::map<Ogre::String, int> &doneProgress){
	std::map<BaseResource*,int>::const_iterator itr;
	for(itr = mDonePogress.begin(); itr != mDonePogress.end(); ++itr){
		if(itr->second>0){
			if(mPogress[itr->first]<itr->second){
				progress[itr->first->mName]=mPogress[itr->first];
			}else{
				progress[itr->first->mName]=itr->second;
			}
			doneProgress[itr->first->mName] = itr->second;
		}
	}

}
Ogre::String BaseResearch::GetName(){
	return mDisplayName;
}
bool  BaseResearch::IsDone(){
	if(mIsDone) return true;
	std::map<BaseResource*,int>::const_iterator itr;
	bool isDone = true;
	for(itr = mDonePogress.begin(); itr != mDonePogress.end(); ++itr){
		isDone =isDone&&(mPogress[itr->first] >=mDonePogress[itr->first]);
		if(isDone)mPogress[itr->first]=mDonePogress[itr->first];
	}
	if(isDone==true&&mIsDone!=true){
		mIsDone= true;
		whenDone();
	}
	return mIsDone; 
}
void BaseResearch::whenDone(void){
	BaseLogic::GetSigleton()->mLuaMan.ResearchScripts(this);
}
ResearchLvl::ResearchLvl(int lvlNum, BaseResearch **researchOnThisLvl,int reseacrhCnt){
	mLvlNum= lvlNum;
	mResearchOnThisLvl= researchOnThisLvl;
	mReseacrhCnt= reseacrhCnt;
}
ResearchLvl::~ResearchLvl(void){
	for(int i=0;i<mReseacrhCnt;i++){
		delete mResearchOnThisLvl[i];
	}
	delete [] mResearchOnThisLvl;
}
bool ResearchLvl::hasIt(BaseResearch *research){
	for(int i=0;i<mReseacrhCnt;i++){
		if(mResearchOnThisLvl[i]==research){
			return true;
		}
	}
	return false;
}
ResearchDepartment::ResearchDepartment(){
	mCurLvlNum = -1;
}
bool ResearchDepartment::hasIt(BaseResearch *research){
	if(mCurLvlNum==-1){
	return mAllDepLvl[0]->hasIt(research);
	}else{
	return mAllDepLvl[mCurLvlNum]->hasIt(research);
	}
}
void ResearchDepartment::StartResearch(){
	if(mCurLvlNum=-1){
		mCurLvlNum=0;
	}
}
void ResearchDepartment::Init(std::vector<ResearchLvl*> allLvl){
	 mAllDepLvl = allLvl;
}
ResearchDepartment::~ResearchDepartment(){
	std::vector<ResearchLvl*>::iterator lvlIt;
	for(lvlIt=mAllDepLvl.begin();lvlIt!= mAllDepLvl.end();++lvlIt){

		delete *lvlIt;
	}
}
void ResearchDepartment::ShowResearch(std::map<Ogre::String, BaseResearch*> &map){
	if(mIsBlock==false){
		return;
	}
	int lvlForReturn=mCurLvlNum+1;
	for(int i=0;i<mAllDepLvl[lvlForReturn]->mReseacrhCnt;i++){
		map[mAllDepLvl[lvlForReturn]->mResearchOnThisLvl[i]->GetName()]=mAllDepLvl[lvlForReturn]->mResearchOnThisLvl[i];
	}
}
BaseReasearchManager::BaseReasearchManager(){
	mCurResearch=0;

}
void BaseReasearchManager::Init(ResearchDepartment **allDep,int allDepNum){
	mAllDep = allDep;
	mAllDepNum = allDepNum;

	mIdle= true;
	mEvnt= true;
}
BaseReasearchManager::~BaseReasearchManager(void)
{
	for(int i=0;i<mAllDepNum;i++){
		delete mAllDep[i];
	}
	delete [] mAllDep;
}
bool BaseReasearchManager::addProgress(BaseResourceObject* nextObject){
	if(mCurResearch!=0){
		if(mCurResearch->addProgress(nextObject)){
			WhenDone();		
			return true;
		}
		return false;
	}
	else return false;


}
bool BaseReasearchManager::addProgress(std::map<BaseResource*,int> AddingProgress){
	if(mCurResearch!=0){
		if(mCurResearch->addProgress(AddingProgress)){
			WhenDone();		
			return true;
		}
		return false;
	}
	else return false;

}
bool BaseReasearchManager::addProgress(int progress){

	if(mCurResearch!=0){
		if(mCurResearch->addProgress(progress)){
			WhenDone();	
			return true;
		}
		return false;
	}
	else return false;
}

void BaseReasearchManager::WhenDone(){
	if(mCurDepartment->mCurLvlNum+1<ResearchDepartment::mLvlNum&&!mIdle){
		mCurDepartment->mCurLvlNum++;
		mIdle=true;
		mEvnt= false;
		if(mCurResearch!=0){
			/*if(mDepartmentPoint.count(mCurResearch->mDepartment)){
			mDepartmentPoint[mCurResearch->mDepartment]++;
			}else{
			mDepartmentPoint[mCurResearch->mDepartment]=0;
			}*/
		}
	}

}
bool BaseReasearchManager::SetResearch(BaseResearch *curResearch){
	for(int i=0;i<mAllDepNum;i++){
		if(mAllDep[i]->hasIt(curResearch)){
			mCurDepartment = mAllDep[i];
			mCurDepartment->StartResearch();
			mCurResearch =curResearch;
			mIdle= false;
			return true;
		}
	}

	return false;

}
GameEvent *  BaseReasearchManager::ReturnResearchInfo(){

	ShowResearchCenter* evnt = new ShowResearchCenter();

	if(mCurResearch!=0){
		evnt->curResearch= mCurResearch->GetName();
		mCurResearch->GetMap(evnt->progress,evnt->doneProgress);
	}else{
		evnt->curResearch= "Empty";

	}
	evnt->isNeedChoice = mIdle;

	evnt->nextLvl=  GetNextInMap();
	return evnt;
}
std::map<Ogre::String, BaseResearch*> BaseReasearchManager::GetNextInMap(){
	std::map<Ogre::String, BaseResearch*> map;
	
	for(int i=0;i<mAllDepNum;i++){
		
		mAllDep[i]->ShowResearch(map);


	}

	return map;

}
bool BaseReasearchManager::isActive(){
	if(mCurResearch!=0){
		return !mCurResearch->IsDone();
	}else{
		return false;
	}
}
void BaseReasearchManager::addResearchPoints(BaseResource *curResource){
	if(mDepartmentPoint.count(curResource)){
		mDepartmentPoint[curResource]++;
	}else{
		mDepartmentPoint[curResource]=0;
	}
}
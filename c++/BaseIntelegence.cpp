#include "BaseIntelegence.h"
#include "BaseListener.h"
#include "BaseLogic.h"
using namespace Logic;
IntelegentManager* IntelegentManager::sigleton=0;


ActionManager::~ActionManager(void)
{
	for(int i =0; i<mActionCount;++i){
		if((mAllAction+i)!=0){
			delete mAllAction[i];
		}	
	}
	delete [] mAllAction;

}
void ActionManager::Init(BaseAction **allAction,int actionCount){
	mAllAction = allAction;
	mActionCount = actionCount;
	for(int i =0; i<mActionCount;++i){
		if((mAllAction+i)!=0){
			mActionMap[mAllAction[i]->name] =mAllAction[i];
		}	
	}
}
void ActionManager::Open(Ogre::String name){
	mActionMap[name]->open =true;
}
void  ActionManager::SayAction(std::map<Ogre::String , BaseAction*> &actions,ActonType type){
	for(int i =0; i<mActionCount;++i){
		if(*(mAllAction+i)!=0){
			if(mAllAction[i]->type ==type&&mAllAction[i]->open){
				actions[mAllAction[i]->name]= mAllAction[i];
			}
		}	
	}

}
ActionIntDialog::ActionIntDialog(){
	type =ET_GAME;
	gameType =GUIET_SHOWACTINTDIALOG;
}
IntelegentManager::IntelegentManager(){
	mTempAction= 0;
	mTempBuilding = 0;	
	mTempReason=0;
	mTempTarget=0;
	sigleton = this;

}
IntelegentManager::~IntelegentManager(){
	for(int i=0;i<mBlackSize;++i){
		delete  mBlackResource[i];
	
	}
	for(int i=0;i<mEmbasySize;++i){
		delete mEmbasyResource[i];
		
	}
	delete[]mBlackResource;
	delete[]mEmbasyResource;

}
IntelegentManager* IntelegentManager::GetSigleton(){
	return sigleton;
}
bool IntelegentManager::GetResource(BaseResourceObject* obj,int amount){
	if(mStockpile[obj]>0){
		mStockpile[obj]-=amount;
		std::map<BaseResource*,int>::iterator itr;
		for(itr = obj->mResource.begin(); itr != obj->mResource.end(); ++itr){		
			*(itr->first)-=itr->second;		
		}
		return true;
	}
	return false;
}
void IntelegentManager::MakeAll(std::vector<BaseResourceObject*> blackVector,  std::vector<BaseResourceObject*>embassyVector,  std::vector<BaseResourceObject*>reconVector){
	mBlackSize =blackVector.size();
	mBlackResource = new BaseResourceObject*[mBlackSize];
	for(int i=0;i<mBlackSize;++i){
		mBlackResource[i] = blackVector[i];
		mStockpile[ blackVector[i]] = 0;
	}
	mEmbasySize =embassyVector.size();
	mEmbasyResource = new BaseResourceObject*[mEmbasySize];
	for(int i=0;i<mEmbasySize;++i){
		mEmbasyResource[i] = embassyVector[i];
		mStockpile[ embassyVector[i]] = 0;
	}
	mReconSize =reconVector.size();
	mReconResource = new BaseResourceObject*[mReconSize];
	for(int i=0;i<mReconSize;++i){
		mReconResource[i] = reconVector[i];
		mStockpile[ reconVector[i]] = 0;
	}

}
void IntelegentManager::GenerateResource(ActonType type){
	std::map<BaseResource*,int>::iterator itr;
	int a =0; 
	switch(type){
	case AT_BLACKOPS:
		a =std::rand() % mBlackSize;
		mStockpile[mBlackResource[a]]++;
		for(itr = mBlackResource[a]->mResource.begin(); itr != mBlackResource[a]->mResource.end(); ++itr){		
			*(itr->first)+=itr->second;		
		}

		break;
	case AT_DIMPLOMATS:
		a =std::rand() % mEmbasySize;
		mStockpile[mEmbasyResource[a]]++;
		for(itr = mEmbasyResource[a]->mResource.begin(); itr != mEmbasyResource[a]->mResource.end(); ++itr){		
			*(itr->first)+=itr->second;		
		}

		break;
	case AT_RECON:
		a =std::rand() % mReconSize;
		mStockpile[mReconResource[a]]++;
		for(itr = mReconResource[a]->mResource.begin(); itr != mReconResource[a]->mResource.end(); ++itr){		
			*(itr->first)+=itr->second;		
		}
		break;
	}
		
		
}
GameEvent* IntelegentManager::GiveActionDialog(BaseBuilding *tempBilding,BaseAction *action){
	mTempAction= action;
	mTempBuilding = tempBilding;
	return RefreshEvent();
}
GameEvent*  IntelegentManager::SetActionDialogSomething(BaseCountry *tempTarget,bool isTarget){
	if(isTarget){
		mTempTarget =tempTarget;
	}else{
		mTempReason = tempTarget;
	}
		return RefreshEvent();
}
bool IntelegentManager::hasResource(BaseResourceObject *Object,int amount){
		return mStockpile[Object]>=amount;
}
GameEvent* IntelegentManager::DeclineActionDialog(){
	mTempAction= 0;
	mTempBuilding = 0;	
	mTempReason=0;
	mTempTarget=0;
	GameEvent *evnt = new GameEvent();
	evnt->gameType =GUIET_CLOSEACTINTDIALOG;
	return evnt;
}
GameEvent* IntelegentManager::AceptActionDialog(){
	if(mTempTarget==0){
		return RefreshEvent();
	}
	if(!PointerGuard<BaseBuilding>::GetSigleton()->Get(mTempBuilding)){
		return DeclineActionDialog();
	}
	BaseEmbassyBuilding *building = static_cast<	BaseEmbassyBuilding *>(mTempBuilding);
	if(mTempAction->doneInSecret&&mTempReason!=0){
		building->SetAction(mTempAction,mTempTarget,mTempReason);
	}else{
		building->SetAction(mTempAction,mTempTarget);
	}
	mTempAction= 0;
	mTempBuilding = 0;	
	mTempReason=0;
	mTempTarget=0;
	return building->ReturnBuildingInfo();
}
GameEvent* IntelegentManager::RefreshEvent(){
	ActionIntDialog * evnt= new ActionIntDialog();
	evnt->canBeReason = mTempAction->doneInSecret;
	evnt->selected = mTempTarget;
	evnt->reason = mTempReason;
	NonCoreBuilding *building = static_cast <NonCoreBuilding*>(mTempBuilding);

	building->mCore->mPlanet->GetAllCountry(evnt->allCountry,mTempAction->name);
	return evnt;
}
void IntelegentManager::ShowObjectsAll(std::map<Ogre::String,int> &amount,std::map<Ogre::String,BaseResourceObject*> &links){
	std::map<BaseResourceObject*,int>::iterator iterStock ;
	for(iterStock= mStockpile.begin();iterStock!=mStockpile.end();++iterStock){
		if(iterStock->second>0){
		amount[iterStock->first->mName] = iterStock->second;
		links[iterStock->first->mName] = iterStock->first;
		}
	}
}
void IntelegentManager::ShowObjectsAll(std::vector<ResourseSend> &allResourse){
	std::map<BaseResourceObject*,int>::iterator iterStock;	
	
	ResourseSend *temp;
	for(iterStock = mStockpile.begin(); iterStock != mStockpile.end(); ++iterStock){
		if(iterStock->second>0){
		allResourse.push_back(ResourseSend());
		temp =&allResourse.back();
	
		temp->name = iterStock->first->mName;
		temp->object = iterStock->first;
		}
	}
}
BaseEmbassyBuilding::BaseEmbassyBuilding(void) :
mAction (0),
	mTarget (0),
	mReason(0),
	informdayCollection(0),
	dayProduction(0){
		mType =TB_EMBASSY;
}
BaseEmbassyBuilding::BaseEmbassyBuilding(ActonType type) :
mAction (0),
	mTarget (0),
	mReason(0),

	informdayCollection(0),
	dayProduction(0){
		mIntType= type;
		switch(type){
		case AT_BLACKOPS:
			mType =TB_BLACKOPS;
			break;
		case AT_DIMPLOMATS:
			mType =TB_EMBASSY;
			break;
		}
		mProductionPower=1;
		if(mType == TB_EMBASSY){
			mProdNeed =BaseAsset::GetSigleton("DAYTOGETINFO_EMMBASSY");
		}else{
			mProdNeed= BaseAsset::GetSigleton("DAYTOGETINFO_BLACKOPS");
		}

}

void BaseEmbassyBuilding::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	if(mAction==0){
	
		
		if(++informdayCollection>=std::floor(mProdNeed/(double)GetProductionPower())){
			mIntMan->GenerateResource(mIntType);
			informdayCollection=0;
		}
	}else{
		if(++dayProduction>=std::floor((double)mAction->dayCount/(double)GetProductionPower())){
			if(mReason!=0){
				BaseLogic::GetSigleton()->mLuaMan.PlanetScript(*static_cast<BasePlanetCountry*>(mTarget),mAction->name,*mReason);
			}else{
				BaseLogic::GetSigleton()->mLuaMan.PlanetScript(*static_cast<BasePlanetCountry*>(mTarget),mAction->name);
			}
			NotificationEvent *evnt=new NotificationEvent();
			evnt->notType = NTE_EMBASSYCOMPLITE;
			evnt->target = this;
			evnt->name = this->mName;
			evnt->name +=" complite "+mAction->name;
			notificationEvent.push(evnt);
			mAction=0;
			mTarget=0;
			mReason=0;
			dayProduction=0;
				
			
		}
	}
	BaseLogic::GetSigleton()->mLuaMan.BuildingPassDay(this);
	BaseBuilding::PassDay(notificationEvent);
}
void BaseEmbassyBuilding::SetManager(IntelegentManager *intMan,ActionManager *actMan){
	mIntMan = intMan;
	mActMan = actMan;
}
GameEvent * BaseEmbassyBuilding::ReturnBuildingInfo(){
	ShowIntelegence *evnt = new ShowIntelegence();
	if(mAction==0){
		evnt->curAction = "None";
		evnt->progress=informdayCollection;
		evnt->needProduction = mProdNeed;

	}else{
		evnt->curAction = mAction->name;
		evnt->progress  =dayProduction;
		evnt->needProduction = mAction->dayCount;
	}
	evnt->name = this->mCore->mCountry->mName;
	if(mType ==TB_EMBASSY){
		evnt->demand = this->mCore->mCountry->GetDemandForEmabassy();
	}
	evnt->target = this;
	mActMan->SayAction(evnt->actions,mIntType);
	return evnt;


}
void BaseEmbassyBuilding::SetAction(BaseAction *action,BaseCountry *target){
	mAction= action;
	mTarget= static_cast<BasePlanetCountry*>(target);
	dayProduction=0;

}
void  BaseEmbassyBuilding::SetAction(BaseAction *action,BaseCountry *target,BaseCountry *reason){
	mAction= action;
	mTarget= static_cast<BasePlanetCountry*>(target);
	mReason = static_cast<BasePlanetCountry*>(reason);
	dayProduction=0;
}
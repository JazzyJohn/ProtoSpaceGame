#include "BaseMilitary.h"
#include "BaseLogic.h"
using namespace Logic;

BaseUnitCharacteristics::BaseUnitCharacteristics(Ogre::String name,bool lifeCare){
			mName = name;
		
			mLifeCare = lifeCare;
};
BaseUnitCharacteristics::~BaseUnitCharacteristics(){
	
}
BaseUnit::~BaseUnit(void){
	PointerGuard<BaseUnit>::GetSigleton()->Remove(this);
	std::map<BaseUnitCharacteristics*,float*>::iterator itr;
	for(itr = mChar.begin(); itr != mChar.end(); ++itr){
		delete itr->second;
	}
	if(mBase!=0){
		BaseMilitaryBuilding *temp = static_cast<BaseMilitaryBuilding *>(mBase);
		temp->DeleteUnit(this);
	}
	if(mCountry!=0){
		BaseLogic::GetSigleton()->mWarFare.DeleteUnit(mCountry,this);
	}
	mCountry= 0;
}

BaseUnit::BaseUnit(Ogre::String name,std::map<BaseUnitCharacteristics*,float> Char,int cost){
	mCountry= 0;
	mBase=0;
	mName =name;
	std::map<BaseUnitCharacteristics*,float>::iterator itr;
	float *temp =0;
	for(itr = Char.begin(); itr != Char.end(); ++itr){
		temp = new float(itr->second);
		mChar[itr->first] =temp;	
		mCharRef[itr->first->mName]  = temp;
	}
	mCost = cost;
	BaseLogic::GetSigleton()->mLuaMan.BuildUnit(this);
}
int BaseUnit::GetFirePower(){
	return *(mCharRef["Firepower"]);
}
void BaseUnit::SetCountry(BaseCountry *country){
	mCountry = country;
}
bool BaseUnit::hasTarget(){
	return mCountry ==0;
}
bool BaseUnit::LowerHp(int damage){
	if(*(mCharRef["Manpower"])<=0){
		return true;
	}
	 *(mCharRef["Manpower"]) -=damage;
	 if( *(mCharRef["Manpower"]) <=0){
		return false;		
	 }
	 else{
		return true; 
	 }
}
void BaseUnit::SayAbility(std::map<Ogre::String,Logic::BaseAbility*> &ability){
	std::vector<BaseAbility*> vct;
	BaseLogic::GetSigleton()->mAbilityMng.GetAbility(vct,ABILT_UNIT);
	std::vector<BaseAbility*>::iterator vctitr;
	for(vctitr= vct.begin();vctitr!=vct.end();++vctitr){
		if(mAllUpgrade.count((*vctitr)->mName)==0){
		ability[(*vctitr)->mDisplayName] =(*vctitr);
		}
	}
}
void BaseUnit::PassDay(){
	BaseLogic::GetSigleton()->mLuaMan.UnitDayPass(this);
}
void  BaseUnit::SayCharacter(std::map<Ogre::String,int> &character){
	std::map<BaseUnitCharacteristics*,float*>::iterator itr;
	for(itr = mChar.begin(); itr != mChar.end(); ++itr){
		character[itr->first->mName]=*(itr->second);
	}
}
float  BaseUnit::GetChar(Ogre::String name){
		return *(mCharRef[name]);
}
void BaseUnit::SetChar(Ogre::String name,float val){
*(mCharRef[name])= val;
}
bool BaseUnit::HasUpgrade(Ogre::String name){
	return mAllUpgrade.count(name)>0;
}
void BaseUnit::AddUpgrade(Ogre::String name){
	mAllUpgrade.insert(name);
}
Ogre::String BaseUnit::SayName(){
	return mName;
}

void BaseUnit::AddFromSupplie(float supplie){
	*(mCharRef["Manpower"])+= *(mCharRef["SupplieCoef"])*supplie;
		
}
ShowUnitInBase::ShowUnitInBase(){
	type =ET_GAME;
	gameType =GUIET_SHOWUNITINBASE;
}
SendUnitToCountry::SendUnitToCountry(){
	type =ET_CONTROL;
	controlType =CTE_SENDUNIT;
}
BaseUnitBlueprint::BaseUnitBlueprint(Ogre::String name){
			mName = name;
		
		};

BaseUnitBlueprint::~BaseUnitBlueprint(){
	
}
void BaseUnitBlueprint::SetCost(int cost){
	mCost =cost;

}
int BaseUnitBlueprint::GetCost(){
	return mCost ;

}
void BaseUnitBlueprint::SetCharacteristik(std::map<BaseUnitCharacteristics*,float> Char){
	std::map<BaseUnitCharacteristics*,float>::const_iterator itr;
	for(itr = Char.begin(); itr != Char.end(); ++itr){
		mChar[itr->first] =itr->second;
	}

}
BaseUnit* BaseUnitBlueprint::GetUnit(){
	BaseUnit * temp = new BaseUnit(mName,mChar,GetCost());
	if(mUniq){
		mOpen=false;
	}
	PointerGuard<BaseUnit>::GetSigleton()->Insert(temp);
	return temp;
}
BaseUnitManager::~BaseUnitManager(){
	std::map<Ogre::String,BaseUnitBlueprint*>::iterator itr;
	for(itr = mAllUnits.begin(); itr != mAllUnits.end(); ++itr){
		delete mAllUnits[itr->first];

	}
	for(int i =0; i<mCharctNum;++i){
		if((mAllCharacteristick+i)!=0){
			delete mAllCharacteristick[i];
		}	
	}
	delete [] mAllCharacteristick;
}
void BaseUnitManager::setAllCharacteristics(BaseUnitCharacteristics ** allCharacteristick,int charctNum){
	mCharctNum =charctNum;
	mAllCharacteristick = allCharacteristick;
}
void BaseUnitManager::AddUnit(Ogre::String name,BaseUnitBlueprint* blueprint){
	mAllUnits[name]= blueprint;
}
BaseUnit* BaseUnitManager::GetUnit(Ogre::String name){
	return mAllUnits[name]->GetUnit();
}
BaseUnitBlueprint* BaseUnitManager::GetUnitBlueprint(Ogre::String name){
	return mAllUnits[name];

}
void BaseUnitManager::Open(Ogre::String name){
	mAllUnits[name]->mOpen= true;
}
void BaseUnitManager::GetUnitBlueprint(std::map<Ogre::String,BaseUnitBlueprint*> &avaibleUnits){
	std::map<Ogre::String,BaseUnitBlueprint*>::iterator itr;
	for(itr = mAllUnits.begin(); itr != mAllUnits.end(); ++itr){
		if(itr->second->mOpen){
		avaibleUnits[itr->first] =itr->second;
		}
	}

}

BaseMilitaryBuilding::BaseMilitaryBuilding(void)
{
	mType = TB_MILITARY;
	mUnitMAxCount =BaseAsset::GetSigleton("MILITAR_BASE_MAX");
	mUnits=  new BaseUnit*[mUnitMAxCount];
	for(int i =0; i<mUnitMAxCount;++i){
		mUnits[i]=0;
		
	}
	mCurProduction=0;
	mProductionPower=BaseAsset::GetSigleton("BASE_PRODUCTION_POWER");
}
AddUnittoQueue::AddUnittoQueue(){
	type =ET_CONTROL;
	controlType =CTE_ADDUNITTOQUEUE;
}
ShowMilitary::ShowMilitary(std::queue<Ogre::String> mUnitQueue ):unitQueue(mUnitQueue){
		gameType =GUIET_SHOWMILITARYBASE;
	type =ET_GAME;
	
}
BaseMilitaryBuilding::~BaseMilitaryBuilding(void)
{
	for(int i =0; i<mUnitMAxCount;++i){
		if(*(mUnits+i)!=0){
			mUnits[i]->mBase =0;
			delete mUnits[i];
		}	
	}
	delete [] mUnits;
}
bool BaseMilitaryBuilding::AddUnit(Ogre::String name){
	 for(int i =0; i<mUnitMAxCount;++i){
		if(*(mUnits+i)==0){
			mUnits[i] = mManager->GetUnit(name);
			mUnits[i]->mBase= this;
			return true;
		}	
	}
	return false;

}
void  BaseMilitaryBuilding::DeleteUnit(BaseUnit* unit){
	for(int i =0; i<mUnitMAxCount;++i){
		if(*(mUnits+i)==unit){
			mUnits[i] =0;
			return;
		}	
	}
}
void BaseMilitaryBuilding::AddManager(	BaseUnitManager *manager){
	mManager= manager;
}
void BaseMilitaryBuilding::AddUnitToQueue(Ogre::String name){
	if(!mUnitQueue.empty()&&mUnitQueue.size()>=5){
		return;
	}
	mUnitQueue.push(name);

}
void BaseMilitaryBuilding::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	if(!mUnitQueue.empty()){
		mCurProduction+= GetProductionPower();
		if(mCurProduction>=BaseAsset::GetSigleton("NEED_FOR_PRODUCTION")){
		Ogre::String name =mUnitQueue.front();
		if(AddUnit(mUnitQueue.front())){
			mUnitQueue.pop();
			
			mCore->ReduceCredit(mManager->GetUnitBlueprint(name)->GetCost());
			mIdle = true;
		}else{
			mIdle = false;
		}
		if(mIdle=true){
			mCurProduction=0;
		}
		}
		
	}else{
		mIdle = false;
	}
	for(int i =0; i<mUnitMAxCount;++i){
		if(*(mUnits+i)!=0){
			mUnits[i]->PassDay();
		}
	}
	BaseBuilding::PassDay(notificationEvent);
}
GameEvent *BaseMilitaryBuilding::ReturnBuildingInfo(){
	ShowMilitary * evnt = new ShowMilitary(mUnitQueue);
	UnitToSend *temp;
	for(int i =0; i<mUnitMAxCount;++i){
		evnt->unitOnBase.push_back(UnitToSend());
		temp= 	&(evnt->unitOnBase.back());
		temp->isEmpty =*(mUnits+i)==0;
		if(*(mUnits+i)==0){
			temp->name = "Null";
			temp->unit = 0;			
		}else{
			temp->name=mUnits[i]->mName;
			temp->unit = mUnits[i];
		}	
	}
	evnt->target = this;
	evnt->procent = mCurProduction/BaseAsset::GetSigleton("NEED_FOR_PRODUCTION");
	mManager->GetUnitBlueprint(evnt->avaibleUnits);
	return evnt;
}
void 	BaseMilitaryBuilding::SomeChange(int type,float multi){
	if(type==SCT_UNITINCREASE){
		for(int i =0; i<mUnitMAxCount;++i){
			if(*(mUnits+i)!=0){
				  mUnits[i]->SetChar("Manpower",mUnits[i]->GetChar("Manpower")+multi);
				  mUnits[i]->SetChar("Firepower",mUnits[i]->GetChar("Firepower")+multi);
				  mUnits[i]->SetChar("Deployment capabilities",mUnits[i]->GetChar("Deployment capabilities")+multi);
			}
		}
	
	
	}
}

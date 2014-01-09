#include "BaseWarfare.h"
#include "BaseListener.h"
#include "BaseLogic.h"
using namespace Logic;
BaseBattle::BaseBattle(BaseCountry *battleField,	std::vector<BaseUnit*> units){
	mBattleField = battleField;
	mStartCountryChar["Economic"] = battleField->GetChar("Economic");
	mNpcUnit =units; 
	mIsSupplied= false;
	mSuppliedAmount =0;
	mPlayerAction= WARA_STAND;
	mIsPartisan = false;
	mSuppliedCoef=1;
}
void BaseBattle::AddUnit(BaseUnit* playerUnit){
	mPlayerUnit.push_back(playerUnit);
}	
void BaseBattle::SetAction(WarfareAction playerAction){
	mPlayerAction= playerAction;

}
BaseUnit& BaseBattle::GetNpcUnit(int i){
	return *mNpcUnit[i];
}
BaseUnit& BaseBattle::GetPlayerUnit(int i){
	return *mPlayerUnit[i];
}
void BaseBattle::DeadUnit(int player,int i){
	switch(player){
	case 0:
		mTempPlayerUnit.push(mPlayerUnit[i]);
		break;
	case 1:
		mTempNpcUnit.push(mNpcUnit[i]);
		break;
	}
}
void DeleteFormVector(std::vector<BaseUnit*> &vector,BaseUnit* unit ){
		std::vector<BaseUnit*>::iterator unitItr;
			for(unitItr=vector.begin();unitItr!=vector.end();++unitItr){
				if((*unitItr)==unit){
					vector.erase(unitItr);	
					return;
				}
			}
}
void BaseBattle::GeneratePartisan(){
	int population = mBattleField->GetChar("Population");
	int relation  =  mBattleField->GetChar("Agression") - mBattleField->GetChar("Worship");
	float coef = 1;
	BaseAsset *asset = BaseAsset::GetSigleton();
	if(relation>(*asset)["PARTISAN_MAX_TRESHOLD"]){
		coef = (*asset)["PARTISAN_MAX_VALUE"];
	}
	if(relation>(*asset)["PARTISAN_HIGH_TRESHOLD"]){
		coef =(*asset)["PARTISAN_HIGH_VALUE"];
	}

	if(relation<(*asset)["PARTISAN_NORMAL_TRESHOLD"]){
		coef=(*asset)["PARTISAN_NORMAL_VALUE"];
	}
	if(relation <(*asset)["PARTISAN_LOW_TRESHOLD"]){
		coef= (*asset)["PARTISAN_LOW_VALUE"];
	}
	if(relation<(*asset)["PARTISAN_MIN_TRESHOLD"]){
		coef=(*asset)["PARTISAN_MIN_VALUE"];
	}
	mPartisansHP = population*coef*(*asset)["PARTISAN_HP_COEF"];
	if(mPartisansHP<=0){
		mPartisansHP=1;
	}
	mPartisansPower = mBattleField->GetChar("Agression")*(*asset)["PARTISAN_POWER_COEF"];
	mIsPartisan= true;
}
bool BaseBattle::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	if(mPlayerAction== WARA_STAND){
		return false;
	}
	if(mPlayerUnit.size()==0){
		return true;
	}

	mPlayerAmount= mPlayerUnit.size();
	mNpcAmount= mNpcUnit.size();
	bool result =BaseLogic::GetSigleton()->mLuaMan.DoBattle(this);
	/*
		std::vector<BaseUnit*>::iterator unitItr;

	if(mNpcUnit.size()==0){
		float supPerUnit=  (double)mSuppliedAmount/mPlayerUnit.size();
		switch(mPlayerAction){
		case WARA_OCUPIE:
			
				 
				for(unitItr=mPlayerUnit.begin();unitItr!=mPlayerUnit.end();++unitItr){
					if(mIsSupplied){
						(*unitItr)->AddFromSupplie(supPerUnit);
					}
					if(!(*unitItr)->LowerHp(mPartisansPower)){
						mTempPlayerUnit.push((*unitItr));
					}
					mPartisansHP-=	(*unitItr)->GetFirePower();
				}
				if(mPartisansHP<=0){
					return true;				
				}

			break;
		case WARA_PILLAGE:
			
				for(unitItr=mPlayerUnit.begin();unitItr!=mPlayerUnit.end();++unitItr){
					if(mIsSupplied){
						(*unitItr)->AddFromSupplie(supPerUnit);
					}
					if(!(*unitItr)->LowerHp(mPartisansPower)){
						mTempPlayerUnit.push((*unitItr));
					}
					if(mBattleField->Pillage((*unitItr)->GetFirePower())){
						return true;					
					}
				}				
			break;
			case WARA_KILLTHEMALL:
			
				for(unitItr=mPlayerUnit.begin();unitItr!=mPlayerUnit.end();++unitItr){
					if(mIsSupplied){
						(*unitItr)->AddFromSupplie(supPerUnit);
					}
					if(!(*unitItr)->LowerHp(mPartisansPower)){
						mTempPlayerUnit.push((*unitItr));
					}
					if(mBattleField->Destroying((*unitItr)->GetFirePower())){
						return true;					
					}
				}				
			break;
		
		}
	
	}else{
		int i = 0;
		for(unitItr=mPlayerUnit.begin();unitItr!=mPlayerUnit.end();++unitItr){
	
			if(!mNpcUnit[i]->LowerHp((*unitItr)->GetFirePower())){
				mTempNpcUnit.push(mNpcUnit[i]);
			}
			i++;
			if(i>=mNpcUnit.size()){
				i=0;
			}
		}
		i=0;
		for(unitItr=mNpcUnit.begin();unitItr!=mNpcUnit.end();++unitItr){
	
			if(!mPlayerUnit[i]->LowerHp((*unitItr)->GetFirePower())){
				mTempPlayerUnit.push(mPlayerUnit[i]);
			}
			i++;
			if(i>=mPlayerUnit.size()){
				i=0;
			}
		}
		
		
		
	}
	*/
	while(!mTempNpcUnit.empty()){
			DeleteFormVector(mNpcUnit,mTempNpcUnit.front());
			PointerGuard<BaseUnit>::GetSigleton()->Remove(mTempNpcUnit.front());
			delete mTempNpcUnit.front();
			mTempNpcUnit.pop();
			mReDraw= true;
	}
	while(!mTempPlayerUnit.empty()){
			DeleteFormVector(mPlayerUnit,mTempPlayerUnit.front());
			
			delete mTempPlayerUnit.front();
			mTempPlayerUnit.pop();
			mReDraw= true;
	}
	if(mNpcUnit.size()==0&&!mIsPartisan){
			 GeneratePartisan();
	}
	if(result){
		return true;
	}
	if(mPlayerUnit.size()==0){
		return true;
	}
	if(mIsSupplied){
		BaseAccount::GetSigleton()->ChangeCredit(-mSuppliedCoef*mSuppliedAmount);
	}
	return false;
}
void BaseBattle::FinishBattle(std::queue<NotificationEvent*> &notificationEvents){
	mBattleField->mAtWar= false;
	if(mPlayerUnit.size()>0){
	switch(mPlayerAction){
		case WARA_OCUPIE:
			mBattleField->mOcuppied= true;
			break;
		case WARA_PILLAGE:
			mBattleField->GenerateToll(mStartCountryChar["Economic"]);
			mBattleField->mOcuppied= true;
			break;
		case WARA_KILLTHEMALL:
			mBattleField->mDestroyed= true;
			break;
	
	}
		std::vector<BaseUnit*>::iterator unitItr;
		for(unitItr=mPlayerUnit.begin();unitItr!=mPlayerUnit.end();++unitItr){
			if((*unitItr)->mName=="messia"){
				mBattleField->SetChar("Propocadaocupied",2);
				mBattleField->mOcuppied= true;
				mBattleField->mDestroyed= false;
			}
			(*unitItr)->mCountry=0;
		}
	}else{
		std::vector<BaseUnit*>::iterator unitItr;
		int milpower=0;
		for(unitItr=mNpcUnit.begin();unitItr!=mNpcUnit.end();++unitItr){
			milpower += (*unitItr)->mCost;
		}
		mBattleField->SetChar("Military Power",milpower);
	}
}
void BaseBattle::DeleteUnit(BaseUnit* playerUnit){
	std::vector<BaseUnit*>::iterator coreIterator;

	for(coreIterator=mPlayerUnit.begin();coreIterator!=mPlayerUnit.end();++coreIterator){
		if((*coreIterator)==playerUnit){
			mPlayerUnit.erase(coreIterator);
		return ;
		}
	}
}
GameEvent * BaseBattle::ShowInfo(){
	Logic::ShowWarfare *evnt = new Logic::ShowWarfare();
	evnt->contry = mBattleField;
	evnt->name = mBattleField->mName;
	UnitForSendWar *temp;
	std::vector<BaseUnit*>::iterator itrUnit;
	for(itrUnit = mNpcUnit.begin();itrUnit!= mNpcUnit.end(); ++itrUnit){
		evnt->NPCUnit.push_back(UnitForSendWar());
		temp =&evnt->NPCUnit.back();
		temp->name = (*itrUnit)->SayName();
		temp->pnt =  (*itrUnit);
		(*itrUnit)->SayCharacter(temp->character);

	}
	for(itrUnit = mPlayerUnit.begin();itrUnit!= mPlayerUnit.end(); ++itrUnit){
		evnt->playerUnit.push_back(UnitForSendWar());
		temp =&evnt->playerUnit.back();
		temp->name = (*itrUnit)->SayName();
		temp->pnt =  (*itrUnit);
		(*itrUnit)->SayCharacter(temp->character);
	}
	evnt->curAction = mPlayerAction;
	mBattleField->GetMap(evnt->countryChar);
	evnt->isSupplied = mIsSupplied;
	evnt->suppliedAmount = mSuppliedAmount;
	if(mPlayerAction==WARA_STAND){
		evnt->status= WARSTS_NONE;		
	}else{
		if(mIsPartisan){
			evnt->status= WARSTS_AFTERMATCH;	
			evnt->partisanHP = mPartisansHP;
		}else{
			evnt->status= WARSTS_BATTLE;	
		}
	}
	return evnt;
}
ShowWarfare::ShowWarfare(){
	gameType =GUIET_SHOWWARFARE;
	type =ET_GAME;
}
CloseWarfare::CloseWarfare(){
	gameType =GUIET_CLOSEWARFARE;
	type =ET_GAME;
}
BaseWarfare::~BaseWarfare(void)
{
	std::map<BaseCountry *,BaseBattle*>::iterator batItr;
	for(batItr = mAllBattles.begin();batItr != mAllBattles.end();++batItr){
		delete batItr->second;
	
	}

}
void BaseWarfare::Init(BaseUnitBlueprint** allNPCUnits, int allNPCUnitsCnt){
	mAllNPCUnits =allNPCUnits;
	mAllNPCUnitsCnt = allNPCUnitsCnt;

}
void BaseWarfare::MoveUnit(BaseCountry * country, BaseUnit* attacker){
	if(mAllBattles.count(country)==0){
		int military_power =country->GetChar("Military Power");
		country->SetChar("Military Power",0);
		std::vector<BaseUnit*> npcUnit;
		BaseUnit *tempUnit;
		int a=0;
		while(military_power>0){
			 a = std::rand()%mAllNPCUnitsCnt;			
			 tempUnit = mAllNPCUnits[a]->GetUnit();
			 military_power-=mAllNPCUnits[a]->GetCost();
			 npcUnit.push_back(tempUnit);
		}
		BaseBattle* temp = new BaseBattle(country,npcUnit);
		country->mAtWar = true;
		temp->AddUnit(attacker);
		mAllBattles[country] = temp;
	}else{

		mAllBattles[country]->AddUnit(attacker);
	}
	attacker->SetCountry(country);
}
void BaseWarfare::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	std::queue<BaseCountry *> tempCountry;
	std::map<BaseCountry *,BaseBattle*>::iterator batItr;
	for(batItr = mAllBattles.begin();batItr != mAllBattles.end();++batItr){
		if(batItr->second->PassDay(notificationEvent)){
			tempCountry.push(batItr->first);
			batItr->second->FinishBattle(notificationEvent);
		}	
	}
	while(!tempCountry.empty()){
		delete mAllBattles[tempCountry.front()];
		mAllBattles.erase(tempCountry.front());
		tempCountry.pop();
	}

}
GameEvent * BaseWarfare::ShowWarfare(BaseCountry * country){
	if(mAllBattles.count(country)>0){
		return mAllBattles[country]->ShowInfo();
		
	}else{
		return new GameEvent();
	}
	
}
GameEvent * BaseWarfare::ShowWarfare(BaseCountry * country,bool &reDraw){
	if(mAllBattles.count(country)>0){
		reDraw =mAllBattles[country]->mReDraw;
		mAllBattles[country]->mReDraw= false;
		return mAllBattles[country]->ShowInfo();
		
	}else{
		return new GameEvent();
	}
	
}
void BaseWarfare::SetAction(BaseCountry * country, WarfareAction action){
	if(mAllBattles.count(country)>0){
		mAllBattles[country]->SetAction(action);
		
	}
}
void BaseWarfare::DeleteUnit(BaseCountry * country, BaseUnit* attacker){
	if(mAllBattles.count(country)>0){
		mAllBattles[country]->DeleteUnit(attacker);
		
	}
}
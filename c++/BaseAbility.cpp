#include "BaseAbility.h"
#include "BaseLogic.h"
using namespace Logic;

BaseAbility::BaseAbility(void)
{
}


BaseAbility::~BaseAbility(void)
{
}

BaseAbility::BaseAbility(AbilityType type,Ogre::String name,	Ogre::String descr){
	mType = type;
	mDisplayName = descr;
	mName = name;
	mOpen = false;
}

bool BaseAbility::MakeItSo(void*target){
	if(mOpen){
		if(mTimeOut!=0){
			int day =BaseLogic::GetSigleton()->GetDay();
			if(mTimeOut+mLastTime<day){
				mLastTime= day;
				return BaseLogic::GetSigleton()->mLuaMan.DoAbility(mName,mType,target);
			}
		}else{
			return BaseLogic::GetSigleton()->mLuaMan.DoAbility(mName,mType,target);
		}
	}else{
		return false;
	}
}
void BaseAbility::Open(){
	mOpen = true;
}
bool BaseAbility::State(){
	return mOpen;
}
ActivateAbility::ActivateAbility(){
	type =ET_CONTROL;
	controlType =CTE_ACTIVATEABILITY;
}
AbilityType BaseAbility::GetType(){
	return mType;
}
AbilityManager::AbilityManager(){

}
AbilityManager::~AbilityManager(){
	std::vector<BaseAbility*>::iterator itr;
	for(itr=mAllAbility.begin(); itr!=mAllAbility.end();++itr){
		delete *itr;
	}
}
void AbilityManager::OpenAbility(Ogre::String name){
	std::vector<BaseAbility*>::iterator itr;
	for(itr=mAllAbility.begin(); itr!=mAllAbility.end();++itr){
		if((*itr)->mName==name){
		(*itr)->Open();
		}
	}
}
void AbilityManager::AddAbility(BaseAbility *abil){
	mAllAbility.push_back(abil);
	mAllMapAbility.insert(std::make_pair<AbilityType,BaseAbility*>(abil->GetType(),abil));
}
void AbilityManager::GetAbility(std::vector<BaseAbility*> &vector,AbilityType type){
	auto range =mAllMapAbility.equal_range(type);
	std::unordered_multimap<AbilityType,BaseAbility*>::iterator itr;
	for(itr=range.first;itr!=range.second;++itr){
		if(itr->second->State()){
			vector.push_back(itr->second);
		}
	}
}
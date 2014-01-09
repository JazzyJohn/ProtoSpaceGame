#include "BaseEffect.h"
using namespace Logic;
BaseEffectManager* BaseEffectManager::sigleton =0;
BaseEffect::BaseEffect(void)
{
}


BaseEffect::~BaseEffect(void)
{
}
BaseEffectManager::	BaseEffectManager(){
	BaseEffectManager::sigleton= this;
}
BaseEffectManager::~BaseEffectManager(){

	for(int i =0;i<mRandEffectCnt;i++){
		delete mRandEffect[i];
	}
	delete mRandEffect;
}
void BaseEffectManager::Init(BaseEffect **randEffect,int randEffectCnt){
	mRandEffect = randEffect;
	mRandEffectCnt = randEffectCnt;
	
	for(int i =0;i<mRandEffectCnt;i++){
		mAllEffect.push_back(mRandEffect[i]);
	}
	
}
BaseEffectManager* BaseEffectManager::GetSigleton(){
	return sigleton;
}

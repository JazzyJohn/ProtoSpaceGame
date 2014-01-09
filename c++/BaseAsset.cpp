#include "BaseAsset.h"

using namespace Logic;
BaseAsset* BaseAsset::sigleton =0;
BaseAsset::BaseAsset(void)
{
	sigleton= this;
}


BaseAsset::~BaseAsset(void)
{
}
BaseAsset* BaseAsset::GetSigleton(){

	return sigleton;
}
float BaseAsset::GetSigleton(Ogre::String name){
	return sigleton->mAssetMap[name];
}
const float& BaseAsset::operator[](Ogre::String idx)const {
	return mAssetMap.find(idx)->second;
	
}
void BaseAsset::Add(Ogre::String name,float value){
	if(mAssetMap.count(name)==0){
	mAssetMap[name] = value;
	}
}
void BaseAsset::Set(Ogre::String name,float value){
	
	mAssetMap[name] = value;

}
float  BaseAsset::Get(Ogre::String name){
		if(mAssetMap.count(name)!=0){
		return mAssetMap[name];
	}
		return 0;
}
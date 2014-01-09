#include "ClickHelper.h"
using namespace Control;
ClickHelper::ClickHelper(Logic::ClickHelperType type){
	mType = type;
	HelperManager::GetSigleton()->mAllHelper.insert(this);

}
HelperManager* HelperManager::sigleton =0;
HelperManager::HelperManager(){
	sigleton = this;
}
HelperManager::~HelperManager(){
	std::set<ClickHelper*>::iterator itr;
	for(itr=mAllHelper.begin();itr!=mAllHelper.end();++itr){
		delete (*itr);
	}

}
HelperManager* HelperManager::GetSigleton(){
	return sigleton;
}
#include "BaseResourceManager.h"
#include "BaseIntelegence.h"
#include "LogicStd.h"
using namespace Logic;
BaseResource::BaseResource(void): 
mQty(0),
	mName()
{
	mName = "NullName";

}
BaseResource::BaseResource(Ogre::String name): 
mQty(0)	
{
	mName = name;
	
//	delete name;
}
BaseResource::~BaseResource(void){
	
}

BaseResource &  BaseResource::operator+= (int  &Qty) {
	
	this->mQty += Qty;
	return *this;
}
BaseResource &  BaseResource::operator-= (int  &Qty) {
	
	this->mQty -= Qty;
	return *this;
}


BaseResourceObject::BaseResourceObject(void) :
mResource()
{
mName = "NullName";
}

BaseResourceObject::BaseResourceObject(Ogre::String name) :
mResource()
{
	
		mName = name;

}
BaseResourceObject::~BaseResourceObject(){
	
}
void BaseResourceObject::SetMap(std::map<BaseResource*,int> Resource){
	std::map<BaseResource*,int>::const_iterator itr;
	mTooltip="";
	for(itr = Resource.begin(); itr != Resource.end(); ++itr){
		mResource[itr->first] =itr->second;	
		mTooltip+=itr->first->mName;
		mTooltip+=" "+LogicStd::IntToString(itr->second)+" ";
	}

}
void BaseResourceObject::SetTooltip(){
	std::map<BaseResource*,int>::const_iterator itr;
	mTooltip="";
	for(itr = mResource.begin(); itr != mResource.end(); ++itr){
		mResource[itr->first] =itr->second;	
		mTooltip+=itr->first->mName;
		mTooltip+=" "+LogicStd::IntToString(itr->second)+" ";
	}
}
Ogre::String BaseResourceObject::GetTooltip(){
	return mTooltip;
}

SendProductionVar::SendProductionVar(){
	active= false;
}
BaseResourceManager::BaseResourceManager(void):
mAllResource(),mAllObjects(),mAllObjectsCount(),mObjectQueue(),mRefineryMap()
{

}
BaseResource * BaseResourceManager::addResource(Ogre::String name,int Qty ){
	BaseResource *temp = new BaseResource(name);
	temp->mQty= Qty;
	mAllResource[temp->mName]= temp; 
	return temp;
}
std::map<Ogre::String,int> BaseResourceManager::ShowResourse(void){
	std::map<Ogre::String,int> map;
	map["CREDIT"] = mAccount->GetCredit();
	std::map<Ogre::String,BaseResource*>::iterator itr;	
	for(itr = mAllResource.begin(); itr != mAllResource.end(); ++itr){
		map[itr->first]= itr->second->mQty;
	}
	return map;

}
void  BaseResourceManager::addResourceObject(BaseResourceObject *Object){
	mAllObjects[Object->mName] =Object;
	mAllObjectsCount[Object]=0;
	mAllObjectSort[Object->mType].push_back(Object);

}

void BaseResourceManager::addResource(BaseResourceObject *Object ){
	std::map<BaseResource*,int>::iterator itr;
	mAllObjectsCount[Object]++;
	for(itr = Object->mResource.begin(); itr != Object->mResource.end(); ++itr){
		
		
		(*itr->first)  +=itr->second;

	}
	
	
}
void BaseResourceManager::lowerResource(BaseResourceObject *Object ){
	std::map<BaseResource*,int>::iterator itr;
	mAllObjectsCount[Object]--;
	for(itr = Object->mResource.begin(); itr != Object->mResource.end(); ++itr){
		
		
		(*itr->first)  -=itr->second;

	}
	
	
}
void BaseResourceManager::addResourceQueue(Ogre::String name){
	if(mAllObjects[name]->mRefinerable){
	mObjectQueue.push(name);
	
	}
	addResource(mAllObjects[name]);
}
void BaseResourceManager::takeResourceObject(Ogre::String name){
	
	lowerResource(mAllObjects[name]);
	

}
bool BaseResourceManager::hasResource(BaseResourceObject *Object){
	return mAllObjectsCount[Object]>0;
}
bool BaseResourceManager::hasResource(BaseResourceObject *Object,int amount){
	return mAllObjectsCount[Object]>=amount;
}
BaseResourceObject * BaseResourceManager::SayRefinery(BaseResourceObject *Object){

	return mRefineryMap[Object].result;
}
bool  BaseResourceManager::GetQueueName(Ogre::String* name){
	if(mObjectQueue.empty()) return false;

	*name =mObjectQueue.front();
	mObjectQueue.pop();
	return true;
}
void BaseResourceManager::ReduceCredit(int cost){
	mAccount->ChangeCredit(-cost);
}
void  BaseResourceManager::AddCredit(int cost){
	mAccount->ChangeCredit(cost);
}
BaseResourceManager::~BaseResourceManager(void)
{
	std::map<Ogre::String,BaseResource*>::iterator resItr;
	for(resItr=mAllResource.begin();resItr!=mAllResource.end(); ++resItr){
		delete resItr->second;
		
	}
	std::map<Ogre::String,BaseResourceObject*>::iterator objItr;
	for(objItr=mAllObjects.begin();objItr!=mAllObjects.end(); ++objItr){
		delete objItr->second;
		
	}
	
}

std::vector<BaseResourceObject*> BaseResourceManager::GetAllResourse(void){
	std::vector<BaseResourceObject*> vector;
	
		std::map<Ogre::String,BaseResourceObject*>::iterator itr;	
	for(itr = mAllObjects.begin(); itr != mAllObjects.end(); ++itr){
		vector.push_back(itr->second);
	}
	return vector;

}
Production *  BaseResourceManager::SayProduction(BaseResourceObject *Object){
	return &mRefineryMap[Object];

}
bool BaseResourceManager::HasProduction(BaseResourceObject *Object,Production **prod){
	if(mRefineryMap.count(Object)>0){
		*prod = &mRefineryMap[Object];
		return true;
	
	}
	return false;
}
std::vector<BaseResourceObject*> BaseResourceManager::GetAllResourse(ResourceType type ){
	
	return mAllObjectSort[type];

}
BaseResourceObject* BaseResourceManager::GetResourceByName(Ogre::String name){
	if(mAllObjects.count(name)>0){
		return mAllObjects[name];
	}else{
		return mAllObjects.begin()->second;
	}
}
BaseResource *  BaseResourceManager::GetBaseResourceByName(Ogre::String name){
	if(mAllObjects.count(name)>0){
		return mAllResource[name];
	}else{
		return mAllResource.begin()->second;
	}
}
std::vector<BaseResourceObject*> BaseResourceManager::GetAllResourse(ResourceType type[],int size ){
		std::vector<BaseResourceObject*> vector;
	
		std::vector<BaseResourceObject*>::iterator itr;	
		for(int j =0;j<size;j++){
			for(itr=mAllObjectSort[type[j]].begin();itr!=mAllObjectSort[type[j]].end();++itr){
				vector.push_back((*itr));
			
			}
			
		}
	
	return vector;

}
void BaseResourceManager::addResourceProduction(BaseResourceObject *source,BaseResourceObject *result,int amount){
	mRefineryMap[source].result =result;
	mRefineryMap[source].from =source;
	mRefineryMap[source].amount =amount;
	mRefineryMap[source].refine= true;
}
void BaseResourceManager::addResourceProduction(std::map<BaseResourceObject*,int> recept,BaseResourceObject *result,int amount){
	mAllProduction.push_back(Production());
	Production *temp = &mAllProduction.back();
	temp->recept =recept;
	temp->result =result;
	temp->amount =amount;
	temp->refine = false;
}
void  BaseResourceManager::ShowAvaibleProduction(std::list<SendProductionVar> &productionVar,Production*curProduction){
	std::map<BaseResourceObject*,Production>::iterator iter;
	SendProductionVar * temp;
	for(iter=mRefineryMap.begin();iter!=mRefineryMap.end();++iter){
		if(mAllObjectsCount[iter->first]>0||&iter->second==curProduction){
			productionVar.push_back(SendProductionVar());
			temp =&productionVar.back();
			temp->amount= iter->second.amount;
			temp->fromPtn= &iter->second;
			if(curProduction==&iter->second){
				temp->active = true;
			}
			temp->refine = true;
			temp->from = iter->first->mName;
			temp->to = iter->second.result->mName;
		}
	}
	std::vector<Production>::iterator prodIter;
	std::map<BaseResourceObject*,int>::iterator recptItr;
	std::map<Ogre::String,int> recpt;
	bool hasresource=true;
	IntelegentManager * intRes= IntelegentManager::GetSigleton();
	for(prodIter=mAllProduction.begin();prodIter!=mAllProduction.end();++prodIter){
		recpt.clear();
		hasresource=true;
		for(recptItr= prodIter->recept.begin();recptItr!= prodIter->recept.end();++recptItr){
			if(recptItr->first->mType==RT_INT){
				hasresource=hasresource&&intRes->hasResource(recptItr->first,recptItr->second);
			}else{
				hasresource=hasresource&&mAllObjectsCount[recptItr->first]>=recptItr->second;
			}			
			recpt[recptItr->first->mName]= recptItr->second;
		}
		if(hasresource||prodIter._Ptr==curProduction){
			productionVar.push_back(SendProductionVar());
			temp =&productionVar.back();
			temp->amount= prodIter->amount;
			temp->fromPtn=prodIter._Ptr;
			if(curProduction==prodIter._Ptr){
				temp->active = true;
			}
			temp->refine = false;
			temp->recept= recpt;
			temp->to =prodIter->result->mName;
		
		}
		
	}
}
void BaseResourceManager::ShowObjectsAll(std::map<Ogre::String,int> &amount,std::map<Ogre::String,BaseResourceObject*> &links){
	std::map<BaseResourceObject*,int>::iterator itr;	
	for(itr = mAllObjectsCount.begin(); itr != mAllObjectsCount.end(); ++itr){
		if(itr->second>0){
		amount[itr->first->mName]= itr->second;
		links[itr->first->mName]= itr->first;
		}
	}
}
void BaseResourceManager::ShowObjectsAll(std::vector<ResourseSend> &allResourse){
	std::map<BaseResourceObject*,int>::iterator itr;	
	
	ResourseSend *temp;
	for(itr = mAllObjectsCount.begin(); itr != mAllObjectsCount.end(); ++itr){
		if(itr->second>0){
		allResourse.push_back(ResourseSend());
		temp =&allResourse.back();
		
		temp->name = itr->first->mName;
		temp->object = itr->first;
		}
	}
}
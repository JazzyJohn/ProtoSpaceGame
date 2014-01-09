#include "BaseRequestManager.h"
#include "BaseListener.h"
#include "BaseLogic.h"
using namespace Logic;
const int GENERATECHANCE=10;
const int GENERATEPROJECTCHANCE=0;
BaseRequestManager::~BaseRequestManager(){
	delete mCorparate;
	for(int i =0; i<mReqCnt;i++){
		delete mAllRequest[i];
	}
	delete[] mAllRequest;
	delete mCorparate;
	for(int i =0; i<mProjectCnt;i++){
		delete mAllProjects[i];
	}
	delete[] mAllProjects;
}
void BaseRequestManager::Init(BaseRequest** allRequest,int reqCnt,BaseCorporate *corp,BaseResourceManager *resMan,IntelegentManager *intMan,	BaseProject** allProject,int projectCnt,  BaseCoreManager *coreMan){
	mAllRequest = allRequest;
	mReqCnt = reqCnt;
	mTarget=0;
	mCorparate =corp;
	mResMan =resMan;
	mCoreManager =coreMan;
	mReadyToSend = false;
	mAllProjects = allProject;
	mProjectCnt=projectCnt;
	mIntMan =intMan;
}
void BaseRequestManager::GenerateNewPending(){
	int a = std::rand() % mReqCnt;
	if(mPendingRequest.count(mAllRequest[a])==0){
		mPendingRequest[mAllRequest[a]] = 0;
	}
}
bool BaseRequestManager::GenerateNewProject(int chance){

	int a = std::rand() % mProjectCnt;
	if(mCorparate->mDepartmetRelation[mAllProjects[a]->mCurResource]->GetTotalKpd()>chance){
		if(mSujestProject.find(mAllProjects[a])==mSujestProject.end()&&mPendingProjects.find(mAllProjects[a])==mPendingProjects.end()){
			mSujestProject.insert(mAllProjects[a]);
			return true;
		}
	}
	return false;
}
bool BaseRequestManager::AddObjectToRequest(BaseResourceObject *object){
	if(!mReadyToSend){
		if(mPendingMap.count(object)==0){
			mPendingMap[object]=0;
		}
		if((object->mType!=RT_INT&&mResMan->hasResource(object,mPendingMap[object]+1))||(object->mType==RT_INT&&mIntMan->hasResource(object,mPendingMap[object]+1))){
			mReadyToSend =mTarget->AddResource(object);
			mPendingMap[object]++;
		}


	}
	return mReadyToSend;
}
bool BaseRequestManager::DeleteObjectToRequest(BaseResourceObject *object){


	if(mPendingMap.count(object)==0){
		return false;
	}
	mPendingMap[object]--;
	if(mPendingMap[object]<0){
		mPendingMap[object]=0;
	}else{
		mReadyToSend =mTarget->DeleteResource(object);
	}


	

	return mReadyToSend;
}
bool BaseRequestManager::SendRequest(){
	if(mReadyToSend){
		std::map<BaseResourceObject *,int>::iterator resItr;
		int i =0;
		for(resItr= mPendingMap.begin(); resItr!=mPendingMap.end();++resItr){
			if(resItr->first->mType!=RT_INT){
				if(!mResMan->hasResource(resItr->first,resItr->second)){
					return false;							
				}
			}else{
				if(!mIntMan->hasResource(resItr->first,resItr->second)){
					return false;							
				}

			}
		}
		for(resItr= mPendingMap.begin(); resItr!=mPendingMap.end();++resItr){
			if(resItr->first->mType!=RT_INT){
				for(i=0;i<resItr->second;++i){	

					mCoreManager->GetResource(resItr->first->mName);

				}
			}
			else{
				mIntMan->GetResource(resItr->first,resItr->second);
			}
		}
		int credit;
		int relation;
		mTarget->SendBack(credit,relation);
		credit  =mCorparate->mCreditCoef*credit;
		mPendingRequest.erase(mTarget);
		mPendingMap.clear();
		mResMan->AddCredit(credit);
		mCorparate->mDepartmetRelation[mTarget->mCurResource]->RequestComplite(relation,credit);
		mReadyToSend= false;
		mTarget= 0;
		return true;
	}
	return false;
}
void BaseRequestManager::DeclineRequest(){
	mTarget->SendBack();
	mReadyToSend=false;
	mTarget= 0;
	mPendingMap.clear();
}
GameEvent*  BaseRequestManager::ReturnAllPendingWithStatus(){
	ShowAllRequest* evnt = new ShowAllRequest();
	std::map<BaseRequest*,int>::iterator itrMap;
	RequestForSend *temp;
	int i=0;
	for(itrMap = 	mPendingRequest.begin();itrMap!= mPendingRequest.end();itrMap++){
		evnt->allRequest.push_back(RequestForSend());
		temp = &(evnt->allRequest[i]);
		i++;
		temp->dep = mCorparate->mDepartmetRelation[itrMap->first->mCurResource]->GetName();
		temp->desc = itrMap->first->mDescription;
		temp->request =itrMap->first;
		temp->timeleft = LogicStd::IntToString(itrMap->first->mTimeOf-itrMap->second);
	}

	std::set<BaseProject*>::iterator itrList;
	ProjectForSend *tempPro;
	i=0;
	for(itrList = 	mSujestProject.begin();itrList!= mSujestProject.end();itrList++){
		evnt->allProject.push_back(ProjectForSend());
		tempPro = &(evnt->allProject[i]);
		i++;
		tempPro->dep = mCorparate->mDepartmetRelation[(*itrList)->mCurResource]->GetName();
		tempPro->desc = (*itrList)->mDescription;
		tempPro->name = (*itrList)->mDescription;
		tempPro->project =(*itrList);

	}
	i=0;
	for(itrList = 	mPendingProjects.begin();itrList!= mPendingProjects.end();itrList++){
		evnt->allPendingProject.push_back(ProjectForSend());
		tempPro = &(evnt->allPendingProject[i]);
		i++;
		tempPro->dep = mCorparate->mDepartmetRelation[(*itrList)->mCurResource]->GetName();
		tempPro->desc = (*itrList)->mDescription;
		tempPro->name = (*itrList)->mDescription;
		tempPro->project =(*itrList);

	}
	return evnt;
}
BaseProject* BaseRequestManager::GetProjectById(int id){
	
	for(int i =0; i<mProjectCnt;i++){
		if(mAllProjects[i]->mId==id){
			return mAllProjects[i];
		}
		
	}
	 return mAllProjects[0];
}
BaseRequest* BaseRequestManager::GetRequestById(int id){
	for(int i =0; i<mReqCnt;i++){
		if(mAllRequest[i]->mId==id){
			return mAllRequest[i];
		}
	}
	return mAllRequest[0];
}
void BaseRequestManager::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	std::map<BaseRequest*,int>::iterator reqItr;
	std::queue<BaseRequest*> que;
	for(reqItr= mPendingRequest.begin();reqItr !=mPendingRequest.end();++reqItr){
		reqItr->second++;
		if(reqItr->second>reqItr->first->mTimeOf){
			reqItr->first->SendBack();
			que.push(reqItr->first);
		};
	}

	NotificationEvent *temp;
	while(!que.empty()){
		mPendingRequest.erase(que.front());
		temp = new NotificationEvent();
		temp->name = mCorparate->mDepartmetRelation[que.front()->mCurResource]->GetName();
		temp->target = que.front();
		temp->notType =NTE_REQUESTEEXPIRATE;
		notificationEvent.push(temp);
		que.pop();
	}

	int a = std::rand() % 100;
	if(a<GENERATECHANCE){
		GenerateNewPending();
	}
	a= std::rand()%1000;
	PeriodPass(notificationEvent);
	if(GenerateNewProject(a)){


	}

}
GameEvent* BaseRequestManager::GiveSendStatus(BaseRequest*target){
	mTarget= target;
	return GiveSendStatus();
}
GameEvent*  BaseRequestManager::GiveSendStatus(){
	ShowSendStatus *evnt = new ShowSendStatus();
	mResMan->ShowObjectsAll(evnt->allResourse);
	mIntMan->ShowObjectsAll(evnt->allResourse);
	std::vector<ResourseSend>::iterator resItr;
	for(resItr=  evnt->allResourse.begin();resItr!=  evnt->allResourse.end();++resItr){
		if(mPendingMap.count(resItr->object)){
			resItr->amount = mPendingMap[resItr->object];
		}else{
			resItr->amount = 0;
		}
	}
	evnt->needAmout = mTarget->mAmount;
	evnt->nowAmount = mTarget->mCurAmount;
	return evnt;
}

GameEvent* BaseRequestManager::GiveProjectStatus(BaseProject*target){
	mProjTarget= target;
	return GiveProjectStatus();
}

GameEvent* BaseRequestManager::GiveProjectStatus(){
	ShowProjectSendStatus *evnt = new ShowProjectSendStatus();
	mResMan->ShowObjectsAll(evnt->allResourse);
	mIntMan->ShowObjectsAll(evnt->allResourse);
	std::vector<ResourseSend>::iterator resItr;
	for(resItr=  evnt->allResourse.begin();resItr!=  evnt->allResourse.end();++resItr){
		if(mPendingMap.count(resItr->object)){
			resItr->amount = mPendingMap[resItr->object];
		}else{
			resItr->amount = 0;
		}
	}
	evnt->needAmout = mProjTarget->mPeriodNeed;
	evnt->periodtype= mProjTarget->mType;
	evnt->nowAmount = mProjTarget->mPeriodProgress;
	evnt->periodCnt = mProjTarget->mPeriodCnt;
	evnt->periodComplite= mProjTarget->mPeriodCntComplite;
	evnt->nowComplite = mProjTarget->mPeriodComplite;
	evnt->name = mProjTarget->mName;
	return evnt;


}
bool BaseRequestManager::AddObjectToProject(BaseResourceObject *object){
	if(!mReadyToSend){
		if(mPendingMap.count(object)==0){
			mPendingMap[object]=0;
		}
		if((object->mType!=RT_INT&&mResMan->hasResource(object,mPendingMap[object]+1))||(object->mType==RT_INT&&mIntMan->hasResource(object,mPendingMap[object]+1))){
			mPendingMap[object]++;
		}
		mReadyToSend =mProjTarget->isDone(mPendingMap);

	}
	return mReadyToSend;
}
bool BaseRequestManager::DeleteObjectToProject(BaseResourceObject *object){


	if(mPendingMap.count(object)==0){
		return false;
	}
	mPendingMap[object]--;
	if(mPendingMap[object]<0){
		mPendingMap[object]=0;
	}


	mReadyToSend =mProjTarget->isDone(mPendingMap);

	return mReadyToSend;
}
bool BaseRequestManager::SendProject(){

	std::map<BaseResourceObject *,int>::iterator resItr;
	int i =0;
	for(resItr= mPendingMap.begin(); resItr!=mPendingMap.end();++resItr){
		if(resItr->first->mType!=RT_INT){
			if(!mResMan->hasResource(resItr->first,resItr->second)){
				return false;							
			}
		}else{
			if(!mIntMan->hasResource(resItr->first,resItr->second)){
				return false;							
			}

		}
	}
	for(resItr= mPendingMap.begin(); resItr!=mPendingMap.end();++resItr){
		if(resItr->first->mType!=RT_INT){
			for(i=0;i<resItr->second;++i){	

				mCoreManager->GetResource(resItr->first->mName);

			}
		}
		else{
			mIntMan->GetResource(resItr->first,resItr->second);
		}

		mProjTarget->AddObject(resItr->first,resItr->second);
	}

	mReadyToSend= mProjTarget->mPeriodComplite;
	mPendingMap.clear();

	return false;
}
void  BaseRequestManager::DeclineProject(){
	mReadyToSend=false;
	mProjTarget= 0;
	mPendingMap.clear();
}



void BaseRequestManager::PeriodPass(std::queue<NotificationEvent*> &notificationEvent){

	std::set<BaseProject*>::iterator projItr;
	std::queue<BaseProject*> tempForClear;
	bool  succsses;
	NotificationEvent* not;
	int credit=0,relation=0;
	for(projItr= mPendingProjects.begin();projItr!= mPendingProjects.end();++projItr){
		if((*projItr)->EndPeriod()){
			succsses=mCorparate->mDepartmetRelation[(*projItr)->mCurResource]->GetProjectResult();
			not = new 	NotificationEvent();
			if((*projItr)->Finish(succsses,credit,relation)){
				not->notType = NTE_PROJECTSUCCSESS;
				succsses=true;
			}else{
				if((*projItr)->mAllComplite){
					not->notType = NTE_PROJECTFAILURE;
				}else{
					not->notType = NTE_PROJECTFAILUREUSERBY;				
				}
				succsses= false;

			}
			tempForClear.push((*projItr));
			mCorparate->mDepartmetRelation[(*projItr)->mCurResource]->ProjectComplite(succsses,credit,relation);
			mResMan->AddCredit(credit);
			not->name = (*projItr)->mName;
			not->target= (*projItr);
			if((*projItr)->mIsCallback){
				BaseLogic::GetSigleton()->CallBack((*projItr)->mCallType,(*projItr));
			}
		}	
	}
	while(!tempForClear.empty()){
		mPendingProjects.erase(tempForClear.front());
		tempForClear.pop();
	}

}

GameEvent* BaseRequestManager::AcceptProject(BaseProject *project){
	mSujestProject.erase(project);
	mPendingProjects.insert(project);
	return ReturnAllPendingWithStatus();
}
GameEvent* BaseRequestManager::DeclineProject(BaseProject *project){
	mSujestProject.erase(project);

	return ReturnAllPendingWithStatus();
}
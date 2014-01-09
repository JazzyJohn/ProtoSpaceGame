#include "BaseCorporate.h"
#include "BaseListener.h"
using namespace Logic;

BaseRequest::BaseRequest(BaseResource * curResource){
	mCurResource = curResource;
	mCurAmount =0;
}
bool BaseRequest::AddResource(BaseResourceObject *object){
	mCurAmount += object->mResource[mCurResource];
	return mCurAmount>=mAmount;

}
bool BaseRequest::DeleteResource(BaseResourceObject *object){
	mCurAmount -= object->mResource[mCurResource];
	if(mCurAmount<0){
		mCurAmount=0;
	}
	return mCurAmount>=mAmount;
}
void BaseRequest::SendBack(int &creditBonus, int &relationDelta){
	if(mCurAmount>=mAmount){
		creditBonus = mSuccsessCredit;
		relationDelta= mSuccsessRelation;
	}
	mCurAmount=0;
}
void BaseRequest::SendBack(){
	mCurAmount=0;
}
BaseProject::BaseProject(BaseResource * curResource){
	mCurResource = curResource;
	mPeriodProgress=0;
	mPeriodComplite = false;
	mPeriodCntComplite=0;
	mAllComplite=false;
	mDaysFromStart=0;
	
}
bool BaseProject::AddObject(BaseResourceObject * object,int amount){
	if(mPeriodComplite){
		return true;
	}
	mPeriodProgress+= object->mResource[mCurResource]*amount;
	if(mPeriodProgress>mPeriodNeed){
		mPeriodComplite=true;	
		mPeriodCntComplite++;
		if(mPeriodCntComplite>=mPeriodCnt){
			mAllComplite = true;
		}
	}
	return mPeriodComplite;
}

bool BaseProject::EndPeriod(){
	mDaysFromStart++;
	bool ifperiodend= false;
	switch(mType){
	case PTP_WEEKLY:
		ifperiodend= mDaysFromStart>=7;
		break;
	case PTP_MONTHLY:
		ifperiodend= mDaysFromStart>=30;
		break;
	
	}
	if(ifperiodend){
		mDaysFromStart=0;
		mPeriodProgress =0;
		if(mPeriodComplite){
			if(mAllComplite){
				return true;
			}else{
				return false;
			}
		}else{
			return true;		
		}	
	}
	return false;
		
	
	

}
void BaseProject::Clear(){
	mPeriodProgress=0;
	mPeriodComplite = false;
	mPeriodCntComplite=0;
	mAllComplite=false;
	mDaysFromStart=0;
}
bool BaseProject::Finish(bool succses,int &credit, int &relation){
	if(mAllComplite){
		if(succses){
			credit = mSuccsessCredit;
			relation =mSuccsessRelation;
			return true;
		}else{
			credit = mRefundCredit;
			relation =mSuccsessRelation;
			return false;
		}

	}
	else{
		credit = 0;
		relation =mUnpaidFaliureRelation;
		return false;
	}
	Clear();

}
bool BaseProject::isDone(	std::map<BaseResourceObject *,int> pendingMap){
	int total=mPeriodProgress;
		std::map<BaseResourceObject *,int>::iterator itr;
		for(itr= pendingMap.begin();itr!=pendingMap.end();++itr){
			total+= itr->first->mResource[mCurResource]*itr->second;
		}
	return  total>=mPeriodNeed;
		
}
BaseDepartment::BaseDepartment(){
	mRequestComplite=0;
	mResearchComplite=0;
	mProjectComplite=0;
	mProjectFailed=0;
	mProjectCompliteNPC=0;
	mProjectFailedNPC=0;
	mTotalKPD=0;
	mCreditSend=0;
	mTimeFromLast=0;
	mSuccsessRate =0;
}
BaseDepartment::BaseDepartment(BaseResource * curResource){
	mRequestComplite=0;
	mResearchComplite=0;
	mProjectComplite=0;
	mProjectFailed=0;
	mProjectCompliteNPC=0;
	mProjectFailedNPC=0;
	mTotalKPD=0;
	mCreditSend=0;
	mTimeFromLast=0;
	mSuccsessRate =0;
	mCurResource=curResource;
	
}
BaseDepartment::~BaseDepartment(){

}
void BaseDepartment::RequestComplite(int relation,int creditSend){
	mRequestComplite++;
	mTimeFromLast = 0;
	mTotalKPD+= relation;
	mCreditSend +=creditSend; 
}
int BaseDepartment::GetTotalKpd(){
	return mTotalKPD;
}

int BaseDepartment::SayNewsCoef(){
	if((mProjectCompliteNPC+mProjectComplite+mProjectFailedNPC+mProjectFailed)==0){
		return BaseAsset::GetSigleton("BASE_NEWS_COEF");
	}
	return  BaseAsset::GetSigleton("BASE_NEWS_COEF")+ ( (mProjectCompliteNPC+mProjectComplite)*2/(mProjectCompliteNPC+mProjectComplite+mProjectFailedNPC+mProjectFailed) -1)*20;

}
void  BaseDepartment::NewsEffect(BaseNews* news){
	if(news->mProjectNpcDelta<0){
		mProjectFailedNPC++;
	}else{
		mProjectCompliteNPC++;
	}
	mTotalKPD +=news->mTotalKPDDelta;
	mSuccsessRate += news->mSuccsessRateDelta;
	if(mSuccsessRate>BaseAsset::GetSigleton("SUCCSESS_MAX")){
		mSuccsessRate=BaseAsset::GetSigleton("SUCCSESS_MAX");
	}
}
Ogre::String BaseDepartment::GetName(){
	return mCurResource->mName;
}
bool BaseDepartment::GetProjectResult(){
	
	int a=std::rand() %100;	
	return mSuccsessRate> a;
}
void  BaseDepartment::ProjectComplite(bool result,int relation,int creditSend){
	if(result){
		mProjectComplite++;
	}else{
		mProjectFailed++;
	}
	mTotalKPD+= relation;
	mCreditSend+= creditSend;

}

BaseCorporate::BaseCorporate()
{

}
BaseCorporate::~BaseCorporate(void){
	std::vector<HistoryNews*>::iterator newItr;
	for(newItr = mAllNewsHistory.begin();newItr!= mAllNewsHistory.end();newItr++){
	
		delete *newItr;
	}

}

void BaseCorporate::WeaklyNews(){
	std::map<BaseResource *,BaseDepartment*>::iterator detItr;
	
	int a = 0;
	HistoryNews*temp;
	for(detItr = mDepartmetRelation.begin();detItr!= mDepartmetRelation.end();++detItr){
		a=std::rand() %100;	
		if(a<BaseAsset::GetSigleton("NEWS_RATE")){
			
			temp = new HistoryNews();
			temp->department = detItr->second;
			a=std::rand() %100;	
			if(a<detItr->second->SayNewsCoef()){
				a=std::rand() %mGoodNewsCnt;	
				temp->news = mGoodNews[a];
			}else{
				a=std::rand() %mBadNewsCnt;	
				temp->news = mBadNews[a];
			}
			detItr->second->NewsEffect(temp->news);
			mAllNewsHistory.push_back(temp);
			mPendingNews.push(temp);
		}
	}

}
void BaseCorporate::Init(BaseResource ** curResource,int amount,BaseNews **goodNews,int goodNewsCnt,BaseNews **badNews,	int badNewsCnt){
	for(int i =0;i<amount;i++){
		mDepartmetRelation[curResource[i]] = new  BaseDepartment(curResource[i]);
	}
	mCreditCoef =1;
	mGoodNews = goodNews;
	mGoodNewsCnt = goodNewsCnt;
	mBadNews = badNews;
	mBadNewsCnt = badNewsCnt;
}

bool BaseCorporate::FireNiews(GameEvent **exEvnt){

	if(!mPendingNews.empty()){
		NewNews*evnt = new NewNews();
		evnt->descr = mPendingNews.front()->news->mDescription;
		evnt->depname = mPendingNews.front()->department->GetName();
		evnt->pointer = mPendingNews.front();
		mPendingNews.pop();
		*exEvnt = evnt;
		return true;
	}
	return false;
}

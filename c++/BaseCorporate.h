#pragma once
#include "BaseResourceManager.h";
#include "BaseConst.h";

namespace Logic{
class BaseRequest
{
public:
	BaseRequest(BaseResource * curResource);
	~BaseRequest(void){};
	BaseResource *mCurResource;
	bool AddResource(BaseResourceObject *object);
	bool DeleteResource(BaseResourceObject *object);
	bool AddToQuery();
	void SendBack(int &creditBonus, int &relationDelta);
	void SendBack();
	int mAmount;
	int mCurAmount;
 	int mTimeOf;
	int mSuccsessCredit;
	int mSuccsessRelation;
	Ogre::String mDescription;
	int mId;
};

class BaseProject{
public:
	BaseProject(BaseResource * curResource);
	~BaseProject(void){};
	bool AddObject(BaseResourceObject * object,int amount=1);
	bool EndPeriod();
	void Clear();
	bool isDone(	std::map<BaseResourceObject *,int> pendingMap);
	bool Finish(bool succses,int &credit, int &relation);
	Ogre::String mName;
	Ogre::String mDescription;
	BaseResource *mCurResource;
	int mPeriodNeed;
	int mPeriodProgress;
	bool mPeriodComplite;
	int mPeriodCnt;
	int mPeriodCntComplite;
	bool mAllComplite;
	int mDaysFromStart;
	ProjectPeriodType mType;	
	int mSuccsessCredit;
	int mSuccsessRelation;
	int mUnpaidFaliureRelation;
	int mRefundCredit;
	bool mIsCallback;
	ProjectCallBackType mCallType;
	int mId;
    
};
class BaseNews{
	public:
		BaseNews(){};
		~BaseNews(){};
	Ogre::String mDescription;
	float mSuccsessRateDelta;
	int mProjectNpcDelta;
	int mTotalKPDDelta;
	
};

class BaseDepartment
{
	friend class  boost::serialization::access;
public:
	LOADSAVE_FUNCTION_GENERATE() 
		BaseDepartment();
	BaseDepartment(BaseResource * curResource);
	~BaseDepartment();
	BaseResource *mCurResource;
	void RequestComplite(int relation,int creditSend);
	void ProjectComplite(bool result,int relation,int creditSend);
	int SayNewsCoef();
	void NewsEffect(BaseNews* news);
	Ogre::String GetName();
	bool GetProjectResult();
	int GetTotalKpd();
protected:
	int mTimeFromLast;
	int mRequestComplite;
	int mResearchComplite;
	int mProjectComplite;
	int mProjectFailed;
	int mProjectCompliteNPC;
	int mProjectFailedNPC;
	int mTotalKPD;
	int mCreditSend;
	float mSuccsessRate;
	
};
struct HistoryNews{
	BaseNews* news;
	BaseDepartment* department;
};
class BaseCorporate
{
	friend class  boost::serialization::access;
public:
		LOADSAVE_FUNCTION_GENERATE() 
	BaseCorporate();
	void Init(BaseResource ** curResource,int amount,BaseNews **goodNews,int goodNewsCnt,BaseNews **badNews,	int badNewsCnt);
	~BaseCorporate(void);
	void WeaklyNews();
	bool FireNiews(GameEvent **evnt);
	std::map<BaseResource *,BaseDepartment*> mDepartmetRelation;
	float mCreditCoef;

protected:
	std::queue<HistoryNews*> mPendingNews;
	std::vector<HistoryNews*> mAllNewsHistory;
	BaseNews **mGoodNews;
	int mGoodNewsCnt;
	BaseNews **mBadNews;
	int mBadNewsCnt;

};
struct RequestForSend{
	Ogre::String dep;
	Ogre::String desc;
	Ogre::String timeleft;
	BaseRequest *request;
};
struct ProjectForSend{
	Ogre::String dep;
	Ogre::String desc;
	Ogre::String name;
	BaseProject *project;
};

}


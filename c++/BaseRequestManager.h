#pragma once
#include "BaseBuilding.h";
#include "BaseIntelegence.h";
#include "BaseCorporate.h"
namespace Logic{

class BaseRequestManager{
friend class  boost::serialization::access;
public:
	LOADSAVE_FUNCTION_GENERATE() 
	BaseRequestManager(){};
	~BaseRequestManager();
	void Init(BaseRequest** allRequest,int reqCnt,
		BaseCorporate *corp,BaseResourceManager *resMan,IntelegentManager *intMan,
		BaseProject** allProject,int projectCnt, 
		BaseCoreManager *coreMan);
	void GenerateNewPending();
	bool GenerateNewProject(int a);
	bool AddObjectToRequest(BaseResourceObject *object);
	bool DeleteObjectToRequest(BaseResourceObject *object);
	bool AddObjectToProject(BaseResourceObject *object);
	BaseProject* GetProjectById(int);
	BaseRequest* GetRequestById(int);
	bool DeleteObjectToProject(BaseResourceObject *object);
	bool SendRequest();
	void DeclineRequest();
	bool SendProject();
	void DeclineProject();
	GameEvent* ReturnAllPendingWithStatus();
	GameEvent* AcceptProject(BaseProject *project);
	GameEvent* DeclineProject(BaseProject *project);
	GameEvent* GiveSendStatus();
	GameEvent* GiveSendStatus(BaseRequest*target);
	GameEvent* GiveProjectStatus();
	GameEvent* GiveProjectStatus(BaseProject*target);

	void PassDay(std::queue<NotificationEvent*> &notificationEvent);
	void PeriodPass(std::queue<NotificationEvent*> &notificationEvent);
protected:
	BaseResourceManager *mResMan;
	IntelegentManager *mIntMan;
	BaseCoreManager *mCoreManager;
	int mReqCnt;
	BaseRequest** mAllRequest;
	BaseCorporate* mCorparate;
	std::map<BaseRequest*,int> mPendingRequest;
	std::map<BaseResourceObject *,int> mPendingMap;
	
	BaseRequest* mTarget;
	BaseProject* mProjTarget;
	bool mReadyToSend;
	std::set<BaseProject*> mPendingProjects;
	std::set<BaseProject*> mSujestProject;
	BaseProject** mAllProjects;
	int mProjectCnt;
};

}


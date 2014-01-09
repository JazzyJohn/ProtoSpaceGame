#include "BaseCounrty.h"
#include "BaseBuilding.h"
#include "BaseResourceManager.h"
#include "BaseConst.h"
#pragma once
namespace Logic{
	
struct BaseAction{
	Ogre::String name;
	ActonType type;
	bool doneInSecret;
	bool open;
	int dayCount;
};
struct ActionIntDialog : public GameEvent {
	ActionIntDialog();
	~ActionIntDialog(){};
	BaseCountry *selected;
	std::map<Ogre::String,BaseCountry *> allCountry;
	bool canBeReason;
	BaseCountry *reason;
};
class ActionManager{
	public:
		ActionManager(void){};
		~ActionManager(void);
	   void Init(BaseAction **allAction,int actionCount);
	   void SayAction(std::map<Ogre::String , BaseAction*> &actions,ActonType type);
	   void Open(Ogre::String);
	private:
		BaseAction **mAllAction;
		std::unordered_map<Ogre::String,BaseAction *> mActionMap;
		int mActionCount;
	};
class IntelegentManager{
public:
	IntelegentManager();
	~IntelegentManager();
	void GenerateResource(ActonType type);
	bool GetResource(BaseResourceObject* obj,int amount=1);
	void MakeAll(  std::vector<BaseResourceObject*>blackVector,  std::vector<BaseResourceObject*>embassyVector,  std::vector<BaseResourceObject*>reconVector);
	GameEvent* GiveActionDialog(BaseBuilding *tempBilding,BaseAction *action);
	GameEvent* SetActionDialogSomething(BaseCountry *tempTarget,bool isTarget);
	GameEvent* DeclineActionDialog();
	GameEvent* AceptActionDialog();
	void ShowObjectsAll(std::map<Ogre::String,int> &amount,std::map<Ogre::String,BaseResourceObject*> &links);
	void ShowObjectsAll(std::vector<ResourseSend> &allResourse);
	static IntelegentManager* GetSigleton();
	bool hasResource(BaseResourceObject *Object,int amount=1);
private:
	static IntelegentManager* sigleton;
	GameEvent* RefreshEvent();
	std::map<BaseResourceObject*,int> mStockpile;
	BaseResourceObject** mBlackResource;
	int mBlackSize;
	BaseResourceObject** mEmbasyResource;
	int mEmbasySize;
	BaseResourceObject** mReconResource;
	int mReconSize;
	
  	BaseAction *mTempAction;
	BaseBuilding *mTempBuilding;
	BaseCountry *mTempTarget;
	BaseCountry *mTempReason;
};
class BaseEmbassyBuilding : public NonCoreBuilding
{
public:
	BaseEmbassyBuilding(void);
	BaseEmbassyBuilding(ActonType type);
	~BaseEmbassyBuilding(void){};

	void PassDay(std::queue<NotificationEvent*> &notificationEvent);
	void SetAction(BaseAction *action,BaseCountry *target);
	void SetAction(BaseAction *action,BaseCountry *target,BaseCountry *reason);
	
	void SetManager(IntelegentManager *intMan,ActionManager *actMan);
    GameEvent *ReturnBuildingInfo();
	ActonType mIntType;
private:
	
	IntelegentManager *mIntMan;
	ActionManager *mActMan;
	int dayProduction;
	int informdayCollection;
	BaseAction *mAction;
	double mProdNeed;
	BasePlanetCountry *mTarget;
	BasePlanetCountry *mReason;
};
}

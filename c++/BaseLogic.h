#include "BaseLogicLua.h"
#include "BaseAI.h"
#include "SaveLoad.h"

#pragma once
namespace Logic{
	
	struct GameDate{
		int days;
		int year;
		int month;
	};
	struct Player{
		float score;
		Ogre::String name;
	};
	
class BaseLogic
{
	   friend class boost::serialization::access;
public:
	BaseLogic(void);
    BaseLogic(BaseListener* listener);
	~BaseLogic(void);
	virtual bool timeElapse(float time);
	virtual void Load(void);
	virtual void Init(void);
	virtual void getFormatedTime(Ogre::String* outbuffer,TimeFormat form) const;
	virtual void gameLoop(PlanetType);
	virtual void CalculateGameLogic();
	void CallBack(ProjectCallBackType type, BaseProject *project);
	void SetMutex(boost::mutex *other);
	void SetState(GameState state);
	
	int GetDay();
	BaseCoreManager *GetCore();
	void InitiateLua();
	float mLoadState;
	void fireGameEvent(ControlEvent *evnt) ;
	virtual void resolvedQueue()  ;
	std::queue<ControlEvent*> mGameEventStack;
	static BaseLogic * GetSigleton();
	BaseLogicLua mLuaMan;
	BaseResourceManager mResMng;
	BaseWarfare mWarFare;
	AbilityManager mAbilityMng;
	void DestroyBuilding(BaseBuilding*);
	void DeleteResolved();
	LOADSAVE_FUNCTION_GENERATE()  
protected:
	SaveLoad mSaveMan;
	static BaseLogic * sigleton;
	float mTime;
	bool mPause;
	PlanetType mGameType;
	int mTimeWarp;
	int mDay;
	DateChageType mDeltaDate;
	DateChageType mDeltaWeek;
	boost::mutex *mMutex;
	GameState mGameState;
	BaseListener* mListener;
	BaseEventGenerator mEventGenerator;
	BaseReasearchManager mReseasrchMng;
	IntelegentManager mIntMng;	
	BaseCoreManager mCoreMng;
	BaseUnitManager mUnitMng;
	BaseAIManager mAiMng;
	CharacteristicManager mCharMng;
	ActionManager mActionMng;
	BaseAsset mAssetMng;
	ReconManadger mRecMng;
	BaseCorporate mCorp;
	BasePlanet mPlanet;
	BaseAccount mAccount;
	GameDate mGameDate;
	BaseEffectManager mEffMan;
	
	Player mPlayer;
	BaseNotificator mNotificator;
	BaseRequestManager mRequestMan;
	PointerGuard<BaseBuilding> mBuildingPointer; 
	std::queue<BaseBuilding*> mToDeleteBuilding;
	PointerGuard<BaseUnit> mUnitPointer; 
	PointerGuard<BaseBattle> mBattlePointer; 
	void getFormatedDateTime(Ogre::String* outbuffer) const;
};
}


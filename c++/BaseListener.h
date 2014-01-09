#include <OgreRoot.h>
#include <stack>
#include "BaseConst.h"
#include "BaseBuilding.h"
#include "BaseCorporate.h"
#include "BaseIntelegence.h"
#pragma once
namespace Logic{
	struct BuildingForSent{
		~BuildingForSent(){};
		BaseBuilding* building;
		TYPE_OF_BUILDING buildingType;
		Ogre::String name;
		BaseCore *Core;
		bool empty;
	};

	struct TimeGameEvent : public BaseEvent {
		DateChageType timeType;

	};

	struct ShowResourceEvnt : public GameEvent{
		ShowResourceEvnt();
		std::map<Ogre::String,int> mResorseCnt;
	};
	struct BuildingBuiltEvnt : public GameEvent{
		BuildingBuiltEvnt(); 
		BaseBuilding *building;
		TYPE_OF_BUILDING buildingType;

	};
	struct CoreBuiltEvnt : public GameEvent{
		CoreBuiltEvnt(); 
		BaseBuilding *building;
		CountryTile * tile;
		BaseCountry* country;

	};
	
	struct SimpleBuiltSend : public GameEvent{
		SimpleBuiltSend();
		Ogre::Entity *bEntitty;
	};
	struct SendCoreToGui : public GameEvent{
		SendCoreToGui(); 
		~SendCoreToGui(); 
		BuildingForSent **allSlots;
		BaseCore *target;
		int SlotCnt;
	};
	struct SendBlueToGui : public GameEvent{
		SendBlueToGui(); 
	
		std::vector<BuildingBlueprint *> allBlue;
	};
	struct ShowMining: public GameEvent{
		ShowMining();
		std::map<Ogre::String,ResourseDeposits *> allDeposit;
		Ogre::String curDeposit;
		float procent;
		BaseBuilding *target;
	};
	struct ShowWarehouse: public GameEvent{
		ShowWarehouse();
		~ShowWarehouse();
		Ogre::String *allSlot;
		int SlotNum;
		Ogre::String *testForLick;
		BaseBuilding* target;
	};
	
	struct ShowProduction: public GameEvent{
		ShowProduction();
		float procent;
		BaseBuilding *target;
		Ogre::String curProduction;
		TYPE_OF_PRODUCTION typeProd;
		std::list<SendProductionVar> mProductionVar;
	};
	struct ShowIntelegence: public GameEvent{
		ShowIntelegence();
		
		Ogre::String curAction;
		std::map<Ogre::String , BaseAction*> actions;
		std::list<SendBaseDemand> demand;
		Ogre::String name;
		int needProduction;
		int progress;
		BaseBuilding* target;
	};
	struct ShowAllResourse: public GameEvent{
		ShowAllResourse();
	
		std::map<Ogre::String,int> amount;
		std::map<Ogre::String,BaseResourceObject*> links;
		std::map<Ogre::String,int> intAmount;
		std::map<Ogre::String,BaseResourceObject*> intLinks;
	};
	
	struct ShowAllRequest: public GameEvent{
		ShowAllRequest();
		
		std::vector<RequestForSend> allRequest;
		std::vector<ProjectForSend> allProject;
		std::vector<ProjectForSend> allPendingProject;
	};
	struct ShowSendStatus: public GameEvent{
		ShowSendStatus();
		
		std::vector<ResourseSend> allResourse;
		int needAmout;
		int nowAmount;
	};
	struct ShowProjectSendStatus: public ShowSendStatus{
		ShowProjectSendStatus();
		
		int periodCnt;
		int periodComplite;
		bool nowComplite;
		Ogre::String name;
		ProjectPeriodType periodtype;
	};
	struct NewNews: public GameEvent{
		NewNews();
		
		HistoryNews *pointer;
		Ogre::String descr;
		Ogre::String depname;
		
	};
	struct SomethingSimple: public ControlEvent{
		SomethingSimple();
		void *target;
	};
	struct SenResorceChng: public ControlEvent{
		SenResorceChng();
		BaseResourceObject *target;
		int sign;
	};
	struct AskIntActionDialog: public ControlEvent{
		AskIntActionDialog();
		BaseBuilding* target;
		BaseAction* action;
		bool isTarget;
	};
	struct SetSomtingIntAction: public ControlEvent{
		SetSomtingIntAction();
		BaseCountry *target;
		bool isTarget;
	};
	struct ShowBlueprint: public ControlEvent{
		ShowBlueprint();
		BaseCore *core;

	};
	struct ShowBuilding: public ControlEvent{
		ShowBuilding();
		BaseBuilding* building;

	};
	
	struct BuildOrder: public ControlEvent{
		BuildOrder();
		BaseCore *core;
		BuildingBlueprint *blueprint;

	};
	struct TradeRequest: public ControlEvent{
		TradeRequest();
		BaseBuilding *building;
		BaseDemand *demand;

	};
	
	struct ProductSet: public ControlEvent{
		ProductSet();
		BaseBuilding *building;
		Production *production;

	};
	struct ActionWarfareSet: public ControlEvent{
		ActionWarfareSet();
		BaseCountry *country;
		WarfareAction action;

	};
	class BaseListener
	{

	public:
		BaseListener(void);
		~BaseListener(void);
		virtual void fireDateChange(TimeGameEvent *evnt) abstract;
		virtual void fireGameEvent(GameEvent *evnt) abstract;
		virtual void resolvedQueue() abstract ;
		void Exit();
		std::queue<GameEvent*> mGameEventStack;
		static BaseListener * GetSigleton();
	protected:
		bool mExit;
		static BaseListener *sigleton;
	};
	class BaseNotificator{
	public:
		BaseNotificator(){};
		~BaseNotificator(){};
		void Init(BaseListener* listener);
		std::queue<NotificationEvent*> mNotificationEvent;
		void DayCheck();
	protected:
		BaseListener* mListener;
	};
	class BaseEventGenerator{
	public :
		BaseEventGenerator(void);
		BaseEventGenerator(GeneratorType type);
		~BaseEventGenerator(void);
		void IncrementTime(float deltaTime);
		bool GenerateEvent();
		bool GetEvent(GameEvent **evnt);
	protected:
		GameEvent* mEvent;
		float mEventTimer;
		GeneratorType mType;
		int Makepercent();
		bool isEvent;



	};

}
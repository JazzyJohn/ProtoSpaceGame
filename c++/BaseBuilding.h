#include <OgreEntity.h>
#include "BaseConst.h"
#include "BaseResourceManager.h"
#include "BaseResearch.h"
#include "BaseCounrty.h"
#pragma once
namespace Logic{
	
	class BaseBuilding
	{
	public:
		BaseBuilding(void);
		virtual ~BaseBuilding(void);
		virtual void PassDay(std::queue<NotificationEvent*> &notificationEvent);
		virtual GameEvent *ReturnBuildingInfo();	
		virtual void SomeChange(int,float){};
		void ChangeUpkeep(float multi);
		void SetEntiry(Ogre::Entity *entity);
		TYPE_OF_BUILDING mType;
		 Ogre::String mName;
		Ogre::String mSysName;
		 int mSlot;
		Ogre::String *mDescriprion;
		float mDaylyCost;
		BuildingWorkMode mMode;
		std::map<BuildingWorkMode,float> mCostMap;
		void AddUpgrade(Ogre::String);
		bool HasUpgrade(Ogre::String name);
		void DeleteUpgrade(Ogre::String);
		std::unordered_set<Ogre::String> mAllUpgrade;
	protected:
		Ogre::Entity *mEntity;
		
	};
	class BuildingBlueprint
	{
		public:
			BuildingBlueprint(int type,Ogre::String name);
			~BuildingBlueprint(void);
			void ChangeUpkeep(float multi);
			TYPE_OF_BUILDING mType;
			BaseBuilding* ReturnBuilding();
			int mCost;
			int mOrbitCost;
			bool mOpen;
			bool mUniq;
			BUILDING_PLACE_TYPE mPlace;
			Ogre::String mDescriprion;
			std::map<BuildingWorkMode,float> mCostMap;
			Ogre::String mName;

	};
	class BaseCore
	{
		 friend class BaseLogicLua;
	public:
		BaseCore(void);
		~BaseCore(void);
		virtual void PassDay(std::queue<NotificationEvent*> &notificationEvent);
		bool addResource(BaseResourceObject *Object);
		bool GetResource(Ogre::String name,BaseResourceObject **object);
		bool GetResource(Ogre::String name);
		bool HasBuilding(TYPE_OF_BUILDING arg[], int length);
		virtual bool GetResource(BaseResourceObject **Object);
		virtual void ReduceCredit(int cost){};
		virtual bool BuildBuilding(BuildingBlueprint *blueprint,BaseBuilding** building);
		bool mMain;
		int mNumber;
		Ogre::String mSysName;
		BaseCountry* mCountry;
	protected:
	
		BaseBuilding**mAllSlot;
		std::vector<BaseBuilding*> mBuildingVector;
	};
	class BasePlanetCountry : public BaseCountry{
		 friend class BaseLogicLua;
		 friend class  boost::serialization::access;
	public:
		LOADSAVE_FUNCTION_GENERATE()  
		BasePlanetCountry(BasePlanet* planet);
		~BasePlanetCountry(void){};
		virtual void PassWeak(std::queue<NotificationEvent*> &notificationEvent);
		
		void ChangeState(std::map<CountryCharacteristic*,float> charMap);
		void SetDeposit(ResourseDeposits **allDeposit,	int depositCount);
		void SetDeposit(ResourseDeposits * deposit);
		void GenerateNewDemand(int amount);
		void ChangeState(Ogre::String name,float delta);
	    void SetDemand(	int a,std::vector<BaseResourceObject*> demand,std::vector<BaseResourceObject*> offers);
		bool Reconing();
		void GenerateToll(float Economy);
		BasePlanet*  GetPlanet();
		 SendCountry* SendEvent();
		 void AddCore(BaseCore* core);
		 void DeleteBuilding(void*);
	protected:
		std::vector<ResourseDeposits*> mCountryDeposit;
		int mOpenDeposit;
		BasePlanet* mPlanet;
		std::vector<BaseCore*> mCore;
	};
	class BaseCoreManager
	{
	public:
		BaseCoreManager(void);
		~BaseCoreManager(void);
		virtual void PassDay(std::queue<NotificationEvent*> &notificationEvent);
		bool addResource(BaseResourceObject *Object);
		void addResource(Ogre::String name);
		bool GetResource(Ogre::String name,BaseResourceObject **Object);
		bool GetResource(BaseResourceObject **Object);
		bool GetResource(Ogre::String name);
		void GenerateRandomResource(int type,int sectype=0);
		BaseResourceObject * SayRefinery(BaseResourceObject *Object);
		void ReduceCredit(int cost);
		void SetAllBuildingBlueprint(	BuildingBlueprint **allBlueperint,int buildCnt);
		bool BuildBuilding(BuildingBlueprint *blueprint,BaseCore *targer,	BaseBuilding** building);
		bool BuildCore(BasePlanetCountry *country ,	BaseBuilding** building,	CountryTile** tile);
		void GenrateInit(BaseResourceManager *resMan,BaseBuilding** building,	BaseBuilding** basewar);
		std::vector<BuildingBlueprint *> avaibleBlueprint(BaseCore *targer);
		std::vector<BuildingBlueprint  *> mVectBlueperint;
		std::vector<BaseCore *> mAllBase;
		BaseCore * mMainBase;
	protected:
	
	
		BaseResourceManager *mResMan;
	
		BuildingBlueprint **mAllBlueperint;
		int mBuildCnt;
	};
	class BaseBaseCore: public BaseBuilding, public BaseCore{
		friend class BaseLogicLua;
	public:
		BaseBaseCore(void);
		BaseBaseCore(BaseCoreManager* coreManager);
		virtual ~BaseBaseCore(void);
		virtual void PassDay(std::queue<NotificationEvent*> &notificationEvent);
		GameEvent *ReturnBuildingInfo();
		bool addResource(BaseResourceObject *Object);
		bool GetResource(Ogre::String name,BaseResourceObject **Object);
		bool GetResource(BaseResourceObject **Object);
		bool BuildBuilding(BuildingBlueprint *blueprint,BaseBuilding** building);
		virtual void ReduceCredit(int cost);
		std::map<Ogre::String,ResourseDeposits *> GetAllDeposit();
		BaseResourceObject * SayRefinery(BaseResourceObject *Object);
		BasePlanet* mPlanet;
		void LowerHp(float delta);
	protected:
		float mBaseHP;
		BaseCoreManager *mCoreManager;
	};
	class NonCoreBuilding: public BaseBuilding
	{	
		 friend class BaseLogicLua;
	public:
		BaseBaseCore *mCore;
		bool mIdle;
		void AddCore(BaseBaseCore *base);
		virtual float GetProductionPower();
		
	protected:
		float mProductionPower;	
		float mCurProduction;

	};
	
	class BaseWarehouse : public NonCoreBuilding {
	public:
		BaseWarehouse(void);
		void SomeChange(int,float);
		virtual ~BaseWarehouse(void);
		BaseResourceObject **mObjects;  
		GameEvent *ReturnBuildingInfo();
		bool AddResource(BaseResourceObject *object);
		bool GetResource(Ogre::String name,BaseResourceObject **object);
		bool GetResource(Ogre::String name);
	};
	struct SetDeposit: public ControlEvent{
		SetDeposit();
		ResourseDeposits *deposit;
		BaseBuilding* building;

	};
	class BaseMining : public NonCoreBuilding{
	public:
		BaseMining(void);
		virtual ~BaseMining(void);
		void PassDay(std::queue<NotificationEvent*> &notificationEvent);
		GameEvent *ReturnBuildingInfo();
		void SetDeposits(ResourseDeposits* deposit);
		void SomeChange(int,float);
		int mDepositCnt;
		std::vector<ResourseDeposits*> mDeposit;
	};
	class BaseOrbitalMining: public NonCoreBuilding{
	public:
		BaseOrbitalMining();
		void PassDay(std::queue<NotificationEvent*> &notificationEvent);
		GameEvent *ReturnBuildingInfo();
		int mOreCnt;
	};
	class BaseProduction : public NonCoreBuilding{
	public:
		BaseProduction(void);
		virtual ~BaseProduction(void);
		GameEvent *ReturnBuildingInfo();
		void PassDay(std::queue<NotificationEvent*> &notificationEvent);
		void SetProductionLine(Production *product,BaseResourceObject *nextObject);
		void SetProductionLine(BaseResourceObject *raw);
		void SetProductionLine(void);
		
		void AddManager(BaseResourceManager *resMan);
	protected:
		BaseResourceManager *mResMan;
		BaseResourceObject *mNextObject;
		BaseResourceObject *mRaw;
		Production *mProduct;
		TYPE_OF_PRODUCTION mProdType;
	
	

	};
}
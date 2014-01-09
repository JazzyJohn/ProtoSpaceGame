#pragma once
#include <OgreRoot.h>
#include "BaseConst.h"
#include "BaseResourceManager.h"
namespace Logic{
	
	
	struct PlanetEvent : public GameEvent{
	 PlanetEvent() 
        {
			gameType = PET_SIMPLE;
			type =ET_PLANET;
        };
	PlanetEventType gameType;
	};
	struct CountryCharacteristic{
		CountryCharacteristic(){
			Open = true;
		}
		Ogre::String name;
		ShowCharType ShowType;
		int type;
		int Max;
		int Min;
		bool Open;
		std::map<BaseResource*,float> TradeDependace;
	};
	class CountryCharValue{
		friend class  boost::serialization::access;
	public:
		LOADSAVE_FUNCTION_GENERATE()  
		CountryCharValue(float test);
		CountryCharValue();
		~CountryCharValue(){};
		int& operator=(const CountryCharValue& rhs) ;
		Ogre::String Text() ;
		CountryCharValue& operator=(const float& rhs) ;
		CountryCharValue& operator+=(CountryCharValue* rhs) ;
		CountryCharValue& operator+=(const float& rhs) ;
		CountryCharValue& operator-=(const float& rhs) ;
		bool operator< ( const float& rhs);
		bool operator> ( const float& rhs);
		void Open();
		void CheckDiapason();
		void CheckDiapason(int recon,float max);
		bool mPlayerKnow;
		float mAmount;
		
		int mLowDeap;
		int mHightDeap;
		
	};
	class CharacteristicManager{
	public:
		CharacteristicManager(void);
		~CharacteristicManager(void);
		std::map<CountryCharacteristic*,CountryCharValue*> ReturnBaseMap();
	    void setAllCharacteristics(CountryCharacteristic ** allCharacteristick,int charctNum);
		static CountryCharacteristic* GetChar(Ogre::String);
	private:
		static CharacteristicManager* sigleton;
		std::map<Ogre::String,CountryCharacteristic *> mNameMap;
		CountryCharacteristic **mAllCharacteristick;
		int mCharCount;
	};
	class BaseDemand{
		friend class  boost::serialization::access;
	public:
		LOADSAVE_FUNCTION_GENERATE()  
		BaseDemand();
		BaseResourceObject* demand;
		BaseResourceObject* offer;
		int amount;
		int demandCnt;
		int	demandProgress;
		bool isTrade;
		bool open;
	};
	struct SendBaseDemand{
		SendBaseDemand(BaseDemand &demandObj);
		Ogre::String demand;
		Ogre::String offer;
		int amount;
		bool isTrade;
		BaseDemand *demandPnt;	
		
	};
	class CountryTile{
	public:
		CountryTile();
		CountryTile(Ogre::Vector2);
		Ogre::Vector2 mPosition;
		bool mCapital;
		bool mMine;
		bool mBase;
		bool operator<( const CountryTile & b ) const;
	};
	struct BaseAINode;
	class CountryRellation{
	public:
		int* mValue;
		CountryRelationStatus* mStatus;
		BaseAINode* mAIScript;
	};
	class BaseCountry
	{
	 friend class BaseLogicLua;
	
	public:

		
		BaseCountry(void);
		virtual ~BaseCountry(void);
		virtual void PassWeak(std::queue<NotificationEvent*> &notificationEvent);
		void ClearRelation(BaseCountry* target);
		void SetMap(std::map<CountryCharacteristic*,CountryCharValue*> map);
		void SetRelations(BaseCountry* target,int relation =0);
		void LoadRelations(BaseCountry* target,int relation,CountryRelationStatus status);
		void SetRelations(BaseCountry* target, int* num,CountryRelationStatus* status);
		virtual void GenerateToll(float Economy){};
		void ChangeState(std::map<CountryCharacteristic*,float> charMap);
		void ChangeState(Ogre::String name,float delta);
		void SetTrade(BaseDemand* demand);
		virtual void SetDemand(	int a,std::vector<BaseResourceObject*> demand,std::vector<BaseResourceObject*> offers);
		void ChangeDelta(BaseResourceObject* object);
		std::list<SendBaseDemand> GetDemandForEmabassy();
		 int* GetRelation(BaseCountry* target);
		 CountryRelationStatus* GetRelatStatus(BaseCountry* target);
		bool Pillage(int damage);
		void CalculateAI();
		bool Destroying(int damage);
		bool Fanatism(int damage);
		void ChangeRelation(BaseCountry* target,int detla);
		void ChangeRelation(BaseCountry& targetRef,int detla);
		int GetChar(CountryCharacteristic* Char);
		int GetChar(Ogre::String name);
		void SetChar(Ogre::String name,float value);
		void GetMap(std::map<Ogre::String,Ogre::String> &map);
		virtual bool Reconing();
		int GetCom();
		void GenerateAi();
		bool mDestroyed;
		bool mOcuppied;
		bool mAtWar;
		Ogre::String mName;
		CountryCharacteristic* mCommunication;
		CountryTile mCapital;
		Ogre::ColourValue mColor;
		std::vector<CountryTile> mCountryTiles;
		std::set<BaseCountry*> mNeibor;
		Ogre::SceneNode *mNode;
		virtual void DeleteBuilding(void*){};
	protected: 
		int mReconTime;
		
		std::list<BaseDemand> mDemand;
		
		std::map<CountryCharacteristic*,CountryCharValue*> mCharMap;
		std::map<Ogre::String,CountryCharValue*> mRefCharMap;
		std::vector<CountryCharValue*> mCharVector;
		std::map<BaseCountry*,CountryRellation> mRelation;
		
	};
	struct PlanetJustGenerateEvent: public GameEvent{
		PlanetJustGenerateEvent();
		
		std::map<Ogre::String,BaseCountry*>country;
		std::map<BaseCountry*,std::vector<CountryTile>> allTiles;

	};

	class ResourseDeposits{
	friend class  boost::serialization::access;
	public:
		LOADSAVE_FUNCTION_GENERATE() 
		ResourseDeposits(){};
		ResourseDeposits(BaseCountry* country);
		~ResourseDeposits(void);
		void SetCountry(BaseCountry* country);
		BaseCountry* GetCountry();
		void SetMap(std::vector<BaseResourceObject*> vector);
		BaseResourceObject * GenerateResource();
		Ogre::Entity *mEntity;
		bool mOcupiet;
		Ogre::String mName;
		bool mOpen;
		bool mProductPower;
		bool mMine;
		
	private:
		BaseCountry *mCountry;
		std::map<float, BaseResourceObject *> mGeneratePattern; 
	};
	struct SendCountry : public GameEvent{
		SendCountry(); 
		BaseCountry* contry;
		std::map<CountryCharacteristic *,Ogre::String> allChar;
		std::map<CountryCharacteristic *,bool> deltaChar;
		std::map<Ogre::String,int> Realtion;
		std::map<Ogre::String,CountryRelationStatus> RealtionStatus;
	};
	
	
	class BasePlanet{
		friend class BaseLogicLua;
		friend class  boost::serialization::access;
	public:
		LOADSAVE_FUNCTION_GENERATE()  
		BasePlanet(void);
		virtual ~BasePlanet(void);
		virtual void PassWeak(std::queue<NotificationEvent*> &notificationEvent);
		virtual void CheckState(){};
		void AddDeposit(BaseCountry *test);
		virtual PlanetJustGenerateEvent* GeneratePlanet(int countryCount,int clusterCnt,
			PlanetEnvorementType type,
			CharacteristicManager *charMan,
			int depositCount,
			BaseResourceManager *resMan
			);
		std::map<Ogre::String,ResourseDeposits *> GetAllDeposit();
		virtual void ChangeState(std::map<CountryCharacteristic*,float> charMap,BaseCountry* iniciator);
		virtual void ChangeState(Ogre::String name,float delta,BaseCountry* iniciator);
		void GetAllCountry(std::map<Ogre::String,BaseCountry *> &allCountry);
		void GetAllCountry(std::map<Ogre::String,BaseCountry *> &allCountry, Ogre::String &action);
		virtual void CallBack(ActionCallBackType type){};
		BaseResourceManager *mResMan;
		std::vector<ResourseDeposits *>mAllDepositVect;
		std::vector<BaseCountry *> mCountryVector;
		BaseCountry*GetNextCountry();
		void GenerateTiles();
		void DeleteBuilding(void*);
	protected:
		PlanetEnvorementType mType;
		CharacteristicManager *mCharMan;
		BaseCountry **mCountry;
		

		int mDepositCount;
		int mCountryCount;
		std::queue<PlanetEvent*> mPlanetEventStack;
	};
	
	

}
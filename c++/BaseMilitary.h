#pragma once
#include "BaseBuilding.h"
#include "BaseAbility.h"
namespace Logic{
	
	class BaseUnitCharacteristics{
		public:
			BaseUnitCharacteristics(void){};
			BaseUnitCharacteristics(Ogre::String name,bool lifeCare);
			~BaseUnitCharacteristics(void);
			Ogre::String mName;	
			bool mLifeCare;
	};

	class BaseUnit{
	public:
		BaseUnit(void){};
		BaseUnit(Ogre::String name,std::map<BaseUnitCharacteristics*,float> Char,int cost);
		~BaseUnit(void);
		void CheckIfAlife();
		void AddFromSupplie(float supplie);
		int GetFirePower();
		float GetChar(Ogre::String);
		void SetChar(Ogre::String,float);
		bool HasUpgrade(Ogre::String);
		void SayAbility(std::map<Ogre::String,Logic::BaseAbility*> &ability);
		void AddUpgrade(Ogre::String);
		void SayCharacter(std::map<Ogre::String,int> &character);
		bool LowerHp(int damage);
		void SetCountry(BaseCountry *country);
		void PassDay();
		bool hasTarget();
		bool mOnMove;
		Ogre::String SayName();
		Ogre::String mName;	
		std::map<BaseUnitCharacteristics*,float*> mChar;
		std::map<Ogre::String,float*> mCharRef;
		BaseCountry *mCountry;
		BaseBuilding *mBase;
		std::unordered_set<Ogre::String> mAllUpgrade;
		int mCost;
	};
	struct ShowUnitInBase: public GameEvent{
		ShowUnitInBase();
		std::map<Ogre::String,Logic::BaseAbility*> abilitys;
		std::map<Ogre::String,BaseCountry*> allCountry;
		BaseUnit* unit;
		Ogre::String name;
		std::map<Ogre::String,int> character;
	};
	struct SendUnitToCountry: public ControlEvent{
		SendUnitToCountry();
		BaseBuilding *base;
		BaseUnit *unit;
		BaseCountry *country;

	};
	
	class BaseUnitBlueprint{
	public:
		BaseUnitBlueprint(){};
		BaseUnitBlueprint(Ogre::String name);
		~BaseUnitBlueprint();
		void SetCost(int cost);
		int GetCost();
		bool mUniq;
		void SetCharacteristik(std::map<BaseUnitCharacteristics*,float> Char);
		Ogre::String mName;	
		bool isNative;
		bool mOpen;
		BaseUnit* GetUnit();
	protected:
		int mCost;		
		std::map<BaseUnitCharacteristics*,float> mChar;
	};
	class BaseUnitManager{
	public:
		BaseUnitManager(){};
		~BaseUnitManager();
		BaseUnit* GetUnit(Ogre::String name);
		BaseUnitBlueprint* GetUnitBlueprint(Ogre::String);
		void Open(Ogre::String name);
		void AddUnit(Ogre::String,BaseUnitBlueprint* blueprint);
		void setAllCharacteristics(BaseUnitCharacteristics ** allCharacteristick,int charctNum);
		void GetUnitBlueprint(std::map<Ogre::String,BaseUnitBlueprint*> &avaibleUnits);
		
	protected:
		std::map<Ogre::String,BaseUnitBlueprint*> mAllUnits;
		BaseUnitCharacteristics **mAllCharacteristick;
		int mCharctNum;
	};
	struct UnitToSend{
		bool isEmpty;
		BaseUnit* unit;
		Ogre::String name;	
	};
	struct AddUnittoQueue: public ControlEvent{
		AddUnittoQueue();
		BaseBuilding* building;
		BaseUnitBlueprint *blueprint;

	};
	
	struct ShowMilitary: public GameEvent{
		ShowMilitary(std::queue<Ogre::String> mUnitQueue);
		

		float procent;
		BaseBuilding* target;
		std::vector<UnitToSend> unitOnBase;
		std::map<Ogre::String,BaseUnitBlueprint*> avaibleUnits;
		std::queue<Ogre::String> unitQueue;
		
	};
class BaseMilitaryBuilding : public NonCoreBuilding
{
public:
	BaseMilitaryBuilding(void);
	~BaseMilitaryBuilding(void);
	void PassDay(std::queue<NotificationEvent*> &notificationEvent);
	void AddUnitToQueue(Ogre::String name);
	void AddManager(BaseUnitManager *manager);
	void SomeChange(int,float);
	GameEvent *ReturnBuildingInfo();
	void DeleteUnit(BaseUnit* unit);
protected:
	BaseUnitManager *mManager;
	bool AddUnit(Ogre::String name);
	BaseUnit **mUnits;
	int mUnitMAxCount;
	std::queue<Ogre::String> mUnitQueue;

};
}


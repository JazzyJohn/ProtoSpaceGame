#pragma once
#include "BaseMilitary.h";
namespace Logic{

class BaseBattle{
	friend class BaseLogicLua;
public:
	BaseBattle(BaseCountry *battleField,	std::vector<BaseUnit*> units);
	~BaseBattle(){};
	bool PassDay(std::queue<NotificationEvent*> &notificationEvent);
	void AddUnit(BaseUnit* playerUnit);
	void SetAction(WarfareAction playerAction);
	void GeneratePartisan();
	BaseUnit& GetNpcUnit(int i);
	BaseUnit& GetPlayerUnit(int i);
	void DeadUnit(int player,int i);
	void DeleteUnit(BaseUnit* playerUnit);
	void FinishBattle(std::queue<NotificationEvent*> &notificationEvents);
	GameEvent * ShowInfo();
	bool mReDraw;
protected:
	BaseCountry *mBattleField;
	std::vector<BaseUnit*> mNpcUnit;
	std::vector<BaseUnit*> mPlayerUnit;
	int mNpcAmount;
	int mPlayerAmount;
	bool mIsSupplied;
	int mSuppliedAmount;
	float mSuppliedCoef;
	std::map<Ogre::String,float> mStartCountryChar;
	WarfareAction mPlayerAction;
	int mPartisansPower;
	int mPartisansHP;
	bool mIsPartisan;
	std::queue<BaseUnit *> mTempNpcUnit,mTempPlayerUnit;
};
struct UnitForSendWar{
	Ogre::String name;
	BaseUnit* pnt;
	std::map<Ogre::String, int> character;
};
struct ShowWarfare : public GameEvent{
	ShowWarfare();
	BaseCountry*contry; 
	Ogre::String name;
	std::vector<UnitForSendWar> playerUnit;
	std::vector<UnitForSendWar>  NPCUnit;
	WarfareAction curAction;
	WarState status;
	std::map<Ogre::String, Ogre::String> countryChar;
	bool isSupplied;
	int suppliedAmount;
	int partisanHP;
};
struct CloseWarfare : public GameEvent{
	CloseWarfare();
	BaseCountry*contry; 
	
};
class BaseWarfare
{
public:
	BaseWarfare(void){};
	~BaseWarfare(void);
	void Init(BaseUnitBlueprint** allNPCUnits, int allNPCUnitsCnt);
	void MoveUnit(BaseCountry * country, BaseUnit* attacker);
	void DeleteUnit(BaseCountry * country, BaseUnit* attacker);
	void PassDay(std::queue<NotificationEvent*> &notificationEvent);
	GameEvent * ShowWarfare(BaseCountry * country);
	GameEvent * ShowWarfare(BaseCountry * country,bool &reDraw);
	void SetAction(BaseCountry * country, WarfareAction action);
protected:
	std::map<BaseCountry *,BaseBattle*> mAllBattles;
	BaseUnitBlueprint** mAllNPCUnits;
	int mAllNPCUnitsCnt;
};
};

#pragma once
#include "BaseConst.h"

namespace Logic{
class BaseAbility
{
public:
	BaseAbility(void);
	BaseAbility(AbilityType,Ogre::String,Ogre::String);
	~BaseAbility(void);
	bool MakeItSo(void*);
	void Open();
	bool State();
	AbilityType GetType();
	Ogre::String mName;
	int mTimeOut;
	int mLastTime;
	Ogre::String mDisplayName;
protected:
	AbilityType mType;
	bool mOpen;
	
};
struct ActivateAbility: public ControlEvent{
		ActivateAbility();
		void* target;
		BaseAbility* ability; 
};
class AbilityManager{
public:
	AbilityManager();
	~AbilityManager();
	void AddAbility(BaseAbility *);
	void OpenAbility(Ogre::String mName);
	void GetAbility(std::vector<BaseAbility*> &,AbilityType);
	std::vector<BaseAbility* > mAllAbility;
	std::unordered_multimap<AbilityType,BaseAbility*> mAllMapAbility;
	
};
};
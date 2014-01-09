#pragma once
#include <OgreRoot.h>
#include "BaseConst.h"
#include "BaseBuilding.h"
namespace Logic{
class BaseEffect
{
		friend class BaseLogicLua;
public:
	BaseEffect(void);
	~BaseEffect(void);
	void MakeCoef(float &power,BaseBuilding* cur);
	EffectType mType;
	ChangeEffectType mChangeType;
	Ogre::String mName;
	float mProductionDelta;
	int mTime;
	int mCurTime;

};
class BaseEffectManager{
	friend class BaseLogicLua;
public:
	BaseEffectManager();
	~BaseEffectManager();
	static BaseEffectManager* GetSigleton();
	void Init(BaseEffect **randEffect,int randEffectCnt);

	void PassDay();
	void MakeKoef(float &power,BaseBuilding* cur);
protected:
	std::vector<BaseEffect*> mCurEffect; 
	std::vector<BaseEffect*> mAllEffect;
	BaseEffect **mRandEffect;
	int mRandEffectCnt;
	static BaseEffectManager* sigleton;

};

}

#pragma once
#include "BaseCounrty.h"

namespace Logic{
class BaseRecon
{
public:
	BaseRecon(void);
	~BaseRecon(void);
	void DoRecon();
	void MoveToNext(BaseCountry *country);
	BaseCountry * GetCountry();
protected:
	int mProductionAmount;
	int mCurProduct;
	BaseCountry *mCountry;
};
struct ReconToSend{
	Ogre::String name;
	BaseRecon* pnt;
	Ogre::String country;
};
struct ReconStateEvnt: public GameEvent{
	ReconStateEvnt();
	std::list<ReconToSend> mAllRecon;

};
class ReconManadger{
public:
	ReconManadger(void);
	~ReconManadger(void);
	void PassDay();
	GameEvent* ReturnReconState();
	void SetPlanet(BasePlanet* planet);
	void LaunchRecont();
protected:
	std::list<BaseRecon*> mAllRecon;
	BasePlanet* mPlanet;
};
}

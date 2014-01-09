#pragma once
#include "BaseCounrty.h";
#include "BaseBuilding.h";
namespace Logic{
	class BugsPlanet :public BasePlanet{
	public:
		BugsPlanet(){};
		~BugsPlanet(){};
		PlanetJustGenerateEvent* GeneratePlanet(int countryCount,int clusterCnt,
			PlanetEnvorementType type,
			CharacteristicManager *charMan,
			int depositCount,
			BaseResourceManager *resMan
			);
	};
class BugsCountry :public BasePlanetCountry
{
public:
	BugsCountry(BugsPlanet* planet);
	~BugsCountry(void);	
	void PassWeak(std::queue<NotificationEvent*> &notificationEvent);
};
};

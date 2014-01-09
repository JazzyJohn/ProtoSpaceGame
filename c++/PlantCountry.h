#pragma once
#include "BaseCounrty.h";
#include "BaseBuilding.h";
namespace Logic{

	class PlantPlanet :public BasePlanet{
	public:
		PlantPlanet(){};
		~PlantPlanet(){};
		PlanetJustGenerateEvent* GeneratePlanet(int countryCount,int clusterCnt,
			PlanetEnvorementType type,
			CharacteristicManager *charMan,
			int depositCount,
			BaseResourceManager *resMan
			);
	};
class PlantCountry :public BasePlanetCountry
{
public:
	PlantCountry(PlantPlanet* planet);
	~PlantCountry(void);
	void PassWeak(std::queue<NotificationEvent*> &notificationEvent);
};
}


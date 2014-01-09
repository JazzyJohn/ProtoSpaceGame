#pragma once
#include "BaseResearch.h"
#include "BaseBuilding.h"
namespace Logic{
	

	class BaseResearchBuilding :public NonCoreBuilding
	{
	public:
		BaseResearchBuilding();
		~BaseResearchBuilding(void);
		void SetManager (BaseReasearchManager *resMan);
		GameEvent *ReturnBuildingInfo();
		void PassDay(std::queue<NotificationEvent*> &notificationEvent);
	protected:
		BaseReasearchManager *mResearchManager;
	};

}


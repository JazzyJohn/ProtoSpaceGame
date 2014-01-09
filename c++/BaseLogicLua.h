extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

// This is the only header we need to include for LuaBind to work
#include "luabind/luabind.hpp"

#include "LogicStd.h"
#include "BaseListener.h"
#include "BaseResourceManager.h"
#include "BaseBuilding.h"
#include "BaseCounrty.h"
#include "BaseConst.h"
#include "BaseIntelegence.h"
#include "BaseMilitary.h"
#include "BaseCorporate.h"
#include "BaseRequestManager.h"
#include "BaseResearchBuilding.h"
#include "BaseEffect.h"
#include "BaseWarfare.h"
#include "BaseRecon.h"
#include "BaseAbility.h"
namespace Logic{
	class SimpleClass{
	public:
		SimpleClass(int a){
			mA= a;
		}
		int mA;
		int mB;
		
	};
	class BaseLogicLua{
public:
	BaseLogicLua(){};
	~BaseLogicLua();
	void Init(PlanetType);
	lua_State *mMyLuaState;
	void ResearchScripts(BaseResearch*);
	void PlanetScript(BasePlanetCountry&,Ogre::String,bool succses= true);
	void PlanetScript(BasePlanetCountry&,Ogre::String,BaseCountry&,bool succses= true);
	void CountryBaseScript(BasePlanetCountry&,Ogre::String,BaseBaseCore&);
	bool CountryCheck(BasePlanetCountry*,Ogre::String);
	void BuildBuilding(BaseBuilding*);
	Ogre::String CalculateInitAI(BaseCountry*,BaseCountry*);
	bool AIScript(Ogre::String,BaseCountry*,BaseCountry*);
	void BuildUnit(BaseUnit*);
	void CountryPassWeak(BasePlanetCountry*);
	void UnitDayPass(BaseUnit*);
	void BuildingPassDay(NonCoreBuilding*);
	void SetPlanet(BasePlanet& planet);
	void SetAbilityMng(AbilityManager* mng);
	void SetActionMng(ActionManager* mng);
	void SetCoreMng(BaseCoreManager* mng);
	void SetUnitMng(BaseUnitManager* mng);
	void SetAsset(BaseAsset* mng);
	bool CheckForFinish();
	void  PassDay();
	void  PassWeek();
	bool DoBattle(BaseBattle* battlefield);
	bool DoAbility(Ogre::String,AbilityType,void*);
	protected:
		std::set<Ogre::String> mAllScript;


	};

}
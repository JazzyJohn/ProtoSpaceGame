#include "BaseLogicLua.h"
#include <luabind/iterator_policy.hpp>
#include "rapidjson/document.h"
using namespace Logic;
class SimpleClassFunc{
public:
	SimpleClassFunc(void){
		mA= 0;
	};
	void ChangeState(int b){
		mB = b;
		b++;
	};
	int mA;
	int mB;

};
class ChildClass : public SimpleClassFunc{
	public:
	int mC;
	void Set(int c){
		mC= c;
	};
};
void BaseLogicLua::Init(PlanetType gameType){
	mMyLuaState =  luaL_newstate();
	luaL_openlibs(mMyLuaState);
	luabind::open(mMyLuaState);
	Ogre::String catalogName;
	switch(gameType){
	case PT_NORMAL:
		catalogName="scripts/people/";
		break;
	case PT_BUGS:
		catalogName="scripts/bugs/";
		break;
	
	}
	std::string s;
	std::ifstream inp(catalogName+"scripts.json");
	if(!inp.good()){
		inp.open("scripts/scripts.json");
	}
	std::getline(inp, s, '\0');

	std::srand(std::time(0));

	rapidjson::Document d;
	d.Parse<0>(s.c_str());
	const rapidjson::Value& res = d["allScripts"]; // Using a reference for consecutive access is handy and faster.
	for (rapidjson::SizeType i = 0; i < res.Size(); i++){ // rapidjson uses SizeType instead of size_t.
		luaL_dofile(
			mMyLuaState,
			res[i]["path"].GetString()
			);
		mAllScript.insert(res[i]["name"].GetString());
	}


	// Define a lua function that we can call
	luabind::module(mMyLuaState) [
		luabind::class_<BaseCountry>("Country")
			.def("setchar", &BaseCountry::SetChar)
			.def("getchar", (int(BaseCountry::*)(Ogre::String))&BaseCountry::GetChar)
			.def("changestate",(void(BaseCountry::*)(Ogre::String,float)) &BaseCountry::ChangeState)
			.def("changeRelation",(void(BaseCountry::*)(BaseCountry&,int)) &BaseCountry::ChangeRelation) 
			.def_readwrite("ocupied",&BaseCountry::mOcuppied) 
			.def_readwrite("recon",&BaseCountry::mReconTime) 
			.def_readwrite("destroyed",&BaseCountry::mDestroyed)
			.def_readwrite("atWar",&BaseCountry::mAtWar) 
			.def_readwrite("allChar",&BaseCountry::mCharVector,luabind::return_stl_iterator) 
			.def_readwrite("demands",&BaseCountry::mDemand,luabind::return_stl_iterator)
			.def("pillage",&BaseCountry::Pillage)
			.def("dorecon",&BaseCountry::Reconing)
			.def("fanatism",&BaseCountry::Fanatism)
			.def("destroying",&BaseCountry::Destroying)
			.def_readwrite("neibor",&BaseCountry::mNeibor,luabind::return_stl_iterator) 
			,
			luabind::class_<BasePlanetCountry,BaseCountry>("PlanetCountry")
			.def_readwrite("allDeposit",&BasePlanetCountry::mCountryDeposit,luabind::return_stl_iterator)
			.def_readwrite("cores",&BasePlanetCountry::mCore,luabind::return_stl_iterator)
	];
	luabind::module(mMyLuaState) [
		luabind::class_<BaseDemand>("demand")

			.def_readwrite("open",&BaseDemand::open)

	];
	luabind::module(mMyLuaState) [
		luabind::class_<BaseCore>("Core")

			.def_readwrite("buildings",&BaseCore::mBuildingVector,luabind::return_stl_iterator)
			.def_readwrite("country",&BaseCore::mCountry),
			luabind::class_<BaseBaseCore,BaseCore>("BaseCore")

			.def("lowerHp",&BaseBaseCore::LowerHp)
			
	];
	luabind::module(mMyLuaState) [
		luabind::class_<BaseCoreManager>("CoreManager")
			.def_readwrite("main",&BaseCoreManager::mMainBase)
			.def_readwrite("blueprints",&BaseCoreManager::mVectBlueperint,luabind::return_stl_iterator)
			.def_readwrite("allbase",&BaseCoreManager::mAllBase,luabind::return_stl_iterator)
			.def("randomresource",&BaseCoreManager::GenerateRandomResource)
			.def("addresource",(bool(BaseCoreManager::*)(BaseResourceObject*))&BaseCoreManager::addResource)

	];
	luabind::module(mMyLuaState) [
		luabind::class_<BuildingBlueprint>("Blueprint")

			.def_readwrite("btype",&BuildingBlueprint::mType)
			.def_readwrite("cost",&BuildingBlueprint::mCost)
			.def_readwrite("open",&BuildingBlueprint::mOpen)
			.def("changeUpkeep",&BuildingBlueprint::ChangeUpkeep)
			
			
			
	];
	luabind::module(mMyLuaState) [
		luabind::class_<BaseBuilding>("BaseBuilding")

			.def_readwrite("btype",&BaseBuilding::mType)
			.def_readwrite("name",&BaseBuilding::mSysName)
			.def("addUpgrade",&BaseBuilding::AddUpgrade)
			.def("hasUpgrade",&BaseBuilding::HasUpgrade)
			.def("deleteUpgarde",&BaseBuilding::DeleteUpgrade)
			.def("changeUpkeep",&BaseBuilding::ChangeUpkeep),
			luabind::class_<NonCoreBuilding,BaseBuilding>("NonCoreBaseBuilding")
			.def_readwrite("power",&NonCoreBuilding::mProductionPower)
			.def_readwrite("production",&NonCoreBuilding::mCurProduction)
			.def_readwrite("core",&NonCoreBuilding::mCore)
			
			
			
	];

	luabind::module(mMyLuaState) [
		luabind::class_<CountryCharValue>("CharValue")

			.def("open",&CountryCharValue::Open)
			.def("deapason",(void(CountryCharValue::*)(int,float))&CountryCharValue::CheckDiapason)

	];

	luabind::module(mMyLuaState) [
		luabind::class_<ResourseDeposits>("Deposit")
			.def("generate",&ResourseDeposits::GenerateResource)
			.def_readwrite("open",&ResourseDeposits::mOpen)
			.def_readwrite("mine",&ResourseDeposits::mMine)
			.def_readwrite("name",&ResourseDeposits::mName)
			.def_readwrite("productPower",&ResourseDeposits::mProductPower)

	];
	luabind::module(mMyLuaState) [
		luabind::class_<BaseResearch>("Research")

			.def_readwrite("name",&BaseResearch::mName)


	];
	luabind::module(mMyLuaState) [
		luabind::class_<BasePlanet>("Planet")
			.def("newDeposit",&BasePlanet::AddDeposit)
			.def_readwrite("type",&BasePlanet::mType) 
			.def_readwrite("allCountry",&BasePlanet::mCountryVector,luabind::return_stl_iterator) 
			.def_readwrite("allDeposit",&BasePlanet::mAllDepositVect,luabind::return_stl_iterator) 
	];
	luabind::module(mMyLuaState) [
		luabind::class_<BaseEffect>("Effect")
			.def_readwrite("btype",&BaseEffect::mType) 
			.def_readwrite("name",&BaseEffect::mName) 
	];
	luabind::module(mMyLuaState) [
		luabind::class_<BaseEffectManager>("EffectMan")
			
			.def_readwrite("allEffects",&BaseEffectManager::mAllEffect,luabind::return_stl_iterator)			
			.def_readwrite("curEffects",&BaseEffectManager::mCurEffect,luabind::return_stl_iterator) 
	];
	luabind::module(mMyLuaState) [
		luabind::class_<BaseUnitManager>("UnitManager")
			
			.def("open",&BaseUnitManager::Open)			
		
	];


	luabind::module(mMyLuaState) [
		luabind::class_<BaseUnit>("Unit")
			.def("addSupplie",&BaseUnit::AddFromSupplie) 
			.def("getFirePower",&BaseUnit::GetFirePower) 
			.def("getChar",&BaseUnit::GetChar) 
			.def("lowerHp",&BaseUnit::LowerHp) 
			.def("setChar",&BaseUnit::SetChar) 
			.def("hasUpgrade",&BaseUnit::HasUpgrade) 
			.def("addUpgrade",&BaseUnit::AddUpgrade) 
			.def("hastarget",&BaseUnit::hasTarget)
			.def_readwrite("country",&BaseUnit::mCountry)
			.def_readwrite("onmove",&BaseUnit::mOnMove)
			.def_readwrite("name",&BaseUnit::mName)
	];
	luabind::module(mMyLuaState) [
		luabind::class_<BaseBattle>("Battle")
			.def("getNpcUnit",&BaseBattle::GetNpcUnit) 
			.def("getPlayerUnit",&BaseBattle::GetPlayerUnit) 
			.def("deadUnit",&BaseBattle::DeadUnit) 
			
			.def_readwrite("battleField",&BaseBattle::mBattleField) 
			.def_readwrite("npcAmount",&BaseBattle::mNpcAmount) 
			.def_readwrite("playerAmount",&BaseBattle::mPlayerAmount) 
			.def_readwrite("partisansPower",&BaseBattle::mPartisansPower) 
			.def_readwrite("partisansHP",&BaseBattle::mPartisansHP) 
			.def_readwrite("playerAction",&BaseBattle::mPlayerAction) 
			.def_readwrite("suppliedAmount",&BaseBattle::mSuppliedAmount) 
			.def_readwrite("suppliedCoef",&BaseBattle::mSuppliedCoef)
			.def_readwrite("isSupplied",&BaseBattle::mIsSupplied) 
			.def_readwrite("npcUnit",&BaseBattle::mNpcUnit,luabind::return_stl_iterator) 
			.def_readwrite("playerUnit",&BaseBattle::mPlayerUnit,luabind::return_stl_iterator) 
	];
	
	luabind::module(mMyLuaState) [
		luabind::class_<BaseAbility>("Ability")
			.def("Open",&BaseAbility::Open) 
			.def_readwrite("name",&BaseAbility::mName) 
	];
	luabind::module(mMyLuaState) [
		luabind::class_<AbilityManager>("AbilityManager")
			.def("OpenAbility",&AbilityManager::OpenAbility) 
			.def_readwrite("abilitys",&AbilityManager::mAllAbility,luabind::return_stl_iterator) 
			
	];
	 luabind::module(mMyLuaState) [
		 luabind::class_<ActionManager>("ActionManager")
			 .def("open",&ActionManager::Open) 
			
	];
	  luabind::module(mMyLuaState) [
		 luabind::class_<BaseAsset>("BaseAsset")
			 .def("set",&BaseAsset::Set) 
			 .def("get",&BaseAsset::Get) 
	];
	    luabind::module(mMyLuaState) [
			luabind::class_<BaseResourceObject>("Resource")
				.def_readwrite("name",&BaseResourceObject::mName) 
			 
	];
}
BaseLogicLua::~BaseLogicLua(){
	if( mMyLuaState)
	{
		lua_close(mMyLuaState);
		mMyLuaState= 0;
	}
}
void  BaseLogicLua::SetAbilityMng(AbilityManager* mng){
	try
	{
		luabind::call_function<void>(mMyLuaState, "setAbilityMng",boost::ref(mng));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}
}
void BaseLogicLua::BuildBuilding(BaseBuilding* build){
	bool test= false;
	try
	{
		test =luabind::call_function<bool>(mMyLuaState, "buildbuilding",boost::ref(build));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}


}
void BaseLogicLua::BuildUnit(BaseUnit* build){
	try
	{
		luabind::call_function<void>(mMyLuaState, "buildunit",boost::ref(build));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}


}
void  BaseLogicLua::SetActionMng(ActionManager* mng){
	try
	{
		luabind::call_function<void>(mMyLuaState, "setActionMng",boost::ref(mng));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}
}
void BaseLogicLua::BuildingPassDay(NonCoreBuilding* build){
	try
	{
		luabind::call_function<void>(mMyLuaState, "buildingPass",boost::ref(build));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}

}
void  BaseLogicLua::SetUnitMng(BaseUnitManager* mng){
	try
	{
		luabind::call_function<void>(mMyLuaState, "setUnitMan",boost::ref(mng));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}
}
void  BaseLogicLua::SetAsset(BaseAsset* mng){
	try
	{
		luabind::call_function<void>(mMyLuaState, "setAsset",boost::ref(mng));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}
}
void  BaseLogicLua::UnitDayPass(BaseUnit*  unit){
		try
	{
		luabind::call_function<void>(mMyLuaState, "unitDayPass",boost::ref(unit));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}
}
void  BaseLogicLua::SetCoreMng(BaseCoreManager* mng){
	try
	{
		luabind::call_function<void>(mMyLuaState, "setcoreman",boost::ref(mng));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}
}
void BaseLogicLua::SetPlanet(BasePlanet& planet){
	try
	{
		luabind::call_function<void>(mMyLuaState, "setplanet",boost::ref(planet));

		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}

}
void BaseLogicLua::PlanetScript(BasePlanetCountry& target ,Ogre::String action,bool succses){

	try
	{
		luabind::call_function<void>(mMyLuaState, "ActionOccur",boost::ref(target),(std::string)action,false,succses);
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}

}
void BaseLogicLua::CountryPassWeak(BasePlanetCountry* target){

	try
	{
		luabind::call_function<void>(mMyLuaState, "countryWeek",boost::ref(target));
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}

}
void BaseLogicLua::PlanetScript(BasePlanetCountry& target,Ogre::String action,BaseCountry& reason,bool succses){

	try
	{
		luabind::call_function<void>(mMyLuaState, "ActionOccur",boost::ref(target),(std::string)action,true,succses,boost::ref(reason));
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}

}
void BaseLogicLua::CountryBaseScript(BasePlanetCountry& country,Ogre::String script,BaseBaseCore& core){
	
	try
	{
		luabind::call_function<void>(mMyLuaState, script.c_str(),boost::ref(core),boost::ref(country));
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}

}
void BaseLogicLua::ResearchScripts(BaseResearch* research){
	try
	{
		luabind::call_function<void>(mMyLuaState, "ResearchFinish",boost::ref(research));
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}

}
bool BaseLogicLua::DoBattle(BaseBattle* battlefield){
	try
	{
		return luabind::call_function<bool>(mMyLuaState, "doBattle",boost::ref(battlefield));
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}
	return false;
}
bool  BaseLogicLua::CountryCheck(BasePlanetCountry* country,Ogre::String action){
	try
	{
		return luabind::call_function<bool>(mMyLuaState, "return",boost::ref(country),boost::ref(action));
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
		
	}
	return true;

}
Ogre::String BaseLogicLua::CalculateInitAI(BaseCountry* country,BaseCountry* target){
	try
	{
		return luabind::call_function<Ogre::String>(mMyLuaState, "initAI",boost::ref(country),boost::ref(target));
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
		
	}
	
}
bool BaseLogicLua::AIScript(Ogre::String script,BaseCountry* country,BaseCountry* target){
	try
	{
		return luabind::call_function<bool>(mMyLuaState,script.c_str(),boost::ref(country),boost::ref(target));
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
		
	}
	

}
bool BaseLogicLua::CheckForFinish(){
	try
	{
		return luabind::call_function<bool>(mMyLuaState, "victoryTest");
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}
	return false;
}
void  BaseLogicLua::PassDay(){
	try
	{
		luabind::call_function<void>(mMyLuaState, "passDay");
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}


}
void  BaseLogicLua::PassWeek(){
	try
	{
		luabind::call_function<void>(mMyLuaState, "passWeek");
		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}


}
bool  BaseLogicLua::DoAbility(Ogre::String name,AbilityType type,void* target){
	try
	{
	 
		switch(type){
			case ABILT_PLANET:
				{
					BasePlanet*targetCast = 	static_cast<BasePlanet*>(target);
					return luabind::call_function<bool>(mMyLuaState,"CallAbility",name,boost::ref(targetCast));
				}
			case ABILT_CORP:
				{
					BaseCorporate*targetCast = 	static_cast<BaseCorporate*>(target);
					return luabind::call_function<bool>(mMyLuaState,"CallAbility",name,boost::ref(targetCast));
				}
				break;
			case ABILT_BUILDING:
				{
					
					NonCoreBuilding*targetCast = 	static_cast<NonCoreBuilding*>(target);
					if(!PointerGuard<BaseBuilding>::GetSigleton()->Get(targetCast)){
						return false;
					}
					return luabind::call_function<bool>(mMyLuaState,"CallAbility",name,boost::ref(targetCast));
				}
				break;
			case ABILT_BATTLE:
				{
					BaseBattle*targetCast = 	static_cast<BaseBattle*>(target);
				
					if(!PointerGuard<BaseBattle>::GetSigleton()->Get(targetCast)){
						return false;
					}
					return luabind::call_function<bool>(mMyLuaState,"CallAbility",name,boost::ref(targetCast));
				}
				break;
			case ABILT_UNIT:
				{

					BaseUnit*targetCast = 	static_cast<BaseUnit*>(target);
					if(!PointerGuard<BaseUnit>::GetSigleton()->Get(targetCast)){
						return false;
					}
					return luabind::call_function<bool>(mMyLuaState,"CallAbility",name,boost::ref(targetCast));
				
				}
				break;
			case ABILT_CORE:
				{
					BaseBaseCore*targetCast = 	static_cast<BaseBaseCore*>(target);
					if(!PointerGuard<BaseBuilding>::GetSigleton()->Get(targetCast)){
						return false;
					}
					BaseCore*targetToRef  = (BaseCore*)targetCast;
					return luabind::call_function<bool>(mMyLuaState,"CallAbility",name,boost::ref(targetToRef));
				}
				break;
			case ABILT_DEPOSIT:
				{
					ResourseDeposits*targetCast = 	static_cast<ResourseDeposits*>(target);
					return luabind::call_function<bool>(mMyLuaState,"CallAbility",name,boost::ref(targetCast));
				}
				break;
			case ABILT_COUNTRY:
				{
					BasePlanetCountry*targetCast = 	static_cast<BasePlanetCountry*>(target);
					return luabind::call_function<bool>(mMyLuaState,"CallAbility",name,boost::ref(targetCast));
				}
				break;
			case ABILT_EMBASSY:
				{
					NonCoreBuilding*targetCast = 	static_cast<NonCoreBuilding*>(target);
					return luabind::call_function<bool>(mMyLuaState,"CallAbility",name,boost::ref(targetCast));
				}
				break;
		}


		/* ... */
	}
	catch(luabind::error& e)
	{      
		MessageBox(NULL, lua_tostring(mMyLuaState, -1), NULL, NULL);
	}
	return false;
}
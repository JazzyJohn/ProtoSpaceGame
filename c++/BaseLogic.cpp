#include "BaseLogic.h"
#include "PlantCountry.h"
#include "rapidjson/document.h"
using namespace Logic;
const int SEC_IN_YEAR =31556926;
const int SEC_IN_MONTH =2629743;
const int SEC_IN_DAY =86400;
BaseLogic::BaseLogic(void) :
mTime(0),
	mPause(false),
	mGameState(NOTSTART),
	mTimeWarp(0),
	mGameDate()

{
	mLoadState=0;

	mPlayer.name = Ogre::String();

}
BaseLogic::BaseLogic(Logic::BaseListener* listener) :
mTime(0),
	mPause(false),
	mGameState(NOTSTART),
	mTimeWarp(0),
	mGameDate(),
	mPlayer(),
	mEventGenerator()
{
	mListener =listener;
	mLoadState=0;
	mPlayer.name = Ogre::String("Player");


}


BaseLogic::~BaseLogic(void)
{


	//delete &mPlayer.name;
}
BaseCoreManager *BaseLogic::GetCore(){
	return &mCoreMng;
}
BaseLogic* BaseLogic::sigleton = 0;
void BaseLogic::Load(void){

	Ogre::String catalogName;
	switch(mGameType){
	case PT_NORMAL:
		catalogName="gamedata/people/";
		break;
	case PT_BUGS:
		catalogName="gamedata/bugs/";
		break;

	}
	sigleton = this;
	mLoadState=0;
	std::string s;
	std::ifstream inp(catalogName+"resource.json");
	if(!inp.good()){
		inp.open("gamedata/resource.json");
	}
	std::getline(inp, s, '\0');
	std::srand(std::time(0));

	rapidjson::Document d;
	d.Parse<0>(s.c_str());
	const rapidjson::Value& res = d["all_resource"]; // Using a reference for consecutive access is handy and faster.
	BaseResource **tempAr = new BaseResource *[ res.Size()];
	for (rapidjson::SizeType i = 0; i < res.Size(); i++){ // rapidjson uses SizeType instead of size_t.
		tempAr[i] =mResMng.addResource(res[i].GetString());
	}
	const rapidjson::Value& resObjs = d["all_resource_phys"];
	BaseResourceObject *object =0;
	Ogre::String str ="";
	std::map<int,	BaseResourceObject *> mapForId;
	for (rapidjson::SizeType i = 0; i < resObjs.Size(); i++){ // rapidjson uses SizeType instead of size_t.
		str= resObjs[i]["name"].GetString();

		object = new BaseResourceObject(str.c_str());

		object->mType =(ResourceType)resObjs[i]["type"].GetInt();
		for (rapidjson::SizeType j = 0; j < res.Size(); j++){
			object->mResource[tempAr[j]]=resObjs[i]["map"][j].GetInt();

		}
		object->SetTooltip();
		object->mRefinerable = resObjs[i]["refine"].GetBool();
		object->mTradePoint= resObjs[i]["tpoint"].GetInt();
		mResMng.addResourceObject(object);
		mapForId[ resObjs[i]["id"].GetInt()] = object;


	}
	const rapidjson::Value& resInt = d["all_resource_int"];

	std::vector<BaseResourceObject*> blackVector;
	std::vector<BaseResourceObject*> embasssyVector;
	std::vector<BaseResourceObject*> reconVector;
	for (rapidjson::SizeType i = 0; i < resInt.Size(); i++){ // rapidjson uses SizeType instead of size_t.
		str= resInt[i]["name"].GetString();

		object = new BaseResourceObject(str.c_str());

		object->mType =(ResourceType)resInt[i]["type"].GetInt();

		for (rapidjson::SizeType j = 0; j < res.Size(); j++){
			object->mResource[tempAr[j]]=resInt[i]["map"][j].GetInt();

		}
		object->SetTooltip();

		switch(resInt[i]["intType"].GetInt()){
		case 0:
			blackVector.push_back(object);
			break;
		case 1:
			embasssyVector.push_back(object);
			break;
		case 2:
			reconVector.push_back(object);
			break;
		}
		mapForId[ resInt[i]["id"].GetInt()] = object;

	}
	mIntMng.MakeAll(blackVector,embasssyVector,reconVector);

	const rapidjson::Value& prodmap = d["production_map"];
	std::map<BaseResourceObject*,int> recept;
	for (rapidjson::SizeType i = 0; i < prodmap.Size(); i++){ 
		if(prodmap[i]["refine"].GetBool()==true){
			mResMng.addResourceProduction(mapForId[prodmap[i]["from"].GetInt()],mapForId[prodmap[i]["result"].GetInt()],prodmap[i]["amount"].GetInt());
		}else{
			recept.clear();
			for(rapidjson::SizeType j=0;j < prodmap[i]["recept"].Size();j++){
				recept[mapForId[prodmap[i]["recept"][j]["from"].GetInt()]]=prodmap[i]["recept"][j]["amount"].GetInt();
			}
			mResMng.addResourceProduction(recept,mapForId[prodmap[i]["result"].GetInt()],prodmap[i]["amount"].GetInt());

		}
	}


	mLoadState=10;

	inp.close();
	inp.open(catalogName+"military.json");
	if(!inp.good()){
		inp.open("gamedata/military.json");
	}
	std::getline(inp, s, '\0');


	rapidjson::Document Mil;
	Mil.Parse<0>(s.c_str());
	const rapidjson::Value& chars = Mil["characteristick"]; // Using a reference for consecutive access is handy and faster.
	BaseUnitCharacteristics **charAr = new BaseUnitCharacteristics *[ chars.Size()];
	for (rapidjson::SizeType i = 0; i < chars.Size(); i++){ // rapidjson uses SizeType instead of size_t.
		charAr[i] = new BaseUnitCharacteristics(chars[i]["name"].GetString(), chars[i]["isLifeDep"].GetBool());
	}
	mUnitMng.setAllCharacteristics(charAr,chars.Size());
	std::map<BaseUnitCharacteristics*,float> mapchar;
	BaseUnitBlueprint *unittemp;
	const rapidjson::Value& units =Mil["units"]; // Using a reference for consecutive access is handy and faster.
	for (rapidjson::SizeType i = 0; i < units.Size(); i++){ // rapidjson uses SizeType instead of size_t.
		unittemp = new BaseUnitBlueprint( units[i]["name"].GetString());
		unittemp->SetCost( units[i]["cost"].GetInt());
		unittemp->mUniq = units[i]["uniq"].GetBool();
		unittemp->isNative = units[i]["isNative"].GetBool();
		for (rapidjson::SizeType j = 0; j< units[i]["map"].Size(); j++){ // rapidjson uses SizeType instead of size_t.
			mapchar[charAr[j]]= units[i]["map"][j].GetDouble();
		}
		unittemp->SetCharacteristik(mapchar);
		mapchar.clear();
		mUnitMng.AddUnit(unittemp->mName,unittemp);
	}

	const rapidjson::Value& unitsNpc =Mil["NPC_units"]; // Using a reference for consecutive access is handy and faster.
	BaseUnitBlueprint **unitAr= new BaseUnitBlueprint*[unitsNpc.Size()] ;
	for (rapidjson::SizeType i = 0; i < unitsNpc.Size(); i++){ // rapidjson uses SizeType instead of size_t.
		str= unitsNpc[i]["name"].GetString();
		unittemp = new BaseUnitBlueprint(str.c_str());
		unittemp->SetCost( unitsNpc[i]["cost"].GetInt());
		unittemp->isNative = unitsNpc[i]["isNative"].GetBool();
		for (rapidjson::SizeType j = 0; j< unitsNpc[i]["map"].Size(); j++){ // rapidjson uses SizeType instead of size_t.
			mapchar[charAr[j]]= unitsNpc[i]["map"][j].GetInt();
		}
		unittemp->SetCharacteristik(mapchar);
		unitAr[i] = unittemp;
		mapchar.clear();

	}
	mWarFare.Init(unitAr,unitsNpc.Size());
	delete []charAr;

	//std::ifstream inpPlanet("gamedata/planet.json");
	inp.close();
	inp.open(catalogName+"planet.json");
	if(!inp.good()){
		inp.open("gamedata/planet.json");
	}
	std::getline(inp, s, '\0');
	rapidjson::Document Dip;
	Dip.Parse<0>(s.c_str());
	const rapidjson::Value& charsCon = Dip["characteristick"]; // Using a reference for consecutive access is handy and faster.
	CountryCharacteristic ** charConAr= new CountryCharacteristic* [charsCon.Size()];
	CountryCharacteristic *temp =0;
	for (rapidjson::SizeType i = 0; i < charsCon.Size(); i++){ // rapidjson uses SizeType instead of size_t.
		temp = new CountryCharacteristic();
		temp->Max  = charsCon[i]["max"].GetInt();
		temp->Min  = charsCon[i]["min"].GetInt();
		temp->name  = charsCon[i]["name"].GetString();

		temp->ShowType =(ShowCharType)charsCon[i]["showType"].GetInt();
		temp->type =(CharacteristicType)charsCon[i]["type"].GetInt();
		if(temp->type==CT_DELTA){
			for(rapidjson::SizeType j = 0; j < charsCon[i]["map"].Size(); j++){

				temp->TradeDependace[tempAr[j]]=charsCon[i]["map"][j].GetDouble();
			}

		}else{
			temp->Open  = charsCon[i]["open"].GetBool();
		}		
		charConAr[i]=temp;

	}
	mCharMng.setAllCharacteristics(charConAr,charsCon.Size());
	inp.close();
	inp.open(catalogName+"buildings.json");
	if(!inp.good()){
		inp.open("gamedata/buildings.json");
	}
	std::getline(inp, s, '\0');
	rapidjson::Document Building;
	Building.Parse<0>(s.c_str());
	const rapidjson::Value& bluperints = Building["blueprints"]; // Using a reference for consecutive access is handy and faster.
	BuildingBlueprint **allBlueperint= new BuildingBlueprint* [bluperints.Size()];
	BuildingBlueprint *tempBlue =0;
	for (rapidjson::SizeType i = 0; i < bluperints.Size(); i++){ 
		tempBlue = new BuildingBlueprint(bluperints[i]["type"].GetInt(),bluperints[i]["name"].GetString());
		tempBlue->mCost=bluperints[i]["cost"].GetInt();
		tempBlue->mDescriprion=bluperints[i]["name"].GetString();
		tempBlue->mPlace =(BUILDING_PLACE_TYPE) bluperints[i]["place"].GetInt();
		if(tempBlue->mPlace!=0){
			tempBlue->mOrbitCost =bluperints[i]["orbitcost"].GetInt(); 
		}
		tempBlue->mOpen= bluperints[i]["open"].GetBool();;
		tempBlue->mUniq= bluperints[i]["uniq"].GetBool();;
		for(rapidjson::SizeType j = 0; j < bluperints[i]["dayly_cost"].Size(); j++){

			tempBlue->mCostMap[(BuildingWorkMode)j]= bluperints[i]["dayly_cost"][j].GetInt();
		}
		allBlueperint[i]= tempBlue;
	}
	mCoreMng.SetAllBuildingBlueprint(allBlueperint,bluperints.Size());

	inp.close();
	inp.open(catalogName+"research.json");
	if(!inp.good()){
		inp.open("gamedata/research.json");
	}
	std::getline(inp, s, '\0');
	rapidjson::Document Researchs;
	Researchs.Parse<0>(s.c_str());
	const rapidjson::Value& allDepartments = Researchs["all_departments"]; 
	rapidjson::Value allResearchLvls; 	// Using a reference for consecutive access is handy and faster.

	ResearchDepartment **allDepRes= new ResearchDepartment* [allDepartments.Size()];
	ResearchDepartment *tempDepRes=0;
	std::vector<ResearchLvl *>allBResLvl;
	ResearchLvl *tempResLvl =0;
	BaseResearch **allLvlResearch =0;
	BaseResearch *tempResearch=0;
	std::map<BaseResource*,int> map;

	for(rapidjson::SizeType dNum = 0; dNum < allDepartments.Size(); dNum++){

		allResearchLvls = Researchs[allDepartments[dNum].GetString()]; 
		tempDepRes =new ResearchDepartment();
		allDepRes[dNum] =tempDepRes;
		allBResLvl.clear();
		ResearchDepartment::mLvlNum = allResearchLvls.Size();
		for (rapidjson::SizeType i = 0; i < allResearchLvls.Size(); i++){ 
			allLvlResearch = new BaseResearch*[allResearchLvls[i].Size()];
			for(rapidjson::SizeType j = 0; j < allResearchLvls[i].Size(); j++){
				tempResearch = new BaseResearch(allResearchLvls[i][j]["url"].GetString(),allResearchLvls[i][j]["name"].GetString());
				map.clear();
				for(rapidjson::SizeType k = 0; k < allResearchLvls[i][j]["map"].Size(); k++){
					map[tempAr[k]]=  allResearchLvls[i][j]["map"][k].GetInt();
				}
				tempResearch->CreateMap(map);
				tempResearch->mDepartment =allDepartments[dNum].GetString();
				allLvlResearch[j] = tempResearch;
			}
			tempResLvl = new ResearchLvl(i,allLvlResearch,allResearchLvls[i].Size());
			allBResLvl.push_back(tempResLvl);
		}

		tempDepRes->Init(allBResLvl);

	}

	mReseasrchMng.Init(allDepRes,allDepartments.Size());


	inp.close();
	inp.open(catalogName+"request.json");
	if(!inp.good()){
		inp.open("gamedata/request.json");
	}
	std::getline(inp, s, '\0');
	rapidjson::Document Requests;
	Requests.Parse<0>(s.c_str());
	const rapidjson::Value& allRequest= Requests["all_request"]; // Using a reference for consecutive access is handy and faster.
	BaseRequest **allRequestAr= new BaseRequest* [allRequest.Size()];
	for(rapidjson::SizeType i = 0; i < allRequest.Size(); i++){
		allRequestAr[i] = new BaseRequest(tempAr[allRequest[i]["dep"].GetInt()]);
		allRequestAr[i]->mAmount =allRequest[i]["amount"].GetInt();
		allRequestAr[i]->mSuccsessCredit =allRequest[i]["credit"].GetInt();
		allRequestAr[i]->mSuccsessRelation =allRequest[i]["relation"].GetInt();
		allRequestAr[i]->mTimeOf =allRequest[i]["time"].GetInt();
		allRequestAr[i]->mDescription =allRequest[i]["name"].GetString();
		allRequestAr[i]->mId =allRequest[i]["id"].GetInt();
	}
	const rapidjson::Value& allProject= Requests["all_project"]; // Using a reference for consecutive access is handy and faster.
	BaseProject **allProjectAr= new BaseProject* [allProject.Size()];
	for(rapidjson::SizeType i = 0; i < allProject.Size(); i++){
		allProjectAr[i] = new BaseProject(tempAr[allProject[i]["dep"].GetInt()]);
		allProjectAr[i]->mName =allProject[i]["name"].GetString();
		allProjectAr[i]->mDescription =allProject[i]["descr"].GetString();
		allProjectAr[i]->mPeriodNeed =allProject[i]["periodNeed"].GetInt();
		allProjectAr[i]->mPeriodCnt =allProject[i]["periodCnt"].GetInt();
		allProjectAr[i]->mType =(ProjectPeriodType)allProject[i]["type"].GetInt();
		allProjectAr[i]->mSuccsessCredit =allProject[i]["succsessCredit"].GetInt();
		allProjectAr[i]->mSuccsessRelation =allProject[i]["succsessRelation"].GetInt();
		allProjectAr[i]->mUnpaidFaliureRelation =allProject[i]["unpaidFaliureRelation"].GetInt();
		allProjectAr[i]->mRefundCredit =allProject[i]["refundCredit"].GetInt();
		allProjectAr[i]->mIsCallback =allProject[i]["isCallback"].GetBool();
		allProjectAr[i]->mCallType =(ProjectCallBackType)allProject[i]["callType"].GetInt();
		allProjectAr[i]->mId =allProject[i]["id"].GetInt();
	}
	mRequestMan.Init(allRequestAr,allRequest.Size(),&mCorp,&mResMng,&mIntMng,allProjectAr,allProject.Size(),&mCoreMng);

	inp.close();
	inp.open(catalogName+"news.json");
	if(!inp.good()){
		inp.open("gamedata/news.json");
	}
	std::getline(inp, s, '\0');
	rapidjson::Document News;
	Requests.Parse<0>(s.c_str());
	const rapidjson::Value& badNews= Requests["badNews"]; // Using a reference for consecutive access is handy and faster.
	const rapidjson::Value& goodNews= Requests["goodNews"]; // Using a reference for consecutive access is handy and faster.
	BaseNews **badNewsAr= new BaseNews* [badNews.Size()];
	for(rapidjson::SizeType i = 0; i < badNews.Size(); i++){
		badNewsAr[i] = new BaseNews();
		badNewsAr[i]->mDescription = badNews[i]["name"].GetString();
		badNewsAr[i]->mSuccsessRateDelta = badNews[i]["succsessRate"].GetDouble();
		badNewsAr[i]->mTotalKPDDelta = badNews[i]["kpdDelta"].GetInt();
		badNewsAr[i]->mProjectNpcDelta = badNews[i]["projectNpc"].GetInt();
	}
	BaseNews **goodNewsAr= new BaseNews* [goodNews.Size()];
	for(rapidjson::SizeType i = 0; i < goodNews.Size(); i++){
		goodNewsAr[i] = new BaseNews();
		goodNewsAr[i]->mDescription = goodNews[i]["name"].GetString();
		goodNewsAr[i]->mSuccsessRateDelta = goodNews[i]["succsessRate"].GetDouble();
		goodNewsAr[i]->mTotalKPDDelta = goodNews[i]["kpdDelta"].GetInt();
		goodNewsAr[i]->mProjectNpcDelta = goodNews[i]["projectNpc"].GetInt();
	}
	mCorp.Init(tempAr,res.Size(),goodNewsAr,goodNews.Size(),badNewsAr, goodNews.Size());
	inp.close();
	inp.open(catalogName+"effects.json");
	if(!inp.good()){
		inp.open("gamedata/effects.json");
	}
	std::getline(inp, s, '\0');
	rapidjson::Document Effects;
	Effects.Parse<0>(s.c_str());
	const rapidjson::Value& effects= Effects["all_effects"]; // Using a reference for consecutive access is handy and faster.
	BaseEffect **effectnArTemp = new BaseEffect*[effects.Size()];
	BaseEffect *tempEffect = 0;
	for(rapidjson::SizeType i = 0; i < effects.Size(); i++){
		tempEffect = new BaseEffect();
		tempEffect->mName  = effects[i]["name"].GetString();
		tempEffect->mType =(EffectType) effects[i]["type"].GetInt();
		tempEffect->mChangeType =(ChangeEffectType) effects[i]["changetype"].GetInt();
		tempEffect->mProductionDelta = effects[i]["mProductionDelta"].GetDouble();
		tempEffect->mTime = effects[i]["time"].GetInt();
		effectnArTemp[i]= tempEffect;
	}
	mEffMan.Init(effectnArTemp,effects.Size());



	inp.close();
	inp.open(catalogName+"action.json");
	if(!inp.good()){
		inp.open("gamedata/action.json");
	}
	std::getline(inp, s, '\0');
	rapidjson::Document Action;
	Action.Parse<0>(s.c_str());
	const rapidjson::Value& actions= Action["all_action"]; // Using a reference for consecutive access is handy and faster.
	BaseAction **actionArTemp = new BaseAction*[actions.Size()];
	BaseAction *tempAction = 0;
	for(rapidjson::SizeType i = 0; i < actions.Size(); i++){
		tempAction = new BaseAction();
		tempAction->name  = actions[i]["name"].GetString();
		tempAction->type =(ActonType) actions[i]["type"].GetInt();
		tempAction->dayCount = actions[i]["dayCount"].GetInt();
		tempAction->doneInSecret =  actions[i]["doneInSecret"].GetBool();
		tempAction->open =  actions[i]["open"].GetBool();
		actionArTemp[i]= tempAction;
	}
	mActionMng.Init(actionArTemp,actions.Size());

	inp.close();
	inp.open(catalogName+"balance.json");
	if(!inp.good()){
		inp.open("gamedata/balance.json");
	}
	std::getline(inp, s, '\0');

	rapidjson::Document Balance;
	Balance.Parse<0>(s.c_str());
	const rapidjson::Value& balans_var= Balance["all_vars"]; // Using a reference for consecutive access is handy and faster.
	for(rapidjson::SizeType i = 0; i < balans_var.Size(); i++){
		mAssetMng.Add(balans_var[i]["name"].GetString(),balans_var[i]["value"].GetDouble());
	}
	inp.close();
	inp.open(catalogName+"ability.json");
	if(!inp.good()){
		inp.open("gamedata/ability.json");
	}
	std::getline(inp, s, '\0');

	rapidjson::Document Ability;
	Ability.Parse<0>(s.c_str());
	BaseAbility*tempAbility;
	const rapidjson::Value& abils= Ability["all_ability"]; // Using a reference for consecutive access is handy and faster.
	for(rapidjson::SizeType i = 0; i < abils.Size(); i++){
		tempAbility=new BaseAbility((AbilityType)abils[i]["type"].GetInt(),abils[i]["url"].GetString(),abils[i]["name"].GetString());
		mAbilityMng.AddAbility(tempAbility);
		if(abils[i]["open"].GetBool()){
			tempAbility->Open();
		}
		if(abils[i].HasMember("timeout")){
			tempAbility->mTimeOut=abils[i]["timeout"].GetInt();

		}else{
			tempAbility->mTimeOut=0;
		}

	}
	inp.close();
	inp.open(catalogName+"ai.json");
	if(!inp.good()){
		inp.open("gamedata/ai.json");
	}
	std::getline(inp, s, '\0');

	rapidjson::Document Aicol;
	Aicol.Parse<0>(s.c_str());
	BaseAINode*tempAiNode;
	std::map<int,BaseAINode*> indexAi;
	std::map<Ogre::String,BaseAINode*> mapAi;
	const rapidjson::Value& ails= Aicol["all_ability"]; // Using a reference for consecutive access is handy and faster.

	for(rapidjson::SizeType i = 0; i < ails.Size(); i++){
		tempAiNode =new BaseAINode();
		tempAiNode->mScriptName  = ails[i]["script"].GetString();
		indexAi[ ails[i]["id"].GetInt()] = tempAiNode;
		mapAi[ails[i]["name"].GetString()] = tempAiNode;
	}
	for(rapidjson::SizeType i = 0; i < ails.Size(); i++){
		tempAiNode = mapAi[ails[i]["name"].GetString()];
		for(rapidjson::SizeType j = 0; j < ails[i]["next"].Size(); j++){
			tempAiNode->NextNodes[ ails[i]["next"][j]["conditionName"].GetString()]= indexAi[ails[i]["next"][j]["Id"].GetInt()];
		}
		
	}
	mAiMng.SetMap(mapAi);
	delete [] tempAr;
	return;
}

void  BaseLogic::Init(void){
	mDay=0;
	mLuaMan.Init(mGameType);
	std::srand(std::time(0));
	mTimeWarp=BaseAsset::GetSigleton("TIME_WARP");
	mNotificator.Init(mListener);
	BaseBuilding *core=0;
	BaseBuilding *war=0;
	mCoreMng.GenrateInit(&mResMng,&core,&war);
	BuildingBuiltEvnt *coreEvnt = new BuildingBuiltEvnt();
	coreEvnt->building = core;
	coreEvnt->buildingType = core->mType;
	mListener->fireGameEvent(coreEvnt);
	coreEvnt = new BuildingBuiltEvnt();
	coreEvnt->building = war;
	coreEvnt->buildingType = war->mType;
	mListener->fireGameEvent(coreEvnt);
	int a = std::rand() % 5;
	mResMng.mAccount =  &mAccount;
	switch(mGameType){
	case PT_BUGS:
		mPlanet= BasePlanet();
		break;	
	case PT_PLANTS:
		mPlanet= PlantPlanet();
		break;
	case PT_NORMAL:
		mPlanet= BasePlanet();
		break;
	}
	
	PlanetJustGenerateEvent *evnt =mPlanet.GeneratePlanet( a+7,3,PET_NORMAL,&mCharMng,3,&mResMng);
	mAiMng.SetPlanet(&mPlanet);
	mLuaMan.SetPlanet(mPlanet);
	mLuaMan.SetAbilityMng(&mAbilityMng);
	mLuaMan.SetActionMng(&mActionMng);
	mLuaMan.SetCoreMng(&mCoreMng);
	mLuaMan.SetAsset(&mAssetMng);
	mLuaMan.SetUnitMng(&mUnitMng);
	mRecMng.SetPlanet(&mPlanet);
	mListener->fireGameEvent(evnt);
	mGameDate.year=3000;
	mGameDate.days =1;
	mGameDate.month =1;
}
void BaseLogic::DestroyBuilding(BaseBuilding* build){
	mToDeleteBuilding.push(build);
}
void BaseLogic::DeleteResolved(){
	while (!mToDeleteBuilding.empty())
	{
		BaseBuilding* pntr =mToDeleteBuilding.front();
		switch(pntr->mType){
		case TB_BASECORE:
			mPlanet.DeleteBuilding(pntr);
			
			
			std::vector<BaseCore*>::iterator coreIterator;

			for(coreIterator=mCoreMng.mAllBase.begin();coreIterator!=mCoreMng.mAllBase.end();++coreIterator){
				if((*coreIterator)==static_cast<BaseBaseCore*>(pntr)){
					mCoreMng.mAllBase.erase(coreIterator);
					return ;
				}
			}
			
			break;
		 

		}
		mBuildingPointer.Remove(pntr);
		delete pntr;
		mToDeleteBuilding.pop();
	}

}
bool BaseLogic::timeElapse(float time){
	if(true){//if(!mPause&&!mGameState==START){
		mTime += time;
	}
	std::div_t  year = std::div(mTime*mTimeWarp,SEC_IN_YEAR);
	std::div_t	month = std::div(year.rem,SEC_IN_MONTH);
	std::div_t	days = std::div(month.rem,SEC_IN_DAY);
	days.quot++;
	month.quot++;
	year.quot+=3000;
	mDeltaDate = DC_NONE;
	mDeltaWeek= DC_NONE;
	if(mGameDate.year!= year.quot){
		TimeGameEvent *evnt = new TimeGameEvent();
		evnt->timeType = DC_YEAR;
		mDeltaDate= DC_YEAR;
		evnt->type = ET_TIME;
		mListener->fireDateChange(evnt);
		delete evnt;
		if(mGameDate.days%7==0){
			mDeltaWeek = DC_WEEK;
		}
	}else{
		if(mGameDate.month!= month.quot){
			TimeGameEvent *evnt = new TimeGameEvent();
			evnt->timeType = DC_MONTH;
			mDeltaDate= DC_MONTH;
			evnt->type = ET_TIME;
			mListener->fireDateChange(evnt);
			delete evnt;
			if(mGameDate.days%7==0){
				mDeltaWeek = DC_WEEK;
			}
		}else{
			if(mGameDate.days!= days.quot){
				TimeGameEvent *evnt = new TimeGameEvent();
				mDeltaDate= DC_DAYS;
				evnt->timeType = DC_DAYS;
				evnt->type = ET_TIME;
				mListener->fireDateChange(evnt);
				delete evnt;
				if(mGameDate.days%7==0){
					mDeltaWeek = DC_WEEK;
				}
			}
		}
	}
	mGameDate.days = days.quot;
	mGameDate.month = month.quot;
	mGameDate.year =year.quot;

	return true;
}

void BaseLogic::getFormatedDateTime(Ogre::String* outbuffer) const{


	std::stringstream ss;

	ss<< mGameDate.days <<"/"<< mGameDate.month<<"/"<<mGameDate.year;

	ss>>*outbuffer;


}
void BaseLogic::getFormatedTime(Ogre::String* outbuffer,TimeFormat form) const{
	outbuffer->clear();
	int gameTime = mTime*mTimeWarp;
	switch(form){
	case DF_SIMPLE:
		LogicStd::strToInt(outbuffer,mTime);
		break;
	case  DF_INT:
		LogicStd::strToInt(outbuffer,gameTime);
		break;
	case  DF_DAYS:
		gameTime =gameTime/SEC_IN_DAY;
		LogicStd::strToInt(outbuffer,gameTime);
		break;
	case  DF_NORMAL:
		getFormatedDateTime(outbuffer);
		break;
	}

}

void   BaseLogic::SetMutex(boost::mutex *other){
	mMutex =other;
}
void  BaseLogic::SetState(GameState state){
	mGameState = state;
}
void BaseLogic::gameLoop(PlanetType gameType){
	mGameType = gameType;
	Load();
	Init();
	while(mGameState!=OVER){
		clock_t start = clock();

		//lock.lock();
		Sleep(50);

		CalculateGameLogic();
		resolvedQueue();
		DeleteResolved();
		if(mLuaMan.CheckForFinish()){
			mGameState =OVER;
		}
		double deltaTime =((double)clock() - start) / CLOCKS_PER_SEC;

		if(mGameState==OVER){
			break;
		}
		if(!mPause){
			boost::mutex::scoped_lock lock(*mMutex);//create a lock
			this->timeElapse(deltaTime);
			lock.unlock();
			mEventGenerator.IncrementTime(deltaTime);


		}
		GameEvent *curEvent=0;
		if(mEventGenerator.GetEvent(&curEvent)){

			mListener->fireGameEvent(curEvent);
		}

	}
	delete mMutex;
	delete this;

}
void BaseLogic::fireGameEvent(ControlEvent *evnt){
	boost::mutex::scoped_lock lock(*mMutex);//create a lock
	mGameEventStack.push(evnt);
	lock.unlock();

}
void BaseLogic::resolvedQueue() {
	ControlEvent *curevent =0;
	while(!mGameEventStack.empty()){

		boost::mutex::scoped_lock lock(*mMutex);//create a lock
		curevent =  mGameEventStack.front();
		mGameEventStack.pop();
		lock.unlock();




		switch(curevent->controlType){
		case CTE_SHOWRESOURCE:
			{
				ShowResourceEvnt *resevnt = new ShowResourceEvnt();  	
				resevnt->mResorseCnt= mResMng.ShowResourse();
				mListener->fireGameEvent(resevnt);
			}
			break;
		case CTE_DELETEBUILDING:
			{
				ShowBuilding *curBuildEvnt = static_cast<ShowBuilding *>(curevent);
				if(!mBuildingPointer.Get(curBuildEvnt->building)){
					continue;
				}

			}
			break;
		case CTE_SHOWBUILDING:
			{


				ShowBuilding *curBuildEvnt = static_cast<ShowBuilding *>(curevent);
				if(!mBuildingPointer.Get(curBuildEvnt->building)){
					continue;
				}
				GameEvent *evnt =curBuildEvnt->building->ReturnBuildingInfo();
				evnt->isReSend = curevent->isReSend;
				mListener->fireGameEvent(evnt);
			}
			break;
		case CTE_ASKFORBLUEPRINT:
			{
				ShowBlueprint *curBlueEvnt = static_cast<ShowBlueprint *>(curevent);
				SendBlueToGui *bluetoSend= new SendBlueToGui();
				bluetoSend->allBlue = 	mCoreMng.avaibleBlueprint(curBlueEvnt->core);
				mListener->fireGameEvent(bluetoSend);
			}
			break;
		case CTE_ASKFORRESEARCH:
			{
				GameEvent *evnt =mReseasrchMng.ReturnResearchInfo();
				evnt->isReSend = curevent->isReSend;
				mListener->fireGameEvent(evnt);
			}
			break;
		case CTE_BUILDSOMETHING:
			{
				BuildOrder *curBlueEvnt = static_cast<BuildOrder *>(curevent);

				BaseBuilding *build=0;
				if(mCoreMng.BuildBuilding(curBlueEvnt->blueprint,curBlueEvnt->core,&build)){
					BuildingBuiltEvnt *coreEvnt = new BuildingBuiltEvnt();
					// POST Building Setting
					switch(build->mType){
					case TB_RESEARCH:
						{
							BaseResearchBuilding *lab = static_cast<BaseResearchBuilding *>(build);
							lab->SetManager(&mReseasrchMng);
						}
						break;
					case TB_MILITARY:
						{
							BaseMilitaryBuilding *mil = static_cast<BaseMilitaryBuilding *>(build);
							mil->AddManager(&mUnitMng);
						}
						break;
					case TB_PRODUCTION:
						{
							BaseProduction *mil = static_cast<BaseProduction *>(build);
							mil->AddManager(&mResMng);
						}
						break;
					case TB_EMBASSY:
					case TB_BLACKOPS:
						{
							BaseEmbassyBuilding *em = static_cast<BaseEmbassyBuilding *>(build);
							em->SetManager(&mIntMng,&mActionMng);
						}
						break;

					}

					coreEvnt->building = build;
					coreEvnt->buildingType = build->mType;
					mListener->fireGameEvent(coreEvnt);

				}
			}
			break;
		case CTE_ASKFORCOUNTRY:
			{
				SomethingSimple *evnt = static_cast<SomethingSimple *>(curevent);
				BasePlanetCountry *cntr =  static_cast<BasePlanetCountry *>(evnt->target);
				mListener->fireGameEvent(cntr->SendEvent());
			}
			break;
		case CTE_BUILDCOREINCOUNTRY:
			{
				SomethingSimple *buildCore = static_cast<SomethingSimple *>(curevent);
				BasePlanetCountry *coreTarg =  static_cast<BasePlanetCountry *>(buildCore->target);
				BaseBuilding *newCore=0;
				CountryTile *temp;
				if(mCoreMng.BuildCore(coreTarg,&newCore,&temp)){
					CoreBuiltEvnt *newCoreEvnt = new CoreBuiltEvnt();
					newCoreEvnt->building = newCore;
					newCoreEvnt->tile = temp;
					newCoreEvnt->country = coreTarg;
					mListener->fireGameEvent(newCoreEvnt);
				}
			}
			break;
		case CTE_SETDEPOSITE:
			{
				SetDeposit *depSet = static_cast<SetDeposit *>(curevent);
				if(!mBuildingPointer.Get(depSet->building)){
					continue;
				}
				BaseMining *mine =  static_cast<BaseMining *>(depSet->building);
				mine->SetDeposits(depSet->deposit);
			}
			break;
		case CTE_SETPRODUCTION:
			{

				ProductSet *depSet = static_cast<ProductSet *>(curevent);
				if(!mBuildingPointer.Get(depSet->building)){
					continue;
				}
				BaseProduction *mine =  static_cast<BaseProduction *>(depSet->building);
				if(depSet->production->refine){
					mine->SetProductionLine(depSet->production->from);
				}else{

					mine->SetProductionLine(depSet->production,depSet->production->result);
				}

			}
			break;
		case CTE_SETRESEARCH:
			{
				SomethingSimple *setResearch = static_cast<SomethingSimple *>(curevent);
				BaseResearch *reserach =  static_cast<BaseResearch *>(setResearch->target);
				if(mReseasrchMng.SetResearch(reserach)){
					mListener->fireGameEvent(mReseasrchMng.ReturnResearchInfo());
				}

			}
			break;
		case CTE_ASKDETAILEDRESOURSE:
			{
				ShowAllResourse *resAllecnt = new ShowAllResourse();
				mResMng.ShowObjectsAll(resAllecnt->amount,resAllecnt->links);
				mIntMng.ShowObjectsAll(resAllecnt->intAmount,resAllecnt->intLinks);
				mListener->fireGameEvent(resAllecnt);
			}
			break;
		case CTE_ASKDETAILEDRESOURSERESEARCH:
			{
				ShowAllResourse *resAllRes = new ShowAllResourse();
				mResMng.ShowObjectsAll(resAllRes->amount,resAllRes->links);
				mIntMng.ShowObjectsAll(resAllRes->intAmount,resAllRes->intLinks);
				resAllRes->gameType = GUIET_SHOWALLRESOURSERES;
				mListener->fireGameEvent(resAllRes);
			}
			break;
		case CTE_USERESOURCEONRESEARCH:
			{

				SomethingSimple *setResearchObject = static_cast<SomethingSimple *>(curevent);
				BaseResourceObject *reserach =  static_cast<BaseResourceObject *>(setResearchObject->target);
				BaseResourceObject *backp=0;
				if(mReseasrchMng.isActive()){
					if(reserach->mType == RT_INT){
						if(mIntMng.GetResource(reserach)){
							mReseasrchMng.addProgress(reserach);
							mListener->fireGameEvent(mReseasrchMng.ReturnResearchInfo());

						}
					}
					else{
						if(mCoreMng.GetResource(reserach->mName,&backp)){
							mReseasrchMng.addProgress(backp);
							mListener->fireGameEvent(mReseasrchMng.ReturnResearchInfo());

						}
					}


				}
			}
			break;
		case CTE_SHOWREQUESTS:
			{
				GameEvent *evnt =	mRequestMan.ReturnAllPendingWithStatus();
				evnt->isReSend = curevent->isReSend;
				mListener->fireGameEvent(evnt);
			}
			break;
		case CTE_RESOURSEREDIALOG:
			{
				mPause = true;
				SomethingSimple *setReqDialogGen = static_cast<SomethingSimple *>(curevent);
				BaseRequest *request =  static_cast<BaseRequest *>(setReqDialogGen->target);
				mListener->fireGameEvent(mRequestMan.GiveSendStatus(request));
			}
			break;
		case CTE_RESOURSEREDIALOGDECLINE:
			{
				mPause= false;	
				mRequestMan.DeclineRequest();
				GameEvent * evnt = new GameEvent();
				evnt->gameType= GUIET_CLOSEREQRESDIALOG;
				mListener->fireGameEvent(evnt);
			}
			break;

		case CTE_RESOURSEREDIALOGSEND:
			{

				if(mRequestMan.SendRequest()){
					GameEvent * evnt = new GameEvent();
					evnt->gameType= GUIET_CLOSEREQRESDIALOG;
					mListener->fireGameEvent(evnt);
					mPause= false;	
					mListener->fireGameEvent(mRequestMan.ReturnAllPendingWithStatus());
				}
			}
			break;
		case CTE_RESOURSEREDIALOGCHANGE:
			{
				mPause = true;
				SenResorceChng *resChageDial = static_cast<SenResorceChng *>(curevent);
				if(resChageDial->sign<0){
					mRequestMan.DeleteObjectToRequest(resChageDial->target);
				}else{
					mRequestMan.AddObjectToRequest(resChageDial->target);
				}

				mListener->fireGameEvent(mRequestMan.GiveSendStatus());
			}
			break; 
		case CTE_DIALOGFORACTION:
			{
				mPause = true;

				AskIntActionDialog *actionDialogAsk = static_cast<AskIntActionDialog *>(curevent);
				if(!mBuildingPointer.Get(actionDialogAsk->target)){
					continue;
				}
				mListener->fireGameEvent(mIntMng.GiveActionDialog(actionDialogAsk->target,actionDialogAsk->action));
			}
			break;
		case CTE_DIALOGFORACTIONSETSOMTHING:
			{
				mPause = true;
				SetSomtingIntAction *somthingSetInt= static_cast<SetSomtingIntAction *>(curevent);
				mListener->fireGameEvent(mIntMng.SetActionDialogSomething(somthingSetInt->target,somthingSetInt->isTarget));
			}
			break;
		case CTE_DIALOGFORACTIONGSEND:
			{
				mPause = false;
				mListener->fireGameEvent(mIntMng.AceptActionDialog());
			}
			break;
		case CTE_DIALOGFORACTIONDECLINE:
			{
				mPause = false;
				mListener->fireGameEvent(mIntMng.DeclineActionDialog());
			}
			break;
		case CTE_ADDUNITTOQUEUE:
			{
				AddUnittoQueue *addUnitQueue= static_cast<AddUnittoQueue *>(curevent);
				BaseMilitaryBuilding *milbuilding=static_cast<BaseMilitaryBuilding*>(addUnitQueue->building);
				if(!mBuildingPointer.Get(addUnitQueue->building)){
					continue;
				}
				milbuilding->AddUnitToQueue(addUnitQueue->blueprint->mName);
				mListener->fireGameEvent(milbuilding->ReturnBuildingInfo());
			}
			break;
		case CTE_ACCEPTPROJECT:
			{
				SomethingSimple *simpleEvnt = static_cast<SomethingSimple *>(curevent);
				BaseProject *project =  static_cast<BaseProject *>(simpleEvnt->target);
				mListener->fireGameEvent(mRequestMan.AcceptProject(project));
			}
			break;
		case CTE_DECLINEPROJECT:
			{
				SomethingSimple *simpleEvnt = static_cast<SomethingSimple *>(curevent);
				BaseProject *project =  static_cast<BaseProject *>(simpleEvnt->target);
				mListener->fireGameEvent(mRequestMan.DeclineProject(project));
			}
			break;
		case CTE_OPENPROJECTDIALOG:
			{
				mPause = true;
				SomethingSimple *simpleEvnt = static_cast<SomethingSimple *>(curevent);
				BaseProject *project =  static_cast<BaseProject *>(simpleEvnt->target);
				mListener->fireGameEvent(mRequestMan.GiveProjectStatus(project));
			}
			break;
		case CTE_RESOURCEPROJDIALOGCHANGE:
			{
				mPause = true;
				SenResorceChng *resChageDial = static_cast<SenResorceChng *>(curevent);
				if(resChageDial->sign<0){
					mRequestMan.DeleteObjectToProject(resChageDial->target);
				}else{
					mRequestMan.AddObjectToProject(resChageDial->target);
				}

				mListener->fireGameEvent(mRequestMan.GiveProjectStatus());
			}
			break; 
		case CTE_RESOURSEPROJDIALOGDECLINE:
			{
				mPause= false;	
				mRequestMan.DeclineProject();
				GameEvent * evnt = new GameEvent();
				evnt->gameType= GUIET_CLOSEPROJRESDIALOG;
				mListener->fireGameEvent(evnt);
			}
			break;

		case CTE_RESOURSEPROJDIALOGSEND:
			{

				mRequestMan.SendProject();
				mListener->fireGameEvent(mRequestMan.GiveProjectStatus());
			}
			break;
		case CTE_ASKTRADE:
			{

				TradeRequest *tradeReqEvnt= static_cast<TradeRequest *>(curevent);

				if(!mBuildingPointer.Get(tradeReqEvnt->building)){
					continue;
				}
				NonCoreBuilding *temp  =static_cast< NonCoreBuilding *>(tradeReqEvnt->building);
				temp->mCore->mCountry->SetTrade(tradeReqEvnt->demand);
			}
			break;
		case CTE_UNITCLICK:
			{
				SomethingSimple *simpleEvnt = static_cast<SomethingSimple *>(curevent);
				BaseUnit *unit =  static_cast<BaseUnit *>(simpleEvnt->target);

				if(!mUnitPointer.Get(unit)){
					continue;
				}
				if(unit->mCountry==0){
					ShowUnitInBase *evnt = new ShowUnitInBase();
					evnt->name = unit->SayName();
					unit->SayCharacter(evnt->character);
					unit->SayAbility(evnt->abilitys);
					evnt->unit = unit;
					mPlanet.GetAllCountry(evnt->allCountry);
					mListener->fireGameEvent(evnt);
				}else{
					mListener->fireGameEvent(mWarFare.ShowWarfare(unit->mCountry));

				}

			}
			break;
		case CTE_SENDUNIT:
			{
				SendUnitToCountry *simpleEvnt = static_cast<SendUnitToCountry *>(curevent);


				if(!mUnitPointer.Get(simpleEvnt->unit)){
					continue;
				}
				mWarFare.MoveUnit(simpleEvnt->country,simpleEvnt->unit);
				GameEvent * evnt = new GameEvent();
				evnt->gameType= GUIET_CLOSEUNITDIALOG;
				mListener->fireGameEvent(evnt);
				mListener->fireGameEvent(mWarFare.ShowWarfare(simpleEvnt->country));

			}
			break;
		case CTE_SHOWWARAFERE:
			{
				SomethingSimple *simpleEvnt = static_cast<SomethingSimple *>(curevent);
				BaseCountry *country =  static_cast<BaseCountry *>(simpleEvnt->target);
				bool reDraw= false;
				GameEvent *evnt=mWarFare.ShowWarfare(country,reDraw);
				evnt->isReSend = simpleEvnt->isReSend&&(!reDraw);
				mListener->fireGameEvent(evnt);
			}
			break;
		case CTE_SETWARFAREACTION:
			{
				ActionWarfareSet *sendEvnt = static_cast<ActionWarfareSet *>(curevent);
				mWarFare.SetAction(sendEvnt->country,sendEvnt->action);
			}
			break;
		case CTE_SHOWRECON:
			{

				GameEvent * recEvnt =mRecMng.ReturnReconState();
				recEvnt->isReSend = curevent->isReSend;
				mListener->fireGameEvent(recEvnt);
			}
			break;
		case CTE_SETRECON:
			{

				mRecMng.LaunchRecont();

			}
			break;
		case CTE_ACTIVATEABILITY:
			{
				ActivateAbility *sendEvnt = static_cast<ActivateAbility *>(curevent);
				sendEvnt->ability->MakeItSo(sendEvnt->target);
			}
			break;
		case CTE_PAUSEGAME:
			mPause= true;
			break;
		case CTE_RESUMEGAME:
			mPause= false;
			break;
		case CTE_SAVEGAME:
			mSaveMan.Save("test");
			break;
		}
		delete curevent;

	}
}
BaseLogic * BaseLogic::GetSigleton(){
	return sigleton;
}
void BaseLogic::CallBack(ProjectCallBackType type, BaseProject *project){
	switch(type){
	case PCBT_ADDRESEARCHPOINT:
		mReseasrchMng.addResearchPoints(project->mCurResource);
		break;
	}

}
int BaseLogic::GetDay(){
	return mDay;
}

void BaseLogic::CalculateGameLogic(){
	if(mEventGenerator.GenerateEvent()){
		GameEvent *curEvent=0;
		mEventGenerator.GetEvent(&curEvent);
		switch(curEvent->type){
		case ET_SOMEDESTRACT:

			break;

		}


	}
	if(mDeltaWeek ==DC_WEEK){
		mLuaMan.PassWeek();
		mCorp.WeaklyNews();
		mPlanet.PassWeak(mNotificator.mNotificationEvent);
	}
	if(mDeltaDate!=DC_NONE){
		mDay++;
		mLuaMan.PassDay();
		mCoreMng.PassDay(mNotificator.mNotificationEvent);
		mRequestMan.PassDay(mNotificator.mNotificationEvent);
		mWarFare.PassDay(mNotificator.mNotificationEvent);
		mRecMng.PassDay();
		mNotificator.DayCheck();
		GameEvent *evnt=0;
		if(mCorp.FireNiews(&evnt)){
			mListener->fireGameEvent(evnt);
		}
		if(mDeltaDate==DC_MONTH){
			mAccount.DoMonthlyCount(mGameDate.month);

		}
	}



}

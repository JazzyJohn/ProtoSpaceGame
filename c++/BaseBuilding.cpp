#include "BaseBuilding.h"
#include "BaseResearchBuilding.h"
#include "BaseMilitary.h"
#include "BaseIntelegence.h"
#include "BaseListener.h"
#include "BaseLogic.h"
using namespace Logic;
BaseBuilding::BaseBuilding(void)
{
	mType = TB_BASE;
	mEntity =0;
	mMode= BWM_NORMAL;
	PointerGuard<BaseBuilding>::GetSigleton()->Insert(this);
	BaseLogic::GetSigleton()->mLuaMan.BuildBuilding(this);
}
BaseBuilding::~BaseBuilding(void)
{
}
void BaseBuilding::AddUpgrade(Ogre::String name){
	mAllUpgrade.insert(name);
}
void BaseBuilding::DeleteUpgrade(Ogre::String name){
	mAllUpgrade.erase(name);
}
bool BaseBuilding::HasUpgrade(Ogre::String name){
	return mAllUpgrade.count(name)>0;
}
void BaseBuilding::SetEntiry(Ogre::Entity *entity){
	mEntity =entity;
}
GameEvent *BaseBuilding::ReturnBuildingInfo(){
	SimpleBuiltSend *evnt = new SimpleBuiltSend();
	evnt->bEntitty = mEntity;
	return evnt;

}
void BaseBuilding::ChangeUpkeep(float multi){
	std::map<BuildingWorkMode,float>::iterator itr;
	for(itr = mCostMap.begin(); itr!= mCostMap.end();++itr){
		itr->second = itr->second/multi; 
	
	}
}
void BaseBuilding::PassDay(std::queue<NotificationEvent*> &notificationEvent){

	BaseAccount::GetSigleton()->AddOutcomeToMonthlyFeed(mCostMap[mMode]);
};
BuildingBlueprint::BuildingBlueprint(int type,Ogre::String name){
	mName = name;
	mType =(TYPE_OF_BUILDING) type;
	
}
BuildingBlueprint::~BuildingBlueprint(void){


}
void BuildingBlueprint::ChangeUpkeep(float multi){
	std::map<BuildingWorkMode,float>::iterator itr;
	for(itr = mCostMap.begin(); itr!= mCostMap.end();++itr){
		itr->second = itr->second/multi; 
	
	}
}
BaseBuilding* BuildingBlueprint::ReturnBuilding(){
	NonCoreBuilding* temp;


	switch(mType){
	case TB_WAREHOUSE:
		temp= new BaseWarehouse();
		break;
	case TB_MINING:
		temp= new BaseMining();
		break;
	case TB_PRODUCTION:
		temp= new BaseProduction();
		break;
	case TB_RESEARCH:
		temp= new BaseResearchBuilding();
		break;
	case TB_MILITARY:
		temp= new BaseMilitaryBuilding();
		break;
	case TB_EMBASSY:
		temp = new BaseEmbassyBuilding(AT_DIMPLOMATS);
		break;
	case TB_BLACKOPS:
		temp = new BaseEmbassyBuilding(AT_BLACKOPS);
		break;
	case TB_ORBMINING:
		temp =new BaseOrbitalMining();
		break;
	}
	temp->mName = mName;
	temp->mDescriprion = &mDescriprion;
	temp->mCostMap.insert(mCostMap.begin(),mCostMap.end());
	if(mUniq){
		mOpen= false;
	}
	return temp;
}
BaseBaseCore::BaseBaseCore():BaseCore(){
	mType = TB_BASECORE;
	mEntity =0;
	mBaseHP = BaseAsset::GetSigleton("BASE_HP");

}
BaseBaseCore::BaseBaseCore(BaseCoreManager* coreManager):BaseCore(){
	mType = TB_BASECORE;
	mEntity =0;
	mCoreManager = coreManager;
	mName = "Base Core";
	mBaseHP = BaseAsset::GetSigleton("BASE_HP");

};
BaseBaseCore::~BaseBaseCore(){

}
void BaseBaseCore::LowerHp(float delta){
	mBaseHP-= delta;
	if(mBaseHP<0){
		BaseLogic::GetSigleton()->DestroyBuilding(this);

	}
}
void BaseBaseCore::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	BaseCore::PassDay(notificationEvent);
	BaseBuilding::PassDay(notificationEvent);
}
bool  BaseBaseCore::BuildBuilding(BuildingBlueprint *blueprint,BaseBuilding** building){
	int i=0;
	while(mAllSlot[i]!=0&&i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT")){
		i++;
	}
	if(i>=BaseAsset::GetSigleton("CORE_SLOT_AMOUNT")){
		return false;
	}
	else{



		mAllSlot[i] =blueprint->ReturnBuilding();
		*building= mAllSlot[i];
		NonCoreBuilding *temp=static_cast<NonCoreBuilding* >(*building);
		temp->mCore = this;
		temp->mSysName = mAllSlot[i]->mName +" "+LogicStd::IntToString(i) + " Core"+ LogicStd::IntToString(mNumber);
		temp->mSlot = i;



		return true;
	}

}

GameEvent *BaseBaseCore::ReturnBuildingInfo(){
	SendCoreToGui *evnt = new SendCoreToGui();
	evnt->allSlots = new BuildingForSent*[(int)BaseAsset::GetSigleton("CORE_SLOT_AMOUNT")];
	evnt->SlotCnt = BaseAsset::GetSigleton("CORE_SLOT_AMOUNT");
	BuildingForSent *buildStruct = 0;
	for(int i =0; i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT");++i){
		buildStruct = new BuildingForSent();
		buildStruct->Core = this;
		if(mAllSlot[i]!=0){
			buildStruct->empty= false;
			buildStruct->building = *(mAllSlot+i);
			buildStruct->buildingType =mAllSlot[i]->mType;
			buildStruct->name =mAllSlot[i]->mName;
		}else{
			buildStruct->empty= true;
		}
		evnt->allSlots[i] = buildStruct;
	}
	evnt->target = this;
	return evnt;

}
bool BaseBaseCore::addResource(BaseResourceObject *Object){
	BaseWarehouse *curWar;
	for(int i =0; i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT");++i){
		if(mAllSlot[i]!=0){
			if(mAllSlot[i]->mType==TB_WAREHOUSE){
				curWar =static_cast<BaseWarehouse *>(mAllSlot[i]);
				if(curWar->AddResource(Object)){
					mCoreManager->addResource(Object->mName);
					return true;
				}
			}
		}	
	}
	return mCoreManager->addResource(Object);
}
bool  BaseBaseCore::GetResource(BaseResourceObject **Object){

	return mCoreManager->GetResource(Object);
}
std::map<Ogre::String,ResourseDeposits *> BaseBaseCore::GetAllDeposit(){
	return mPlanet->GetAllDeposit();
}
bool  BaseBaseCore::GetResource(Ogre::String name,BaseResourceObject **Object){
	return mCoreManager->GetResource(name,Object);
}

void BaseBaseCore::ReduceCredit(int cost){
	mCoreManager->ReduceCredit(cost);
}

BaseResourceObject * BaseBaseCore::SayRefinery(BaseResourceObject *Object){
	return mCoreManager->SayRefinery(Object);
}
void NonCoreBuilding::AddCore(BaseBaseCore *base){
	mCore = base;
}
float NonCoreBuilding::GetProductionPower(){
	return mProductionPower;
}

BaseWarehouse::BaseWarehouse(){
	mType = TB_WAREHOUSE;
	mObjects = new BaseResourceObject*[(int)BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT")];
	int i=0;
	while(i<BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT")){
		mObjects[i] =0;
		i++;
	}
}
BaseWarehouse::~BaseWarehouse(void){
	int i=0;
	while(i<BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT")){
		BaseLogic::GetSigleton()->mResMng.lowerResource(mObjects[i]);
		mObjects[i] = 0;
		i++;
	}
	delete [] mObjects;
}

bool BaseWarehouse::AddResource(BaseResourceObject *object){
	int i=0;
	while(*(mObjects+i)!=0&&i<BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT")){
		i++;
	}
	if(i>=BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT")){
		return false;
	}
	else{
		mObjects[i] = object;
		return true;
	}
}
bool BaseWarehouse::GetResource(Ogre::String name,BaseResourceObject **object){
	int i=0;
	for(int i =0; i<BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT");i++){
		if(!mObjects[i]==0){
			if(mObjects[i]->mName==name){
				*object = mObjects[i];
				mObjects[i]= 0;
				return true;	
			}


		}	
	}
	return false;

}
bool BaseWarehouse::GetResource(Ogre::String name){
	int i=0;

	while((mObjects[i]==0||mObjects[i]->mName!=name)&&i<BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT")){
		i++;
	}
	if(i>=BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT")){
		return false;
	}
	else{
		mObjects[i]= 0;
		return true;
	}

}
GameEvent *BaseWarehouse::ReturnBuildingInfo(){
	ShowWarehouse * evnt = new ShowWarehouse();
	Ogre::String *allSlot = new Ogre::String[(int)BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT")];
	int i=0;
	evnt->SlotNum = BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT");
	while(i<BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT")){
		if(mObjects[i] == 0){
			allSlot[i] = "Null";
		}else{

			allSlot[i] =mObjects[i]->mName;
		}
		i++;
	}

	evnt->target = evnt->target = this;
	evnt->allSlot = allSlot;
	return evnt;
}
void BaseWarehouse::SomeChange(int type,float multiplie){
	if((SOME_CHANGE_TYPE)type==SCT_WARHEOUSESPACEINCREASE){
		BaseResourceObject **mtemp =mObjects;  
		int baseAmnt =BaseAsset::GetSigleton("WARHOUSESLOT_AMOUNT");
		mObjects=  new BaseResourceObject*[baseAmnt*(int)multiplie];
		for(int i=0;i<(int)baseAmnt;++i){
			mObjects[i]= mtemp[i];
		
		}
		for(int i=baseAmnt;i<(int)baseAmnt*multiplie;++i){
			mObjects[i]=0;
		
		}
		delete []mtemp;

	}

}
SetDeposit::SetDeposit(){
	type =ET_CONTROL;
	controlType =CTE_SETDEPOSITE;
}
BaseMining::BaseMining(){
	
	mType = TB_MINING;
	mDepositCnt=1;
	mProductionPower =BaseAsset::GetSigleton("MININGPRODUCTION_POWER");
	mCurProduction=0;
}
BaseMining::~BaseMining(){
	for(int i=0;i<mDeposit.size();i++){
		mDeposit[i]->mOcupiet= true;
	}
}
void BaseMining::SomeChange(int type,float amount){
	if((SOME_CHANGE_TYPE)type==SCT_INCRESEDEPODIT){
		mDepositCnt+= amount;
	
	}

}
void BaseMining::PassDay(std::queue<NotificationEvent*> &notificationEvent){

	if(mDeposit.size()!=0){
		mIdle= true;
		mCurProduction+= GetProductionPower();
		if(mCurProduction>=BaseAsset::GetSigleton("NEED_FOR_PRODUCTION")){
			
			for(int i=0;i<mDeposit.size();i++){
				if(mAllUpgrade.count("outmine")>0&&mAllUpgrade.count("mobileref")>0){
					BaseResourceObject* temp = mDeposit[i]->GenerateResource();
					if(mAllUpgrade.count("outmine")>0){
						BaseAccount::GetSigleton()->ChangeCredit(temp->mTradePoint*BaseAsset::GetSigleton("TRADE_TO_CREDIT_COEF"));
					}
					if(mAllUpgrade.count("mobileref")>0){
						Production *product =mCore->mPlanet->mResMan->SayProduction(temp);
						if(product->refine){
							mCore->addResource(product->result);
						}else{
							mCore->addResource(temp);
						}
					}
				} else{
					mIdle =mIdle&&mCore->addResource(mDeposit[i]->GenerateResource());
					if(mIdle){
						mCurProduction=0;
					}
				}
				BaseLogic::GetSigleton()->mLuaMan.PlanetScript(*(static_cast<BasePlanetCountry*>(mDeposit[i]->GetCountry())),"MiningEffect",true);
				
			}
			
		}
		if(!mIdle){
			NotificationEvent *evnt=new NotificationEvent();
			evnt->notType = NTE_BUILDINGIDLE;
			evnt->target = this;
			evnt->name = this->mName;
			notificationEvent.push(evnt);
		}

	}else{
		if(mAllUpgrade.count("outmine")>0){
			mCurProduction+= GetProductionPower()/4;
			if(mCurProduction>=BaseAsset::GetSigleton("NEED_FOR_PRODUCTION")){
			
				mCurProduction=0;
				std::vector<BaseResourceObject*> offers =mCore->mPlanet->mResMan->GetAllResourse(RT_ORES);

				int a =std::rand()%offers.size();
				mCore->addResource(offers[a]);
			}
		
		}
		
	}
	BaseBuilding::PassDay(notificationEvent);
}
GameEvent *BaseMining::ReturnBuildingInfo(){
	ShowMining * evnt = new ShowMining();
	if(mDeposit.size()!=0){
		evnt->curDeposit="";
		for(int i=0;i<mDeposit.size();i++){
		evnt->curDeposit += mDeposit[i]->mName+" ";
		}
	}else{
		evnt->curDeposit="None";
	}
	evnt->allDeposit = mCore->GetAllDeposit();
	evnt->procent = mCurProduction/BaseAsset::GetSigleton("NEED_FOR_PRODUCTION");
	evnt->target = this;
	return evnt;
}

void BaseMining::SetDeposits(ResourseDeposits* deposit){
	if(mDeposit.size()==mDepositCnt){
		std::vector<ResourseDeposits*> temp;
		mDeposit[0]->mOcupiet= false;
		for(int i=1;i<mDeposit.size();i++){
			temp.push_back(mDeposit[i]);

		}
		mDeposit =temp;
	}
	mDeposit.push_back(deposit);
	mDeposit.back()->mOcupiet= true;

}
BaseOrbitalMining::BaseOrbitalMining(){
		mType = TB_ORBMINING;
	mOreCnt =(int)BaseAsset::GetSigleton("ORBITAL_MINING_PER_CYCLE"); 
	mProductionPower =20;
	mCurProduction=0;
}
void BaseOrbitalMining::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	std::vector<BaseResourceObject*> offers =mCore->mPlanet->mResMan->GetAllResourse(RT_ORES);
	int i=0;
	int a = std::rand()/offers.size();
	while(i<mOreCnt&&mCore->addResource(offers[a])){
	a= std::rand()/offers.size();
	i++;
	}

}
GameEvent *BaseOrbitalMining::ReturnBuildingInfo(){
	ShowMining * evnt = new ShowMining();

	evnt->curDeposit="Orbital Object";
	evnt->allDeposit = mCore->GetAllDeposit();
	evnt->procent = mCurProduction/BaseAsset::GetSigleton("NEED_FOR_PRODUCTION");
	evnt->target = this;
	return evnt;

}
BaseCore::BaseCore(void)
{
	mAllSlot = new BaseBuilding*[(int)BaseAsset::GetSigleton("CORE_SLOT_AMOUNT")];
	int i=0;
	while(i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT")){
		mAllSlot[i]=0;
		mBuildingVector.push_back(mAllSlot[i]);
		i++;
	}
}
BaseCore::~BaseCore(void)
{
	int i=0;
	while(i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT")){
		if(mAllSlot[i]!=0){
			BaseLogic::GetSigleton()->DestroyBuilding(mAllSlot[i]);
		}
		i++;
	}

	delete [] mAllSlot;
}

void BaseCore::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	for(int i =0; i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT");++i){
		if(*(mAllSlot+i)!=0){
			mAllSlot[i]->PassDay(notificationEvent);		
		}	
	}
}
bool BaseCore::addResource(BaseResourceObject *Object){
	BaseWarehouse *curWar;
	for(int i =0; i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT");++i){
		if(*(mAllSlot+i)!=0){
			if(mAllSlot[i]->mType==TB_WAREHOUSE){
				curWar =static_cast<BaseWarehouse *>(mAllSlot[i]);
				if(curWar->AddResource(Object)){
					return true;
				}
			}
		}	
	}
	return false;
}
bool  BaseCore::GetResource(Ogre::String name,BaseResourceObject **object){
	BaseWarehouse *curWar;
	for(int i =0; i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT");++i){
		if(mAllSlot[i]!=0){
			if(mAllSlot[i]->mType==TB_WAREHOUSE){
				curWar =static_cast<BaseWarehouse *>(mAllSlot[i]);
				if(curWar->GetResource(name,object)){

					return true;
				}
			}
		}	
	}
	return false;
}
bool  BaseCore::GetResource(Ogre::String name){
	BaseWarehouse *curWar;
	for(int i =0; i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT");++i){
		if(mAllSlot[i]!=0){
			if(mAllSlot[i]->mType==TB_WAREHOUSE){
				curWar =static_cast<BaseWarehouse *>(mAllSlot[i]);
				if(curWar->GetResource(name)){

					return true;
				}
			}
		}	
	}
	return false;
}
bool BaseCore::GetResource(BaseResourceObject **Object){
	return false;
}
bool BaseCore::BuildBuilding(BuildingBlueprint *blueprint,BaseBuilding** building){
	int i=0;
	while(mAllSlot[i]!=0&&i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT")){
		i++;
	}
	if(i>=BaseAsset::GetSigleton("CORE_SLOT_AMOUNT")){
		return false;
	}
	else{

		mAllSlot[i] =blueprint->ReturnBuilding();
		mBuildingVector[i] = mAllSlot[i];
		mAllSlot[i]->mSysName = mAllSlot[i]->mName +" "+LogicStd::IntToString(i) + " Core"+ LogicStd::IntToString(mNumber);
		*building= mAllSlot[i];

		return true;
	}
}
bool BaseCore::HasBuilding(TYPE_OF_BUILDING arg[], int length){
	bool temp = false;
	for(int i =0; i<BaseAsset::GetSigleton("CORE_SLOT_AMOUNT");++i){
		if(mAllSlot[i]!=0){
			for(int j=0; j<length;j++){
				temp= temp||mAllSlot[i]->mType==arg[j];
			}
		}
	}
	return temp;
}
BaseCoreManager::BaseCoreManager(void){
	mAllBase =   std::vector<BaseCore *>();

}
BaseCoreManager::~BaseCoreManager(void){
	std::vector<BaseCore *>::iterator the_iterator;
	the_iterator = mAllBase.begin();
	while (the_iterator != mAllBase.end()) {
		delete *the_iterator;
		++the_iterator;
	}
	for(int i =0; i<mBuildCnt;++i){
		if((mAllBlueperint+i)!=0){
			delete mAllBlueperint[i];
		}	
	}
	delete [] mAllBlueperint;
}
void BaseCoreManager::GenrateInit(BaseResourceManager *resMan,	BaseBuilding** building,	BaseBuilding** basewar){
	mResMan = resMan;
	mMainBase = new BaseBaseCore( this);
	mMainBase->mMain = true;
	mMainBase->mCountry = 0;
	for(int i=0; i<mBuildCnt;i++){
		if(mAllBlueperint[i]->mType==TB_WAREHOUSE){
			mMainBase->BuildBuilding(mAllBlueperint[i],	basewar);
			break;
		}

	}
	*building = static_cast<BaseBaseCore*> (mMainBase);


}
void BaseCoreManager::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	std::vector<BaseCore *>::iterator the_iterator;
	the_iterator = mAllBase.begin();
	while (the_iterator != mAllBase.end()) {
		(*the_iterator)->PassDay(notificationEvent);
		++the_iterator;
	}
	mMainBase->PassDay(notificationEvent);
}

bool BaseCoreManager::addResource(BaseResourceObject *Object){
	std::vector<BaseCore *>::iterator the_iterator;
	the_iterator = mAllBase.begin();
	while (the_iterator != mAllBase.end()) {
		if((*the_iterator)->addResource(Object)){
			addResource(Object->mName);
			return true;
		}
		++the_iterator;
	}
	if(mMainBase->addResource(Object)){
		addResource(Object->mName);
		return true;
	}
	return false;
}
void BaseCoreManager::ReduceCredit(int cost){
	mResMan->ReduceCredit(cost);
}

void BaseCoreManager::SetAllBuildingBlueprint(	BuildingBlueprint **allBlueperint,int buildCnt){
	mAllBlueperint = allBlueperint;
	mBuildCnt = buildCnt;
	for(int i=0; i<mBuildCnt;i++){
	mVectBlueperint.push_back(mAllBlueperint[i]);
	}

}
std::vector<BuildingBlueprint *>  BaseCoreManager::avaibleBlueprint(BaseCore *targer){
	bool orbital = targer == mMainBase;
	std::vector<BuildingBlueprint *> bluVect;
	for(int i=0; i<mBuildCnt;i++){
		if(mAllBlueperint[i]->mOpen){
		switch(mAllBlueperint[i]->mPlace){
		case BPT_GROUD:
			if(!orbital){
				bluVect.push_back(mAllBlueperint[i]);
			}
			break;
		case BPT_GROUND_ORBITAL:
				bluVect.push_back(mAllBlueperint[i]);
			break;
		case BPT_ORBITAL:
			if(orbital){
				bluVect.push_back(mAllBlueperint[i]);
			}
			break;
	}
		}
	}
	return bluVect;
}

bool BaseCoreManager::BuildCore(BasePlanetCountry *country,	BaseBuilding** building,	CountryTile** tile){
	if(mAllBase.size()>=BaseAsset::GetSigleton("CORE_ALLOWEDAMOUNT")){
		return false;
	}
	BaseBaseCore* newCore = new BaseBaseCore( this);
	newCore->mCountry = country;
	std::vector<CountryTile *> free;
	std::vector<CountryTile>::iterator itr;
	for(itr= country->mCountryTiles.begin();itr!= country->mCountryTiles.end();++itr){
		if(itr->mBase==false&&itr->mMine==false){
			free.push_back(&(*itr));
		}
	}
	if(free.size()==0){
		return false;
	}
	int a = std::rand()%free.size();
	free[a]->mBase= true;
	*tile= free[a];
	*building = newCore;
	
	mAllBase.push_back(newCore);
	newCore->BaseCore::mSysName =newCore->mName + " "+LogicStd::IntToString(mAllBase.size());
	newCore->BaseBuilding::mSysName =newCore->mName + " "+LogicStd::IntToString(mAllBase.size());
	newCore->mNumber= mAllBase.size();
	newCore->mMain =false;
	newCore->mSlot = mAllBase.size();
	newCore->mPlanet = country->GetPlanet();
	mResMan->ReduceCredit(BaseAsset::GetSigleton("CORE_BUILDING_COST"));
	country->AddCore(newCore);
	return true; 
}
void BaseCoreManager::GenerateRandomResource(int type,int sectype){
	if(type==RT_INT){
		IntelegentManager::GetSigleton()->GenerateResource((ActonType)sectype);
	}else{
	
		std::vector<BaseResourceObject*> offers =mResMan->GetAllResourse((ResourceType)type);

		int a =std::rand()%offers.size();
		addResource(offers[a]);
			
	}
	
}
bool BaseCoreManager::BuildBuilding(BuildingBlueprint *blueprint,BaseCore *targer,	BaseBuilding** building){
	bool orbital=targer == mMainBase;
	
	switch(blueprint->mPlace){
		case BPT_GROUD:
			if(orbital){
				return false;
			}
			break;
		case BPT_ORBITAL:
			if(!orbital){
				return false;
			}
			break;
	}
	if(targer->BuildBuilding(blueprint,building)){
		if(targer == mMainBase){
			mResMan->ReduceCredit(blueprint->mOrbitCost);
		}else{
			mResMan->ReduceCredit(blueprint->mCost);
		}

		return true;
	}else{
		return false;
	}

}
BaseResourceObject * BaseCoreManager::SayRefinery(BaseResourceObject *Object){
	return mResMan->SayRefinery(Object);
}
void BaseCoreManager::addResource(Ogre::String name){
	mResMan->addResourceQueue(name);
}
bool BaseCoreManager::GetResource(BaseResourceObject **Object){
	Ogre::String name="";
	while(mResMan->GetQueueName(&name)){
		if(GetResource(name,Object)){
			return true;
		}
	}
	return false;

}
bool BaseCoreManager::GetResource(Ogre::String name,BaseResourceObject **Object){
	std::vector<BaseCore *>::iterator the_iterator;
	the_iterator = mAllBase.begin();
	while (the_iterator != mAllBase.end()) {
		if((*the_iterator)->GetResource(name,Object)){
			mResMan->takeResourceObject((*Object)->mName);
			return true;
		}
		++the_iterator;
	}
	if( mMainBase->GetResource(name,Object)){
		mResMan->takeResourceObject((*Object)->mName);
		return true;
	}else{
		return false;
	}


}
bool BaseCoreManager::GetResource(Ogre::String name){
	std::vector<BaseCore *>::iterator the_iterator;
	the_iterator = mAllBase.begin();
	while (the_iterator != mAllBase.end()) {
		if((*the_iterator)->GetResource(name)){
			mResMan->takeResourceObject(name);
			return true;
		}
		++the_iterator;
	}
	if( mMainBase->GetResource(name)){
		mResMan->takeResourceObject(name);
		return true;
	}else{
		return false;
	}


}
BaseProduction::BaseProduction(){
	mRaw=0;
	mNextObject=0;
	mProdType = TYPROD_REFINERY;
	mType = TB_PRODUCTION;
	mProductionPower =BaseAsset::GetSigleton("REFINERYPRODUCTION_POWER");
	mCurProduction =0;
	mProduct=0;
}
BaseProduction::~BaseProduction(){

}
void BaseProduction::SetProductionLine(void){
	mRaw=0;
	mProdType = TYPROD_REFINERY;
}
void BaseProduction::SetProductionLine(BaseResourceObject *raw){
	mRaw=raw;
	mProduct= mResMan->SayProduction(raw);
	mProdType = TYPROD_REFINERY;
}
void BaseProduction::SetProductionLine(Production *product,BaseResourceObject *nextObject){
	mProduct=product;
	mProdType = TYPROD_PRODUCTION;
	mNextObject =nextObject;
}
void BaseProduction::AddManager(BaseResourceManager *resMan){
	mResMan = resMan;
}
void BaseProduction::PassDay(std::queue<NotificationEvent*> &notificationEvent){
	bool isResAv =false;
	mIdle=true;
	BaseResourceObject *raw =0;

	mCurProduction+=GetProductionPower();


	if(mCurProduction>BaseAsset::GetSigleton("NEED_FOR_PRODUCTION")){
		IntelegentManager * intRes= IntelegentManager::GetSigleton();
		switch(mProdType){
		case TYPROD_REFINERY:
			if(mRaw==0){
				return;
			}else{
				isResAv=mCore->GetResource(mRaw->mName,&raw);
			}
			if(!isResAv){
				mIdle = true;
				return;
			}
			else{
				mIdle = false;
				mCore->addResource(mResMan->SayRefinery(raw));

			}
			break;
		case TYPROD_PRODUCTION:
			raw =0;
			isResAv =false;
			bool hasResource = true;
			std::map<BaseResourceObject*,int>::iterator recptItr;

			for(recptItr= mProduct->recept.begin();recptItr!= mProduct->recept.end();++recptItr){
				if(recptItr->first->mType==RT_INT){
					hasResource=hasResource&&intRes->hasResource(recptItr->first,recptItr->second);
				}else{
					hasResource=hasResource&&mResMan->hasResource(recptItr->first,recptItr->second);
				}	
			}
			if(hasResource){
				for(recptItr= mProduct->recept.begin();recptItr!= mProduct->recept.end();++recptItr){
					if(recptItr->first->mType==RT_INT){
						intRes->GetResource(recptItr->first,recptItr->second);
					}else{
						for(int i=0;i<recptItr->second;++i){
							mCore->GetResource(recptItr->first->mName,&raw);
						}
					}

				}	
				int resultamount=mProduct->amount;
				for(int i =0; i<resultamount;++i){
					mCore->addResource(mProduct->result);	

				}
				mIdle = false;

			}else{
				mIdle = true;

			}
			break;

		}
		if(!mIdle){
			mCurProduction=0;
		}

	}

	BaseBuilding::PassDay(notificationEvent);
}
GameEvent *BaseProduction::ReturnBuildingInfo(){
	ShowProduction *evnt = new ShowProduction();
	switch(mProdType){
	case TYPROD_REFINERY:
		if(mRaw==0){
			evnt->curProduction = "Refinery";
		}else{
			evnt->curProduction =(mRaw->mName);
			evnt->curProduction  =  "Refinery:"+evnt->curProduction; 
		}
		break;
	case TYPROD_PRODUCTION:
		evnt->curProduction =(mNextObject->mName);
		evnt->curProduction  =  "Production:"+evnt->curProduction; 
		break;
	}
	evnt->target = this;
	evnt->typeProd =mProdType;
	evnt->procent = mCurProduction/BaseAsset::GetSigleton("NEED_FOR_PRODUCTION");
	mResMan->ShowAvaibleProduction(evnt->mProductionVar,mProduct);
	return evnt;
}

BasePlanetCountry::BasePlanetCountry(BasePlanet* planet):BaseCountry(){
	mPlanet = planet;
	mOpenDeposit=0;

}
void BasePlanetCountry::ChangeState(std::map<CountryCharacteristic*,float> charMap){
	std::map<CountryCharacteristic*,float>::iterator charIterator;
	for(charIterator = charMap.begin(); charIterator != charMap.end(); ++charIterator){
		*(mCharMap[charIterator->first])  += 	charIterator->second;
	}
	mPlanet->ChangeState(charMap, this);
}
void BasePlanetCountry::ChangeState(Ogre::String name,float delta){
	*(mRefCharMap[name])  += 	delta;
	mPlanet->ChangeState(name, delta,this);
}
SendCountry* BasePlanetCountry::SendEvent(){
	SendCountry *evnt = new SendCountry();
	std::map<CountryCharacteristic*,CountryCharValue*>::iterator charIterator;
	for(charIterator = mCharMap.begin(); charIterator != mCharMap.end(); ++charIterator){
		if(charIterator->first->type==CT_DELTA){
			continue;
		}
		if(charIterator->first->ShowType ==SCT_SIMPLE){
			evnt->allChar[charIterator->first]= charIterator->second->Text();
			if(mRefCharMap.count(charIterator->first->name+" Delta")>0){
				evnt->deltaChar[charIterator->first] = *(mRefCharMap[charIterator->first->name+" Delta"])>0;
			}
		}

	}
	std::map<BaseCountry*,CountryRellation>::iterator relIt;
	for(relIt = mRelation.begin(); relIt != mRelation.end(); ++relIt){
		evnt->Realtion[relIt->first->mName]= *(relIt->second.mValue);
		evnt->RealtionStatus[relIt->first->mName]= *(relIt->second.mStatus);
	}
	
	evnt->contry = this;
	return evnt;

}
void  BasePlanetCountry::AddCore(BaseCore* core){

	mCore.push_back(core);
}
void BasePlanetCountry::GenerateToll(float Economy){

	BaseCoreManager* coreMng= BaseLogic::GetSigleton()->GetCore();

	int max = Economy*BaseAsset::GetSigleton("PILLAGE_TOLL"),a=0;
	std::vector<BaseResourceObject*> offers =mPlanet->mResMan->GetAllResourse(RT_PLANETPRODUCTION);
	for(int i=0;i<max;i++){
		a =std::rand()%offers.size();
		coreMng->addResource(offers[a]);
	}
}
BasePlanet*   BasePlanetCountry::GetPlanet(){
	return mPlanet;

}
void BasePlanetCountry::SetDeposit(ResourseDeposits **allDeposit,	int depositCount){
	for(int i =0;i<depositCount;i++){
		mCountryDeposit.push_back(allDeposit[i]);

	}
}
void  BasePlanetCountry::SetDeposit(ResourseDeposits * deposit){
	mCountryDeposit.push_back(deposit);
}
bool BasePlanetCountry::Reconing(){
	if(mOpenDeposit<mCountryDeposit.size()){
		mCountryDeposit[mOpenDeposit]->mOpen= true;
	}
	bool depo=++mOpenDeposit>=mCountryDeposit.size();
	bool rec =BaseCountry::Reconing();
	return depo&&rec;
}
void BasePlanetCountry::PassWeak(std::queue<NotificationEvent*> &notificationEvent){
	if(mDestroyed||mAtWar){
		return;
	}
	if(mOcuppied){
		bool isEmmabasy= mRefCharMap["Embassilocal"]->mAmount==1;
		
		if(isEmmabasy){
			int amount= mRefCharMap["Population"]->mAmount/BaseAsset::GetSigleton("PEOPLEWORKCOEF");
			if(amount==0){
				amount=1;
			}
			BaseBaseCore * temp= static_cast<BaseBaseCore*>(mCore[0]);
			std::vector<ResourseDeposits*>::iterator listItr = mCountryDeposit.begin();
			std::vector<BaseResourceObject*> offers =mPlanet->mResMan->GetAllResourse(RT_PLANETPRODUCTION);

			int a =0;
			for(int i =0; i<amount;++i){
				a = std::rand()%100;
				if(*(mRefCharMap["Economic"])<a){
					temp->addResource((*listItr)->GenerateResource());	
					if(++listItr==mCountryDeposit.end()){
						listItr= mCountryDeposit.begin();
					}
				}else{
					a = std::rand()%offers.size();
					temp->addResource(offers[a]);

				}
			}
		}
	}else{
		if(*(mRefCharMap["IIF"])>0){
			int tradePoint = mRefCharMap["Trade"]->mAmount*BaseAsset::GetSigleton("TRADE_COEF");
			std::list<BaseDemand>::iterator itr= mDemand.begin();

			if(mCore.size()>0){


				BaseBaseCore * tempCore	= static_cast<BaseBaseCore*>(mCore[0]);
				BaseResourceObject *tmpobj;
				while(tradePoint>0&&itr!=mDemand.end()){

					if((*itr).isTrade&&mPlanet->mResMan->hasResource((*itr).demand)){
						tempCore->GetResource((*itr).demand->mName,&tmpobj);
						ChangeDelta(tmpobj);
						for(int i=0;i<(*itr).amount;++i){
							tempCore->addResource((*itr).offer);						
						}
						tradePoint-= (*itr).demand->mTradePoint;
						(*itr).demandCnt++;
					}
					++itr;
				}
				bool checked = false;
				int newDemand =0;
				while(!checked){
					checked= true;
					for(itr= mDemand.begin();itr!=mDemand.end();++itr){
						if((*itr).demandProgress>=(*itr).demandCnt){
							mDemand.erase(itr);
							newDemand++;
							checked=false;
							break;
						}
					}
				}
				GenerateNewDemand(newDemand);
			}

		}

	}
	BaseLogic::GetSigleton()->mLuaMan.CountryPassWeak(this);
	BaseCountry::PassWeak(notificationEvent);
}

void  BasePlanetCountry::GenerateNewDemand(int amount){
	ResourceType typeArgs[2];
	typeArgs[0] = RT_PLANETPRODUCTION;
	typeArgs[1] = RT_ORES;
	std::vector<BaseResourceObject*> production=mPlanet->mResMan->GetAllResourse(RT_PRODUCTION), offers =mPlanet->mResMan->GetAllResourse(typeArgs,2);
	SetDemand(amount,production,offers);

}
void BasePlanetCountry::DeleteBuilding(void* pntr){
	
	std::vector<BaseCore*>::iterator coreIterator;
	
	for(coreIterator=mCore.begin();coreIterator!=mCore.end();++coreIterator){
		if((*coreIterator)==pntr){
			mCore.erase(coreIterator);
			return ;
		}
	}
}
void  BasePlanetCountry::SetDemand(int a,std::vector<BaseResourceObject*> production,std::vector<BaseResourceObject*> offers){
	int prodRand,offrRand;
	BaseDemand* temp;
	Production *prodTemp;
	int baseDemand =BaseAsset::GetSigleton("BASE_DEMAND_CNT");
	for(int i =0; i<a; i++){
		prodRand = std::rand()%production.size();
		offrRand = std::rand()%offers.size();
		mDemand.push_back(BaseDemand());
		temp = &mDemand.back();
		temp->amount =1+std::rand()%2;
		temp->demand = production[prodRand];

		temp->offer = offers[offrRand];
		if(mPlanet->mResMan->HasProduction(temp->demand,&prodTemp)){
			if(prodTemp->amount>=temp->amount){
				temp->amount = prodTemp->amount+1;

			}		
		}
		temp->demandCnt =baseDemand +std::rand()%baseDemand;
	}
}
#include "PlantCountry.h"
#include "BaseLogic.h"
using namespace Logic;
PlanetJustGenerateEvent* PlantPlanet::GeneratePlanet(int countryCount,int clusterCnt,
			PlanetEnvorementType type,
			CharacteristicManager *charMan,
			int depositCount,
			BaseResourceManager *resMan
			){
	countryCount=1;
	clusterCnt=1;
	mCountryCount = countryCount;
	mCountry = new BaseCountry*[countryCount];
	PlanetJustGenerateEvent *evnt = new PlanetJustGenerateEvent();
	mResMan = resMan;
	ResourceType typeArgs[2];
	typeArgs[0] = RT_PLANETPRODUCTION;
	typeArgs[1] = RT_ORES;
	std::vector<BaseResourceObject*> oresVector = mResMan->GetAllResourse(RT_ORES),
		productionVector = mResMan->GetAllResourse(RT_PRODUCTION),
		tradeVector= mResMan->GetAllResourse(typeArgs,2);
	mCharMan  = charMan;
	mType = type;
	int a;
	for(int i =0; i<mCountryCount;++i){

		mCountry[i]= new PlantCountry(this);
		mCountry[i]->SetMap(mCharMan->ReturnBaseMap());
		mCountry[i]->mName ="Country #"+LogicStd::IntToString(i);
		evnt->country[mCountry[i]->mName]=mCountry[i];
		mCountryVector.push_back(mCountry[i]);

	}
	
	mDepositCount= depositCount*mCountryCount;

	int num=0;
	ResourseDeposits * temp =0; 
	for(int i =0;i<mDepositCount;i++){
		temp =new  ResourseDeposits(mCountry[0]);
		temp->SetMap(oresVector);
		temp->mName ="deposit"+ LogicStd::IntToString(i);
	
		mAllDepositVect.push_back(temp);
	}
	GenerateTiles();
	std::set<Ogre::Vector2>::iterator itr;

	for(int i =0; i<mCountryCount;++i){
		evnt->allTiles[mCountry[i]] = mCountry[i]->mCountryTiles;

	}

	int size =evnt->allTiles.size();
	return evnt;
}
PlantCountry::PlantCountry(PlantPlanet* planet):BasePlanetCountry(planet)
{

}


PlantCountry::~PlantCountry(void)
{
}
void PlantCountry::PassWeak(std::queue<NotificationEvent*> &notificationEvent){
		std::vector<BaseCore*>::iterator coreIterator;
		TYPE_OF_BUILDING  args[1];
		args[0]= TB_EMBASSY;
		for(coreIterator=mCore.begin();coreIterator!=mCore.end();++coreIterator){
			BaseLogic::GetSigleton()->mLuaMan.CountryBaseScript(*this,"plantAttack",*(static_cast<BaseBaseCore*>(*coreIterator)));
		}
}
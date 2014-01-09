#include "BugsCountry.h"
#include "BaseLogic.h"
using namespace Logic;
PlanetJustGenerateEvent* BugsPlanet::GeneratePlanet(int countryCount,int clusterCnt,
			PlanetEnvorementType type,
			CharacteristicManager *charMan,
			int depositCount,
			BaseResourceManager *resMan
			){

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

		mCountry[i]= new BugsCountry(this);
		mCountry[i]->SetMap(mCharMan->ReturnBaseMap());
		mCountry[i]->mName ="Country #"+LogicStd::IntToString(i);
		a = 1+std::rand()%3;
		mCountry[i]->SetDemand(a,productionVector,tradeVector);

		evnt->country[mCountry[i]->mName]=mCountry[i];
		mCountryVector.push_back(mCountry[i]);

	}
	int rel;
	for(int i =0; i<mCountryCount;++i){
		for(int j=0;j<i;++j){
			mCountry[i]->SetRelations(mCountry[j],mCountry[j]->GetRelation(mCountry[i]),mCountry[j]->GetRelatStatus(mCountry[i])); 
		}
		for(int j =i;j<mCountryCount;++j){
			if(i!=j){
				rel = -10 - (std::rand()%90);
				
				mCountry[i]->SetRelations(mCountry[j],rel); 
				mCountry[i]->ChangeRelation(mCountry[j],0);
			}
		}
	}

	mDepositCount= depositCount*mCountryCount;

	int num=0;
	ResourseDeposits * temp =0; 
	for(int i =0;i<mCountryCount;i++){
		temp =new  ResourseDeposits(mCountry[i]);
		temp->SetMap(oresVector);
		temp->mName ="deposit"+ LogicStd::IntToString(i);
	
		mAllDepositVect.push_back(temp);
	}
	for(int i=mCountryCount;i<mDepositCount;i++){
		num = std::rand() % (mCountryCount);
		temp =new  ResourseDeposits(mCountry[num]);
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


BugsCountry::BugsCountry(BugsPlanet* planet):BasePlanetCountry(planet)
{
}


BugsCountry::~BugsCountry(void)
{
}
void BugsCountry::PassWeak(std::queue<NotificationEvent*> &notificationEvent){


}
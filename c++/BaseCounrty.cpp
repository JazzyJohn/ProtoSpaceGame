#include "BaseCounrty.h"
#include "BaseBuilding.h"
#include "LogicStd.h"
#include "BaseLogic.h"
#include "BaseAI.h"
using namespace Logic;
CharacteristicManager* CharacteristicManager::sigleton=0;
const char *COM_NAME ="communication";

PlanetJustGenerateEvent::PlanetJustGenerateEvent(){
	gameType =GUIET_PLANETFINISHGENERATION;
	type =ET_GAME;

}
SendCountry::SendCountry(){
	gameType =GUIET_COUNTRYDATA;
	type =ET_GAME;
} 
SendBaseDemand::SendBaseDemand(BaseDemand&demandObj){
	demand = demandObj.demand->mName;
	offer= demandObj.offer->mName;
	amount = demandObj.amount;
	demandPnt= &demandObj;
	isTrade= demandObj.isTrade;
}
BaseDemand::BaseDemand(){
	isTrade=false;
	open = false;
	demandProgress=0;
}

CharacteristicManager::CharacteristicManager(){
	sigleton = this;

}
CountryCharacteristic* CharacteristicManager::GetChar(Ogre::String name){
	if(sigleton->mNameMap.count(name)>0){
		return sigleton->mNameMap[name];
	}else{
		return sigleton->mNameMap.begin()->second;
	}

}
CharacteristicManager::~CharacteristicManager(){
	for(int i =0; i<mCharCount;++i){
		if((mAllCharacteristick+i)!=0){
			delete mAllCharacteristick[i];
		}	
	}
	delete [] mAllCharacteristick;

}
void CharacteristicManager::setAllCharacteristics(CountryCharacteristic ** allCharacteristick,int charctNum){
	mCharCount =charctNum;
	mAllCharacteristick = allCharacteristick;
	for(int i =0; i<mCharCount;++i){
		if((mAllCharacteristick+i)!=0){
			mNameMap[mAllCharacteristick[i]->name]=mAllCharacteristick[i];
		}	
	}
}
std::map<CountryCharacteristic*,CountryCharValue*> CharacteristicManager::ReturnBaseMap(){
	std::map<CountryCharacteristic*,CountryCharValue*> map;
	int a;
	int test =0;
	for(int i =0; i<mCharCount;++i){
		test =mAllCharacteristick[i]->Max/2;
		map[mAllCharacteristick[i]] =new CountryCharValue();
		map[mAllCharacteristick[i]]->mPlayerKnow =mAllCharacteristick[i]->Open; 
		switch(mAllCharacteristick[i]->type){
		case CT_INT:
			*(map[mAllCharacteristick[i]]) = 0;
			break;
		case CT_BOOL:
			*(map[mAllCharacteristick[i]]) = 0;
			break;
		case CT_SIGNINT:
			*(map[mAllCharacteristick[i]]) = 0;
			break;
		case CT_DECS100:
			*(map[mAllCharacteristick[i]]) =(float)mAllCharacteristick[i]->Max;
			break;
		case CT_DECS100RN:
			*(map[mAllCharacteristick[i]]) =std::rand()%(test)+(test);
			break;
		case CT_DESCMANY:
			a =std::rand()%test;
			*(map[mAllCharacteristick[i]]) = a +test;
			break;
		case CT_DESCREMENT:
			*(map[mAllCharacteristick[i]]) = 0;
			break;
		case CT_DELTA:

			*(map[mAllCharacteristick[i]])=(float)std::rand()/((float)RAND_MAX/(mAllCharacteristick[i]->Max*2))-1;
			break;
		default:
			*(map[mAllCharacteristick[i]]) = 0;
			break;
		}

	}
	return map;
}
CountryCharValue::CountryCharValue(float test){
	mAmount = test;
	CountryCharValue();
}
CountryCharValue::CountryCharValue(){
	mPlayerKnow= true;
	mHightDeap=0;
	mLowDeap=0;
}
int& CountryCharValue::operator=(const CountryCharValue& rhs) {
	int i =rhs.mAmount;
	return i;
}
CountryCharValue& CountryCharValue::operator+=(const float& rhs) {
	mAmount+= rhs;
	CheckDiapason();
	return *this;
}
CountryCharValue& CountryCharValue::operator-=(const float& rhs){
	mAmount-= rhs;
	CheckDiapason();
	return *this;
}
CountryCharValue& CountryCharValue::operator+=(CountryCharValue* rhs) {
	mAmount+= rhs->mAmount;
	CheckDiapason();
	return *this;
}
bool CountryCharValue::operator< ( const float& rhs){
	return mAmount<rhs;
}
bool CountryCharValue::operator> ( const float& rhs){
	return operator<(rhs);
}
Ogre::String CountryCharValue::Text(){
	if(mPlayerKnow){
		return LogicStd::IntToString(mAmount);
	}
	if(!mPlayerKnow&&mLowDeap==0&&mHightDeap==0){
		return "Unknow";
	}
	if(!mPlayerKnow){

		return LogicStd::IntToString(mLowDeap) +"-"+LogicStd::IntToString(mHightDeap);
	}

	return "Unknow";

}
void CountryCharValue::Open(){

	mPlayerKnow= true;
}
void CountryCharValue::CheckDiapason(){
	if(!mPlayerKnow&&mLowDeap!=0&&mHightDeap!=0){
		float delta=0;
		if(mAmount>mHightDeap){
			delta =  mAmount-mHightDeap;
		}
		if(mAmount<mLowDeap){
			delta =  mAmount -mLowDeap;
		}
		if(mAmount!=0){
			mHightDeap+= delta;
			mLowDeap+= delta;
		}
	}
}
void CountryCharValue::CheckDiapason(int recon,float max){
	if(mPlayerKnow){
		return;
	}
	if(recon==1){

		if(max!=0){
			mLowDeap = mAmount-(std::rand()%(int)(max/4));
			mHightDeap = mAmount+(std::rand()%(int)(max/4));
		}else{
			if((int)mAmount==0){
				mPlayerKnow = true;
				return;
			}
			mLowDeap = mAmount-(std::rand()%(int)(mAmount));
			mHightDeap = mAmount+(std::rand()%(int)(mAmount));
		}
	};

	if(recon>1&&recon<BaseAsset::GetSigleton("RECON_TIME_NEED_TO_OPEN")){
		int half =(mHightDeap-mLowDeap)/2;
		if(half==0){
			half= 2;
		}
		mLowDeap = mAmount-std::rand()%half;
		mHightDeap = mAmount+std::rand()%half;
	}
	if(recon>=BaseAsset::GetSigleton("RECON_TIME_NEED_TO_OPEN")){
		mPlayerKnow = true;
	}

}
CountryCharValue& CountryCharValue::operator=(const float& rhs) {
	mAmount = rhs;
	CheckDiapason();
	return *this;
}

CountryTile::CountryTile(){

}
CountryTile::CountryTile(Ogre::Vector2 vect){
	mPosition = vect;
	mCapital = false;
	mMine= false;
	mBase= false;

}
bool  CountryTile::operator<( const CountryTile & b ) const {

	if(mPosition.x < b.mPosition.x )
	{
		return true ;
	}
	else{ 
		if(mPosition.x == b.mPosition.x)
		{
			if( mPosition.y < b.mPosition.y )
			{
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}


}

BaseCountry::BaseCountry(void)
{
	mDestroyed= false;
	mOcuppied = false;
	mAtWar= false;
	mReconTime=0;
	mColor = Ogre::ColourValue((float)std::rand()/((float)RAND_MAX),(float)std::rand()/((float)RAND_MAX),(float)std::rand()/((float)RAND_MAX));
}


BaseCountry::~BaseCountry(void)
{
	std::map<CountryCharacteristic*,CountryCharValue*>::iterator charIterator;
	for(charIterator = mCharMap.begin(); charIterator != mCharMap.end(); ++charIterator){

		delete mCharMap[charIterator->first];

	}
}
void BaseCountry::ClearRelation(BaseCountry* target){
	delete mRelation[target].mValue;
	delete mRelation[target].mStatus;
}
void BaseCountry::PassWeak(std::queue<NotificationEvent*> &notificationEvent)
{
	if(mAtWar||mDestroyed||mOcuppied){
		return;
	}
	CountryCharValue* delta =0;
	std::map<CountryCharacteristic*,CountryCharValue*>::iterator charItr;
	for(charItr= mCharMap.begin();charItr!=mCharMap.end();++charItr){
		if(	mRefCharMap.count(charItr->first->name+" Delta")){
			delta =mRefCharMap[charItr->first->name+" Delta"];
			if(*(charItr->second)<charItr->first->Max&&*(charItr->second)>charItr->first->Min){
				*(charItr->second)+= delta;

			}
			if(*(charItr->second)>charItr->first->Max&&delta<0){
				*(charItr->second)+= delta;
			}
			if(*(charItr->second)<charItr->first->Min&&delta>0){
				*(charItr->second)+= delta;
			}	
		}



	}

}
void BaseCountry::ChangeRelation(BaseCountry* target,int detla){
	*(mRelation[target].mValue) +=detla;

	if(*(mRelation[target].mValue)>=80){
		*(mRelation[target].mStatus)= CRS_ALLIANCE;
		if(*(mRelation[target].mValue)>100){
			*(mRelation[target].mValue)=100;
		}
	}
	if(*(mRelation[target].mValue)>=50&&*(mRelation[target].mValue)<80){
		*(mRelation[target].mStatus)= CRS_TRADE;

	}
	if(*(mRelation[target].mValue)>-50&&*(mRelation[target].mValue)<50){
		*(mRelation[target].mStatus)= CRS_NEUTRAL;

	}
	if(*(mRelation[target].mValue)<-50&&*(mRelation[target].mValue)>-100){
		*(mRelation[target].mStatus)= CRS_COLDWAR;

	}
	if(*(mRelation[target].mValue)<=-100){
		*(mRelation[target].mStatus)= CRS_WAR;
		*(mRelation[target].mValue)=-100;	
	}

}
void BaseCountry::ChangeRelation(BaseCountry& targetRef,int detla){
	BaseCountry *target =&targetRef;
	*(mRelation[target].mValue) +=detla;

	if(*(mRelation[target].mValue)>=80){
		*(mRelation[target].mStatus)= CRS_ALLIANCE;
		if(*(mRelation[target].mValue)>100){
			*(mRelation[target].mValue)=100;
		}
	}
	if(*(mRelation[target].mValue)>=50&&*(mRelation[target].mValue)<80){
		*(mRelation[target].mStatus)= CRS_TRADE;

	}
	if(*(mRelation[target].mValue)>-50&&*(mRelation[target].mValue)<50){
		*(mRelation[target].mStatus)= CRS_NEUTRAL;

	}
	if(*(mRelation[target].mValue)<-50&&*(mRelation[target].mValue)>-100){
		*(mRelation[target].mStatus)= CRS_COLDWAR;

	}
	if(*(mRelation[target].mValue)<=-100){
		*(mRelation[target].mStatus)= CRS_WAR;
		*(mRelation[target].mValue)=-100;	
	}
}
bool BaseCountry::Pillage(int damage){
	*(mRefCharMap["Economic"])-= damage;
	return *(mRefCharMap["Economic"])<0;
}
bool BaseCountry::Destroying(int damage){
	*(mRefCharMap["Population"])-= damage;
	return *(mRefCharMap["Population"])<0;

}
bool  BaseCountry::Fanatism(int damage){
	*(mRefCharMap["Worship"])+= damage;
	return *(mRefCharMap["Worship"])>100;
}
void BaseCountry::ChangeState(std::map<CountryCharacteristic*,float> charMap){
	std::map<CountryCharacteristic*,float>::iterator charIterator;
	for(charIterator = charMap.begin(); charIterator != charMap.end(); ++charIterator){
		*(mCharMap[charIterator->first])  += 	charIterator->second;
	}

}
int BaseCountry::GetChar(Ogre::String name){
	return  mRefCharMap[name]->mAmount;

}
void BaseCountry::SetChar(Ogre::String name,float value){
	*(mRefCharMap[name])= value;
}
void BaseCountry::ChangeState(Ogre::String name,float delta){


	*(mRefCharMap[name])+= 	delta;


}

void BaseCountry::SetMap(std::map<CountryCharacteristic*,CountryCharValue*> map){

	std::map<CountryCharacteristic*,CountryCharValue*>::iterator charIterator;

	for(charIterator = map.begin(); charIterator != map.end(); ++charIterator){
		if(charIterator->first->name==COM_NAME){
			mCommunication =charIterator->first;

		}

		mCharMap[charIterator->first]=  charIterator->second;
		mRefCharMap[charIterator->first->name]= charIterator->second;
		mCharVector.push_back(charIterator->second);

	}

}
void  BaseCountry::SetDemand(int a,std::vector<BaseResourceObject*> production,std::vector<BaseResourceObject*> offers){
	int prodRand,offrRand;
	BaseDemand* temp;
	int dnmCnt =BaseAsset::GetSigleton("BASE_DEMAND_CNT");
	for(int i =0; i<a; i++){
		prodRand = std::rand()%production.size();
		offrRand = std::rand()%offers.size();
		mDemand.push_back(BaseDemand());
		temp = &mDemand.back();
		temp->amount =1+std::rand()%2;
		temp->demand = production[prodRand];
		temp->offer = offers[offrRand];
		temp->demandCnt =dnmCnt +std::rand()%dnmCnt;
	}
	mDemand.front().open= true;
}


void BaseCountry::SetTrade(BaseDemand* demand){
	std::list<BaseDemand>::iterator itr;
	if(*(mRefCharMap["IIF"])<0){
		return;
	}
	for(itr  = mDemand.begin();itr!=mDemand.end();++itr){
		if(&(*itr) ==demand){
			(*itr).isTrade = !	(*itr).isTrade;
		}	
	}
}
std::list<SendBaseDemand> BaseCountry::GetDemandForEmabassy(){
	std::list<BaseDemand>::iterator itr;
	std::list<SendBaseDemand> list;
	for(itr  = mDemand.begin();itr!=mDemand.end();++itr){
		if(itr->open){
			list.push_back(SendBaseDemand((*itr)));
		}
	}
	return list;

}
int BaseCountry::GetChar(CountryCharacteristic* Char){
	return mCharMap[Char]->mAmount;
}
int  BaseCountry::GetCom(){
	return GetChar(mCommunication)/mCommunication->Max;
}
void  BaseCountry::GetMap(std::map<Ogre::String,Ogre::String> &map){
	std::map<CountryCharacteristic*,CountryCharValue*>::iterator charIterator;

	for(charIterator = mCharMap.begin(); charIterator != mCharMap.end(); ++charIterator){
		if(charIterator->first->type!= CT_DELTA){
			map[charIterator->first->name]= charIterator->second->Text();
		}
	}

}
void BaseCountry::SetRelations(BaseCountry* target,int relation){
	mRelation[target].mValue = new int();
	*(mRelation[target].mValue ) =relation;
	mRelation[target].mStatus = new CountryRelationStatus();

	*(mRelation[target].mStatus)= CRS_NEUTRAL;
	mRelation[target].mAIScript= BaseAIManager::GetSigleton()->GetAINode(BaseLogic::GetSigleton()->mLuaMan.CalculateInitAI(this,target));
}
void BaseCountry::LoadRelations(BaseCountry* target,int relation,CountryRelationStatus status){
	mRelation[target].mValue  = new int();
	*(mRelation[target].mValue ) =relation;
	mRelation[target].mStatus = new CountryRelationStatus();
	mRelation[target].mAIScript= BaseAIManager::GetSigleton()->GetAINode(BaseLogic::GetSigleton()->mLuaMan.CalculateInitAI(this,target));
	*(mRelation[target].mStatus)= status;
}
void BaseCountry::SetRelations(BaseCountry* target, int* num,CountryRelationStatus* status){
	mRelation[target].mValue = num;
	mRelation[target].mAIScript= BaseAIManager::GetSigleton()->GetAINode(BaseLogic::GetSigleton()->mLuaMan.CalculateInitAI(this,target));
	mRelation[target].mStatus=status;
}
int* BaseCountry::GetRelation(BaseCountry* target){
	return mRelation[target].mValue ;
}
bool BaseCountry::Reconing(){
	if(++mReconTime<=BaseAsset::GetSigleton("RECON_TIME_NEED_TO_OPEN")){
		std::map<CountryCharacteristic*,CountryCharValue*>::iterator charIter;
		for(charIter = mCharMap.begin();charIter!= mCharMap.end();++charIter){
			charIter->second->CheckDiapason(mReconTime,	charIter->first->Max);
		}
		return false;
	}
	return true;
}
void BaseCountry::CalculateAI(){
	std::set<BaseCountry*>::iterator neiborIter;
	BaseAINode*tempNode;
	std::map<Ogre::String ,BaseAINode*>::iterator nextItter;
	for(neiborIter=mNeibor.begin();neiborIter!= mNeibor.end();neiborIter++){
		tempNode =mRelation[*neiborIter].mAIScript;
		BaseLogic::GetSigleton()->mLuaMan.AIScript(tempNode->mScriptName,this,*neiborIter);
		for(nextItter=tempNode->NextNodes.begin(); nextItter!=tempNode->NextNodes.end();nextItter++){
			if(BaseLogic::GetSigleton()->mLuaMan.AIScript(nextItter->first,this,*neiborIter)){
				mRelation[*neiborIter].mAIScript= nextItter->second;

			}
		}
	}
	
}
void BaseCountry::ChangeDelta(BaseResourceObject* object){
	float coef =BaseAsset::GetSigleton("DELTA_COEF_TRADE"),procent=0;
	std::map<CountryCharacteristic*,CountryCharValue*>::iterator charIter;
	std::map<BaseResource*,int>::const_iterator resItr;
	for(charIter = mCharMap.begin();charIter!= mCharMap.end();++charIter){
		if(charIter->first->type!= CT_DELTA){
			continue;
		}
		procent =coef*(charIter->second->mAmount)/100;
		for(resItr =object->mResource.begin();resItr!=object->mResource.end();++resItr){
			*(charIter->second)+= charIter->first->TradeDependace[resItr->first]*resItr->second*procent;
		}
	}
}
CountryRelationStatus* BaseCountry::GetRelatStatus(BaseCountry* target){
	return mRelation[target].mStatus;
}
ResourseDeposits::ResourseDeposits(BaseCountry* country){
	mOcupiet= false;
	mOpen= false;
	mMine = false;
	mCountry= country;
	BasePlanetCountry*temp = static_cast<BasePlanetCountry*>(country);
	temp->SetDeposit(this);
}
ResourseDeposits::~ResourseDeposits(void){

}
void ResourseDeposits::SetCountry(BaseCountry* country){
	mCountry= country;
	BasePlanetCountry*temp = static_cast<BasePlanetCountry*>(country);
	temp->SetDeposit(this);
}
BaseCountry* ResourseDeposits::GetCountry(){
	return mCountry;
}
void ResourseDeposits::SetMap(std::vector<BaseResourceObject*> vector){
	int i=0,perc=0,num=0;

	int a = std::rand() % 100;
	while(i<4&&perc<100){
		perc+=a;
		i++;
		num = std::rand() % (vector.size());
		std::cout<<"Num:"<<num;
		mGeneratePattern[perc]= vector[num];
	}
	mProductPower= 1+std::rand() % (int)BaseAsset::GetSigleton("MAX_MINEPOWER");
}
BaseResourceObject * ResourseDeposits::GenerateResource(){

	int a = std::rand() % 100;
	std::map<float,BaseResourceObject*>::iterator itr;

	for(itr =mGeneratePattern.begin(); itr != mGeneratePattern.end(); ++itr){
		if(itr->first>a){
			return itr->second;
		}
	}
	itr--;
	return itr->second;
}
BasePlanet::BasePlanet(void){
	mCountryCount=0;
	mDepositCount=0;
}
//TODO CLEAN FOR RELATION;
BasePlanet::~BasePlanet(){
	for(int i =0; i<mCountryCount;++i){
		
		for(int j =i;j<mCountryCount;++j){
			if(i!=j){
				
				mCountry[i]->ClearRelation(mCountry[j]); 
				
			}
		}
	}
	for(int i =0; i<mCountryCount;++i){
		if(*(mCountry+i)!=0){
			delete mCountry[i];
		}	
	}
	for(int i=0;i<mDepositCount;i++){
		
			delete mAllDepositVect[i];
	}
	if(mCountryCount!=0){
		delete [] mCountry;
	}
}
void BasePlanet::PassWeak(std::queue<NotificationEvent*> &notificationEvent){
	for(int i =0; i<mCountryCount;++i){
		if((mCountry+i)!=0){
			mCountry[i]->PassWeak(notificationEvent);
		}	
	}
	
}
void BasePlanet::DeleteBuilding(void* pnt){
	for(int i =0; i<mCountryCount;++i){
		if((mCountry+i)!=0){
			mCountry[i]->DeleteBuilding(pnt);
		}	
	}
}
BaseCountry*BasePlanet::GetNextCountry(){
	static int i=std::rand()%mCountryVector.size();
	i++;
	if(i>=mCountryVector.size()){
		i=0;
	}
	return mCountryVector[i];

}
void BasePlanet::AddDeposit(BaseCountry *test){
		std::vector<BaseResourceObject*> oresVector = mResMan->GetAllResourse(RT_ORES);
		mDepositCount++;
		ResourseDeposits* temp =new  ResourseDeposits(test);
		temp->SetMap(oresVector);
		temp->mName ="deposit"+ LogicStd::IntToString(mDepositCount);		
		mAllDepositVect.push_back(temp);
}
PlanetJustGenerateEvent* BasePlanet::GeneratePlanet(int countryCount,int clusterCnt,PlanetEnvorementType type,CharacteristicManager *charMan,int depositCount,BaseResourceManager *resMan){
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

		mCountry[i]= new BasePlanetCountry(this);
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
				if((j-i)%clusterCnt==0){
					rel = 10+ (std::rand()%90);
				}else{
					rel = -10 - (std::rand()%90);
				}
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
struct CompareVectors
{
	bool operator()(const  Ogre::Vector2& a, const  Ogre::Vector2& b)
	{
		if(a.x<b.x){
			return true;
		}else{
			if(a.x==b.x){
				if(a.y<b.y){
					return true;
				}
				else{
					return false;
				}

			}else{
				return false;
			}
		}

		// insert comparison code from question
	}
};

void BasePlanet::GenerateTiles(){
	bool generate= true;
	int maxX = (int)BaseAsset::GetSigleton("MAP_X"),maxY=(int)BaseAsset::GetSigleton("MAP_Y");
	int r= 0;
	std::set<Ogre::Vector2,CompareVectors> usedTiles;
	std::map<Ogre::Vector2,BaseCountry*,CompareVectors> map;
	
	Ogre::Vector2 tempVect =	Ogre::Vector2(0,0);
	for(int i =0; i<mCountryCount;++i){
		tempVect = Ogre::Vector2(std::rand()%maxX,std::rand()%maxY);
		while(usedTiles.count(tempVect)!=0){
			tempVect = Ogre::Vector2(std::rand()%maxX,std::rand()%maxY);
		}
		usedTiles.insert(tempVect);
		map[tempVect]= mCountry[i];
		mCountry[i]->mCapital = CountryTile(tempVect);
	}
	BaseCountry * tempCountry =0;
	int count =0,xeffct;
	while(generate){
		generate= false;
		r++;
		for(int i =0; i<mCountryCount;++i){
			tempCountry= mCountry[i];
			for(int x=tempCountry->mCapital.mPosition.x-r; x<=tempCountry->mCapital.mPosition.x+r;++x){
				xeffct= x;
				if(xeffct<0){
					xeffct= maxX+xeffct+1;
				}
				if(xeffct>maxX){
					xeffct =xeffct - maxX-1;
				}
				
				for(int y=tempCountry->mCapital.mPosition.y-r; y<=tempCountry->mCapital.mPosition.y+r;++y){
					if(y<0){
						continue;
					}
					if(y>maxY){
						break;
					}
					tempVect = Ogre::Vector2(xeffct,y);
					count =usedTiles.count(tempVect);
					if(count==0){
						generate= true;
						usedTiles.insert(tempVect);
						tempCountry->mCountryTiles.push_back(CountryTile(tempVect));
						map[tempVect]= tempCountry;
					}
				}				
			}
		}	
	}
	int size= map.size();
		std::vector<CountryTile>::iterator tileItr;
	for(int i =0; i<mCountryCount;++i){
		for(tileItr =mCountry[i]-> mCountryTiles.begin();tileItr!= mCountry[i]-> mCountryTiles.end();++tileItr){
		
				for(int x=tileItr->mPosition.x-1; x<=tileItr->mPosition.x+1;++x){
				xeffct= x;
				if(xeffct<0){
					xeffct= maxX+xeffct+1;
				}
				if(xeffct>maxX){
					xeffct =xeffct - maxX-1;
				}
				
				for(int y=tileItr->mPosition.y-1; y<=tileItr->mPosition.y+1;++y){
					if(y<0){
						continue;
					}
					if(y>maxY){
						break;
					}
					tempVect = Ogre::Vector2(xeffct,y);
					if(map[tempVect]!=mCountry[i]){
						mCountry[i]->mNeibor.insert(map[tempVect]);
					}
				}
				}
		}
	}
	generate=false;

}
void BasePlanet::ChangeState(std::map<CountryCharacteristic*,float> charMap,BaseCountry* iniciator){
	std::map<CountryCharacteristic*,float>::iterator charIterator;

	for(charIterator = charMap.begin(); charIterator != charMap.end(); ++charIterator){
		charIterator->second  = 	charIterator->second*iniciator->GetCom();
	}


	for(int i =0; i<mCountryCount;++i){
		if(mCountry[i]!=iniciator){
			mCountry[i]->ChangeState(charMap);
		}	
	}

}
void BasePlanet::ChangeState(Ogre::String name,float delta,BaseCountry* iniciator){

	delta = delta*iniciator->GetCom();
	for(int i =0; i<mCountryCount;++i){
		if(mCountry[i]!=iniciator){
			mCountry[i]->ChangeState(name,delta);
		}	
	}


}
void BasePlanet::GetAllCountry(std::map<Ogre::String,BaseCountry *> &allCountry){
	for(int i =0; i<mCountryCount;++i){
		allCountry[mCountry[i]->mName]= mCountry[i];
	}
}
void BasePlanet::GetAllCountry(std::map<Ogre::String,BaseCountry *> &allCountry, Ogre::String &action){
	BaseLogicLua &lua =	BaseLogic::GetSigleton()->mLuaMan;
	for(int i =0; i<mCountryCount;++i){
		if(lua.CountryCheck(static_cast<BasePlanetCountry*>(mCountry[i]),action)){
		allCountry[mCountry[i]->mName]= mCountry[i];
		}
	}
}
std::map<Ogre::String,ResourseDeposits *> BasePlanet::GetAllDeposit(){
	std::map<Ogre::String,ResourseDeposits *> map;
	int num=0;
	ResourseDeposits * temp =0; 
	for(int i=0;i<mDepositCount;i++){
		if(!mAllDepositVect[i]->mOcupiet&&mAllDepositVect[i]->mOpen){
			map[mAllDepositVect[i]->mName]= mAllDepositVect[i];
		}
	}
	return map;
}



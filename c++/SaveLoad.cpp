#include "SaveLoad.h"
#include "BaseLogic.h"


using namespace Logic;

SaveLoad::SaveLoad(void)
{
}


SaveLoad::~SaveLoad(void)
{
}

void SaveLoad::Save(Ogre::String name){
	std::ofstream ofs("saves/"+name+".data");
	BaseLogic * temp = BaseLogic::GetSigleton();

	// save data to archive
	{
		boost::archive::text_oarchive  oa(ofs);

		// write class instance to archive
		oa << temp;

		// archive and stream closed when destructors are called
	}

	/*std::ifstream ifs("saves/test.data");
	{
	boost::archive::text_iarchive ia(ifs);
	// read class state from archive
	ia >> temp;

	}*/

}
template<class Archive>void BaseLogic::save(Archive & ar, const unsigned int version) const{
	ar & mPlanet;
	ar & mResMng;
	ar & mCorp;
	ar & mRequestMan;
	ar & mAccount;
}
template<class Archive>void BaseLogic::load(Archive & ar, const unsigned int version){
	ar & mPlanet;
	ar & mResMng;
	ar & mCorp;
	ar & mRequestMan;
	ar & mAccount;
}
template<class Archive>void BaseAccount::save(Archive & ar, const unsigned int version) const{
	ar & mMonthlyIncome;
	ar & mMonthlyOutcome;
	ar & mMonthlyFeed;
	ar & mCredit;
}
template<class Archive>void BaseAccount::load(Archive & ar, const unsigned int version){
	ar & mMonthlyIncome;
	ar & mMonthlyOutcome;
	ar & mMonthlyFeed;
	ar & mCredit;
}
template<class Archive>void BasePlanet::save(Archive & ar, const unsigned int version) const{
	ar & mType;
	std::vector<BasePlanetCountry*> vectContry;
	std::map<BaseCountry*,int> countryMap;
	for(int i =0; i<mCountryCount; ++i){
		vectContry.push_back(static_cast<BasePlanetCountry*>(mCountry[i]));
		countryMap[mCountry[i]]=i;
	}
	ar &vectContry;
	std::vector<int> allRelation;
	std::vector<CountryRelationStatus> allStatusRelation;
	for(int i =0; i<mCountryCount;++i){
		for(int j=0;j<i;++j){
			mCountry[i]->SetRelations(mCountry[j],mCountry[j]->GetRelation(mCountry[i]),mCountry[j]->GetRelatStatus(mCountry[i])); 
		}
		for(int j =i;j<mCountryCount;++j){
			if(i!=j){
				allRelation.push_back(*mCountry[j]->GetRelation(mCountry[i])); 
				allStatusRelation.push_back(*mCountry[j]->GetRelatStatus(mCountry[i]));
			}
		}
	}
	ar &allRelation;
	ar &allStatusRelation;
	std::vector<ResourseDeposits *> vectorDep;
	std::map<int,int> depositMap;
	for(int i=0;i<mDepositCount;i++){
		if(mAllDepositVect[i]!=0){
			vectorDep.push_back(mAllDepositVect[i]);
			depositMap[i]= countryMap[mAllDepositVect[i]->GetCountry()];
		}
	}
	ar &vectorDep;
	ar &depositMap;
}
template<class Archive>void BasePlanet::load(Archive & ar, const unsigned int version){
	ar & mType;
	std::vector<BasePlanetCountry*> vectContry;
	ar &vectContry
		for(int i =0; i<mCountryCount; ++i){
			delete mCountry[i];

		}
		for(int i=0;i<mDepositCount;i++){
		
				delete mAllDepositVect[i];
		
		}
		delete [] mCountry;
	

		mCountryCount = vectContry.size();
		mCountry= new BaseCountry*[mCountryCount];
		for(int i =0; i<mCountryCount; ++i){
			mCountry[i] =vectContry[i];
			mCountryVector[i] = vectContry[i];
		}
		std::vector<int> allRelation;
		std::vector<CountryRelationStatus> allStatusRelation;
		ar &allRelation;
		ar &allStatusRelation;
		int rel;k=0;
		for(int i =0; i<mCountryCount;++i){
			for(int j=0;j<i;++j){
				mCountry[i]->SetRelations(mCountry[j],mCountry[j]->GetRelation(mCountry[i]),mCountry[j]->GetRelatStatus(mCountry[i])); 
			}
			for(int j =i;j<mCountryCount;++j){
				if(i!=j){
					mCountry[i]->LoadRelations(mCountry[j],allRelation[k],allStatusRelation[k]); 

					k++
				}
			}
		}
		std::vector<ResourseDeposits *> vectorDep;
		std::map<int,int> depositMap
			ar &vectorDep;
		ar &depositMap;
		mDepositCount = vectorDep.size();
		mAllDepositVect.clear();
		for(int i =0; i<mDepositCount; ++i){
			mAllDepositVect.push_back(vectorDep[i]);
			mAllDepositVect.back()->SetCountry(mCountry[depositMap[i]]);
		}



}
template<class Archive>void BasePlanetCountry::save(Archive & ar, const unsigned int version) const{
	ar & mReconTime;
	ar & mDestroyed;
	ar & mOcuppied;
	ar & mDestroyed;
	ar & mName;
	ar &mRefCharMap;
	ar &mDemand;
}
template<class Archive>void BasePlanetCountry::load(Archive & ar, const unsigned int version){
	ar & mReconTime;
	ar & mDestroyed;
	ar & mOcuppied;
	ar & mDestroyed;
	ar & mName;
	ar &mRefCharMap
		std::map<Ogre::String,CountryCharValue*>::iterator charIter;
	for(charIter= mRefCharMap.begin();charIter!= mRefCharMap.end();++charIter){
		mCharMap[CharacteristicManager::GetChar(charIter->first)] = charIter->second;
	}
	ar &mDemand;

}
template<class Archive>void CountryCharValue::save(Archive & ar, const unsigned int version) const{
	ar & mPlayerKnow;
	ar & mHightDeap;
	ar & mLowDeap;
	ar & mAmount;

}
template<class Archive>void CountryCharValue::load(Archive & ar, const unsigned int version){
	ar & mPlayerKnow;
	ar & mHightDeap;
	ar & mLowDeap;
	ar & mAmount;
}
template<class Archive>void BaseDemand::save(Archive & ar, const unsigned int version) const{
	ar & amount;
	ar & demandCnt;
	ar & demandProgress;
	ar & isTrade;
	ar & demand->mName;
	ar & offer->mName;
}
template<class Archive>void BaseDemand::load(Archive & ar, const unsigned int version){
	ar & amount;
	ar & demandCnt;
	ar & demandProgress;
	ar & isTrade;
	Ogre::String name;
	ar & name;
	demand = 	BaseLogic::GetSigleton()->mResMng.GetResourceByName(name);
	ar & name;
	offer = 	BaseLogic::GetSigleton()->mResMng.GetResourceByName(name);
}
template<class Archive>void BaseResourceManager::save(Archive & ar, const unsigned int version) const{
	std::map<Ogre::String, int> amountMap;
	std::map<BaseResourceObject*,int>::const_iterator resItr;

	for(resItr= mAllObjectsCount.begin();resItr!= mAllObjectsCount.end();++resItr){
		amountMap[resItr->first->mName]= resItr->second;		
	}
	ar & amountMap;

}
template<class Archive>void BaseResourceManager::load(Archive & ar, const unsigned int version){
	std::map<Ogre::String, int> amountMap;
	std::map<BaseResourceObject*,int>::iterator resItr;
	ar & amountMap;
	for(resItr= mAllObjectsCount.begin();resItr!= mAllObjectsCount.end();++resItr){
		if(amountMap.count(resItr->first->mName)){
			resItr->second= amountMap[resItr->first->mName];		
		}else{
			resItr->second= 0;
		}
	}

}
template<class Archive>void ResourseDeposits::save(Archive & ar, const unsigned int version) const{
	ar & mOcupiet;
	ar & mOpen;
	ar & mName;
	std::map<float, Ogre::String> map; 
	std::map<float, BaseResourceObject *>::const_iterator mapItr;
	for(mapItr=mGeneratePattern.begin(); mapItr!=mGeneratePattern.end();++mapItr){
		map[mapItr->first]= mapItr->second->mName;
	} 
	ar & map;
}
template<class Archive>void ResourseDeposits::load(Archive & ar, const unsigned int version){
	ar & mOcupiet;
	ar & mOpen;
	ar & mName;
	std::map<float, Ogre::String> map; 
	ar & map;
	std::map<float,  Ogre::String>::const_iterator mapItr;
	for(mapItr=map.begin(); mapItr!=map.end();++mapItr){
		map[mapItr->first]= 	BaseLogic::GetSigleton()->mResMng.GetResourceByName(mapItr->second);
	} 

}
template<class Archive>void BaseCorporate::save(Archive & ar, const unsigned int version) const{
	ar & mCreditCoef;
	std::map<Ogre::String,BaseDepartment*> map;
	std::map<BaseResource *,BaseDepartment*>::const_iterator deprItr;
	for(deprItr=mDepartmetRelation.begin();deprItr!= mDepartmetRelation.end(); ++deprItr){
		map[deprItr->first->mName] = deprItr->second;
	}
	ar & map;

}
template<class Archive>void BaseCorporate::load(Archive & ar, const unsigned int version){
	ar & mCreditCoef;
	std::map<Ogre::String,BaseDepartment*> map;
	std::map<BaseResource *,BaseDepartment*>::iterator deprItr;
	std::map<Ogre::String,BaseDepartment*>::iterator mapItr;
	for(deprItr=mDepartmetRelation.begin();deprItr!= mDepartmetRelation.end(); ++deprItr){
		delete deprItr->second;
	}
	mDepartmetRelation.clear();
	ar & map;
	for(mapItr=map.begin();mapItr!= map.end(); ++mapItr){
		deprItr->second->mCurResource = BaseLogic::GetSigleton()->mResMng.GetResourceByName(deprItr->first);
		mDepartmetRelation[	deprItr->second->mCurResource]=deprItr->second;
	}

}
template<class Archive>void BaseDepartment::save(Archive & ar, const unsigned int version) const{
	ar & mTimeFromLast;
	ar & mRequestComplite;
	ar & mResearchComplite;
	ar & mProjectComplite;
	ar & mProjectFailed;
	ar & mProjectCompliteNPC;
	ar & mProjectFailedNPC;
	ar & mTotalKPD;
	ar & mCreditSend;
	ar &mSuccsessRate;
}
template<class Archive>void BaseDepartment::load(Archive & ar, const unsigned int version){
	ar & mTimeFromLast;
	ar & mRequestComplite;
	ar & mResearchComplite;
	ar & mProjectComplite;
	ar & mProjectFailed;
	ar & mProjectCompliteNPC;
	ar & mProjectFailedNPC;
	ar & mTotalKPD;
	ar & mCreditSend;
	ar &mSuccsessRate;

}
class RequsetForSave{
	friend class boost::serialization::access;
public:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & mAmount;
		ar & mTime;
	}
	int mAmount;
	int mTime;
};
class ProjectForSave{
	friend class boost::serialization::access;
public:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & mPeriodProgress;
		ar & mPeriodCntComplite;
		ar & mAllComplite;
		ar & mPeriodComplite;
		ar & mDaysFromStart;
	}
	int mPeriodProgress;
	int mPeriodCntComplite;
	int mPeriodComplite;
	int mAllComplite;
	int mDaysFromStart;
};

template<class Archive>void BaseRequestManager::save(Archive & ar, const unsigned int version) const{
	std::map<int,RequsetForSave> reqMap;
	std::map<BaseRequest*,int>::const_iterator reqItr;
	int id;
	for(reqItr = mPendingRequest.begin();reqItr!=mPendingRequest.end();++reqItr){
		id =reqItr->first->mId;
		reqMap[id] = RequsetForSave();
		reqMap[id].mAmount = reqItr->first->mCurAmount;
		reqMap[id].mTime=reqItr->second;
	}
	ar & reqMap;
	std::map<int,ProjectForSave> projMap;
	std::vector<int> sujestMap;
	std::set<BaseProject*>::const_iterator proItr;
	for( proItr= mPendingProjects.begin();proItr!= mPendingProjects.end();++proItr){
		id =(*proItr)->mId;
		projMap[id] = ProjectForSave();
		projMap[id].mAllComplite  =(*proItr)->mAllComplite;
		projMap[id].mPeriodProgress  =(*proItr)->mPeriodProgress;
		projMap[id].mPeriodCntComplite  =(*proItr)->mPeriodCntComplite;
		projMap[id].mPeriodComplite  =(*proItr)->mPeriodComplite;
		projMap[id].mDaysFromStart  =(*proItr)->mDaysFromStart;
	}

	for( proItr= mSujestProject.begin();proItr!= mSujestProject.end();++proItr){
		sujestMap.push_back((*proItr)->mId);
	}
	ar & projMap;
	ar & sujestMap;
}
template<class Archive>void BaseRequestManager::load(Archive & ar, const unsigned int version){
	std::map<int,RequsetForSave> reqMap;
	std::map<BaseRequest*,int>::iterator reqItr;
	std::map<int,RequsetForSave>::iterator reqMapItr;
	for(reqItr = mPendingRequest.begin();reqItr!=mPendingRequest.end();++reqItr){
		reqItr->first->mAmount= 0;
	}
	mPendingMap.clear();
	BaseRequest *temp;
	ar & reqMap;
	for(reqMapItr = reqMap.begin();reqMapItr!=reqMap.end();++reqMapItr){
		temp = GetRequestById(reqMapItr->first);
		mPendingRequest[temp]= reqMapItr->second.mTime;
		temp->mCurAmount = reqMapItr->second.mAmount;
	}

	std::map<int,ProjectForSave> proMap;
	std::set<BaseProject*>::iterator proItr;
	std::map<int,ProjectForSave>::iterator proMapItr;
	BaseProject *tempPro;
	for(reqItr = mPendingProjects.begin();reqIrr!=mPendingProjects.end();++reqItr){
		*(reqItr)->Clear();
	}
	mPendingProjects.clear();
	ar & projMap;
	for(proMapItr = projMap.begin();proMapItr!=projMap.end();++proMapItr){
		tempPro = GetProjectById(proMapItr->first);
		mPendingProjects.insert(tempPro);
		tempPro.mAllComplite  =proMapItr->second.mAllComplite;
		tempPro.mPeriodProgress  =proMapItr->second.mPeriodProgress;
		tempPro.mPeriodCntComplite  =proMapItr->second.mPeriodCntComplite;
		tempPro.mPeriodComplite  =proMapItr->second.mPeriodComplite;
		tempPro.mDaysFromStart  =proMapItr->second.mDaysFromStart;
	}
	std::vector<int> sujestMap;
	ar & sujestMap;
	mSujestProject.clear();
	std::vector<int>::iterator sujestItr;
	for(sujestItr = sujestMap.begin();sujestItr!= sujestMap.end(); ++sujestItr){
		mSujestProject.insert(GetProjectById(*sujestItr));
	}
}
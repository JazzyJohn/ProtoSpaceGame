#include "BaseRecon.h"
#include "BaseIntelegence.h"
using namespace Logic;

BaseRecon::BaseRecon(void)
{
	mProductionAmount=BaseAsset::GetSigleton("RECON_PRODUCTION");
	mCountry=0;
}


BaseRecon::~BaseRecon(void)
{
}
void BaseRecon::DoRecon(){
	if(mCountry!=0){
		if(mCountry->Reconing()){
			if(mCurProduct<BaseAsset::GetSigleton("RECON_PRODUCTION_NEED")){
				mCurProduct+= mProductionAmount;
			}else{
				mCurProduct=0;
				IntelegentManager::GetSigleton()->GenerateResource(AT_RECON);
			}
		}
	}
}
void BaseRecon::MoveToNext(BaseCountry *country){
	mCountry = country;
}
BaseCountry *BaseRecon::GetCountry(){
	return mCountry;
}
ReconStateEvnt::ReconStateEvnt(){
	type =ET_GAME;
	gameType =GUIET_SHOWRECON;
}
ReconManadger::ReconManadger(){

};
ReconManadger::~ReconManadger(){
	std::list<BaseRecon*>::iterator recItr;
	for(recItr= mAllRecon.begin();recItr!= mAllRecon.end();++recItr){
		delete (*recItr);
	}
}
GameEvent* ReconManadger::ReturnReconState(){
	ReconStateEvnt * evnt = new ReconStateEvnt();
	std::list<BaseRecon*>::iterator recItr;
	ReconToSend* recSend=0;
	int i=0;
	for(recItr= mAllRecon.begin();recItr!= mAllRecon.end();++recItr){
		i++;
		evnt->mAllRecon.push_back(ReconToSend());
		recSend= 	&evnt->mAllRecon.back();
		recSend->country= (*recItr)->GetCountry()->mName;
		recSend->name = LogicStd::IntToString(i);
	}
	return evnt;
}
void ReconManadger::LaunchRecont(){
	BaseRecon *temp =  new BaseRecon();
	temp->MoveToNext(mPlanet->GetNextCountry());
	mAllRecon.push_back(temp);
}
void ReconManadger::PassDay(){
	std::list<BaseRecon*>::iterator recItr;
	for(recItr= mAllRecon.begin();recItr!= mAllRecon.end();++recItr){
		(*recItr)->DoRecon();
		(*recItr)->MoveToNext(mPlanet->GetNextCountry());
	}

}
void ReconManadger::SetPlanet(BasePlanet* planet){
	mPlanet= planet;
}
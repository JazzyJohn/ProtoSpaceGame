#include "BaseListener.h"

using namespace Logic;
BaseListener* BaseListener::sigleton = 0;
ShowResourceEvnt::ShowResourceEvnt() 
{
	gameType =GUIET_SHOWRESOURSE;
	type =ET_GAME;
}
BuildingBuiltEvnt::BuildingBuiltEvnt() 
{
	gameType =GUIET_BUILDINGDUILT;
	type =ET_GAME;
}
CoreBuiltEvnt::CoreBuiltEvnt(){
	gameType =GUIET_COREBUILT;
	type =ET_GAME;
}
SimpleBuiltSend::SimpleBuiltSend() 
{
	gameType =GUIET_SENDCORETOGUI;
	type =ET_GAME;
}
SendCoreToGui::SendCoreToGui() 
{
	gameType =GUIET_SENDCORETOGUI;
	type =ET_GAME;
}
SendBlueToGui::SendBlueToGui(){
	gameType =GUIET_SENDBLUETOGUI;
	type =ET_GAME;
}
ShowMining::ShowMining(){
	type =ET_GAME;
	gameType =GUIET_SHOWMINING;
}
ShowWarehouse::ShowWarehouse(){
	type =ET_GAME;
	gameType =GUIET_SHOWWAREHOUSE;
}
ShowProduction::ShowProduction(){
	type =ET_GAME;
	gameType =GUIET_SHOWPRODUCTION;
}

ShowWarehouse::~ShowWarehouse(){
	delete []allSlot;
}
ShowIntelegence::ShowIntelegence(){
	type =ET_GAME;
	gameType =GUIET_SHOWINTELEGANCE;
}
ShowAllRequest::ShowAllRequest(){
	type =ET_GAME;
	gameType =GUIET_SHOWALLREQUEST;
}

ShowSendStatus::ShowSendStatus(){
	type =ET_GAME;
	gameType =GUIET_SHOWREQRESDIALOG;
}
ShowProjectSendStatus::ShowProjectSendStatus(){
	type =ET_GAME;
	gameType =GUIET_SHOWPROJDIALOG;
}
ShowAllResourse::ShowAllResourse():amount(),links(),intLinks(),intAmount(){
	type =ET_GAME;
	gameType =GUIET_SHOWALLRESOURSE;
	
	
}
NewNews::NewNews(){
	type =ET_GAME;
	gameType =GUIET_ADDNEWNEWS;
}
SendCoreToGui::~SendCoreToGui() 
{
	
	delete [] allSlots;
}
ControlEvent::ControlEvent(){
	type =ET_CONTROL;
	controlType =CTE_SHOWRESOURCE;
}
ShowBuilding::ShowBuilding(){
	type =ET_CONTROL;
	controlType =CTE_SHOWBUILDING;
}

SomethingSimple::SomethingSimple(){
	type =ET_CONTROL;
	controlType =CTE_SHOWRESOURCE;
}
SenResorceChng::SenResorceChng(){
		type =ET_CONTROL;
	controlType =	CTE_RESOURSEREDIALOGCHANGE;
}
AskIntActionDialog::AskIntActionDialog(){
	type =ET_CONTROL;
	controlType =	CTE_DIALOGFORACTION;
}
SetSomtingIntAction::SetSomtingIntAction(){
	type =ET_CONTROL;
	controlType =	CTE_DIALOGFORACTIONSETSOMTHING;
}
ShowBlueprint::ShowBlueprint(){
	type =ET_CONTROL;
	controlType =CTE_ASKFORBLUEPRINT;

}
BuildOrder::BuildOrder(){
		type =ET_CONTROL;
	controlType =CTE_BUILDSOMETHING;

}
TradeRequest::TradeRequest(){
	type =ET_CONTROL;
	controlType =CTE_ASKTRADE;
}
ProductSet::ProductSet(){
	type =ET_CONTROL;
	controlType =CTE_SETPRODUCTION;
}
ActionWarfareSet::ActionWarfareSet(){
	type =ET_CONTROL;
	controlType =CTE_SETWARFAREACTION;
}
void BaseNotificator::Init(BaseListener* listener){
	mListener= listener;

}
void BaseNotificator::DayCheck(){
	while(!mNotificationEvent.empty()){
		mListener->fireGameEvent(mNotificationEvent.front());
		mNotificationEvent.pop();
	}
}
BaseListener::BaseListener(void)
{
	mExit= false;
}


BaseListener::~BaseListener(void)
{
	while(!mGameEventStack.empty()){
		delete mGameEventStack.front();
		mGameEventStack.pop();
	}
}
BaseListener * BaseListener::GetSigleton(){
	return sigleton;
}
void BaseListener::Exit(){
	mExit= true;
}
BaseEventGenerator::BaseEventGenerator(void): mEventTimer(0){

	mType = GENT_SIMPLE;
}
BaseEventGenerator::BaseEventGenerator(GeneratorType type): mEventTimer(0){

	mType = type;
}
BaseEventGenerator::~BaseEventGenerator(void){

}
void BaseEventGenerator::IncrementTime(float deltaTime){
	mEventTimer+= deltaTime;
}
int BaseEventGenerator::Makepercent(){

	return (int) mEventTimer*AX_GENERATOT;

}
bool BaseEventGenerator::GenerateEvent(){


	int a = std::rand() % 100;
	isEvent= a<Makepercent();
	if(isEvent){
		mEvent = new GameEvent();
		mEventTimer=0;
	}
	return isEvent;
}

bool BaseEventGenerator::GetEvent(GameEvent **evnt){
	if(isEvent)
		*evnt = mEvent;
	return isEvent; 

}


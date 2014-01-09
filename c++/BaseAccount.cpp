#include "BaseAccount.h"
using namespace Logic;
BaseAccount* BaseAccount::sigleton= 0;
BaseAccount::BaseAccount(void)
{
	sigleton= this;
	mCredit=1000;
	mMonthlyOutcome=0;
	mMonthlyIncome= 0;
	mMonthlyFeed = 0;
}


BaseAccount::~BaseAccount(void)
{
}
BaseAccount* BaseAccount::GetSigleton(){
	return sigleton;
}
void BaseAccount::ChangeCredit(float credit){
	mCredit +=credit;
	if(credit<0){
		mMonthlyOutcome += credit;
	}else{
		mMonthlyIncome += credit;
	}
}
void BaseAccount::AddOutcomeToMonthlyFeed(float credit){
	mMonthlyFeed+= credit;
}
void BaseAccount::DoMonthlyCount(int month){
	
	mAllMonthly.push_back(Monthly());
	Monthly *cur = &mAllMonthly.back();
	mCredit -= mMonthlyFeed;
	cur->monthlyFeed =mMonthlyFeed;
	cur->credit = mCredit;
	cur->monthlyIncome = mMonthlyIncome;
	cur->monthlyOutcome = mMonthlyOutcome;
	mMonthlyFeed=0;
	mMonthlyOutcome=0;
	mMonthlyIncome=0;
}
int BaseAccount::GetCredit(){
	return mCredit;
}
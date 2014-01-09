#pragma once
#include <vector>
#include "BaseConst.h"
namespace Logic{
	struct Monthly{
		float monthlyIncome;
		float monthlyOutcome;
		float monthlyFeed;
		float credit;
		int date;
	};

	class BaseAccount
	{
	   friend class boost::serialization::access;
	public:
		LOADSAVE_FUNCTION_GENERATE()  
		BaseAccount(void);
		~BaseAccount(void);
		static BaseAccount* GetSigleton();
		void ChangeCredit(float credit);
		void AddOutcomeToMonthlyFeed(float credit);
		void DoMonthlyCount(int months);
		int GetCredit();
		GameEvent* ReturnInfo();
	protected:
		float mMonthlyIncome;
		float mMonthlyOutcome;
		float mMonthlyFeed;
		float mCredit;
		static BaseAccount* sigleton;
		std::vector<Monthly> mAllMonthly;
	};
}


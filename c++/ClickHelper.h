#pragma once
#include "BaseConst.h";
namespace Control{
	struct ClickHelper{
	public:
		ClickHelper(Logic::ClickHelperType type);
		void* target;
		Logic::ClickHelperType mType;
	};

	class HelperManager{
	public:
		HelperManager();
		~HelperManager();
		std::set<ClickHelper*> mAllHelper;
		static HelperManager* GetSigleton();
	protected:
		static HelperManager* sigleton;
	
	};

}
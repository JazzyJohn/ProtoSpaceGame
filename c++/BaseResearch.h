#pragma once
#include "LogicStd.h"
#include "BaseResourceManager.h"
namespace Logic{
	
	class BaseResearch {
		 friend class BaseLogicLua;
	public:
		BaseResearch(void);
		BaseResearch(Ogre::String name,Ogre::String disname);
		~BaseResearch(void);
		bool mIsDone;
		bool addProgress(BaseResourceObject* nextObject);
		bool addProgress(std::map<BaseResource*,int> AddingProgress);
		bool addProgress(int progress);
		void GetMap(std::map<Ogre::String, int> &progress,std::map<Ogre::String, int> &doneProgress);
		Ogre::String GetName();
		bool IsDone();
		void whenDone(void);
		void CreateMap(std::map<BaseResource*,int> donePogress);
		int mLvlNum;
		Ogre::String mDepartment;
	protected:
		Ogre::String mDisplayName;
		std::map<BaseResource*,int> mPogress;
		std::map<BaseResource*,int> mDonePogress;
		Ogre::String mName; 
	};
	struct ShowResearchCenter: public GameEvent{
		ShowResearchCenter();
		
		Ogre::String curResearch;
		bool isNeedChoice;
		std::map<Ogre::String, int> progress;
		std::map<Ogre::String, int> doneProgress;
		std::map<Ogre::String, BaseResearch*> nextLvl;

	};
	
	class ResearchLvl{
	public:
		ResearchLvl(int lvlNum, BaseResearch **researchOnThisLvl,int reseacrhCnt);
		~ResearchLvl(void);
		BaseResearch **mResearchOnThisLvl;
		bool mIsDone;
		bool hasIt(BaseResearch *research);
		int mReseacrhCnt;
		int mLvlNum;
	
	};
	class ResearchDepartment{
	public:
		ResearchDepartment();
		void Init(std::vector<ResearchLvl*> allLvl);
		~ResearchDepartment();
		void StartResearch();
		bool hasIt(BaseResearch *research);
		void ShowResearch(std::map<Ogre::String, BaseResearch*> &map);
		bool mIsBlock;
		Ogre::String mDepartment;
		std::vector<ResearchLvl*> mAllDepLvl;
		static int mLvlNum;
		int mCurLvlNum;
	};
	class BaseReasearchManager{
	public:
		BaseReasearchManager(void);
		void Init(ResearchDepartment **allDep,int allDepNum);
		~BaseReasearchManager(void);
		ResearchDepartment **mAllDep;
		
		
		bool mIdle;
		bool mEvnt;
		int mAllDepNum;
		void WhenDone();
		GameEvent * ReturnResearchInfo();
		bool SetResearch(BaseResearch *curResearch);
		bool addProgress(BaseResourceObject* nextObject);
		bool addProgress(std::map<BaseResource*,int> AddingProgress);
		bool addProgress(int progress);
		void addResearchPoints(BaseResource *curResearch);
		bool isActive();
		std::map<Ogre::String, BaseResearch*> GetNextInMap();
		std::map<BaseResource*,int> mDepartmentPoint;
	protected:
		BaseResearch *mCurResearch;
		ResearchDepartment *mCurDepartment;
	};



}

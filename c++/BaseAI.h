#pragma once
#include "BaseConst.h"
#include "BaseCounrty.h"
namespace Logic{
	struct BaseAINode{
		Ogre::String mScriptName;
		std::map<Ogre::String ,BaseAINode*> NextNodes;

	};
class BaseAIManager
{
public:

	BaseAIManager(void);
	~BaseAIManager(void);
	void SetMap(std::map<Ogre::String,BaseAINode*> allAi);
	BaseAINode* GetAINode(Ogre::String);
	void SetPlanet(BasePlanet* Planet);
	void DoAIUpdate();
	static BaseAIManager* GetSigleton();
protected:
	static BaseAIManager* sigleton;
	std::map<Ogre::String,BaseAINode*> mAllAi;
	std::vector<BaseCountry *>::iterator mAiitr;
	BasePlanet* mPlanet;
};
};


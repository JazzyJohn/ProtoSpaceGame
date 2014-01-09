#pragma once
#include <OgreRoot.h>
namespace Logic{
class BaseAsset
{
public:
	BaseAsset(void);
	~BaseAsset(void);
	static BaseAsset* GetSigleton();
	static float GetSigleton(Ogre::String);
	void Add(Ogre::String,float);
	void Set(Ogre::String,float);
		float Get(Ogre::String);
	const float& operator[](Ogre::String idx) const; 
protected:
	std::unordered_map<Ogre::String,float> mAssetMap;
	static BaseAsset* sigleton;
};
}


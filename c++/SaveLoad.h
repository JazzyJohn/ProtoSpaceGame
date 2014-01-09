#pragma once
#include <OgreString.h>

namespace Logic{
class SaveLoad
{
public:
	SaveLoad(void);
	~SaveLoad(void);
	void Save(Ogre::String name);
};
}


#pragma once
#include <OgreRoot.h>
namespace Logic{
class LogicStd
{
public:
	LogicStd(void){};
	~LogicStd(void){};
	static void strToInt(Ogre::String* outbuffer,int value);
	static Ogre::String IntToString(int value);
	
};
}


#include "LogicStd.h"
using namespace Logic;


void LogicStd::strToInt(Ogre::String* outbuffer,int value)
{
	  std::stringstream ss;
  
    ss<<value;
    ss>>*outbuffer;
	
	} 
Ogre::String LogicStd::IntToString(int val)
{
    std::stringstream ss;
    ss << val;
 
    return ss.str();
}

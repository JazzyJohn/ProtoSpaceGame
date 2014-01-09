#pragma once
#include "basedraw.h"
namespace GUI{
class DrawAbility : public BaseDraw
{
public:
	DrawAbility(CEGUI::Window* mainWindow);
	~DrawAbility(void);
	static DrawAbility *GetSigleton();
	void DoDraw(void* target,std::map<Ogre::String,Logic::BaseAbility*> abilitys,CEGUI::UVector2 popup_pos,void* open);
	void DoDraw(void* target,std::map<Ogre::String,Logic::BaseAbility*> abilitys,void* open);
	bool Close(void* open);
	
	bool AbilityClick(const CEGUI::EventArgs &e);
	
protected:
	void *mOpenBy;
	void* mTarget;
	static DrawAbility *sigleton;
};
};


#pragma once
#include <CEGUI.h>
#include "BaseLogic.h"
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
namespace GUI{
class BaseDraw
{
public:
	BaseDraw(){};
	BaseDraw(CEGUI::Window* mainWindow);
	~BaseDraw(void);
	virtual bool Close(const CEGUI::EventArgs &e);
	 bool PopUp(const CEGUI::EventArgs &e);
	virtual void IsNeedRefresh(){};
	bool ClearUserData(const CEGUI::EventArgs &e);
	void ClearFromAllChild(CEGUI::Window *win);
	bool ClosePopUp();
protected:
	CEGUI::Window* mMainWindow;
	CEGUI::Window* mFrameWindow;
	CEGUI::Window* mCloseBtn;
	Ogre::String  mName;
	std::map<Ogre::String,Logic::BaseAbility*> mAbilitys;
	bool isPopMenu;
	bool isOpen;
	bool mIsResend;
	bool PriorIdDraw;

};
}


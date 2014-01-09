#include "BaseDraw.h"
#include "DrawAbility.h"
using namespace GUI;

BaseDraw::BaseDraw(CEGUI::Window* mainWindow)
{
	mMainWindow = mainWindow;
	mFrameWindow= 0;
		isOpen = false;
		mIsResend= true;
		
		isPopMenu= false;
}
bool BaseDraw::PopUp(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::UVector2 popup_pos;
	
        // get x position of mouse as UDim absolute position within the
        // window
        popup_pos.d_x = cegui_absdim(
            CEGUI::CoordConverter::screenToWindowX(*mFrameWindow, args.position.d_x));

        // get y position of mouse as UDim absolute position within the
        // window
        popup_pos.d_y = cegui_absdim(
            CEGUI::CoordConverter::screenToWindowY(*mFrameWindow, args.position.d_y));



	DrawAbility::GetSigleton()->DoDraw(args.window->getUserData(),mAbilitys,popup_pos,this);
		isPopMenu= true;
	return true;
}
bool BaseDraw::Close(const CEGUI::EventArgs &e){
		const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	if(mMainWindow->isChild(args.window->getParent()->getName())){

		if(args.window->getParent()==mFrameWindow){
			isOpen = false;
		}
		mMainWindow->removeChildWindow(args.window->getParent()->getName());
	}
	if(DrawAbility::GetSigleton()->Close(this)){
	ClosePopUp();
	}
	return true;
}

bool BaseDraw::ClosePopUp(){
	isPopMenu= false;
	return true;
}
BaseDraw::~BaseDraw(void)
{
}
bool BaseDraw::ClearUserData(const CEGUI::EventArgs &e){
	const CEGUI::WindowEventArgs & args = reinterpret_cast<const CEGUI::WindowEventArgs &>(e);
	delete args.window->getUserData();
	return true;
}
void  BaseDraw::ClearFromAllChild(CEGUI::Window *win){
	
	while(win->getChildCount()>0){
		win->removeChildWindow(win->getChildAtIdx(0)->getName());

	}

}

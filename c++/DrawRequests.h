#pragma once
#include "basedraw.h"
namespace GUI{
class DrawRequests :
	public BaseDraw
{
public:
	DrawRequests(CEGUI::Window* mainWindow);
	~DrawRequests(void);
	bool OpenResourseDialog(const CEGUI::EventArgs &e);
	bool ReqResMin(const CEGUI::EventArgs &e);
	bool ReqResPlus(const CEGUI::EventArgs &e);
	bool CloseResReqDialog(const CEGUI::EventArgs &e);
	bool SendResReqDialog(const CEGUI::EventArgs &e);
	bool DeclineProject(const CEGUI::EventArgs &e);
	bool AcceptProject(const CEGUI::EventArgs &e);
	bool OpenProjectDialog(const CEGUI::EventArgs &e);
	bool ProResMin(const CEGUI::EventArgs &e);
	bool ProResPlus(const CEGUI::EventArgs &e);
	bool CloseResProDialog(const CEGUI::EventArgs &e);
	bool SendResProDialog(const CEGUI::EventArgs &e);
	void DoDraw(Logic::ShowAllRequest *evnt);
	void DoDialogDraw(Logic::ShowSendStatus *resevnt);
	void DoDialogDraw(Logic::ShowProjectSendStatus *resevnt);
	void IsNeedRefresh();
protected: 
	CEGUI::Window* mDialog;
};
}


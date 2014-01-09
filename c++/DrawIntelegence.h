#pragma once
#include "DrawBuilding.h"
namespace GUI{
class DrawIntelegence :
	public DrawBuilding
{
public:
	DrawIntelegence(CEGUI::Window* mainWindow);
	~DrawIntelegence(void);
	bool OpenIntActionDialog(const CEGUI::EventArgs &e);
	bool SendIntActionDialog(const CEGUI::EventArgs &e);
	bool DeclineIntActionDialog(const CEGUI::EventArgs &e);
	bool SetTargetIntActionDialog(const CEGUI::EventArgs &e);
	bool SetReasonIntActionDialog(const CEGUI::EventArgs &e);
	bool AskForTrade(const CEGUI::EventArgs &e);
	void DoDraw(Logic::ShowIntelegence *evnt);
	void DoDrawDialog(Logic::ActionIntDialog *evnt );
	void CloseDialog();
	void IsNeedRefresh();
protected:
	CEGUI::Window *mDialog;
	
};
}


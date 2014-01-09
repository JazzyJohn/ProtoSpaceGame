#include "DrawRequests.h"
using namespace GUI;

DrawRequests::DrawRequests(CEGUI::Window* mainWindow):BaseDraw(mainWindow)
{
}


DrawRequests::~DrawRequests(void)
{
}

bool DrawRequests::AcceptProject(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
	sendEvnt->target =args.window->getUserData();
	sendEvnt->controlType = Logic::CTE_ACCEPTPROJECT;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawRequests::DeclineProject(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
	sendEvnt->target =args.window->getUserData();
	sendEvnt->controlType = Logic::CTE_DECLINEPROJECT;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawRequests::OpenProjectDialog(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
	sendEvnt->target =args.window->getUserData();
	sendEvnt->controlType = Logic::CTE_OPENPROJECTDIALOG;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}
bool DrawRequests::OpenResourseDialog(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SomethingSimple *sendEvnt = new Logic::SomethingSimple();
	sendEvnt->target =args.window->getUserData();
	sendEvnt->controlType = Logic::CTE_RESOURSEREDIALOG;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;
}

bool DrawRequests::ReqResMin(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SenResorceChng *sendEvnt = new Logic::SenResorceChng();
	sendEvnt->target = static_cast<Logic::BaseResourceObject*>(args.window->getUserData());
	sendEvnt->sign =-1;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;

}
bool DrawRequests::ReqResPlus(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SenResorceChng *sendEvnt = new Logic::SenResorceChng();
	sendEvnt->target = static_cast<Logic::BaseResourceObject*>(args.window->getUserData());
	sendEvnt->sign =1;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;

}
bool DrawRequests::CloseResReqDialog(const CEGUI::EventArgs &e){
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType =Logic::CTE_RESOURSEREDIALOGDECLINE;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	return true;
}
bool DrawRequests::SendResReqDialog(const CEGUI::EventArgs &e){
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType =Logic::CTE_RESOURSEREDIALOGSEND;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	return true;
}
bool DrawRequests::ProResMin(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SenResorceChng *sendEvnt = new Logic::SenResorceChng();
	sendEvnt->target = static_cast<Logic::BaseResourceObject*>(args.window->getUserData());
	sendEvnt->sign =-1;
	sendEvnt->controlType = Logic::CTE_RESOURCEPROJDIALOGCHANGE;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;

}
bool DrawRequests::ProResPlus(const CEGUI::EventArgs &e){
	const CEGUI::MouseEventArgs& args = reinterpret_cast<const CEGUI::MouseEventArgs&>(e);
	Logic::SenResorceChng *sendEvnt = new Logic::SenResorceChng();
	sendEvnt->target = static_cast<Logic::BaseResourceObject*>(args.window->getUserData());
	sendEvnt->sign =1;
	sendEvnt->controlType = Logic::CTE_RESOURCEPROJDIALOGCHANGE;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(sendEvnt);
	return true;

}
bool DrawRequests::CloseResProDialog(const CEGUI::EventArgs &e){
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType =Logic::CTE_RESOURSEPROJDIALOGDECLINE;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	return true;
}
bool DrawRequests::SendResProDialog(const CEGUI::EventArgs &e){
	Logic::ControlEvent *evnt = new Logic::ControlEvent();
	evnt->controlType =Logic::CTE_RESOURSEPROJDIALOGSEND;
	Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
	return true;
}
void DrawRequests::DoDraw(Logic::ShowAllRequest *evnt){
	if(!isOpen&&evnt->isReSend){
	return;
	}
	if(evnt->isReSend&& PriorIdDraw){
		return;
	}
	
	isOpen = true;
	mIsResend= false;
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window *res,*btn,*decbtn;

	Ogre::String name ="RequestSheet/";
	Ogre::String text;
	
	if(wmgr.isWindowPresent("RequestSheet")){
		if(!evnt->isReSend){
		ClearFromAllChild(mFrameWindow);
		
		mFrameWindow->addChildWindow(mCloseBtn); 
		}

	}else{
		mFrameWindow= wmgr.createWindow("TaharezLook/StaticImage",  "RequestSheet");
		mFrameWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
		mCloseBtn= wmgr.createWindow("TaharezLook/Button",  "RequestSheet/Close");
		mCloseBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		mCloseBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		mCloseBtn->setText("Close");
		mCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BaseDraw::Close,(BaseDraw*) this));
		mFrameWindow->addChildWindow(mCloseBtn); 

	}
	
	std::vector<Logic::ProjectForSend>::iterator projItr;

	int i=0;
	int iproj=0;
	for(projItr = evnt->allProject.begin();projItr != evnt->allProject.end();++projItr){
		name="RequestSheet/sujestprojects"+Logic::LogicStd::IntToString(iproj);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);

			if(!evnt->isReSend){
			mFrameWindow->addChildWindow(res);
			}
			name="RequestSheet/sujestprojects/accept"+Logic::LogicStd::IntToString(iproj);
			btn= wmgr.getWindow(name);
			name="RequestSheet/sujestprojects/decline"+Logic::LogicStd::IntToString(iproj);
			decbtn= wmgr.getWindow(name);
			if(!evnt->isReSend){
			mFrameWindow->addChildWindow(decbtn);
			mFrameWindow->addChildWindow(btn);
			}

		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.1*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.1, 0)));
			mFrameWindow->addChildWindow(res);
			name="RequestSheet/sujestprojects/decline"+Logic::LogicStd::IntToString(iproj);
			decbtn= wmgr.createWindow("TaharezLook/Button", name);
			decbtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0.1*(i+1), 0)));
			decbtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
			mFrameWindow->addChildWindow(decbtn);
			decbtn->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawRequests::DeclineProject, this));
			decbtn->setText("Decline");
			name="RequestSheet/sujestprojects/accept"+Logic::LogicStd::IntToString(iproj);
			btn= wmgr.createWindow("TaharezLook/Button", name);
			btn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.7,0), CEGUI::UDim(0.1*(i+1), 0)));
			btn->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
			mFrameWindow->addChildWindow(btn);
			btn->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawRequests::AcceptProject, this));
			btn->setText("Accept");
		}
		decbtn->setUserData(projItr->project);
		btn->setUserData(projItr->project);
		i++;
		iproj++;
		res->setText(projItr->dep+'\n'+projItr->desc+'\n');
	}	
	i++;
	iproj=0;
	for(projItr = evnt->allPendingProject.begin();projItr != evnt->allPendingProject.end();++projItr){
		name="RequestSheet/projects"+Logic::LogicStd::IntToString(iproj);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);

			if(!evnt->isReSend){
			mFrameWindow->addChildWindow(res);
			}
			name="RequestSheet/projects/sendresource"+Logic::LogicStd::IntToString(iproj);
			btn= wmgr.getWindow(name);
			if(!evnt->isReSend){
			mFrameWindow->addChildWindow(btn);
			}

		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.1*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.1, 0)));
			mFrameWindow->addChildWindow(res);
			name="RequestSheet/projects/sendresource"+Logic::LogicStd::IntToString(iproj);
			btn= wmgr.createWindow("TaharezLook/Button", name);
			btn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0.1*(i+1), 0)));
			btn->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
			mFrameWindow->addChildWindow(btn);
			btn->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawRequests::OpenProjectDialog, this));
			btn->setText("Send Resource");
		}
		
		btn->setUserData(projItr->project);
		i++;
		iproj++;
		res->setText(projItr->dep+'\n'+projItr->desc+'\n');
	}	



	std::vector<Logic::RequestForSend>::iterator reqItr;
	i++;
	int ireq=0;
	for(reqItr = evnt->allRequest.begin();reqItr != evnt->allRequest.end();++reqItr){
		name="RequestSheet/requests"+Logic::LogicStd::IntToString(ireq);

		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);

			if(!evnt->isReSend){
			mFrameWindow->addChildWindow(res);
			}
			name="RequestSheet/requestsbtn"+Logic::LogicStd::IntToString(ireq);
			btn= wmgr.getWindow(name);
			if(!evnt->isReSend){
			mFrameWindow->addChildWindow(btn);
			}

		}else{
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.1*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.1, 0)));
			mFrameWindow->addChildWindow(res);
			name="RequestSheet/requestsbtn"+Logic::LogicStd::IntToString(ireq);
			btn= wmgr.createWindow("TaharezLook/Button", name);
			btn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0.1*(i+1), 0)));
			btn->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
			mFrameWindow->addChildWindow(btn);
			btn->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawRequests::OpenResourseDialog, this));
			btn->setText("Make It SO");
		}
		btn->setUserData(reqItr->request);
		i++;
		ireq++;
		res->setText(reqItr->dep+'\n'+reqItr->desc+'\n'+reqItr->timeleft+" days left");
	}	
	if(!evnt->isReSend){
	mMainWindow->addChildWindow(mFrameWindow);
	PriorIdDraw= true;
	}else{
	PriorIdDraw= false;
	}

}
void DrawRequests::DoDialogDraw(Logic::ShowSendStatus *resevnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *res,*sendstatus;
	
	Ogre::String name;

	int x=0,y=0;
	name= "DetailedResReq";
	if(wmgr.isWindowPresent(name)){
		mDialog= wmgr.getWindow(name);
		ClearFromAllChild(mDialog);


	}else{
		mDialog= wmgr.createWindow("TaharezLook/StaticImage", name);
		mDialog->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
		mDialog->setAlwaysOnTop(true);
	}
	if(wmgr.isWindowPresent("DetailedResReq/CloseDialog")){

		res= wmgr.getWindow("DetailedResReq/CloseDialog");
		mDialog->addChildWindow(res);
		res= wmgr.getWindow("DetailedResReq/Send");
		mDialog->addChildWindow(res);
		sendstatus = wmgr.getWindow("DetailedResReq/sendstatus");
		mDialog->addChildWindow(sendstatus);
	}else{
		res= wmgr.createWindow("TaharezLook/Button",  "DetailedResReq/CloseDialog");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Close");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&DrawRequests::CloseResReqDialog, this));
		mDialog->addChildWindow(res); 
		res= wmgr.createWindow("TaharezLook/Button",  "DetailedResReq/Send");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Send");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&DrawRequests::SendResReqDialog, this));
		mDialog->addChildWindow(res); 
			sendstatus = wmgr.createWindow("TaharezLook/StaticText","DetailedResReq/sendstatus");
		sendstatus->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.1, 0)));
		sendstatus->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
		mDialog->addChildWindow(sendstatus);
	}
	sendstatus->setText(Logic::LogicStd::IntToString(resevnt->nowAmount)+"/"+Logic::LogicStd::IntToString(resevnt->needAmout));

	int i=1;
	std::vector<Logic::ResourseSend>::iterator resItr;
	for(resItr = resevnt->allResourse.begin(); resItr != resevnt->allResourse.end();++resItr){
		name= "DetailedResReq/NameAnd_Amount"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			mDialog->addChildWindow(res); 
		}else{
			res= wmgr.createWindow("TaharezLook/Titlebar", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.05, 0)));
			mDialog->addChildWindow(res);


		}
		res->setText(resItr->name);
		res->setTooltipText(resItr->object->GetTooltip());
		name= "DetailedResReq/Minus"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			mDialog->addChildWindow(res);
			res->setText("-");
			res->setUserData(resItr->object);
			name= "DetailedResReq/Amount"+Logic::LogicStd::IntToString(i);
			res= wmgr.getWindow(name);
			res->setText(Logic::LogicStd::IntToString(resItr->amount));
			mDialog->addChildWindow(res);
			name= "DetailedResReq/Plus"+Logic::LogicStd::IntToString(i);
			res= wmgr.getWindow(name);
			res->setText("+");
			res->setUserData(resItr->object);
			mDialog->addChildWindow(res);
		}
		else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
			res->setText("-");
			res->setUserData(resItr->object);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawRequests::ReqResMin, this));
			mDialog->addChildWindow(res);
			name= "DetailedResReq/Amount"+Logic::LogicStd::IntToString(i);
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
			res->setText(Logic::LogicStd::IntToString(resItr->amount));
			mDialog->addChildWindow(res);
			name= "DetailedResReq/Plus"+Logic::LogicStd::IntToString(i);
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
			res->setText("+");
			res->setUserData(resItr->object);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawRequests::ReqResPlus, this));
			mDialog->addChildWindow(res);
		}
		i++;
	}
	mMainWindow->addChildWindow(mDialog);


}
void DrawRequests::DoDialogDraw(Logic::ShowProjectSendStatus *resevnt){
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *res,*status,*periodcnt,*sendstatus;
	
	Ogre::String name;

	int x=0,y=0;
	name= "DetailedResProj";
	if(wmgr.isWindowPresent(name)){
		mDialog= wmgr.getWindow(name);
		ClearFromAllChild(mDialog);
		status= wmgr.getWindow(name+"/status");
		periodcnt = wmgr.getWindow(name+"/periodcnt");
		sendstatus = wmgr.getWindow(name+"/sendstatus");
		mDialog->addChildWindow(sendstatus);
		mDialog->addChildWindow(periodcnt);
		mDialog->addChildWindow(status);
	}else{
		mDialog= wmgr.createWindow("TaharezLook/StaticImage", name);
		mDialog->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
		mDialog->setAlwaysOnTop(true);
		status=wmgr.createWindow("TaharezLook/StaticText",name+"/status");
		status->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.0, 0)));
		status->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
		periodcnt = wmgr.createWindow("TaharezLook/StaticText",name+"/periodcnt");
		periodcnt->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.05, 0)));
		periodcnt->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
		sendstatus = wmgr.createWindow("TaharezLook/StaticText",name+"/sendstatus");
		sendstatus->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.1, 0)));
		sendstatus->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
		mDialog->addChildWindow(sendstatus);
		mDialog->addChildWindow(periodcnt);
		mDialog->addChildWindow(status);
	}
	if(resevnt->periodComplite){
	status->setText("Send");
	}else{
	status->setText("Not Send");
	}
	name = Logic::LogicStd::IntToString(resevnt->periodComplite)+"/"+Logic::LogicStd::IntToString(resevnt->periodCnt);
	switch(resevnt->periodtype){
	case Logic::PTP_WEEKLY:
		name+=" weeks";
		break;
	case Logic::PTP_MONTHLY:
		name+=" month";
		break;
	}
	periodcnt->setText(name);
	sendstatus->setText(Logic::LogicStd::IntToString(resevnt->nowAmount)+"/"+Logic::LogicStd::IntToString(resevnt->needAmout));
	if(wmgr.isWindowPresent("DetailedResProj/CloseDialog")){

		res= wmgr.getWindow("DetailedResProj/CloseDialog");
		mDialog->addChildWindow(res);
		res= wmgr.getWindow("DetailedResProj/Send");
		mDialog->addChildWindow(res);
	}else{
		res= wmgr.createWindow("TaharezLook/Button",  "DetailedResProj/CloseDialog");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Close");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&DrawRequests::CloseResProDialog, this));
		mDialog->addChildWindow(res); 
		res= wmgr.createWindow("TaharezLook/Button",  "DetailedResProj/Send");
		res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0, 0)));
		res->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
		res->setText("Send");
		res->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&DrawRequests::SendResProDialog, this));
		mDialog->addChildWindow(res); 
	}

	int i=0;
	std::vector<Logic::ResourseSend>::iterator resItr;
	for(resItr = resevnt->allResourse.begin(); resItr != resevnt->allResourse.end();++resItr){
		name= "DetailedResProj/NameAnd_Amount"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			mDialog->addChildWindow(res); 
		}else{
			res= wmgr.createWindow("TaharezLook/Titlebar", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.05, 0)));
			mDialog->addChildWindow(res);


		}
		res->setText(resItr->name);
		name= "DetailedResProj/Minus"+Logic::LogicStd::IntToString(i);
		if(wmgr.isWindowPresent(name)){
			res= wmgr.getWindow(name);
			mDialog->addChildWindow(res);
			res->setText("-");
			res->setUserData(resItr->object);
			name= "DetailedResProj/Amount"+Logic::LogicStd::IntToString(i);
			res= wmgr.getWindow(name);
			res->setText(Logic::LogicStd::IntToString(resItr->amount));
			mDialog->addChildWindow(res);
			name= "DetailedResProj/Plus"+Logic::LogicStd::IntToString(i);
			res= wmgr.getWindow(name);
			res->setText("+");
			res->setUserData(resItr->object);
			mDialog->addChildWindow(res);
		}
		else{
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
			res->setText("-");
			res->setUserData(resItr->object);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawRequests::ProResMin, this));
			mDialog->addChildWindow(res);
			name= "DetailedResProj/Amount"+Logic::LogicStd::IntToString(i);
			res= wmgr.createWindow("TaharezLook/StaticText", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
			res->setText(Logic::LogicStd::IntToString(resItr->amount));
			mDialog->addChildWindow(res);
			name= "DetailedResProj/Plus"+Logic::LogicStd::IntToString(i);
			res= wmgr.createWindow("TaharezLook/Button", name);
			res->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6,0), CEGUI::UDim(0.05*(i+1), 0)));
			res->setSize(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
			res->setText("+");
			res->setUserData(resItr->object);
			res->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&DrawRequests::ProResPlus, this));
			mDialog->addChildWindow(res);
		}
		i++;
	}
	mMainWindow->addChildWindow(mDialog);


}
void DrawRequests::IsNeedRefresh(){
	if(isOpen&&!mIsResend){
		Logic::ControlEvent *evnt = new Logic::ControlEvent();
		evnt->controlType =Logic::CTE_SHOWREQUESTS;
		evnt->isReSend = true;
			Logic::BaseLogic::GetSigleton()->fireGameEvent(evnt);
			mIsResend= true;
			PriorIdDraw= false;
	}
}
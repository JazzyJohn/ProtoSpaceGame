 
#include "TestGame.h" 
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <exception>
//-------------------------------------------------------------------------------------
TestGame::TestGame(void):mRoot(0),
	mWindow(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),

	mInputManager(0),
	mMouse(0),
	mGameLogic(),
	mYawCoefcient(0),
	mKeyboard(0)
{

}

//-------------------------------------------------------------------------------------
TestGame::~TestGame(void){
	mSceneMgr->destroyQuery(mRaySceneQuery);
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
	boost::mutex::scoped_lock lock(*mMutex);//create a lock
	mGameLogic->SetState(Logic::OVER);
	while(!mGameEventStack.empty()){
		delete mGameEventStack.front();
		mGameEventStack.pop();
	}
	lock.unlock();
	
	
}

//Adjust mouse clipping area
void TestGame::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void TestGame::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}
bool TestGame::frameStarted(const Ogre::FrameEvent& evt)
{ 
 
  // Make sure the CEGUI System is initialized and running
  // and if it is, inject a time pulse
  if ( CEGUI::System::getSingletonPtr() )
    CEGUI::System::getSingleton().injectTimePulse( evt.timeSinceLastFrame );
 
  return true;
} //frameStarted


bool TestGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{


	if(mWindow->isClosed())
		return false;

	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();




	boost::mutex::scoped_lock lock(*mMutex);//create a lock

	mGameLogic->getFormatedTime(&mGui.mDate,Logic::DF_NORMAL);
	lock.unlock();
	resolvedQueue();

	mGui.Rendering(evt.timeSinceLastFrame);
	std::set<Ogre::AnimationState*>::iterator animIter;
	std::queue<Ogre::AnimationState*> toDelete;
	for(animIter=mAllAnimation.begin();animIter!=mAllAnimation.end();++animIter){
		(*animIter)->addTime(evt.timeSinceLastFrame);
		if((*animIter)->hasEnded()){
			toDelete.push((*animIter));
		}
	
	
	}
	while(!toDelete.empty()){
		mAllAnimation.erase(toDelete.front());
		toDelete.pop();
	}
	
	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		mGui.ShowMenu();
	if(mExit){
		return false;
	}
	return true;
}
void TestGame::createFrameListener(void){

	Ogre::SceneNode* headNode = mSceneMgr->getSceneNode("HeadNode");

}
bool TestGame::go(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	sigleton = this;
	mRoot = new Ogre::Root(mPluginsCfg);

	// setup resources
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
	// configure
	// Show the configuration dialog and initialise the system
	// Do not add this to the application
	Ogre::RenderSystem *rs = mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
	// or use "OpenGL Rendering Subsystem"
	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	mWindow = mRoot->initialise(true, "WINDOW");

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	// initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamPoint_MAIN");
	// Position it at 500 in Z direction
	node->setPosition(Ogre::Vector3(0,0,80));
	node->lookAt(Ogre::Vector3(0,0,-300), Ogre::Node::TS_WORLD);
	
	// Look back along -Z
	
	mCamera->setNearClipDistance(5);
	//mCamera->setFarClipDistance(100);
	node->attachObject(mCamera);
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));


	
	

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);




	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);


	mRoot->addFrameListener(this);

	mGameLogic = new Logic::BaseLogic(this);
	SetMutex();	
	StartThread();  
	mGui.Init(mGameLogic);
	mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	mRoot->startRendering();

	return true;
}
bool  TestGame::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	 
	if(mGui.mousePressed(id))
		return true;
	 if (id == OIS::MB_Left)
            {
                // Setup the ray scene query, use CEGUI's mouse position
                CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
                Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/float(evt.state.width), mousePos.d_y/float(evt.state.height));
                mRaySceneQuery->setRay(mouseRay);
				 ObjectClick(mRaySceneQuery->execute(),OIS::MB_Left);
			}
	  if (id == OIS::MB_Right)
            {
				CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
                Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/float(evt.state.width), mousePos.d_y/float(evt.state.height));
                mRaySceneQuery->setRay(mouseRay);
				 ObjectClick(mRaySceneQuery->execute(),OIS::MB_Right);
			}
	return true;
}

bool  TestGame::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	if(mGui.mouseReleased(id)) return true;
	return true;
}

bool  TestGame::mouseMoved(const OIS::MouseEvent& evt)
{
	if (evt.state.buttonDown(OIS::MB_Right))
				{
					mCamera->getParentSceneNode()->yaw(Ogre::Degree( -0.13*evt.state.X.rel), Ogre::Node::TS_PARENT);
					mCamera->getParentSceneNode()->pitch(Ogre::Degree( -0.13*evt.state.Y.rel), Ogre::Node::TS_LOCAL);
				}
	
	if(mGui.mouseMoved(evt)) return true;
	float width =mWindow->getWidth();
	float x =evt.state.X.abs;
	float YawCoefcient=(width/2-x)/(width/2);
	mYawCoefcient =-YawCoefcient ;
	return true;
} 
bool TestGame::keyPressed( const OIS::KeyEvent& evt ){return true;}
bool TestGame::keyReleased( const OIS::KeyEvent& evt ){return true;}
void   TestGame::fireDateChange(Logic::TimeGameEvent *evnt ){



	switch(evnt->timeType){
	case Logic::DC_DAYS:

		break;
	case Logic::DC_MONTH:

		break;
	case Logic::DC_YEAR:

		break;

	}


}
void   TestGame::StartGameLoop(){
	try
  {
   mGameLogic->gameLoop(Logic::PT_NORMAL);
  }
  catch (std::exception& e)
  {
   e.what();
  }
	
}
void   TestGame::SetMutex(void){
	mMutex = new boost::mutex();
	mGameLogic->SetMutex(mMutex);
}
void TestGame::fireGameEvent(Logic::GameEvent *evnt){
	boost::mutex::scoped_lock lock(*mMutex);//create a lock
	mGameEventStack.push(evnt);
	lock.unlock();
}
void TestGame::resolvedQueue(){
	static int i;
	mGui.RefreshOpenWindow();
	Logic::GameEvent *curevent =0;
	while(!mGameEventStack.empty()){
		i+=1;
		boost::mutex::scoped_lock lock(*mMutex);//create a lock
		curevent =  mGameEventStack.front();
		mGameEventStack.pop();
		lock.unlock();
		switch(curevent->gameType){
		case Logic::ET_NOTIFY:
			mGui.AdNotify(curevent);
			break;
		case Logic::GUIET_SHOWRESOURSE:
			{
				Logic::ShowResourceEvnt *resevnt = static_cast<Logic::ShowResourceEvnt *>(curevent);
				mGui.DrawResource(resevnt);
			}
			break;
		case Logic::GUIET_BUILDINGDUILT:
			{
				Logic::BuildingBuiltEvnt *builevnt = static_cast<Logic::BuildingBuiltEvnt *>(curevent);
				if(builevnt->buildingType == Logic::TB_BASECORE){
					mGui.DrawBuilding(builevnt);
				}
			}
			break;
		case Logic::GUIET_SENDCORETOGUI:
			{
				Logic::SendCoreToGui *coreshowevnt = static_cast<Logic::SendCoreToGui *>(curevent);
				mGui.DrawBase(coreshowevnt);

				if(!coreshowevnt->isReSend){
				//Logic::BaseBaseCore *core = static_cast<Logic::BaseBaseCore *>(coreshowevnt->target);
				MoveCamTo(coreshowevnt->target->mSysName);
				}
			}
			break;
		case Logic::GUIET_SENDBLUETOGUI:
			{
				Logic::SendBlueToGui *coreshowevnt = static_cast<Logic::SendBlueToGui *>(curevent);
				mGui.DrawBlueprint(coreshowevnt);
			}
			break;
		case Logic::GUIET_PLANETFINISHGENERATION:
			{
				Logic::PlanetJustGenerateEvent *coreshowevnt = static_cast<Logic::PlanetJustGenerateEvent *>(curevent);
				mGui.DrawPlanet(coreshowevnt);
				DrawPlanetMap(coreshowevnt);
			}
			break;


		case Logic::GUIET_COUNTRYDATA:
			{
				Logic::SendCountry *Countreshowevnt = static_cast<Logic::SendCountry *>(curevent);
				mGui.DrawCountry(Countreshowevnt);
			}
			break;
		case Logic::GUIET_SHOWMINING:
			{
				Logic::ShowMining *mininshowevnt = static_cast<Logic::ShowMining *>(curevent);
				mGui.DrawMining(mininshowevnt);
			}
			break;
		case Logic::GUIET_SHOWPRODUCTION:
			{
				Logic::ShowProduction *mininshowevnt = static_cast<Logic::ShowProduction *>(curevent);
				mGui.DrawProduction(mininshowevnt);
			}
			break;
		case Logic::GUIET_SHOWWAREHOUSE:
			{
				Logic::ShowWarehouse *warshowevnt = static_cast<Logic::ShowWarehouse *>(curevent);
				mGui.DrawWarehouse(warshowevnt);
			}
			break;
		case Logic::GUIET_SHOWRESEARCHCENTER:
			{
				Logic::ShowResearchCenter *researchEvnt = static_cast<Logic::ShowResearchCenter *>(curevent);
				mGui.DrawResearchScreen(researchEvnt);
			}
			break;
		case Logic::GUIET_SHOWINTELEGANCE:
			{
				Logic::ShowIntelegence *intBilEvnt = static_cast<Logic::ShowIntelegence *>(curevent);
				if(!curevent->isReSend){
					mGui.CloseIntelegenceDialog();
				}
				mGui.DrawIntelegence(intBilEvnt);
			}
			break;
		case Logic::GUIET_SHOWACTINTDIALOG:
			{
				Logic::ActionIntDialog *intDialEvnt = static_cast<Logic::ActionIntDialog *>(curevent);
				mGui.DrawIntelegenceDialog(intDialEvnt);
			}
			break;
		case Logic::GUIET_CLOSEACTINTDIALOG:
			{
				mGui.CloseIntelegenceDialog();
			}
			break;
		case Logic::GUIET_SHOWALLRESOURSE:
			{
				Logic::ShowAllResourse *researchEvnt = static_cast<Logic::ShowAllResourse *>(curevent);
				mGui.DrawAllResourse(researchEvnt);
			}
			break;
		case Logic::GUIET_SHOWALLRESOURSERES:
			{
				Logic::ShowAllResourse *researchEvnt = static_cast<Logic::ShowAllResourse *>(curevent);
				mGui.DrawAllResourseResearch(researchEvnt);
			}
			break;
		case Logic::GUIET_SHOWMILITARYBASE:
			{
				Logic::ShowMilitary *researchEvnt = static_cast<Logic::ShowMilitary *>(curevent);
				mGui.DrawMilitaryBase(researchEvnt);
			}
			break;
		case Logic::GUIET_SHOWALLREQUEST:
			{
				Logic::ShowAllRequest *requestEvntAll = static_cast<Logic::ShowAllRequest *>(curevent);
				mGui.DrawAllRequest(requestEvntAll);
			}
			break;
		case Logic::GUIET_SHOWREQRESDIALOG:
			{
				Logic::ShowSendStatus *resForReqDialog= static_cast<Logic::ShowSendStatus *>(curevent);
				mGui.DrawAllResourse(resForReqDialog);
			}
			break;
		case Logic::GUIET_CLOSEREQRESDIALOG:
			mGui.ClearResourseDialog();
			break;
		case Logic::GUIET_SHOWPROJDIALOG:
			{
				Logic::ShowProjectSendStatus *resForReqDialog= static_cast<Logic::ShowProjectSendStatus *>(curevent);
				mGui.DrawAllResourse(resForReqDialog);
			}
			break;
		case Logic::GUIET_CLOSEPROJRESDIALOG:
				mGui.ClearResourseDialog();
			break;
		case Logic::GUIET_ADDNEWNEWS:
			{
				Logic::NewNews *newNews= static_cast<Logic::NewNews *>(curevent);
				mGui.AddNewNews(newNews);
			}
			break;
		case Logic::GUIET_SHOWUNITINBASE:
			{
				Logic::ShowUnitInBase *newNews= static_cast<Logic::ShowUnitInBase *>(curevent);
				mGui.DrawUnitInBase(newNews);
			}
			break;
		case Logic::GUIET_CLOSEUNITDIALOG:
			{
				mGui.ClearUnitDialog();
			}
			break;
		case Logic::GUIET_SHOWWARFARE:
			{
				Logic::ShowWarfare *evnt= static_cast<Logic::ShowWarfare *>(curevent);
				mGui.DrawBattlifield(evnt);
			}
			break;
		case Logic::GUIET_CLOSEWARFARE:
			{
				Logic::CloseWarfare *evnt= static_cast<Logic::CloseWarfare *>(curevent);
				mGui.CloseBattlifield(evnt);
			}
			break;
		case Logic::GUIET_SHOWRECON:
			{
				Logic::ReconStateEvnt *evnt= static_cast<Logic::ReconStateEvnt *>(curevent);
				mGui.DrawRecon(evnt);
			}
			break;
		case Logic::GUIET_COREBUILT:
			{
				Logic::CoreBuiltEvnt *evnt= static_cast<Logic::CoreBuiltEvnt *>(curevent);
				mGui.ClearDialog("CountrySheet");
				DrawNewCore(evnt);
			}
			break;
		case Logic::GUIET_RESETCAM:
			{
				MoveCamTo("MAIN");
				break;
			}
		}
		delete curevent;

	}
	
}
TestGame app;

void logicThread(){
	
	app.StartGameLoop();
}

void TestGame::StartThread(){
	boost::thread workerThread(logicThread);
	
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object


		try {

			app.go();
		} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif
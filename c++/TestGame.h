#pragma once
#include <OgreRoot.h>

#include <OgreWindowEventUtilities.h>
#include "BaseGUI.h"

#include "BaseLogic.h"
#include "ClickHelper.h"
class TestGame: public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener,  Logic::BaseListener
{
public:
	TestGame(void);
	virtual ~TestGame(void);
	bool go(void);
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool frameStarted(const Ogre::FrameEvent& evt);
	virtual void fireDateChange(Logic::TimeGameEvent *evnt );
	bool  mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool  mouseMoved(const OIS::MouseEvent& evt);
	bool  mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool keyPressed( const OIS::KeyEvent& evt );
    virtual bool keyReleased( const OIS::KeyEvent& evt );
	virtual void resolvedQueue();
	void createFrameListener(void);
	void StartGameLoop();
	void StartThread();
	void SetMutex(void);
	void fireGameEvent(Logic::GameEvent *evnt);
	void DrawPlanetMap(Logic::PlanetJustGenerateEvent *);
	void DrawNewCore(Logic::CoreBuiltEvnt *);
	void ObjectClick(Ogre::RaySceneQueryResult &result,int mouse);
private:
	void MoveCamTo(Ogre::String);
	std::set<Ogre::AnimationState*> mAllAnimation;
	Ogre::RaySceneQuery *mRaySceneQuery;    
	 Ogre::Root* mRoot;
	 Control::HelperManager mHelpMng;
     Ogre::String mPluginsCfg;
	 Ogre::String mResourcesCfg;
	 Ogre::RenderWindow *mWindow;
	 OIS::InputManager* mInputManager;
	 Ogre::SceneManager* mSceneMgr;
     Ogre::Camera* mCamera;
	 GUI::BaseGUI mGui;
     OIS::Mouse*    mMouse;
     OIS::Keyboard* mKeyboard;
	 Logic::BaseLogic* mGameLogic;
	 float mYawCoefcient;
	 boost::mutex *mMutex;
	
};


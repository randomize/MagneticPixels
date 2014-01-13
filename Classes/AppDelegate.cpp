#include "AppDelegate.h"
#include "MPix.h"
#include "GameStateManager.h"
#include "HistoryManager.h"
#include "Cmd.h"

using namespace MPix;

AppDelegate::AppDelegate()
{

   // Create base loop
   master = new EMBaseMasterLoop();
   //master->retain();

   // Early init (cocos not started yet)
   master->InitGame();

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto eglView = EGLView::getInstance();

    director->setOpenGLView(eglView);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    eglView->setDesignResolutionSize(640, 960, ResolutionPolicy::SHOW_ALL );
#else
    eglView->setDesignResolutionSize(640, 960, ResolutionPolicy::NO_BORDER );
#endif

   auto frameSize = eglView->getFrameSize();
   auto size = director->getWinSize();
	

#ifdef MPIX_DEVELOPERS_BUILD
    director->setDisplayStats(true);
#else
   director->setDisplayStats(false);
#endif
    
   // TODO: setup based on platform and resolution
   vector<string> v;
   v.push_back("Resources");
   FileUtils::getInstance()->setSearchPaths(v);

    director->setAnimationInterval(1.0 / 60);

   CCLOG(" ");
   EM_LOG_INFO("==== Magnetic pixels started =====");
   CCLOG(" ");

   // Always switch to start
   master->RunGame();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
   EM_LOG_INFO("MPix did enter BG");
    Director::getInstance()->stopAnimation();
   CmdDidEnterBG cmd; cmd.Execute();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
   EM_LOG_INFO("MPix will enter FG");
    Director::getInstance()->startAnimation();
   CmdWillEnterFG cmd; cmd.Execute();
}

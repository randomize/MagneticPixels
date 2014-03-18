#include "AppDelegate.h"
#include "MPix.h"
#include "Cmd.h"

using namespace MPix;

const int DESIGN_WIDTH  = 640;
const int DESIGN_HEIGHT = 960;

AppDelegate::AppDelegate()
{

   // Create base loop
   master = new EMBaseMasterLoop();

   // Early init (cocos not started yet)
   master->InitGame();

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {

   // initialize director
   auto director = Director::getInstance();
   auto glview = director->getOpenGLView();
   if(!glview) {
       glview = GLView::create("Cpp Tests");
       director->setOpenGLView(glview);
   }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
   glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::SHOW_ALL);
#else
   glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::NO_BORDER );
#endif

   auto frameSize = glview->getFrameSize();
   auto size = director->getWinSize();
   EM_LOG_DEBUG("Screen metrics: ");
   EM_LOG_DEBUG(" frameSize: " + frameSize.width + ", " + frameSize.height );
   EM_LOG_DEBUG("   winSize: " + size.width + ", " + size.height );

#ifdef MPIX_DEVELOPERS_BUILD
   director->setDisplayStats(true);
#else
   director->setDisplayStats(false);
#endif

   bool cocos_fits_width = frameSize.width / frameSize.height > DESIGN_WIDTH / DESIGN_HEIGHT;
   float screen_cell_size;

   // In NO_BORDER mode cocos fits width or height depending on ratio;
   if (cocos_fits_width)
   {
      EM_LOG_DEBUG("Cocos fits by width");
      screen_cell_size = frameSize.width / 8;
   }
   else
   {
      EM_LOG_DEBUG("Cocos fits by height");
      screen_cell_size = frameSize.height / 12;
   }

   EM_LOG_DEBUG("Required cell size: " + screen_cell_size);

   list<std::pair<int, string>> resolutions;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   resolutions = { 
      { 40,  "iOS/40" },
      { 80,  "iOS/80" },
      { 85,  "iOS/85" },
      { 171, "iOS/171" }
   };
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   resolutions = { 
      { 40,  "Android/40" },
      { 60,  "Android/60" },
      { 135, "Android/135" },
      { 180, "Android/180" }
   };
#else // On other platforms just use all
   resolutions = { 
      { 40,  "Android/40" },
      { 60,  "Android/60" },
      { 80,  "iOS/80" },
      { 85,  "iOS/85" },
      { 135, "Android/135" },
      { 171, "iOS/171" },
      { 180, "Android/180" }
   };
#endif

   assert(resolutions.empty() == false);

   // Setup resources directory
   vector<string> v;

   for (auto & r : resolutions) {
      if (r.first >= screen_cell_size )  {
         // select first that is greater or eq
         director->setContentScaleFactor(r.first / 80.0);
         EM_LOG_DEBUG("Selected cell size: " + r.second);
         v.push_back(r.second);
         break;
      }
   }

   if (v.empty()) { // If not found optimal resolution - use maximum
      v.push_back(resolutions.back().second);
      EM_LOG_DEBUG("Selected ma][ cell size ");
   }

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

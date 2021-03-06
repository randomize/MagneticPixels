#include "SplashState.h"
#include "ContentManager.h"
#include "GameStateManager.h"

using namespace MPix;

//====---------------------------------------------======//


MPix::SplashState::SplashState()
{

}

MPix::SplashState::~SplashState()
{

}

bool MPix::SplashState::init()
{

   // 1. super init first
   if ( !Scene::init() )
   {
      return false;
   }

   auto s = Director::getInstance()->getWinSize();
   Vector2 center(s.width / 2, s.height / 2);

   auto lb = Label::createWithTTF("Loading...", ContentManager::getInstance().GetBaseFontLight(), 72.0f);
   lb->setColor(Color3B::GRAY);
   lb->setPosition(center);
   lb->setOpacity(0);
   lb->runAction(FadeIn::create(0.2f));

   addChild(lb);

   return true;
}

void MPix::SplashState::onEnter()
{
   Scene::onEnter();

   this->runAction(
      Sequence::createWithTwoActions(
         DelayTime::create(0.2f),
         CallFunc::create(
            []() {
               // Load resources
               ContentManager::getInstance().LoadResources();

               // Swithc
               GameStateManager::getInstance().SwitchToMenu();
            }
         )
      )
   );


}

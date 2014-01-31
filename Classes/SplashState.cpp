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
   Point center(s.width / 2, s.height / 2);

   auto lb = LabelTTF::create("Loading...", ContentManager::getInstance().GetBaseFontLight(), 72.0f);
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
         DelayTime::create(0.3f),
         CallFunc::create(
            []() {
               GameStateManager::getInstance().SwitchToMenu();
            }
         )
      )
   );
}

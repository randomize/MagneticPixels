#include "CreditsState.h"
#include "ContentManager.h"
#include "GameStateManager.h"

using namespace MPix;

//====---------------------------------------------======//







MPix::CreditsState::CreditsState()
{

}

MPix::CreditsState::~CreditsState()
{

}

bool MPix::CreditsState::init()
{

   if ( !Scene::init() )
   {
      return false;
   }

   auto s = Director::getInstance()->getWinSize();
   Vector2 center(s.width / 2, s.height / 2);

   auto lb = LabelTTF::create("TODO: scrolling credits\n will be here", ContentManager::getInstance().GetLogoFont(), 48.0f);
   lb->setColor(Color3B::GRAY);
   lb->setPosition(center);
   lb->setOpacity(0);
   lb->runAction(FadeIn::create(0.2f));

   addChild(lb);

   return true;
}

void MPix::CreditsState::onEnter()
{
   Scene::onEnter();

   runAction(
      Sequence::createWithTwoActions(
         DelayTime::create(2.0f),
         CallFunc::create(
            [](){ GameStateManager::getInstance().SwitchToMenu(); }
         )
      )
   );

}

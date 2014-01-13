#include "ResultsState.h"
#include "LevelManager.h"
#include "GameplayManager.h"
#include "GameStateManager.h"
#include "LevelStorage.h"

using namespace MPix;

//====---------------------------------------------======//

MPix::ResultsState::ResultsState()
{
}

MPix::ResultsState::~ResultsState()
{
}

bool MPix::ResultsState::init()
{

   // 1. super init first
   if ( !Scene::init() )
   {
      return false;
   }

   return true;

}

void MPix::ResultsState::onEnter()
{

   Scene::onEnter();

   MenuItemFont::setFontSize(64);
   auto menu = Menu::create();

   MenuItemFont* item = nullptr;

   item = MenuItemFont::create("Replay", [&](Object *sender) {
      ToReplay();
   }); menu->addChild(item);

   item = MenuItemFont::create("Next", [&](Object *sender) {
      ToNext();
   }); menu->addChild(item);

#ifdef MPIX_DEVELOPERS_BUILD
   item = MenuItemFont::create("Crash", [&](Object *sender) {
      assert(false);
   }); menu->addChild(item); item->setColor(Color3B::RED);
#endif

   menu->alignItemsVertically();

   auto s = Director::getInstance()->getWinSize();

   menu->setPosition(Point(s.width/2, s.height/2));
   addChild(menu);

}

void MPix::ResultsState::onExit()
{
   Scene::onExit();

}

void MPix::ResultsState::ToReplay()
{
   GameStateManager::getInstance().SwitchToGame();
}

void MPix::ResultsState::ToNext()
{
   GameStateManager::getInstance().SwitchToSelector();
}

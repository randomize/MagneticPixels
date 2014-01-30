#include "ResultsState.h"
#include "LevelManager.h"
#include "GameplayManager.h"
#include "GameStateManager.h"
#include "LevelStorage.h"
#include "ContentManager.h"

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
   
   auto bg = LayerColor::create(Color4B::MAGENTA);
   addChild(bg, 0);

   MenuItemFont::setFontSize(64);
   auto menu = Menu::create();

   MenuItemFont* item = nullptr;

   item = MenuItemFont::create("Replay", [&](Object *sender) {
      ToReplay();
   }); menu->addChild(item);

   item = MenuItemFont::create("Next", [&](Object *sender) {
      ToNext();
   }); menu->addChild(item);

   menu->alignItemsVertically();

   auto s = Director::getInstance()->getWinSize();
   Point center(s.width / 2, s.height / 2);

   menu->setPosition(center);
   addChild(menu, 2);


   auto& cnt = ContentManager::getInstance();

   auto n = cnt.GetNode("screen_for_blur");
   assert(n);
   n->setOpacity(200);
   n->setPosition(center);
   // TODO: Add blur
   addChild(n, 1); 
   cnt.RemoveNode("screen_for_blur");

   return true;

}

void MPix::ResultsState::ToReplay()
{
   GameplayManager::getInstance().Reset();

   auto lvl = LevelManager::getInstance().GetPlayableLastLevel();
   GameplayManager::getInstance().LoadLevel(lvl);

   GameStateManager::getInstance().SwitchToGame();
}

void MPix::ResultsState::ToNext()
{
   GameplayManager::getInstance().Reset();

   auto lvl = LevelManager::getInstance().GetPlayableNextLevel();
   GameplayManager::getInstance().LoadLevel(lvl);

   GameStateManager::getInstance().SwitchToGame();
}

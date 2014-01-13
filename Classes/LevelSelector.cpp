#include "LevelSelector.h"


#include "GameStateManager.h"
#include "GameplayManager.h"
#include "LevelManager.h"
#include "World.h"
#include "Level.h"

using namespace MPix;

//====---------------------------------------------======//


MPix::LevelSelector::LevelSelector()
{
   level_m = nullptr;
   world_m = nullptr;
}

MPix::LevelSelector::~LevelSelector()
{

}

bool MPix::LevelSelector::init()
{

   // 1. super init first
   if ( !Scene::init() )
   {
      return false;
   }

   auto & lm = LevelManager::getInstance();

   // Create worlds menu
   MenuItemFont::setFontSize(64);
   auto ids = lm.GetWorldsIDs();
   auto menu = Menu::create();
   for (auto id : ids ) {
      auto w = lm.GetWorldByID(id);
      auto item = MenuItemFont::create(
         (w->GetName() + " (" + ToString(w->GetLevelCount()) + ")").c_str(),
         [&](Object *sender) {
         auto it = static_cast<MenuItemFont*>(sender);
         SelectedWorld( it->getTag() );
         }
      );
      item->setTag(id);
      menu->addChild(item);
   }
   // Add default back button
   auto back = MenuItemFont::create(
      LocalUTF8Char("Back"),
      [&](Object *sender) {
         BackToMainMenu();
      }
   ); menu->addChild(back);
   menu->alignItemsVertically();

   auto s = Director::getInstance()->getWinSize();
   menu->setPosition(Point(s.width/2, s.height/2));
   addChild(menu);
   world_m = menu;
   return true;

}

void MPix::LevelSelector::onEnter()
{
   Scene::onEnter();

}

void MPix::LevelSelector::onExit()
{
   Scene::onExit();

}

void MPix::LevelSelector::SelectedLevel( unsigned int id )
{
   auto lvl = LevelManager::getInstance().GetLevelByID(id);
   GameplayManager::getInstance().LoadLevel(lvl);

   // And play it
   GameStateManager::getInstance().SwitchToGame();

}

void MPix::LevelSelector::SelectedWorld( int wid )
{
   world_m->setEnabled(false);
   world_m->runAction( FadeOut::create(0.4f) );
   // Create levels menu
   auto & lm = LevelManager::getInstance();

   MenuItemFont::setFontSize(48);

   auto ids = lm.GetLevelsInWorld(wid);
   auto menu = Menu::create();

   for (auto id : ids ) {
      string n = lm.GetLevelNameByID(id) + " (id=" + ToString(id) + ")";
      auto item = MenuItemFont::create(
         n.c_str(),
         [&](Object *sender) {
         auto it = static_cast<MenuItemFont*>(sender);
         SelectedLevel( (unsigned) it->getTag() );
         }
      );
      item->setTag((int)id);
      item->setColor(Color3B::GREEN);
      menu->addChild(item);
   }
   // Add default back button
   auto back = MenuItemFont::create(
      LocalUTF8Char("Back"),
      [&](Object *sender) {
         BackToWorlds();
      }
   ); menu->addChild(back);
   menu->alignItemsVertically();

   auto s = Director::getInstance()->getWinSize();
   menu->setPosition(Point(s.width/2, s.height/2));

   addChild(menu);
   level_m = menu;

}

void MPix::LevelSelector::BackToWorlds()
{
   world_m->setEnabled(true);
   world_m->runAction( FadeIn::create(0.4f) );

   level_m->setEnabled(false);
   level_m->runAction( 
    Sequence::create(
      FadeOut::create(0.4f),
      RemoveSelf::create(),
      nullptr
    )
   );

}

void MPix::LevelSelector::BackToMainMenu()
{
   GameStateManager::getInstance().SwitchToMenu();
}

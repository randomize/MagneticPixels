#include "EditorMenu.h"

#include "MPix.h"

#include "LevelManager.h"
#include "GameStateManager.h"
#include "LevelStorage.h"
#include "Level.h"
//#include "World.h"

using namespace MPix;

//====---------------------------------------------======//


MPix::EditorMenu::EditorMenu()
{
   level_m = nullptr;
   world_m = nullptr;
   edit_m = nullptr;
   confirmed = 0;
   mode = Mode::IDLE;
}

MPix::EditorMenu::~EditorMenu()
{

}

bool MPix::EditorMenu::init()
{

   // 1. super init first
   if ( !Scene::init() )
   {
      return false;
   }

   // 2. Build BG
   auto g = LayerColor::create(Color4B::BLACK);
   addChild(g, 1);

   // 3. Build main menu
   BuildEditorsMenu();

   return true;

}

void MPix::EditorMenu::onEnter()
{
   Scene::onEnter();
}

void MPix::EditorMenu::onExit()
{
   Scene::onExit();
}


void MPix::EditorMenu::BuildWorldsMenu()
{

   // Create worlds menu
   auto &lm = LevelManager::getInstance();
   MenuItemFont::setFontSize(64);

   auto &ids = lm.GetWorldIDs();
   auto menu = Menu::create();
   for (auto id : ids ) {
      auto item = MenuItemFont::create(
         (lm.GetNameByWorldID(id) + " (" + ToString(lm.GetLevelCountByWorldID(id)) + ")").c_str(),
         [&](Ref *sender) {
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
      [&](Ref *sender) {
         BackToEditorMenu();
      }
   ); menu->addChild(back);
   menu->alignItemsVertically();

   auto s = Director::getInstance()->getWinSize();
   menu->setPosition(Vector2(s.width/2, s.height/2));
   addChild(menu, 3);
   world_m = menu;

}

void MPix::EditorMenu::BuildLevelsMenu( int w )
{
   // Create levels menu
   auto &lm = LevelManager::getInstance();
   confirmed = 0;
   MenuItemFont::setFontSize(48);

   auto ids = lm.GetLevelsByWorldID(w);
   auto menu = Menu::create();

   for (auto id : ids ) {
      string n = lm.GetNameByLevelID(id) + " (id=" + ToString(id) + ")";
      auto item = MenuItemFont::create(
         n.c_str(),
         [&](Ref *sender) {
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
      [&](Ref *sender) {
         BackToWorlds();
      }
   ); menu->addChild(back);
   menu->alignItemsVertically();

   auto s = Director::getInstance()->getWinSize();
   menu->setPosition(Vector2(s.width/2, s.height/2));

   addChild(menu, 4);
   level_m = menu;

}

void MPix::EditorMenu::BuildEditorsMenu()
{
   MenuItemFont::setFontSize(64);
   auto menu = Menu::create();

   MenuItemFont* item = nullptr;

   // New
   item = MenuItemFont::create(LocalUTF8Char("New"), [&](Ref *sender) {
      SelectedNewLevel();
   }); menu->addChild(item);

   // Last
   item = MenuItemFont::create(LocalUTF8Char("Last"), [&](Ref *sender) {
      SelectedLast();
   }); menu->addChild(item);

   auto ss = LocalUTF8Str("Export") + "\n" + LevelStorage::getInstance()->GetExportDir();
   item = MenuItemFont::create(ss.c_str(), [&](Ref *sender) {
      SelectedExportMap();
   }); menu->addChild(item); item->setScale(0.5f); item->setColor(Color3B::GREEN);

   item = MenuItemFont::create(LocalUTF8Char("Edit"), [&](Ref *sender) {
      SelectedEdit();
   }); menu->addChild(item);

   item = MenuItemFont::create(LocalUTF8Char("Delete"), [&](Ref *sender) {
      SelectedDelete();
   }); menu->addChild(item); item->setColor(Color3B::RED);

   item = MenuItemFont::create(LocalUTF8Char("Back"), [&](Ref *sender) {
      BackToMainMenu();
   }); menu->addChild(item);

   menu->alignItemsVertically();

   auto s = Director::getInstance()->getWinSize();
   menu->setPosition(Vector2(s.width/2, s.height/2));
   addChild(menu, 2);
   edit_m = menu;

}

//////////////////////////////////////////////////////////////////////////
// Backs

void MPix::EditorMenu::BackToWorlds()
{
   removeChild(level_m);
   BuildWorldsMenu();
}

void MPix::EditorMenu::BackToEditorMenu()
{
   removeChild(world_m);
   mode = Mode::IDLE;
   confirmed = 0;
   BuildEditorsMenu();
}

void MPix::EditorMenu::BackToMainMenu()
{
   GameStateManager::getInstance().SwitchToMenu();
}

//////////////////////////////////////////////////////////////////////////
// Action

// Editor menu

void MPix::EditorMenu::SelectedNewLevel()
{
   removeChild(edit_m);
   mode = Mode::NEWFAB;
   BuildWorldsMenu();
}

void MPix::EditorMenu::SelectedLast()
{
   auto & lm = LevelManager::getInstance();

   auto lvl = lm.GetPlayableLastLevel();
   if (!lvl)
      lvl = lm.GetEmpty();

   lm.SetEditorsLevel(lvl);

   GameStateManager::getInstance().SwitchToEditor();
}

void MPix::EditorMenu::SelectedEdit()
{
   removeChild(edit_m);
   mode = Mode::EDIT;
   BuildWorldsMenu();
}

void MPix::EditorMenu::SelectedDelete()
{
   removeChild(edit_m);
   mode = Mode::DELETE;
   BuildWorldsMenu();
}

void MPix::EditorMenu::SelectedExportMap()
{
   LevelStorage::getInstance()->ExportMap();
}


// Edit/Delete menu

void MPix::EditorMenu::SelectedWorld( int id )
{
   if (mode == Mode::NEWFAB) {
      auto & lm = LevelManager::getInstance();
      auto lvl = lm.GetEmpty();
      lvl->SetWorld(id);
      lm.SetEditorsLevel(lvl);
      GameStateManager::getInstance().SwitchToEditor();
   } else if (mode == Mode::EDIT || mode == Mode::DELETE ) {
      removeChild(world_m);
      BuildLevelsMenu(id);
   } else {
      ECLOG_ERROR("Wrong state");
   }
}

// Edit/Delete level

void MPix::EditorMenu::SelectedLevel( unsigned int id )
{
   assert(level_m);
   auto & lm = LevelManager::getInstance();

   // Depending on mode

   if (mode == Mode::EDIT) {
      auto lvl = lm.GetPlayableLevelByID(id);
      assert(lvl);
      lm.SetEditorsLevel(lvl);
      GameStateManager::getInstance().SwitchToEditor();
   }
   else if (mode == Mode::DELETE)
   {
      if (confirmed == id) {
         lm.DeleteLevelByID(id);
         auto it = dynamic_cast<MenuItemFont*>(level_m->getChildByTag((int)confirmed));
         assert(it);
         it->setEnabled(false);
         confirmed = 0;
      }
      else
      {
         if (confirmed) {
            auto it = dynamic_cast<MenuItemFont*>(level_m->getChildByTag((int)confirmed));
            assert(it);
            it->setScale(1.0f);
            it->setColor(Color3B::GREEN);
         }
         confirmed = id;
         auto it = dynamic_cast<MenuItemFont*>(level_m->getChildByTag((int)confirmed));
         assert(it);
         it->setScale(1.1f);
         it->setColor(Color3B::RED);
      }
   }
   else
   {
      ECLOG_ERROR("Wrong state");
   }

}


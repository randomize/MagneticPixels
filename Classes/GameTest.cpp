#include "GameTest.h"
#include "Cmd.h"

#include "TouchLayer.h"
#include "PixelsLayer.h"

#include "GameplayManager.h"
#include "GameStateManager.h"
#include "LevelManager.h"

using namespace MPix;
using namespace std::placeholders;


bool GameTest::init()
{
   return GameMain::init();
}


void MPix::GameTest::BtnHnadler(Ref* sender)
{
   auto id = (dynamic_cast<Node*>(sender))->getTag();

   switch(id) {
   case 101: {
      GameplayManager::getInstance().Reset();
      auto lvl = LevelManager::getInstance().GetPlayableLastLevel();
      GameplayManager::getInstance().LoadLevel(lvl);
      pixels->Reset();
      ResetLocks();
      GameplayManager::getInstance().Play();
      return;
   }
   case 102: {
      GameplayManager::getInstance().PostCommand(new CmdGameplayUndoMove);
      return;

   }
   case 103: {
      GameplayManager::getInstance().PostCommand(new CmdGameplayRestartAssembly);
      return;
   }
   case 104: {
      GameplayManager::getInstance().Reset();
      GameStateManager::getInstance().SwitchToEditor();
      return;
   }
   case 105: {
      GameplayManager::getInstance().Reset();
      GameStateManager::getInstance().SwitchToMenu();
      return;
   }

   }


}

EmbossLib::ErrorCode MPix::GameTest::FinishedGame()
{
   // Switch back to editor
   GameStateManager::getInstance().SwitchToEditor();
   return ErrorCode::RET_OK;
}

void MPix::GameTest::CreateButtons()
{
   Vector2 origin = Director::getInstance()->getVisibleOrigin();
   Size visibleSize = Director::getInstance()->getVisibleSize();
   // Create menu
   auto menu = Menu::create();
   menu->setPosition(Vector2::ZERO);
   int base_tag = 101;
   const char* names[] = {
      "replay.png", // 101
      "undo.png",   // 102
      "shake.png",  // 103
      "save.png",  // 104
      "menu.png"    // 105
   };
   for (unsigned i = 0; i < sizeof(names)/sizeof(names[0]); ++i) {
      auto btn = MenuItemImage::create( names[i], names[i], CC_CALLBACK_1(GameTest::BtnHnadler, this));
      btn->setTag(base_tag + i);
      btn->setPosition(Vector2(
         origin.x + visibleSize.width - btn->getContentSize().width/2 * (2*i+1) ,
         origin.y + btn->getContentSize().height/2)
         );
      menu->addChild(btn);
   }
   this->addChild(menu, 3);

}



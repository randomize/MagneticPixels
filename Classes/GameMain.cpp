#include "GameMain.h"
#include "Cmd.h"

#include "TouchLayer.h"
#include "PixelsLayer.h"

#include "GameplayManager.h"
#include "GameStateManager.h"
#include "LevelManager.h"

using namespace MPix;
using namespace std::placeholders;

// Constants
int const MPix::GameMain::GAMEPLAY_TICK_RATIO = 6;

// UI Z orders
int const MPix::GameMain::Z_PIXEL_MIMICS = 5;
int const MPix::GameMain::Z_PIXEL_BG = 4;
int const MPix::GameMain::Z_WALLS = 10;

MPix::GameMain::GameMain()
{
   CmdUIGameFinished::listners["GameMain"] = std::bind( &GameMain::FinishGame, this );
}

MPix::GameMain::~GameMain()
{
   CmdUIGameFinished::listners.erase("GameMain");
}

bool GameMain::init()
{
   EM_LOG_ERROR("init");

   // 1. super init first
   if ( !Scene::init() )
   {
      return false;
   }

   // Check that game play is ready
   assert(GameplayManager::getInstance().GetState() == GameplayManager::State::READY);

   // Take some metrics
   Size fullSize = Director::getInstance()->getWinSize();
   Size halfSize =  fullSize / 2.0f;
   Size visibleSize = Director::getInstance()->getVisibleSize();
   Point origin = Director::getInstance()->getVisibleOrigin();

   float contentScale = (visibleSize.height/fullSize.height * visibleSize.width/fullSize.width);

   // Create scene:
   // bg = 0
   //  bg1
   // content = 1
   //   pixels = 1
   //   touch = 2
   
   auto bg1 = Sprite::create("bg/02.jpg");
   bg1->setScale(visibleSize.height / bg1->getContentSize().height);
   bg1->setPosition(halfSize.width, halfSize.height);

   bg = Layer::create();
   bg->addChild(bg1, 1);
   addChild(bg, 0);

   pixels = PixelsLayer::create();
   assert(pixels);

   touch = TouchLayer::create();
   assert(touch);

   // Content
   content = Node::create();
   content->addChild(touch, 2);
   content->addChild(pixels, 1);
   content->setScale(contentScale);
   content->setPosition(halfSize.width, halfSize.height);
   addChild(content, 1);

   // Create buttons
   CreateButtons();

   return true;
}


void GameMain::onEnter()
{
   Scene::onEnter();


   // Run GameplayManager
   GameplayManager::getInstance().Play();
}

void MPix::GameMain::CreateButtons()
{
   Point origin = Director::getInstance()->getVisibleOrigin();
   Size visibleSize = Director::getInstance()->getVisibleSize();
   // Create menu
   auto menu = Menu::create();
   menu->setPosition(Point::ZERO);
   int base_tag = 101;
   const char* names[] = {
      "replay.png", // 101
      "undo.png",   // 102
      "shake.png",  // 103
      "menu.png"    // 104
   };
   for (int i = 0; i < sizeof(names)/sizeof(names[0]); ++i) {
      auto btn = MenuItemImage::create( names[i], names[i], CC_CALLBACK_1(GameMain::BtnHnadler, this));
      btn->setTag(base_tag + i);
      btn->setPosition(Point(
         origin.x + visibleSize.width - btn->getContentSize().width/2 * (2*i+1) ,
         origin.y + btn->getContentSize().height/2)
         );
      menu->addChild(btn);
   }
   this->addChild(menu, 3);
}

void MPix::GameMain::BtnHnadler(Object* sender)
{
   auto id = (dynamic_cast<Node*>(sender))->getTag();

   switch(id) {
   case 101: {
      GameplayManager::getInstance().Reset();
      auto lvl = LevelManager::getInstance().GetLastLevel();
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
      GameStateManager::getInstance().SwitchToMenu();
      return;
   }

   }

}

ErrorCode GameMain::Tick( float t )
{

   static int cnt = 0;

   GameState::Tick(t);

   cnt++;
   // Frame skipper
   if (cnt > GAMEPLAY_TICK_RATIO) {

      cnt = 0;

      // Keep calling GameplayManager command until UI gets busy or commands end
      ErrorCode ret = ErrorCode::RET_OK;
      while ( !IsBusy() && ret == ErrorCode::RET_OK) {
         ret = GameplayManager::getInstance().ProcessOneCommand();
      }

   }

   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::GameMain::FinishedGame()
{
   // TODO:
   // Get a snapshot render texture and filter it(blur or someth)
   // Store to content manager
   // SwithcTo final state
   GameStateManager::getInstance().SwitchToResults();
   return ErrorCode::RET_OK;

}

void GameMain::onExit()
{
   Scene::onExit();

}

//////////////////////////////////////////////////////////////////////////
// Commands handlers

EmbossLib::ErrorCode MPix::GameMain::FinishGame()
{
   // Polymorphic call to finish
   return FinishedGame();
}



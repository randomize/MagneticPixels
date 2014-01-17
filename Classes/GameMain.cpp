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
   CmdUIContentTransform::listners["GameMain"] = std::bind( &GameMain::ContentTransform, this, _1, _2);
   CmdUIUpdateViewport::listners["GameMain"] = std::bind( &GameMain::UpdateViewport, this );
   CmdUIGameFinished::listners["GameMain"] = std::bind( &GameMain::FinishGame, this );
}

MPix::GameMain::~GameMain()
{
   CmdUIContentTransform::listners.erase("GameMain"); 
   CmdUIUpdateViewport::listners.erase("GameMain");
   CmdUIGameFinished::listners.erase("GameMain");
}

bool GameMain::init()
{

   // 1. super init first
   if ( !Scene::init() )
   {
      return false;
   }

   // Check that game play is ready
   assert(GameplayManager::getInstance().GetState() == GameplayManager::State::READY);

   return true;
}


void GameMain::onEnter()
{
   Scene::onEnter();

   // Create scene
   Size fullSize = Director::getInstance()->getWinSize();
   Size halfSize =  fullSize / 2.0f;
   Size visibleSize = Director::getInstance()->getVisibleSize();
   Point origin = Director::getInstance()->getVisibleOrigin();

   contentScale = visibleSize.height/fullSize.height * visibleSize.width/fullSize.width;

   // Setup layers
   // bg = 0
   // content = 1
   //   pixels = 1
   //   touch = 2


   bg = LayerColor::create(Color4B::GRAY);
   //bg->setPosition(halfSize.width, halfSize.height);
   //bg->setScale(2.0f);
   addChild(bg, 0);

   auto bg1 = Sprite::create("bg/02.jpg");
   //bg1->setPosition(Point(halfSize.width, halfSize.height));
   bg->addChild(bg1, 1);

   content = Node::create();
   addChild(content, 1);
 
   pixels = PixelsLayer::create();
   assert(pixels);
   //pixels->setPosition(halfSize.width, halfSize.height);
   pixels->setScale(contentScale);
   content->addChild(pixels, 1);

   touch = TouchLayer::create();
   assert(touch);
   //touch->setPosition(halfSize.width, halfSize.height);
   touch->setScale(contentScale); 
   content->addChild(touch, 2);

   // Create buttons
   CreateButtons();

   // save original viewport and content pos
   viewport = GameplayManager::getInstance().GetViewport();
   Coordinates p = viewport.BL + Coordinates(4,6); // pos defined with BL only
   content->setPosition(-LogicToScreen(p) + Point(halfSize.width, halfSize.height));
   initial_pos = content->getPosition();

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
      UpdateViewport();
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

ErrorCode GameMain::ContentTransform( float scale, Point position )
{
   const float speed = 1.8f;
   const float dist = 0.9f;
   position = position * contentScale + initial_pos;
   // Move content
   if (content->getNumberOfRunningActions()) {
      auto act = Spawn::createWithTwoActions(ScaleTo::create(speed, scale), EaseInOut::create(MoveTo::create(speed/2.0, position),1.5f));
      content->stopAllActions();
      content->runAction(act);
      bg->stopAllActions();
      bg->runAction(MoveTo::create(speed/2.0, position * dist));
   } else {
      auto act = Spawn::createWithTwoActions(ScaleTo::create(speed, scale), EaseInOut::create(MoveTo::create(speed, position),1.5f));
      content->runAction(act);
      bg->runAction(MoveTo::create(speed, position * dist));
   }

   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::GameMain::UpdateViewport()
{
   auto & newv = GameplayManager::getInstance().GetCurrentViewport();
   // Currently only pan
   int x = newv.BL.x - viewport.BL.x;
   int y = newv.BL.y - viewport.BL.y;
   ContentTransform(1.0f, LogicToScreen(-x, -y));

   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::GameMain::FinishGame()
{
   // Polymorphic call to finish
   return FinishedGame();
}



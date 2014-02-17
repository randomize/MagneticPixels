#include "GameMain.h"
#include "Cmd.h"

#include "TouchLayer.h"
#include "PixelsLayer.h"

#include "GameplayManager.h"
#include "GameStateManager.h"
#include "LevelManager.h"
#include "ContentManager.h"
#include "ViewManager.h"

using namespace MPix;
using namespace std::placeholders;

// Constants
int const GAMEPLAY_TICK_RATIO = 6;
int const IDLE_TRICKS_TICK_RATIO = 100;

GameMain::GameMain()
{
   CmdUIGameFinished::listners["GameMain"] = std::bind( &GameMain::onCmdGameFinished, this );
   CmdUIShowNotification::listners["GameMain"] = std::bind( &GameMain::onCmdShowNotification, this, _1, _2, _3 );
}

GameMain::~GameMain()
{
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

   // Take some metrics
   m_full_size = Director::getInstance()->getWinSize();
   m_half_size =  m_full_size / 2.0f;
   m_visible_size = Director::getInstance()->getVisibleSize();
   m_origin = Director::getInstance()->getVisibleOrigin();
   m_center = Point(m_half_size.width, m_half_size.height);
   m_content_scale = (m_visible_size.height/m_full_size.height * m_visible_size.width/m_full_size.width);

   // Create scene:
   // bg = 0
   //  bg1
   // content = 1
   //   pixels = 1
   //   touch = 2
   // UI: = 2
   // Notifications = 3
   

   bg = Layer::create();
   bg->addChild(ContentManager::getInstance().GetScrollingBG(rand() % 5 + 1), 1);
   addChild(bg, 0);

   pixels = PixelsLayer::create();
   assert(pixels);

   touch = TouchLayer::create();
   assert(touch);

   // Content
   content = Node::create();
   content->addChild(pixels, 1);
   content->addChild(touch, 2);
   content->setScale(m_content_scale);
   content->setPosition(m_half_size.width, m_half_size.height);
   addChild(content, 1);

   // Notificator =================================

   // Background 
   auto notification_bg = LayerColor::create(Color4B(255, 255, 255, 80));

   // Content
   notification_content = Node::create();
   notification_content->setCascadeOpacityEnabled(true);
   notification_content->setOpacityModifyRGB(true);

   notification = Node::create();
   notification->setOpacity(0);
   notification->addChild(notification_bg, 0);
   notification->addChild(notification_content, 1);
   notification->setCascadeOpacityEnabled(true);
   notification->setOpacityModifyRGB(true);
   addChild(notification, 4);
   notification_active = false;

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

void GameMain::onExit()
{
   Scene::onExit();
}

void GameMain::CreateButtons()
{
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
   for (unsigned i = 0; i < sizeof(names)/sizeof(names[0]); ++i) {
      auto btn = MenuItemImage::create( names[i], names[i], CC_CALLBACK_1(GameMain::BtnHnadler, this));
      btn->setTag(base_tag + i);
      btn->setPosition(Point(
         m_origin.x + m_visible_size.width - btn->getContentSize().width/2 * (2*i+1) ,
         m_origin.y + btn->getContentSize().height/2)
         );
      menu->addChild(btn);
   }
   this->addChild(menu, 3);
   menu->setAnchorPoint(Point(1, 0));
   menu->setScale(Director::getInstance()->getContentScaleFactor()); // FIXME
}

void GameMain::BtnHnadler(Object* sender)
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
      GameStateManager::getInstance().SwitchToMenu();
      return;
   }

   }

}

ErrorCode GameMain::Tick( float t )
{

   GameState::Tick(t);

   logic_tick++;
   // Frame skipper
   if (logic_tick > GAMEPLAY_TICK_RATIO) {

      logic_tick = 0;

      // Keep calling GameplayManager command until UI gets busy or commands end
      ErrorCode ret = ErrorCode::RET_OK;
      while ( !IsBusy() && ret == ErrorCode::RET_OK) {
         ret = GameplayManager::getInstance().ProcessOneCommand();
      }

   }

   views_tick++;

   if (views_tick > IDLE_TRICKS_TICK_RATIO) {

      views_tick = 0;

      ViewManager::getInstance().RunIdleUpdateOnRandomPixel();
   }

   if (notifications.empty() == false && notification_active == false) {
      auto &n = notifications.front();
      ShowNotification(n);
      notifications.pop_front();
   }

   return ErrorCode::RET_OK;
}

ErrorCode GameMain::FinishedGame()
{
   // Get a snapshot render texture
   Size visibleSize = Director::getInstance()->getVisibleSize();
   auto rt = RenderTexture::create(visibleSize.width, visibleSize.height);
   rt->beginWithClear(1, 0, 0, 1);
   // TODO: WTF is wrong
   this->visit();
   rt->end();

   // Store to content manager
   ContentManager::getInstance().AddNode(rt, "screen_for_blur");

   // SwithcTo final state
   GameStateManager::getInstance().SwitchToResults();

   return ErrorCode::RET_OK;
}


//////////////////////////////////////////////////////////////////////////
// Commands handlers

ErrorCode GameMain::onCmdGameFinished()
{
   // Delegate to virtual method
   return FinishedGame();
}


EmbossLib::ErrorCode MPix::GameMain::onCmdShowNotification(const string& message, bool boundled, Point pos)
{
   EM_LOG_INFO("Pushing notification \""+ message +"\"");
   notifications.push_back({ message, boundled, pos });
   return ErrorCode::RET_OK;
}

void MPix::GameMain::onNotificationClick()
{
   notification->runAction(
     Sequence::create(
      FadeOut::create(1.5f),
      CallFunc::create([this](){ notification_active = false; }),
      nullptr
     )
   );
}

ErrorCode GameMain::ShowNotification(const Notification& n)
{
   notification_active = true;
   auto listener = EventListenerTouchOneByOne::create();
   notification_toucher = listener;
   listener->setSwallowTouches(false);
   listener->onTouchBegan = [this](Touch*, Event*)
   { 
      notification->getEventDispatcher()->removeEventListener(notification_toucher);
      onNotificationClick();
      return true;
   };
   notification->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, notification);

   LabelTTF* label;
   if (n.boundled) { // Issued by pixel
      label = LabelTTF::create(n.message, ContentManager::getInstance().GetBaseFont(), 32.0f);
      label->setPosition(n.pos);
   }
   else { // Issued by scene
      label = LabelTTF::create(n.message, ContentManager::getInstance().GetBaseFont(), 32.0f);
      //label->runAction(MoveTo::create(0.5f, m_center + Point(0, m_visible_size.height/4.0)));
      label->setPosition(m_center + Point(0, m_visible_size.height/4.0));
   }

   label->setColor(Color3B::BLACK);
   notification_content->removeAllChildren();
   notification_content->addChild(label);
   notification->runAction(
     Sequence::create(
      FadeIn::create(1.0f),
      nullptr
     )
   );
   return ErrorCode::RET_OK;
}




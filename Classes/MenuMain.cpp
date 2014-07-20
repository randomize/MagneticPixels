#include "MenuMain.h"

#include "CuteBlocksLogo.h"

#include "LevelManager.h"
#include "GameplayManager.h"
#include "GameStateManager.h"
#include "ContentManager.h"
#include "SettingsManager.h"

using namespace MPix;

//====---------------------------------------------======//

const int Z_BACKGROUND = 1;
const int Z_MENU = 2;
const int Z_PANELS = 3;
const int Z_UPPER_PANE_FONT = 4;
const int Z_OVERLAY = 5;

const float PANEL_HEIGHT = 200.0f;
const float MENU_FONT_SIZE = 76.0f;

//====---------------------------------------------======//

Label* createMenuItem(const char* text, float fact = 1.0f) {
   auto & cm = ContentManager::getInstance();
   auto label = Label::createWithTTF(LocalUTF8Char(text), cm.GetBaseFont(), MENU_FONT_SIZE * fact, Size::ZERO, TextHAlignment::LEFT);
   assert(label);
   label->setColor(Color3B::BLACK);
   return label;
}


//====---------------------------------------------======//

bool MPix::MenuMain::init()
{

   if ( !Scene::init() )
   {
      return false;
   }

   return true;

}

void MPix::MenuMain::onEnter()
{

   Scene::onEnter();

   // 1. Some metrics vars
   auto d = Director::getInstance();
   auto & cm = ContentManager::getInstance();

   auto fullSize = d->getWinSize();
   auto halfSize =  fullSize / 2.0f;
   auto visibleSize = d->getVisibleSize();
   auto visibleOrigin = d->getVisibleOrigin();
   auto center = Vector2(halfSize.width, halfSize.height);

   auto upperLeft  = Vector2(visibleOrigin.x, visibleOrigin.y + visibleSize.height);
   auto upperRight = Vector2(upperLeft.x + visibleSize.width, upperLeft.y);


   // 2. Background
   addChild(cm.GetScrollingBG(4), Z_BACKGROUND);

   // 3. Upper panel

   // BG
   auto pn = DrawNode::create();
   Vector2 p[4] = {
      upperLeft,
      upperRight,
      upperRight + Vector2(0, -PANEL_HEIGHT),
      upperLeft  + Vector2(0, -PANEL_HEIGHT)
   };
   pn->drawPolygon(p, 4, Color4F(1, 1, 1, 0.3f), 0, Color4F(0, 0, 0, 0));
   addChild(pn, Z_PANELS);

   // Logo
   auto title_label = CuteBlocksLogo::create();
   title_label->setPosition((upperLeft + upperRight) / 2 + Vector2(0, -PANEL_HEIGHT / 2.0));
   addChild(title_label, Z_UPPER_PANE_FONT);

   // 4. Main menu
   auto menu = Menu::create();
   menu->setPosition(Vector2::ZERO);
   addChild(menu, Z_MENU);

   MenuItemLabel* item = nullptr;
   Label* label = nullptr;

   label = createMenuItem("Play", 1.4f);
   item = MenuItemLabel::create(label, [&](Ref *sender) {
      ToSelector();
   });
   menu->addChild(item);

#ifdef MPIX_DEVELOPERS_BUILD
   label = createMenuItem("Editor");
   item = MenuItemLabel::create(label, [&](Ref *sender) {
      ToEditor();
   });
   menu->addChild(item);

   label = createMenuItem("Test lvl");
   item = MenuItemLabel::create(label, [&](Ref *sender) {
      ToTest();
   });
   menu->addChild(item);

#endif

   label = createMenuItem("Exit");
   item = MenuItemLabel::create(label, [&](Ref *sender) {
      ToExit();
   });
   menu->addChild(item);

#ifdef MPIX_DEVELOPERS_BUILD
   label = createMenuItem("Crash");
   label->setColor(Color3B::RED);
   item = MenuItemLabel::create(label, [&](Ref *sender) {
      assert(false);
   });
   menu->addChild(item);
#endif

   label = createMenuItem("Credits");
   item = MenuItemLabel::create(label, [&](Ref *sender) {
      ToCredits();
   });
   menu->addChild(item);


   // Align items
   auto init_pos = visibleOrigin + Vector2(visibleSize.width / 5, visibleSize.height - PANEL_HEIGHT - 50);
   bool one = true;
   for (auto c : menu->getChildren()) {
      c->setAnchorPoint(Vector2(0, 1));
      c->setPosition(init_pos);
      init_pos += Vector2(0, -90);
      if (one) {
         init_pos += Vector2(0, -40);
         one = false;
      }
   }

   // Version string
   string vers(MPIX_VERSION);

#ifdef MPIX_DEVELOPERS_BUILD
   auto N_RUNS = SettingsManager::getInstance().GetKey(SettingsManager::Key::N_OF_RUNS);
   vers += "dev, run #" + ToString(N_RUNS);
#endif

   auto vlabel = Label::createWithTTF(vers.c_str(), cm.GetBaseFont(), 24, Size::ZERO, TextHAlignment::RIGHT);
   vlabel->setAnchorPoint(Vector2(1,0));
   auto orig = Director::getInstance()->getVisibleOrigin();
   auto sz = Director::getInstance()->getVisibleSize();
   vlabel->setPosition(Vector2(orig.x + sz.width, orig.y));
   addChild(vlabel, 2);



}

void MPix::MenuMain::ToEditor()
{
   GameStateManager::getInstance().SwitchToEditorMenu();
}

void MPix::MenuMain::ToExit()
{
   GameStateManager::getInstance().SwitchToExit();
}

void MPix::MenuMain::ToTest()
{
   // Load test level
   auto lvl = LevelManager::getInstance().GetPlayableTestLevel();
   GameplayManager::getInstance().LoadLevel(lvl);

   // And play it
   GameStateManager::getInstance().SwitchToGame();

}

void MPix::MenuMain::ToSelector()
{
   GameStateManager::getInstance().SwitchToSelector();

}

void MPix::MenuMain::ToCredits()
{
   GameStateManager::getInstance().SwitchToCredits();
}



#include "MenuMain.h"
#include "LevelManager.h"
#include "GameplayManager.h"
#include "GameStateManager.h"
#include "ContentManager.h"

#include "CuteBlocksLogo.h"

using namespace MPix;

//====---------------------------------------------======//

const int Z_BACKGROUND = 1;
const int Z_MENU = 2;
const int Z_UPPER_PANE = 3;
const int Z_UPPER_PANE_FONT = 4;
const int Z_OVERLAY = 5;

const float UPPER_PANE_HEIGHT = 200.0f;
const float MENU_FONT_SIZE = 76.0f;

//====---------------------------------------------======//

LabelTTF* createMenuItem(const char* text, float fact = 1.0f) {
   auto & cm = ContentManager::getInstance();
   auto label = LabelTTF::create(LocalUTF8Char(text), cm.GetBaseFont(), MENU_FONT_SIZE * fact, Size::ZERO, TextHAlignment::LEFT);
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
   auto center = Point(halfSize.width, halfSize.height);

   auto upperLeft  = Point(visibleOrigin.x, visibleOrigin.y + visibleSize.height);
   auto upperRight = Point(upperLeft.x + visibleSize.width, upperLeft.y);


   // Background
   auto bg = Sprite::create("bg/04.jpg");
   bg->setScale(visibleSize.height / bg->getContentSize().height);
   bg->setPosition(center);
   bg->runAction(
      RepeatForever::create(
         Sequence::create(
            MoveTo::create(2, center + Point(400, 0)),
            MoveTo::create(2, center - Point(400, 0)),
            nullptr
         )
      )
   );
   addChild(bg, Z_BACKGROUND);


   // 2. Upper panel

   // BG
   auto pn = DrawNode::create();
   Point p[4] = {
      upperLeft,
      upperRight,
      upperRight + Point(0, -UPPER_PANE_HEIGHT),
      upperLeft  + Point(0, -UPPER_PANE_HEIGHT)
   };
   pn->drawPolygon(p, 4, Color4F(1, 1, 1, 0.3f), 0, Color4F(0, 0, 0, 0));
   addChild(pn, Z_UPPER_PANE);

   // Logo
   auto title_label = CuteBlocksLogo::create();
   title_label->setPosition((upperLeft + upperRight) / 2 + Point(0, -UPPER_PANE_HEIGHT / 2.0));
   addChild(title_label, Z_UPPER_PANE_FONT);

   // 3. Main menu
   auto menu = Menu::create();
   menu->setPosition(center);
   addChild(menu, Z_MENU);

   MenuItemLabel* item = nullptr;
   LabelTTF* label = nullptr;

   label = createMenuItem("Play", 1.2f);
   item = MenuItemLabel::create(label, [&](Object *sender) {
      ToSelector();
   }); 
   menu->addChild(item);

#ifdef MPIX_DEVELOPERS_BUILD
   label = createMenuItem("Editor");
   item = MenuItemLabel::create(label, [&](Object *sender) {
      ToEditor();
   });
   menu->addChild(item);

   label = createMenuItem("Play test");
   item = MenuItemLabel::create(label, [&](Object *sender) {
      ToTest();
   });
   menu->addChild(item);

#endif

   label = createMenuItem("Exit");
   item = MenuItemLabel::create(label, [&](Object *sender) {
      ToExit();
   });
   menu->addChild(item);

#ifdef MPIX_DEVELOPERS_BUILD
   label = createMenuItem("Crash");
   label->setColor(Color3B::RED);
   item = MenuItemLabel::create(label, [&](Object *sender) {
      assert(false);
   });
   menu->addChild(item);
#endif
   menu->alignItemsVertically();


   // Version string

   string vers(MPIX_VERSION);

#ifdef MPIX_DEVELOPERS_BUILD
   vers += "dev ";
#endif

   auto vlabel = LabelTTF::create(vers.c_str(), cm.GetBaseFont(), 24, Size::ZERO, TextHAlignment::RIGHT);
   vlabel->setAnchorPoint(Point(1,0));
   auto orig = Director::getInstance()->getVisibleOrigin();
   auto sz = Director::getInstance()->getVisibleSize();
   vlabel->setPosition(Point(orig.x + sz.width, orig.y));
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
   auto lvl = LevelManager::getInstance().GetTestLevel();
   GameplayManager::getInstance().LoadLevel(lvl);

   // And play it
   GameStateManager::getInstance().SwitchToGame();

}

void MPix::MenuMain::ToSelector()
{
   GameStateManager::getInstance().SwitchToSelector();

}


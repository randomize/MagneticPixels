#include "MenuMain.h"
#include "LevelManager.h"
#include "GameplayManager.h"
#include "GameStateManager.h"

using namespace MPix;

//====---------------------------------------------======//

MPix::MenuMain::MenuMain()
{
}

MPix::MenuMain::~MenuMain()
{
}

bool MPix::MenuMain::init()
{

   // 1. super init first
   if ( !Scene::init() )
   {
      return false;
   }

   return true;

}

void MPix::MenuMain::onEnter()
{

   Scene::onEnter();

   MenuItemFont::setFontSize(64);
   MenuItemFont::setFontName("fonts/markerfelt.ttf");
   auto menu = Menu::create();

   MenuItemFont* item = nullptr;

#ifdef MPIX_DEVELOPERS_BUILD
   item = MenuItemFont::create(LocalUTF8Char("Editor"), [&](Object *sender) {
      ToEditor();
   }); menu->addChild(item);

   item = MenuItemFont::create(LocalUTF8Char("Test level"), [&](Object *sender) {
      ToTest();
   }); menu->addChild(item);

#endif

   item = MenuItemFont::create(LocalUTF8Char("Play"), [&](Object *sender) {
      ToSelector();
   }); menu->addChild(item); item->setScale(1.5f);

   item = MenuItemFont::create(LocalUTF8Char("Exit"), [&](Object *sender) {
      ToExit();
   }); menu->addChild(item);

#ifdef MPIX_DEVELOPERS_BUILD
   item = MenuItemFont::create(LocalUTF8Char("Crash"), [&](Object *sender) {
      assert(false);
   }); menu->addChild(item); item->setColor(Color3B::RED);
#endif

   menu->alignItemsVertically();

   // elastic effect
   auto s = Director::getInstance()->getWinSize();

   /* int i=0;
   //Node* child;
   auto pArray = menu->getChildren();
   Object* pObject = nullptr;
   CCARRAY_FOREACH(pArray, pObject)
   {
   if(pObject == nullptr)
   break;

   auto child = static_cast<Node*>(pObject);

   auto dstPoint = child->getPosition();
   int offset = (int) (s.width/2 + 50);
   if( i % 2 == 0)
   offset = -offset;

   child->setPosition( Point( dstPoint.x + offset, dstPoint.y) );
   child->runAction( EaseElasticOut::create(MoveBy::create(2, Point(dstPoint.x - offset,0)), 0.35f) );
   i++;
   }*/

   menu->setPosition(Point(s.width/2, s.height/2));
   addChild(menu);

   string vers(MPIX_VERSION);

#ifdef MPIX_DEVELOPERS_BUILD
   vers += "dev ";
#endif

   auto vlabel = LabelTTF::create(vers.c_str(), "Arial", 36, Size::ZERO, TextHAlignment::RIGHT);
   vlabel->setAnchorPoint(Point(1,0));
   auto orig = Director::getInstance()->getVisibleOrigin();
   auto sz = Director::getInstance()->getVisibleSize();
   vlabel->setPosition(Point(orig.x + sz.width, orig.y));
   addChild(vlabel);

}

void MPix::MenuMain::onExit()
{
   Scene::onExit();
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

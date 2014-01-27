#include "LevelSelector.h"


#include "GameStateManager.h"
#include "GameplayManager.h"
#include "LevelManager.h"
#include "World.h"
#include "Level.h"

using namespace MPix;


//====---------------------------------------------======//

const int Z_BACKGROUND = 1;
const int Z_WORLDS_LAYER = 2;
const int Z_UPPER_PANE = 3;

const float UPPER_PANE_HEIGHT = 100.0f;

//====---------------------------------------------======//

MPix::LevelSelector::LevelSelector():
   world_m(nullptr),
   level_m(nullptr),
   state(State::WAIT),
   worlds_layer(nullptr)
{
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

   auto d = Director::getInstance();
   auto fullSize = d->getWinSize();
   auto halfSize =  fullSize / 2.0f;
   auto visibleSize = d->getVisibleSize();
   auto visibleOrigin = d->getVisibleOrigin();
   auto center = Point(halfSize.width, halfSize.height);

   auto upperLeft  = Point(visibleOrigin.x, visibleOrigin.y + visibleSize.height);
   auto upperRight = Point(upperLeft.x + visibleSize.width, upperLeft.y);


   // Background
   auto bg1 = Sprite::create("bg/04.jpg");
   bg1->setScale(visibleSize.height / bg1->getContentSize().height);
   bg1->setPosition(center);
   addChild(bg1, Z_BACKGROUND);


   // Upper pane
   auto pn = DrawNode::create();
   Point p[4] = {
      upperLeft,
      upperRight,
      upperRight + Point(0, -UPPER_PANE_HEIGHT),
      upperLeft  + Point(0, -UPPER_PANE_HEIGHT)
   };
   pn->drawPolygon(p, 4, Color4F(1, 1, 1, 0.3f), 0, Color4F(0, 0, 0, 0));
   addChild(pn, Z_UPPER_PANE);


   // Scrollable layer with worlds
   worlds_layer = LayerColor::create(Color4B::GRAY);
   addChild(worlds_layer, Z_WORLDS_LAYER);

   auto& lm = LevelManager::getInstance();

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
   worlds_layer->addChild(menu, 1);
   world_m = menu;

   // Register touch
   auto listener = EventListenerTouchOneByOne::create();
   listener->setSwallowTouches(false);
   listener->onTouchBegan     = CC_CALLBACK_2(LevelSelector::onTouchBegan, this);
   listener->onTouchMoved     = CC_CALLBACK_2(LevelSelector::onTouchMoved, this);
   listener->onTouchEnded     = CC_CALLBACK_2(LevelSelector::onTouchEnded, this);
   listener->onTouchCancelled = CC_CALLBACK_2(LevelSelector::onTouchCancelled, this);
   _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

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

   worlds_layer->addChild(menu, 1);
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

bool MPix::LevelSelector::onTouchBegan(Touch *touch, Event *event)
{
   EM_LOG_DEBUG("Touch began");
   switch (state)
   {
   case State::WAIT:
      state = State::SCROLL;
      initial_pos = worlds_layer->getPosition();
      return true;
   case State::SCROLL:
      return false;
   default:
      assert(false);
      return false;
   }
}

void MPix::LevelSelector::onTouchCancelled(Touch *touch, Event *event)
{
   state = State::WAIT;
}

void MPix::LevelSelector::onTouchEnded(Touch *touch, Event *event)
{
   EM_LOG_DEBUG("Touch ended");
   state = State::WAIT;
   // TODO: start boucing animation
}

void MPix::LevelSelector::onTouchMoved(Touch *touch, Event *event)
{
   EM_LOG_DEBUG("Touch moved");
   // Update position
   auto new_position = initial_pos + Point(touch->getLocationInView().x, 0);
   worlds_layer->setPosition(new_position);

}

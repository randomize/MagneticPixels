#include "LevelSelector.h"


#include "GameStateManager.h"
#include "GameplayManager.h"
#include "LevelManager.h"
#include "World.h"
#include "Level.h"
#include "SettingsManager.h"
#include "LevelView.h"
#include "ContentManager.h"

using namespace MPix;


//====---------------------------------------------======//

const int Z_BACKGROUND = 1;
const int Z_WORLDS_LAYER = 2;
const int Z_PANELS = 3;
const int Z_UPPER_PANE_FONT = 4;
const int Z_OVERLAY = 5;

const float PANEL_HEIGHT = 100.0f;
const float UPPER_PANE_FONT = 64.0f;

const float FLIP_PAGE_TRESHOLD_PERCENT = 0.25; // 1/4 away from center => flip

const int LEVEL_BUTTON_COLS_COUNT = 3;
const int LEVEL_BUTTON_ROWS_COUNT = 5;

const float BUTTON_TOUCH_RADIUS = 40.0f;

/*
const float LEVEL_BUTTON_COLS_SPACING = 180.0f;
const float LEVEL_BUTTON_ROWS_SPACING = 150.0f;

const float LEVEL_BUTTON_WIDTH  = (LEVEL_BUTTON_COLS_COUNT - 1) * LEVEL_BUTTON_COLS_SPACING;
const float LEVEL_BUTTON_HEIGHT = (LEVEL_BUTTON_ROWS_COUNT - 1) * LEVEL_BUTTON_ROWS_SPACING;
*/
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

   // Collect metrics
   fullSize = d->getWinSize();
   halfSize =  fullSize / 2.0f;
   visibleSize = d->getVisibleSize();

   centerPoint = Point(halfSize.width, halfSize.height);

   lowerLeft = d->getVisibleOrigin();
   lowerRight = Point(lowerLeft.x + visibleSize.width, lowerLeft.y);
   upperLeft  = Point(lowerLeft.x, lowerLeft.y + visibleSize.height);
   upperRight = Point(upperLeft.x + visibleSize.width, upperLeft.y);

   return true;
}

void MPix::LevelSelector::onEnter()
{
   Scene::onEnter();


   // Background
   auto bg1 = Sprite::create("bg/04.jpg");
   bg1->setScale(visibleSize.height / bg1->getContentSize().height);
   bg1->setPosition(centerPoint);
   addChild(bg1, Z_BACKGROUND);


   // Upper pane
   auto pn = DrawNode::create();
   Point p_u[4] = {
      upperLeft,
      upperRight,
      upperRight + Point(0, -PANEL_HEIGHT),
      upperLeft  + Point(0, -PANEL_HEIGHT)
   };
   pn->drawPolygon(p_u, 4, Color4F(1, 1, 1, 0.3f), 0, Color4F(0, 0, 0, 0));
   // Lower pane
   Point p_l[4] = {
      lowerLeft,
      lowerLeft + Point(0, PANEL_HEIGHT / 2.0),
      lowerRight + Point(0, PANEL_HEIGHT / 2.0),
      lowerRight
   };
   pn->drawPolygon(p_l, 4, Color4F(1, 1, 1, 0.3f), 0, Color4F(0, 0, 0, 0));
   addChild(pn, Z_PANELS);


   // Scrollable layer with worlds
   //worlds_layer = LayerColor::create(Color4B::GRAY);
   worlds_layer = Layer::create();
   addChild(worlds_layer, Z_WORLDS_LAYER);

   auto& lm = LevelManager::getInstance();

   // Create worlds menu
   auto& ids = lm.GetWorldIDs();
   auto ids_cout = ids.size();
   assert(ids_cout);

   current_index = SettingsManager::getInstance().GetKey(SettingsManager::Key::LAST_WORLD);
   title_lables.reserve(ids_cout);
   indexed_ids.reserve(ids_cout);
   indexed_positions.reserve(ids_cout);
   indexed_views.reserve(ids_cout);

   float level_pane_spacing_x = (visibleSize.width + 80) / (LEVEL_BUTTON_COLS_COUNT + 1);
   float level_pane_spacing_y = (visibleSize.height - 2 * PANEL_HEIGHT + 80) / (LEVEL_BUTTON_ROWS_COUNT + 1);
   float level_pane_w = (LEVEL_BUTTON_COLS_COUNT - 1) * level_pane_spacing_x;
   float level_pane_h = (LEVEL_BUTTON_ROWS_COUNT - 1) * level_pane_spacing_y;

   for (auto id : ids ) {

      // Setting up search maps
      int i = indexed_ids.size();

      indexed_ids.push_back(id);
      ids_indexes.emplace(id, i);
      float base_offset = i * visibleSize.width;
      indexed_positions.push_back(base_offset);

      // Take world
      auto w = lm.GetWorldByID(id);
      assert(w);

      // Create Label, place and hide them all
      auto label = LabelTTF::create(w->GetName(), ContentManager::getInstance().GetBaseFontLight(), UPPER_PANE_FONT);
      label->setPosition((upperLeft + upperRight)/2 + Point(0, -PANEL_HEIGHT/2.0));
      label->setOpacity(0);
      label->setColor(Color3B::BLACK);
      title_lables.emplace(id, label);
      addChild(label, Z_UPPER_PANE_FONT);

      Point cur_center = Point(base_offset, 0) + centerPoint;
      //auto cur_center = Point(base_offset, -UPPER_PANE_HEIGHT / 2) + center;

      vector<LevelView*> world_buttons;
      world_buttons.reserve(15);

      int j = 0;
      for (auto lvl_id : lm.GetLevelsByWorldID(id)) {
         // Create view
         auto level_button = LevelView::create(lvl_id, j);
         
         int row = j / LEVEL_BUTTON_COLS_COUNT;
         int col = j % LEVEL_BUTTON_COLS_COUNT;

         Point pos{ col * level_pane_spacing_x, row * -level_pane_spacing_y };
         Point shift{ -level_pane_w / 2, level_pane_h / 2 };

         level_button->setPosition(cur_center + pos + shift); 
         level_button->setAnchorPoint({0.5f, 0.5f});

         level_button->setScale(0.02f);
         level_button->runAction(Sequence::createWithTwoActions(DelayTime::create(j*0.10f), ScaleTo::create(0.4f, 1.0f)));

         j++; // counting level index

         worlds_layer->addChild(level_button);
         world_buttons.push_back(level_button);
      }

      indexed_views.push_back(world_buttons);

   }

   // Register touch
   auto touch_listener = EventListenerTouchOneByOne::create();
   touch_listener->setSwallowTouches(false);
   touch_listener->onTouchBegan     = CC_CALLBACK_2(LevelSelector::onTouchBegan, this);
   touch_listener->onTouchMoved     = CC_CALLBACK_2(LevelSelector::onTouchMoved, this);
   touch_listener->onTouchEnded     = CC_CALLBACK_2(LevelSelector::onTouchEnded, this);
   touch_listener->onTouchCancelled = CC_CALLBACK_2(LevelSelector::onTouchCancelled, this);
   _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);

   // Register back button
   auto back_listener = EventListenerKeyboard::create();
   back_listener->onKeyReleased = [this](EventKeyboard::KeyCode k, Event*) {
      if (k == EventKeyboard::KeyCode::KEY_BACKSPACE) {
         BackToMainMenu();
      }
   };
   _eventDispatcher->addEventListenerWithSceneGraphPriority(back_listener, this);

   // Reveal current title label
   title_lables[indexed_ids[current_index]]->runAction(FadeIn::create(1.0f));
   worlds_layer->setPosition(indexed_positions[current_index], 0);


   auto buttons = Menu::create();
   buttons->setPosition(0, 0);

   auto bt_s1 = ContentManager::getInstance().GetSimpleSprite("right_arrow");
   auto bt_s2 = ContentManager::getInstance().GetSimpleSprite("right_arrow");
   //bt_s1->setPosition(upperRight.x - 20, center.y);
   //bt_s2->setPosition(upperRight.x - 20, center.y);
   bt_s1->setOpacity(200);
   auto bt = MenuItemSprite::create(bt_s1, bt_s2, [this](Object*) { NextWorld(); });
   bt->setPosition(upperRight.x - 40, centerPoint.y);
   buttons->addChild(bt);

   bt_s1 = ContentManager::getInstance().GetSimpleSprite("right_arrow");
   bt_s2 = ContentManager::getInstance().GetSimpleSprite("right_arrow");
   //bt_s1->setPosition(upperLeft.x + 20, center.y);
   //bt_s2->setPosition(upperLeft.x + 20, center.y);
   bt_s1->setOpacity(200);
   bt = MenuItemSprite::create(bt_s1, bt_s2, [this](Object*) { PrewWorld(); });
   bt->setPosition(upperLeft.x + 40, centerPoint.y);
   bt->setRotation(180);
   bt_s1->setOpacity(220);
   buttons->addChild(bt);

   addChild(buttons, Z_OVERLAY);
}

void MPix::LevelSelector::onExit()
{
   Scene::onExit();

}

void MPix::LevelSelector::SelectedLevel( unsigned int id )
{
   auto lvl = LevelManager::getInstance().GetPlayableLevelByID(id);
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

   auto ids = lm.GetLevelsByWorldID(wid);
   auto menu = Menu::create();

   for (auto id : ids ) {
      string n = lm.GetNameByLevelID(id) + " (id=" + ToString(id) + ")";
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

/////////////////// TOUCH HANDLING ////////////////////////////////////////////////////

bool MPix::LevelSelector::onTouchBegan(Touch *touch, Event *event)
{
   switch (state)
   {
   case State::WAIT:
   {
      initial_touch = convertTouchToNodeSpace(touch);
      auto button = GetViewAtPoint(initial_touch);
      if (button) { // Touched button

         // Save current button
         m_cur_button = button;

         // Scale it little bit
         m_cur_button->stopAllActions();
         m_cur_button->runAction(ScaleTo::create(0.3f, 1.1f));

         // goto button state
         state = State::BUTTON;

         return true;
      }
      state = State::SCROLL;
      initial_pos = worlds_layer->getPosition();
      gesture_action = Gesture::SAME;
      worlds_layer->stopAllActions();
      return true;
   }
   case State::SCROLL:
   case State::BUTTON:
   case State::IGNORING:
   case State::ANIMATING:
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
   if (state == State::BUTTON) // Finger not left radius and went up => button click
   {

      // Click
      m_cur_button->stopAllActions();
      m_cur_button->runAction(ScaleTo::create(0.1f, 1));

      SelectedLevel(m_cur_button->GetLevelID());

      // Back to default
      state = State::WAIT;
   }
   else if (state == State::SCROLL)
   {
      switch (gesture_action) {
      case Gesture::TO_NEXT:
         NextWorld();
         return;
      case Gesture::TO_PREW:
         PrewWorld();
         return;
      default:
         auto p = indexed_positions[current_index];
         worlds_layer->runAction(
            Sequence::create(
               EaseBounceOut::create(
                  MoveTo::create(0.6f, Point(p,0))
               ),
               CallFunc::create(
                  [this](){
                     state = State::WAIT;
                  }
               ),
               nullptr
            )
         );
         state = State::ANIMATING;
         return;
      }
   } 
   else if (state == State::IGNORING)
   {
      state = State::WAIT;
   }
}

void MPix::LevelSelector::onTouchMoved(Touch *touch, Event *event)
{
   switch (state)
   {
   case State::SCROLL:
   {
      // Update position
      auto pos = convertTouchToNodeSpace(touch) - initial_touch;
      auto new_position = initial_pos + Point(pos.x, 0);
      worlds_layer->setPosition(new_position);

      // Update gesture
      // TODO:

      break;
   }
   case State::WAIT:
   case State::BUTTON:
   {
      auto pos = convertTouchToNodeSpace(touch) - initial_touch;
      if (pos.getLengthSq() > BUTTON_TOUCH_RADIUS*BUTTON_TOUCH_RADIUS) {
         EM_LOG_DEBUG("Touch left button");
         state = State::IGNORING;
         m_cur_button->stopAllActions();
         m_cur_button->runAction(ScaleTo::create(0.15f, 1));
      }
      break;
   }
   case State::IGNORING:
   case State::ANIMATING:
      break;
   default:
      assert(false);
      break;
   }

}

void MPix::LevelSelector::NextWorld()
{
   EM_LOG_DEBUG("Next world");

}

void MPix::LevelSelector::PrewWorld()
{
   EM_LOG_DEBUG("Prew world");

}

LevelView* MPix::LevelSelector::GetViewAtPoint(Point touch_pos)
{
   int j = 0;
   for (auto v : indexed_views[current_index]) {
      if ((touch_pos - v->getPosition()).getLengthSq() < BUTTON_TOUCH_RADIUS*BUTTON_TOUCH_RADIUS) {
         EM_LOG_DEBUG("Matched buttton with id " + j);
         return v;
      }
      j++;
   }
   return nullptr;
}

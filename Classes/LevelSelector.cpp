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

const float FLIP_PAGE_TRESHOLD_PERCENT = 0.15f; //  => flip

const int LEVEL_BUTTON_COLS_COUNT = 3;
const int LEVEL_BUTTON_ROWS_COUNT = 5;

const float BUTTON_TOUCH_RADIUS = 40.0f;

const int IDLING_RATE = 2; // Use only each 2rd

const float PANEL_LABEL_FADE = 0.6f;

//====---------------------------------------------======//


bool MPix::LevelSelector::init()
{

   // 1. super init first
   if ( !Scene::init() )
   {
      return false;
   }

   // Init members
   state = State::WAIT;
   worlds_layer = nullptr;
   world_count = 0;

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


   // Levels buttons, worlds labels and worlds_layer
   CreateLevelButtons();

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


   CreateArrowButtons();

   UpdateButtons();
}

void MPix::LevelSelector::onExit()
{
   Scene::onExit();
   SettingsManager::getInstance().SetKey(SettingsManager::Key::LAST_WORLD, current_index);
   SettingsManager::getInstance().SaveSettings();
}

void MPix::LevelSelector::SelectedLevel( unsigned int id )
{

   // Take level
   auto lvl = LevelManager::getInstance().GetPlayableLevelByID(id);
   GameplayManager::getInstance().LoadLevel(lvl);

   // And play it
   GameStateManager::getInstance().SwitchToGame();

}

void MPix::LevelSelector::BackToMainMenu()
{
   GameStateManager::getInstance().SwitchToMenu();
}

void MPix::LevelSelector::CreateLevelButtons()
{
   auto& lm = LevelManager::getInstance();

   // Scrollable layer with worlds
   worlds_layer = Layer::create();
   addChild(worlds_layer, Z_WORLDS_LAYER);

   // Create worlds menu
   auto& ids = lm.GetWorldIDs();
   world_count = ids.size();
   assert(world_count);

   current_index = SettingsManager::getInstance().GetKey(SettingsManager::Key::LAST_WORLD);
   title_lables.reserve(world_count);
   indexed_ids.reserve(world_count);
   indexed_positions.reserve(world_count);
   indexed_views.reserve(world_count);

   float level_pane_spacing_x = (visibleSize.width + 80) / (LEVEL_BUTTON_COLS_COUNT + 1);
   float level_pane_spacing_y = (visibleSize.height - 2 * PANEL_HEIGHT + 80) / (LEVEL_BUTTON_ROWS_COUNT + 1);
   float level_pane_w = (LEVEL_BUTTON_COLS_COUNT - 1) * level_pane_spacing_x;
   float level_pane_h = (LEVEL_BUTTON_ROWS_COUNT - 1) * level_pane_spacing_y;

   int i = 0; // World indexer
   for (auto id : ids ) {

      indexed_ids.push_back(id);
      ids_indexes.emplace(id, i);
      float base_offset = i * visibleSize.width;
      indexed_positions.push_back(-base_offset);

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

      int j = 0; // Level indexer
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

      } // Levels for

      indexed_views.push_back(world_buttons);

      i++; // counting world index

   } // Worlds for

   // Reveal current title label
   title_lables[indexed_ids[current_index]]->runAction(FadeIn::create(PANEL_LABEL_FADE));
   worlds_layer->setPosition(indexed_positions[current_index], 0);

}

/////////////////// TOUCH HANDLING ////////////////////////////////////////////////////

bool MPix::LevelSelector::onTouchBegan(Touch *touch, Event *event)
{
   switch (state)
   {
   case State::WAIT:
   {
      initial_touch = convertTouchToNodeSpace(touch);
      if (worlds_layer->getNumberOfRunningActions() == 0) { // Disable buttons while animating 
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
      }
      if (indexed_positions[current_index] - worlds_layer->getPositionX() > 50.0) { // Too far to be catched
         return false;
      }
      state = State::SCROLL;
      indling_couner = 0;
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

      auto id = m_cur_button->GetLevelID();

      if (LevelManager::getInstance().GetLockedByLevelID(id) == false) {
         SelectedLevel(id);
      }
      else
      {
#ifdef MPIX_DEVELOPERS_BUILD
         if (last_locked == id && last_locked == pre_last_locked) {
            SelectedLevel(last_locked);
         }
         else {
            pre_last_locked = last_locked;
            last_locked = id;
         }
#endif // MPIX_DEVELOPERS_BUILD
         m_cur_button->PlayLockedAnimation();
      }

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
         ElasticBounceToCurrentWorld();
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
      worlds_layer->setPosition(NormalizePozition(new_position));

      if (indling_couner == 0) 
      {
         float threshold = visibleSize.width * FLIP_PAGE_TRESHOLD_PERCENT;
         // Update gesture
         if (pos.x > 0) {
            gesture_action = pos.x > threshold ? Gesture::TO_PREW : Gesture::SAME;
         } else {
            gesture_action = pos.x < -threshold ? Gesture::TO_NEXT : Gesture::SAME;
         }
         indling_couner = IDLING_RATE;
      }
      else {
         indling_couner--;
         assert(indling_couner >= 0);
      }

      break;
   }
   case State::WAIT:
   case State::BUTTON:
   {
      auto pos = convertTouchToNodeSpace(touch) - m_cur_button->getPosition();
      if (pos.getLengthSq() > BUTTON_TOUCH_RADIUS*BUTTON_TOUCH_RADIUS) {
         EM_LOG_DEBUG("Touch left button");
         //state = State::IGNORING;
         m_cur_button->stopAllActions();
         m_cur_button->runAction(ScaleTo::create(0.15f, 1));

         // Back to scroll
         initial_touch = convertTouchToNodeSpace(touch);
         state = State::SCROLL;
         indling_couner = 0;
         initial_pos = worlds_layer->getPosition();
         gesture_action = Gesture::SAME;
         worlds_layer->stopAllActions();
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

   if (current_index < world_count-1) {
      title_lables[indexed_ids[current_index]]->stopAllActions();
      title_lables[indexed_ids[current_index]]->runAction(FadeOut::create(0.1f));
      current_index++;
      title_lables[indexed_ids[current_index]]->stopAllActions();
      title_lables[indexed_ids[current_index]]->runAction(FadeIn::create(PANEL_LABEL_FADE));
   }

   UpdateButtons();
   ElasticBounceToCurrentWorld();
}

void MPix::LevelSelector::PrewWorld()
{
   EM_LOG_DEBUG("Prew world");

   if (current_index > 0) {
      title_lables[indexed_ids[current_index]]->stopAllActions();
      title_lables[indexed_ids[current_index]]->runAction(FadeOut::create(0.1f));
      current_index--;
      title_lables[indexed_ids[current_index]]->stopAllActions();
      title_lables[indexed_ids[current_index]]->runAction(FadeIn::create(PANEL_LABEL_FADE));
   }

   UpdateButtons();
   ElasticBounceToCurrentWorld();
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

void MPix::LevelSelector::ElasticBounceToCurrentWorld()
{
   auto p = indexed_positions[current_index];
   worlds_layer->stopAllActions();
   worlds_layer->runAction(
      Sequence::create(
         EaseBounceOut::create(
            MoveTo::create(0.5f, Point(p,0))
         ),
         CallFunc::create(
            [this](){
               state = State::WAIT;
            }
         ),
         nullptr
      )
   );
   //state = State::ANIMATING;
   state = State::WAIT;
}

cocos2d::Point MPix::LevelSelector::NormalizePozition(Point pos)
{
   if (pos.x > indexed_positions[0]) {
      pos.x = indexed_positions[0];
   }
   if (pos.x < indexed_positions[world_count-1]) {
      pos.x = indexed_positions[world_count-1];
   }
   return pos;
}

void MPix::LevelSelector::UpdateButtons()
{
   prew_button->setVisible(true);
   next_button->setVisible(true);

   if (current_index == 0) {
      prew_button->setVisible(false);
   }

   if (current_index == world_count - 1){
      next_button->setVisible(false);
   }

}

void MPix::LevelSelector::CreateArrowButtons()
{
   auto buttons = Menu::create();
   buttons->setPosition(0, 0);

   auto bt_s1 = ContentManager::getInstance().GetSimpleSprite("right_arrow");
   auto bt_s2 = ContentManager::getInstance().GetSimpleSprite("right_arrow");
   bt_s1->setOpacity(200);
   auto bt = MenuItemSprite::create(bt_s1, bt_s2, [this](Object*) { NextWorld(); });
   bt->setPosition(upperRight.x - 40, centerPoint.y);
   buttons->addChild(bt);
   next_button = bt;

   bt_s1 = ContentManager::getInstance().GetSimpleSprite("right_arrow");
   bt_s2 = ContentManager::getInstance().GetSimpleSprite("right_arrow");
   bt_s1->setOpacity(200);
   bt = MenuItemSprite::create(bt_s1, bt_s2, [this](Object*) { PrewWorld(); });
   bt->setPosition(upperLeft.x + 40, centerPoint.y);
   bt->setRotation(180);
   bt_s1->setOpacity(220);
   buttons->addChild(bt);
   prew_button = bt;

   addChild(buttons, Z_OVERLAY);
}


#include "LevelView.h"
#include "Level.h"

#include "GoalView.h"
#include "Goal.h"

#include "LevelManager.h"
#include "ContentManager.h"

using namespace MPix;

//====---------------------------------------------======//

const int Z_BORDER = 1;
const int Z_NAME = 2;
const int Z_GOAL = 3;
const int Z_LOCK = 4;

const float FNT_LAYER_NAME = 20.0f;
const float MAX_GOAL_DIM = 70.0f;

//====---------------------------------------------======//

LevelView* MPix::LevelView::create(int lvl_id, int label_number)
{
   auto fab = new LevelView;
   if (fab->initWithId(lvl_id, label_number)) {
      fab->autorelease();
      return fab;
   }
   delete fab;
   return nullptr;
}


bool MPix::LevelView::initWithId(int lvl_id, int label_number)
{
   if (!init()) {
      return false;
   }

   auto & lm = LevelManager::getInstance();
   auto & cm = ContentManager::getInstance();

   auto level = lm.GetLevelByID(lvl_id);
   assert(level);

   level_id = level->GetID();
   assert(level_id == lvl_id); // Who knows, JIC ;)

   auto lvl = level->GetName() + " " + ToString(label_number);

   auto sp = cm.GetSimpleSprite("level_border");
   addChild(sp, Z_BORDER);

   auto name = LabelTTF::create(lvl, cm.GetBaseFont(), FNT_LAYER_NAME);
   name->setColor(Color3B::BLACK);
   name->setPosition(0, -60.0f);
   addChild(name, Z_NAME);

   auto lock = cm.GetSimpleSprite("level_lock");
   lock->setPosition(0, 0);
   lock->setPosition(35, 35);
   lock->setScale(0.5f);
   lock->setVisible( level->GetState() == Level::State::IS_LOCKED );
   addChild(lock, Z_LOCK);

   auto g_node = Node::create();
   g_node->setCascadeColorEnabled(true);
   g_node->setCascadeOpacityEnabled(true);
   g_node->setPosition(0, 0);
   if (level->GetState() == Level::State::IS_OPEN)
   {
      // Pulse
      g_node->runAction(
         RepeatForever::create(
            Sequence::createWithTwoActions(
               FadeTo::create(0.6f, 150),
               FadeTo::create(0.3f, 255)
            )
         )
      );
   }
   else if (level->GetState() == Level::State::IS_LOCKED) {
      g_node->setColor(Color3B(60,60,60));
      g_node->setOpacity(80);
   }
   addChild(g_node, Z_GOAL);

   auto level_goals = level->GetGoals();
   assert(level_goals->GoalsCount() == 1);

   auto goal = level_goals->GetGoalByID(Goals::FIRST_GOAL);
   auto & tasks = goal->GetTasks();
   assert(tasks.empty() == false);

   Coordinates min, max;
   min = max = (tasks.begin())->first; // init with coordinates of one of the goals
   for (auto & t : goal->GetTasks())
   {
      auto p = t.first;
      if (min.x > p.x) min.x = p.x;
      if (min.y > p.y) min.y = p.y;
      if (max.x < p.x) max.x = p.x;
      if (max.y < p.y) max.y = p.y;
   }
   Point screen_min = MPix::LogicToScreen(min);
   Point screen_max = MPix::LogicToScreen(max);
   Point center = -(screen_max + screen_min) / 2 - MPIX_CELL_SIZE_HALF_P;
   Point dims = (screen_max - screen_min + MPIX_CELL_SIZE_P);
   float w = fabs(dims.x);
   float h = fabs(dims.y);
   float fitter = w > h ? w : h;

   g_node->setScale( MAX_GOAL_DIM / fitter );

   gv = make_shared<GoalView>();
   gv->Build(goal);
   gv->setPosition(center);
   // TODO: Render and bake to rendertexture! save some CPU life ;)
   gv->BindContents(g_node);
   gv->Update(CmdUIUpdateGoalView::Reason::CREATED);

   return true;
}

unsigned MPix::LevelView::GetLevelID() const
{
   return level_id;
}






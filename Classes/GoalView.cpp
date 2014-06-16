#include "GoalView.h"
#include "Goal.h"

#include "HSVSprite.h"
#include "ViewsConstants.h"
#include "ContentManager.h"

using namespace MPix;


GoalView::GoalView()
{
   tasks.reserve(16);
}

void GoalView::Build( shared_ptr<Goal> model )
{
   goal = model;
   contents = Node::create();
   contents->setCascadeOpacityEnabled(true);
   contents->setCascadeColorEnabled(true);

   for (auto p : goal->GetTasks() ) {
      auto p1 = LogicToScreen(p.first.x, p.first.y) + MPIX_CELL_SIZE_HALF_P;
      auto cp = ContentManager::getInstance().GetHSVSprite("goal_bg");
      cp->SetHSV(PixelColorToHSV(p.second.GetColor()));
      cp->setPosition(p1);
      tasks.emplace(p.first, cp);
      contents->addChild(cp, 0);
   }

}

Node* MPix::GoalView::GetTaskAt( Coordinates pos )
{
   auto t_it = tasks.find(pos);
   assert(t_it != tasks.end() && "Task at pos not found");
   assert(t_it->second != nullptr && "Task at pos is null");
   return t_it->second;
}

bool GoalView::Update( CmdUIUpdateGoalView::Reason reason, Coordinates pos )
{
   switch (reason)
   {
   case CmdUIUpdateGoalView::Reason::CREATED:
      {
         // For all tasks, making random delay upscaling to normal size
         for (auto t_it : tasks ) {
            auto t = t_it.second;
            t->setScale(0.01f);
            t->setVisible(true);
            t->runAction(
               Sequence::create(
                  ScaleTo::create(0.5f, 1.0f),
                  DelayTime::create(rand()%100 / 100.0f),
                  nullptr
               )
            );
         }
         break;
      }
   case CmdUIUpdateGoalView::Reason::HIGHLIGHT:
      {
         auto t = GetTaskAt(pos);

         //Check if exists and warn
         if (t->getActionByTag(ViewsConstants::ActionTags::GOAL_HIGHLIGHT_PULSE) != nullptr) {
            ECLOG_WARNING("Goal HIGHLIGHT called twice");
            break;
         }

         // Else create new one
         auto act = RepeatForever::create(
            Sequence::create(
               ScaleTo::create(0.15f, 1.1f),
               ScaleTo::create(0.15f, 1.0f),
               nullptr
            )
         );

         // Put tag and run
         act->setTag(ViewsConstants::ActionTags::GOAL_HIGHLIGHT_PULSE);
         t->runAction(act);

         break;
      }
   case CmdUIUpdateGoalView::Reason::UNHIGHLIGHT:
      {
         auto t = GetTaskAt(pos);

         //Check if not exists and warn
         if (t->getActionByTag(ViewsConstants::ActionTags::GOAL_HIGHLIGHT_PULSE) == nullptr) {
            ECLOG_WARNING("Goal UNHIGHLIGHT called twice");
            break;
         }

         t->stopActionByTag(ViewsConstants::ActionTags::GOAL_HIGHLIGHT_PULSE);
         t->setScale(1.0f);

         break;
      }
   case CmdUIUpdateGoalView::Reason::ACCEPTED:
      {
         auto t = GetTaskAt(pos);

         t->stopAllActions();

         auto sq = Sequence::createWithTwoActions(
            FadeOut::create(0.2f),
            Hide::create()
         );

         t->runAction(sq);

         break;
      }
   case CmdUIUpdateGoalView::Reason::UNACCEPTED:
      {
         auto t = GetTaskAt(pos);
         t->stopAllActions();
         t->setVisible(true);
         t->setOpacity(255);
         break;
      }
   default:
      return false;
   }
   return true;


}

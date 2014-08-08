#include "EventProcessor.h"

#include "GameStateManager.h"
#include "Cmd.h"

using namespace MPix;


//====-----EventPacket----------------------------------------======//

EventPacket::EventPacket( int pri ) : 
   priority(pri),
   isActive(true)
{
}

MPix::EventPacket::~EventPacket()
{
}

bool EventPacket::operator<( const EventPacket& rhs ) const
{
   return priority < rhs.priority ;
}

void MPix::EventPacket::ProcessEvent()
{
   if (isActive) 
      this->Dispatch();
}

void MPix::EventPacket::Deactivate()
{
   isActive = false;
}


//====-----PixelEventPacket----------------------------------------======//


// Rules for event overriding
MPix::PixelRules MPix::PixelEventPacket::rules = [] () {

   PixelRules rules;
   std::forward_list<PixelEvent> lst;

   lst.push_front(PixelEvent::UNDO_MOVED);
   lst.push_front(PixelEvent::UNDO_ADDED_TO_ASSEMBLY);
   rules[PixelEvent::RESET] = lst;

   return rules;

}();

PixelEventPacket::PixelEventPacket( PixelEvent e, int id, int pri ) : 
   EventPacket(pri), 
   event(e), 
   pixelID(id)
{
}

size_t PixelEventPacket::hash() const 
{
   auto res = ( static_cast<unsigned>(pixelID) << 16 ) | ( static_cast<unsigned>(event) & 0x0000FFFF );
   return res ^ 0x1A70BFF4; // Some magic
}

bool PixelEventPacket::operator==( const EventPacket& rhs_base ) const 
{
   auto rhs = dynamic_cast<const PixelEventPacket*>(&rhs_base);
   if (rhs == nullptr)
      return false;
   return event == rhs->event && pixelID == rhs->pixelID;
}

void PixelEventPacket::Dispatch()
{
   int id = pixelID;
   switch (event)
   {
   case PixelEvent::MOVED:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Pixel moves " );
      GameStateManager::getInstance().CurrentState()->Execute( new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::MOVING) );
      break;
   case PixelEvent::UNDO_MOVED:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Pixel undo moves " );
      GameStateManager::getInstance().CurrentState()->Execute( new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::MOVING) );
      break;
   case  PixelEvent::CHANGED_DATA:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Changed data " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::CHANGED));
      break;
   case PixelEvent::LOST_FROM_ASSEMBLY:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Lost " );
      break;
   case PixelEvent::UNDO_LOST_FROM_ASSEMBLY:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Undo Lost " );
      GameStateManager::getInstance().CurrentState()->Execute( new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::WAKE) );
      break;
   case PixelEvent::LOST_FROM_WORLD:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Lost " );
      break;
   case PixelEvent::UNDO_LOST_FROM_WORLD:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Undo Lost " );
      break;
   case PixelEvent::DIED:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Pixel died " );
      GameStateManager::getInstance().CurrentState()->Execute( new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::DIED) );
      break;
   case PixelEvent::UNDO_DIED:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Pixel undo died " );
      GameStateManager::getInstance().CurrentState()->Execute( new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::RESURRECT) );
      break;
   case PixelEvent::BLOCKED_MOVE:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Blocked move " );
      GameStateManager::getInstance().CurrentState()->Execute( new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::BLOCKING) );
      break;
   case PixelEvent::ADDED_TO_ASSEMBLY:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Waked " );
      GameStateManager::getInstance().CurrentState()->Execute( new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::WAKE) );
      break;
   case PixelEvent::UNDO_ADDED_TO_ASSEMBLY:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Asleep " );
      GameStateManager::getInstance().CurrentState()->Execute( new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::ASLEEP) );
      break;
   case PixelEvent::SMILED:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Smiled " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::SMILED));
      break;
   case PixelEvent::UNSMILED:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Unsmiled " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::UNSMILED));
      break;
   case PixelEvent::ACCEPTED_GOAL:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Accepted " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::ACCEPT));
      break;
   case PixelEvent::UNDO_ACCEPTED_GOAL:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Undo accepted " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::RESURRECT));
      break;
   case PixelEvent::RESET:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Reset " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::RESET));
      break;
   case PixelEvent::CREATED:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Created " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::CREATED));
      break;
   case PixelEvent::TAPPED:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Tapped " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::TAPPED));
      break;
   case PixelEvent::CAN_FALL:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Can fall " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::CANFALL));
      break;
   case PixelEvent::CANT_FALL:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Cant fall " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::CANTFALL));
      break;
   case PixelEvent::KILLED:
      ECLOG_DEBUG(" Pixel ["+ id + "] => Killed someone " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdatePixelView(id, CmdUIUpdatePixelView::Reason::KILLED_SOMEONE));
      break;

   default:
      ECLOG_WARNING(" Pixel ["+ id + "] => Unknown event " );
      break;
   }
}

void MPix::PixelEventPacket::RuleCheck( EventList & tokill )
{
   auto rule = rules.find(event);
   if ( rule != rules.end() ) {
      for (auto k : rule->second ) {
         for (auto e: tokill) {
            auto dp = dynamic_pointer_cast<PixelEventPacket> (e);
            if (dp != nullptr && dp->event == k && dp->pixelID == pixelID) 
               dp->Deactivate();
         }
      }
   }

}

//====-----GoalEventPacket----------------------------------------======//

GoalEventPacket::GoalEventPacket( GoalEvent e, int id, Coordinates task, int pri ) : 
   EventPacket(pri), 
   event(e), 
   goalID(id), 
   task(task)
{
}

size_t GoalEventPacket::hash() const 
{
   unsigned res = 0x12345678;
   res ^= ( static_cast<unsigned>(goalID) << 16 ) | ( static_cast<unsigned>(event) & 0x0000FFFF );
   res ^= 0x5B70CD34; res = (res >> 8) | (res << 24);
   res ^= ( static_cast<unsigned>(task.x) << 16 ) | ( static_cast<unsigned>(task.y) & 0x0000FFFF );
   res ^= 0xBC4F5DC0; res = (res >> 8) | (res << 24);
   return res;
}

bool GoalEventPacket::operator==( const EventPacket& rhs_base ) const 
{
   auto rhs = dynamic_cast<const GoalEventPacket*>(&rhs_base);
   if (rhs == nullptr)
      return false;
   return event == rhs->event && goalID == rhs->goalID && rhs->task == task;
}

void GoalEventPacket::Dispatch()
{
   int id = goalID;
   switch (event)
   {
   case GoalEvent::ACCEPTED:
      ECLOG_DEBUG(" Goal ["+ id + "]" + task + " => Accepted " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateGoalView(id, task, CmdUIUpdateGoalView::Reason::ACCEPTED));
      break;
   case GoalEvent::UNDO_ACCEPTED:
      ECLOG_DEBUG(" Goal ["+ id + "]" + task + " => Undo Accepted " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateGoalView(id, task, CmdUIUpdateGoalView::Reason::UNACCEPTED));
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateGoalView(id, task, CmdUIUpdateGoalView::Reason::HIGHLIGHT));
      break;
   case GoalEvent::HIGHTLIGHT:
      ECLOG_DEBUG(" Goal ["+ id + "]" + task + " => Highlight " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateGoalView(id, task, CmdUIUpdateGoalView::Reason::HIGHLIGHT));
      break;
   case GoalEvent::UNDO_HIGHLIGHT:
      ECLOG_DEBUG(" Goal ["+ id + "]" + task + " => Undo Highlight " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateGoalView(id, task, CmdUIUpdateGoalView::Reason::UNHIGHLIGHT));
      break;
   case GoalEvent::UNHIGHLIGHT:
      ECLOG_DEBUG(" Goal ["+ id + "]" + task + " => Unhighlight " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateGoalView(id, task, CmdUIUpdateGoalView::Reason::UNHIGHLIGHT));
      break;
   case GoalEvent::UNDO_UNHIGHLIGHT:
      ECLOG_DEBUG(" Goal ["+ id + "]" + task + " => Undo Unhighlight " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateGoalView(id, task, CmdUIUpdateGoalView::Reason::HIGHLIGHT));
      break;
   case GoalEvent::CREATED:
      ECLOG_DEBUG(" Goal ["+ id + "]" + task + " => Goal Create " );
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateGoalView(id, task, CmdUIUpdateGoalView::Reason::CREATED));
      break;
   default:
      ECLOG_WARNING(" Goal ["+ id + "]" + task + " => Unknown event " );
      break;
   }
}

void MPix::GoalEventPacket::RuleCheck( EventList & events )
{
   // No overrides for now
}

//====-----Processor----------------------------------------======//

EventProcessor::EventProcessor()
{

}

void MPix::EventProcessor::PostEvent( shared_ptr<EventPacket> e )
{
   events.push_front(e);
}

MPix::EventProcessor::~EventProcessor()
{
}

void EventProcessor::ClearEvents()
{
   events.clear();
}

EndlessCatLib::ErrorCode EventProcessor::ProcessEvents()
{
   ECLOG_INFO(" ==== Processing events === ");

   PreprocessEvents();

   for (auto it = events.rbegin(); it != events.rend(); ++it) {
      (*it)->ProcessEvent();
   }

   ClearEvents();

   ECLOG_INFO(" ==== End of events === ");

   return ErrorCode::RET_OK;
}

EndlessCatLib::ErrorCode EventProcessor::PreprocessEvents()
{

   // Kill duplicates
   forward_list<shared_ptr<EventPacket>> foundElements;
   events.remove_if( [&] (shared_ptr<EventPacket> a){

        for (auto el : foundElements ) {
           if ( *el == *a ) {
              return true;
           }
        }
        foundElements.push_front(a);
        return false;
    });

   // Kill by rules
   for (auto e : events ) {
      e->RuleCheck(events);
   }

   // Sort by priority
   events.sort([](shared_ptr<EventPacket> a, shared_ptr<EventPacket> b) { return *a < *b; });

   return ErrorCode::RET_OK;

}


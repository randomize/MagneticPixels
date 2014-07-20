#include "IAlive.h"
#include "IKilling.h"
#include "BehaviorManager.h"
#include "Context.h"

using namespace MPix;

//====---------------------------------------------======//


MPix::IAlive::IAlive()
{
   life = State::ALIVE;
}

bool MPix::IAlive::canLive( const Context& context )
{
   assert(alive); // AliveBehavior not set
   auto me = dynamic_pointer_cast<IAlive> ( context.GetByID(this->GetID()));
   assert(me); // will be strange having null here...
   return alive->canLive(me, context);
}

void MPix::IAlive::SetAliveBehavior( shared_ptr<AliveBehavior> new_behavior )
{
   assert(new_behavior);
   alive = new_behavior;
}

void MPix::IAlive::SetAliveBehavior( const string& name )
{
   auto b = BehaviorManager::getInstance().GetSharedBehaviour(name);
   SetAliveBehavior(std::dynamic_pointer_cast<AliveBehavior>(b));
}

bool MPix::IAlive::IsAlive() const
{
   return life == State::ALIVE;
}

void MPix::IAlive::Kill( const Context& context, State cause )
{
   assert(IsAlive()); // Killing dead is bad
   life = cause;
   context.PostEvent(PixelEvent::DIED, GetID());
}

void MPix::IAlive::Kill( const Context& context, const IKilling& killer )
{
   Kill(context, GetStateByKiller(killer));
}

void MPix::IAlive::Kill(State cause)
{
   assert(IsAlive()); // Killing dead is bad
   life = cause;
}


//====---------------------------------------------======//

void MPix::IAlive::PopSnapshots( const Context& context, int n )
{
   if (IsAlive()) { // Undo can't kill a pixel, only revive
      life.PopSnapshots(n);
   } else {
      // was dead
      life.PopSnapshots(n);
      if (IsAlive()) { // now alive
         context.PostEvent(PixelEvent::UNDO_DIED, GetID());
      }
   }
}

void MPix::IAlive::InitSnapshots( const Context& context )
{
   life.InitSnapshots();
}

void MPix::IAlive::PushSnapshot( const Context& context )
{
   life.PushSnapshot();
}

void MPix::IAlive::ClearSnapshots( const Context& context )
{
   life.ClearSnapshots();
}

bool MPix::IAlive::KilledBy( const IKilling& k ) const
{
   return life == GetStateByKiller(k);
}

MPix::IAlive::State MPix::IAlive::GetStateByKiller( const IKilling& k ) const
{
   switch (k.GetType())
   {
   case Pixel::PixelType::CACTUS_PIX:
      return State::KILLED_BY_NEEDLE;
   case Pixel::PixelType::PITFALL_PIX:
      return State::KILLED_BY_PITTRAP;
   case Pixel::PixelType::STONE_PIX:
      return State::KILLED_BY_STONE;
   default:
      ECLOG_WARNING("Unknow dead cause");
      return State::KILLED_BY_HUNGRY_WOLF;
   }
}



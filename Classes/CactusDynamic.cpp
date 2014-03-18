#include "CactusDynamic.h"
#include "Ranking.h"
#include "Context.h"

using namespace MPix;


// Allocating memory for EM_NODE_CHILD, see header
EM_NODE_CHILD_CPP(CactusDynamic);

MPix::CactusDynamic::CactusDynamic( 
   NeedleType t /*= NeedleType::STEPPER*/,
   Direction way /*= Direction::DIR_RIGHT*/,
   bool sleeping /*= false*/ 
):
   Pixel(0),
   type(t),
   way(way),
   sleeping(sleeping)
{
   srandy = rand();
   // Needle pixel can be killed with pitfall
   SetAliveBehavior("alivesimple");

   // Kills with spikes
   SetKillingBehavior("killingspikes");
   SetKillingRank(KillersRanking::CACTUS);

   // Moves step by step
   SetStepperBehavior("stepperstandard");

   // Blocks others
   SetMoveBlockerBehavior("moveblockermytype");
}



void CactusDynamic::updatePrelude( const Context& context )
{
   if (!canMoveThis(context, way)) {
      way = ReverseDirection(way);
      if (!canMoveThis(context, way)) {
         way = ReverseDirection(way);
      }
   }
}

void MPix::CactusDynamic::updateAction( const Context& context )
{
   if (canMoveThis(context, way)) {
      Move(context, way);
   } else {
      way = ReverseDirection(way);
      if (canMoveThis(context, way)) {
         Move(context, way);
      } else {
         way = ReverseDirection(way);
      }
   }

   UpdateMyWay(context);
}


const char* MPix::CactusDynamic::TypeToString( NeedleType t )
{
   switch (t)
   {
   case MPix::CactusDynamic::NeedleType::STEPPER:
      return "Stepper ";
   case MPix::CactusDynamic::NeedleType::MOVER:
      return "Mover";
   case NeedleType::PREDATOR:
      return "Predator";
   case NeedleType::RANDOM:
      return "Crazy";
   default:
      return "WTF";
   }
}

void MPix::CactusDynamic::InitSnapshots( const Context& context )
{
   Pixel::InitSnapshots(context);
   IAlive::InitSnapshots(context);
   IDynamic::InitSnapshots(context);
   IKilling::InitSnapshots(context);
   IMovableStepper::InitSnapshots(context);
   IMoveBlocker::InitSnapshots(context);
   way.InitSnapshots();
}

void MPix::CactusDynamic::PushSnapshot( const Context& context )
{
   Pixel::PushSnapshot(context);
   IAlive::PushSnapshot(context);
   IDynamic::PushSnapshot(context);
   IKilling::PushSnapshot(context);
   IMovableStepper::PushSnapshot(context);
   IMoveBlocker::PushSnapshot(context);
   way.PushSnapshot();
}

void MPix::CactusDynamic::ClearSnapshots( const Context& context )
{
   Pixel::ClearSnapshots(context);
   IAlive::ClearSnapshots(context);
   IDynamic::ClearSnapshots(context);
   IKilling::ClearSnapshots(context);
   IMovableStepper::ClearSnapshots(context);
   IMoveBlocker::ClearSnapshots(context);
   way.ClearSnapshots();
}

void MPix::CactusDynamic::PopSnapshots( const Context& context, int n )
{
   Pixel::PopSnapshots(context, n);
   IAlive::PopSnapshots(context, n);
   IDynamic::PopSnapshots(context, n);
   IKilling::PopSnapshots(context, n);
   IMovableStepper::PopSnapshots(context, n);
   IMoveBlocker::PopSnapshots(context, n);
   way.PopSnapshots(n);
   context.PostEvent(PixelEvent::CHANGED_DATA, GetID());
}

void MPix::CactusDynamic::UpdateMyWay( const Context& context )
{
   Direction d;
   switch (type)
   {
   case NeedleType::STEPPER: // Each step changes
      d = ReverseDirection(way);
      break;
   case NeedleType::MOVER: // Never changes by itself
      d = way;
      break;
   case NeedleType::RANDOM: // Random direction
      //srand
      d = RandomDirection();
      break;
   case NeedleType::PREDATOR: 
      { // Seeks for pixels 
         auto s = GetPosAsPoint();
         auto e = context.GetAssemblyCenter();
         d = GetDirectionVector(s, e);
         break;
      }
   default:
      assert(false);
      return;
   }
   way = d;

   // sen UI change
   context.PostEvent(PixelEvent::CHANGED_DATA, GetID());

}


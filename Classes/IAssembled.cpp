#include "IAssembled.h"
#include "Context.h"
#include "BehaviorManager.h"

using namespace MPix;

//====---------------------------------------------======//

MPix::IAssembled::IAssembled( PixelColor color ) :
   IColorful(color)
{
   in_assembly = false;
   canFall = false;
   hintCanFall = false;
}

void MPix::IAssembled::AddToAssembly( const Context& context )
{
   assert(IsInAssembly() == false);
   context.PostEvent(PixelEvent::ADDED_TO_ASSEMBLY, GetID());
   UpdateSmile(context);
   in_assembly = true;

}
void MPix::IAssembled::RemoveFromAssembly( const Context& context )
{
   assert(IsInAssembly() == true);
   context.PostEvent(PixelEvent::LOST_FROM_ASSEMBLY, GetID());
   SetSmile(context, false);
   in_assembly = false;
}

void MPix::IAssembled::Move( const Context& context, Direction dir )
{
   //auto pwas = GetPos();
   IMovableStepper::Move(context, dir);
   UpdateSmile(context);
}

void MPix::IAssembled::InitSnapshots( const Context& context )
{
   IColorful::InitSnapshots(context);
   IAlive::InitSnapshots(context);
   IMovableStepper::InitSnapshots(context);
   in_assembly.InitSnapshots();
   canFall.InitSnapshots();
}

void MPix::IAssembled::PushSnapshot( const Context& context )
{
   IColorful::PushSnapshot(context);
   IAlive::PushSnapshot(context);
   IMovableStepper::PushSnapshot(context);
   in_assembly.PushSnapshot();
   canFall.PushSnapshot();
}

void MPix::IAssembled::ClearSnapshots( const Context& context )
{
   IColorful::ClearSnapshots(context);
   IAlive::ClearSnapshots(context);
   IMovableStepper::ClearSnapshots(context);
   in_assembly.ClearSnapshots();
   canFall.ClearSnapshots();
}

void MPix::IAssembled::PopSnapshots( const Context& context, int n )
{
   IColorful::PopSnapshots(context, n);
   IAlive::PopSnapshots(context, n);
   IMovableStepper::PopSnapshots(context, n);
   bool was = in_assembly;
   in_assembly.PopSnapshots(n);
   if ( was != in_assembly ) {
      if (was) {
         context.PostEvent(PixelEvent::UNDO_ADDED_TO_ASSEMBLY, GetID());
      } else {
         context.PostEvent(PixelEvent::UNDO_LOST_FROM_ASSEMBLY, GetID());
      }
   }
   bool was2 = canFall;
   canFall.PopSnapshots(n);
   if ( was2 != canFall ) {
      if (was2) {
         context.PostEvent(PixelEvent::CANT_FALL, GetID());
      } else {
         context.PostEvent(PixelEvent::CAN_FALL, GetID());
      }
   }
}

void MPix::IAssembled::UpdateSmile( const Context& context )
{
   if ( context.ExistsMatchingGoal(GetPos(), GetColor())) {
      SetSmile(context, true);
   } else 
      SetSmile(context, false);
}

bool MPix::IAssembled::growThis( const Context& context )
{
   assert(grower);
   auto me = dynamic_pointer_cast<IAssembled> ( context.GetByID(this->GetID())); 
   assert(me); // will be strange having null here...
   return grower->growThis(me, context);
}

void MPix::IAssembled::SetGrowBehavior( shared_ptr<GrowBehavior> new_behavior )
{
   assert(new_behavior);
   grower = new_behavior;
}

void MPix::IAssembled::SetGrowBehavior( const string& name )
{

   auto b = BehaviorManager::getInstance().GetSharedBehaviour(name);
   SetGrowBehavior(std::dynamic_pointer_cast<GrowBehavior>(b));
}



bool MPix::IAssembled::IsInAssembly()
{
   return in_assembly;
}

bool MPix::IAssembled::CheckCanFall( const Context& context )
{
   if (hintCanFall) // Hinted
   {
      if (!canFall) {
         canFall = true;
         context.PostEvent(PixelEvent::CAN_FALL, GetID());
      }
   }
   else 
   {
      if (canFall) {
         canFall = false;
         context.PostEvent(PixelEvent::CANT_FALL, GetID());
      }
   }
   hintCanFall = false;
   return canFall;
}

void MPix::IAssembled::HintCanFall()
{
   hintCanFall = true;
}

#include "IMovable.h"
#include "Field.h"
#include "Context.h"
#include "BehaviorManager.h"

using namespace MPix;

//====---------------------------------------------======//



////////// Base movable ////////////////////////////////////////////////////////////////
/*
void MPix::IMovable::PopSnapshots( const Context& context, int n )
{

}

void MPix::IMovable::ClearSnapshots( const Context& context )
{

}

void MPix::IMovable::PushSnapshot( const Context& context )
{

}

void MPix::IMovable::InitSnapshots( const Context& context )
{

}

bool MPix::IMovable::canMoveThis( const Context& context, Coordinates pos )
{
   return true;
}
*/
/////////// Step mover ///////////////////////////////////////////////////////////////

bool MPix::IMovableStepper::canMoveThis( const Context& context, Direction dir )
{
   assert(stepper);
   auto me = dynamic_pointer_cast<IMovableStepper> ( context.GetByID(this->GetID()));
   assert(me); // will be strange having null here...
   return stepper->canMoveThis(me, context, dir);
}

void MPix::IMovableStepper::Move( const Context& context, Direction dir )
{
   context.field->MovePixel(context.GetByID(GetID()), dir);
   context.PostEvent(PixelEvent::MOVED, GetID());
}

void MPix::IMovableStepper::InitSnapshots( const Context& context )
{
}

void MPix::IMovableStepper::PushSnapshot( const Context& context )
{
}

void MPix::IMovableStepper::ClearSnapshots( const Context& context )
{
}

void MPix::IMovableStepper::PopSnapshots( const Context& context, int n )
{
}

void MPix::IMovableStepper::SetStepperBehavior( shared_ptr<StepperBehavor> new_behavior )
{
   assert(new_behavior);
   stepper = new_behavior;
}

void MPix::IMovableStepper::SetStepperBehavior( const string& name )
{
   auto b = BehaviorManager::getInstance().GetSharedBehaviour(name);
   SetStepperBehavior(std::dynamic_pointer_cast<StepperBehavor>(b));
}

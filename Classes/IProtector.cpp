#include "IProtector.h"
#include "Context.h"
#include "BehaviorManager.h"

using namespace MPix;

//====---------------------------------------------======//

void MPix::IProtector::SetProtectorBehavior( shared_ptr<ProtectorBehavior> new_behavior )
{
   assert(new_behavior);
   protector = new_behavior;
}

void MPix::IProtector::SetProtectorBehavior( const string& name )
{
   auto b = BehaviorManager::getInstance().GetSharedBehaviour(name);
   SetProtectorBehavior(std::dynamic_pointer_cast<ProtectorBehavior>(b));
}

bool MPix::IProtector::isProtecting( const Context& context, shared_ptr<IAlive> victim, shared_ptr<IKilling> killer )
{
   assert(protector);
   auto me = dynamic_pointer_cast<IProtector> ( context.GetByID(this->GetID())); 
   assert(me); // will be strange having null here...
   return protector->isProtecting(me, context, victim, killer);
}

void MPix::IProtector::PopSnapshots( const Context& context, int n )
{
}

void MPix::IProtector::ClearSnapshots( const Context& context )
{
}

void MPix::IProtector::PushSnapshot( const Context& context )
{
}

void MPix::IProtector::InitSnapshots( const Context& context )
{
}


#include "IKilling.h"
#include "BehaviorManager.h"
#include "Context.h"

using namespace MPix;

//====---------------------------------------------======//
MPix::IKilling::IKilling() :
   rank(0)
{
}

MPix::IKilling::~IKilling()
{
}


bool MPix::IKilling::tryKillThat( const Context& context, int target )
{
   assert(killing);
   auto me = dynamic_pointer_cast<IKilling> ( context.GetByID(this->GetID())); 
   assert(me); // will be strange having null here...
   return killing->tryKillThat(me, context, target);
}

void MPix::IKilling::SetKillingBehavior( shared_ptr<KillingBehavior> new_behavior )
{
   assert(new_behavior);
   killing = new_behavior;
}

void MPix::IKilling::SetKillingBehavior( const string& name )
{
   auto b = BehaviorManager::getInstance().GetSharedBehaviour(name);
   SetKillingBehavior(std::dynamic_pointer_cast<KillingBehavior>(b));
}

//====---------------------------------------------======//

void MPix::IKilling::PopSnapshots( const Context& context, int n )
{
}

void MPix::IKilling::ClearSnapshots( const Context& context )
{
}

void MPix::IKilling::PushSnapshot( const Context& context )
{
}

void MPix::IKilling::InitSnapshots( const Context& context )
{
}


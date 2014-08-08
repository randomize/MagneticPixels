#include "IMoveBlocker.h"
#include "BehaviorManager.h"
#include "Context.h"

using namespace MPix;

//====---------------------------------------------======//


bool MPix::IMoveBlocker::blocksMoves( const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix /*= nullptr*/ )
{
   assert(blocker);
   auto me = dynamic_pointer_cast<IMoveBlocker> ( context.GetByID(this->GetID()));
   assert(me); // will be strange having null here...
   return blocker->blocksMoves(me, context, from, to, pix);
}

void MPix::IMoveBlocker::PopSnapshots( const Context& context, int n )
{
}

void MPix::IMoveBlocker::ClearSnapshots( const Context& context )
{
}

void MPix::IMoveBlocker::PushSnapshot( const Context& context )
{
}

void MPix::IMoveBlocker::InitSnapshots( const Context& context )
{
}

void MPix::IMoveBlocker::SetMoveBlockerBehavior( shared_ptr<MoveBlockerBehavior> new_behavior )
{
   assert(new_behavior);
   blocker = new_behavior;
}

void MPix::IMoveBlocker::SetMoveBlockerBehavior( const string& name )
{
   auto b = BehaviorManager::getInstance().GetSharedBehaviour(name);
   SetMoveBlockerBehavior(std::dynamic_pointer_cast<MoveBlockerBehavior>(b));
}

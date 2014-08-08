#include "SokobanPixel.h"
#include "Context.h"

using namespace MPix;

//====---------------------------------------------======//

// Allocating memory for ECNODE_CHILD, see header
ECNODE_CHILD_CPP(SokobanPixel);

MPix::SokobanPixel::SokobanPixel( PixelColor color ) :
   SokobanBase(),
   IColorful(color)
{
   // Sokoban pixel can be killed with pitfall and cactus
   SetAliveBehavior("alivestandard");

   // Blocked by any blocker
   SetStepperBehavior("stepperstandard");

   // Simple blocker, blocks anythin on its point, and diagonals
   SetMoveBlockerBehavior("moveblockersimple");
}

MPix::SokobanPixel::~SokobanPixel()
{

}

void MPix::SokobanPixel::InitSnapshots( const Context& context )
{
   SokobanBase::InitSnapshots(context);
   IColorful::InitSnapshots(context);
}

void MPix::SokobanPixel::PushSnapshot( const Context& context )
{
   SokobanBase::PushSnapshot(context);
   IColorful::PushSnapshot(context);
}

void MPix::SokobanPixel::ClearSnapshots( const Context& context )
{
   SokobanBase::ClearSnapshots(context);
   IColorful::ClearSnapshots(context);
}

void MPix::SokobanPixel::PopSnapshots( const Context& context, int n )
{
   SokobanBase::PopSnapshots(context, n);
   IColorful::PopSnapshots(context, n);
}

void MPix::SokobanPixel::Move( const Context& context, Direction dir )
{
   IMovableStepper::Move(context, dir);
   UpdateSmile(context);
}

void MPix::SokobanPixel::UpdateSmile( const Context& context )
{
   if ( context.ExistsMatchingGoal(GetPos(), GetColor())) {
      SetSmile(context, true);
   } else
      SetSmile(context, false);
}


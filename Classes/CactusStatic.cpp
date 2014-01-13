#include "CactusStatic.h"
#include "Ranking.h"

using namespace MPix;


// Allocating memory for EM_NODE_CHILD, see header
EM_NODE_CHILD_CPP(CactusStatic);


CactusStatic::CactusStatic() :
   Pixel(0)
{
   // Needle pixel can be killed with pitfall
   SetAliveBehavior("alivesimple");

   // Needle kills with spikes
   SetKillingBehavior("killingspikes");
   SetKillingRank(KillersRanking::CACTUS);

   // Needle blocks each other
   SetMoveBlockerBehavior("moveblockermytype");
}

void MPix::CactusStatic::InitSnapshots( const Context& context )
{
   Pixel::InitSnapshots(context);
   IAlive::InitSnapshots(context);
   IKilling::InitSnapshots(context);
   IMoveBlocker::InitSnapshots(context);
}

void MPix::CactusStatic::PushSnapshot( const Context& context )
{
   Pixel::PushSnapshot(context);
   IAlive::PushSnapshot(context);
   IKilling::PushSnapshot(context);
   IMoveBlocker::PushSnapshot(context);
}

void MPix::CactusStatic::ClearSnapshots( const Context& context )
{
   Pixel::ClearSnapshots(context);
   IAlive::ClearSnapshots(context);
   IKilling::ClearSnapshots(context);
   IMoveBlocker::ClearSnapshots(context);
}

void MPix::CactusStatic::PopSnapshots( const Context& context, int n )
{
   Pixel::PopSnapshots(context, n);
   IAlive::PopSnapshots(context, n);
   IKilling::PopSnapshots(context, n);
   IMoveBlocker::PopSnapshots(context, n);
}


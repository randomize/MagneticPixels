#include "StonePixel.h"
#include "Ranking.h"

using namespace MPix;

//====---------------------------------------------======//
EM_NODE_CHILD_CPP(StonePixel)

MPix::StonePixel::StonePixel():
   SokobanBase()
{
   // Alive checks only self pos
   SetAliveBehavior("alivesimple");

   // Kills anything with less rank on it's pos
   SetKillingBehavior("killingatpoint");
   SetKillingRank(KillersRanking::STONE);

   // Blocks its pos and diagonal
   SetMoveBlockerBehavior("moveblockersimple");

   // Blocked only by walls ans itself // TODO: block rank
   SetStepperBehavior("stepperhard");
}

MPix::StonePixel::~StonePixel()
{

}

void MPix::StonePixel::InitSnapshots( const Context& context )
{
   SokobanBase::InitSnapshots(context);
   IKilling::InitSnapshots(context);
}

void MPix::StonePixel::PushSnapshot( const Context& context )
{
   SokobanBase::PushSnapshot(context);
   IKilling::PushSnapshot(context);
}

void MPix::StonePixel::ClearSnapshots( const Context& context )
{
   SokobanBase::ClearSnapshots(context);
   IKilling::ClearSnapshots(context);
}

void MPix::StonePixel::PopSnapshots( const Context& context, int n )
{
   SokobanBase::PopSnapshots(context, n);
   IKilling::PopSnapshots(context, n);
}

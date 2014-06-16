#include "Pitfall.h"
#include "Ranking.h"

using namespace MPix;

//====---------------------------------------------======//

ECNODE_CHILD_CPP(Pitfall);

MPix::Pitfall::Pitfall()
{
   SetKillingBehavior("killingpitfall");
   SetKillingRank(KillersRanking::PITFALL);  // Very powerfull
}

MPix::Pitfall::~Pitfall()
{

}

void MPix::Pitfall::InitSnapshots( const Context& context )
{
}

void MPix::Pitfall::PushSnapshot( const Context& context )
{
}

void MPix::Pitfall::ClearSnapshots( const Context& context )
{
}

void MPix::Pitfall::PopSnapshots( const Context& context, int n )
{
}

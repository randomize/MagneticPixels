#include "BomberPixel.h"
#include "Context.h"
#include "Ranking.h"

using namespace MPix;

// Allocating memory for EM_NODE_CHILD, see header
EM_NODE_CHILD_CPP(BomberPixel);


MPix::BomberPixel::BomberPixel( PixelColor color, int t ):
   AssembledMagneticBase(color)
{
   this->time = t;

   // Bomb cannot be killed by cactus once is not disposed, but dropped to pit if on it
   SetAliveBehavior("alivesimple");

   // Blocked by any blocker
   SetStepperBehavior("steppermagnetic");

   // Simple blocker, blocks anythin on its point, and diagonals
   SetMoveBlockerBehavior("moveblockersimple");
   ///SetMoveBlockerRan // TODO: !!!!!!!

   // Grows in 4 dirs
   SetGrowBehavior("magneticgrowstandard");

}

MPix::BomberPixel::~BomberPixel()
{
}

void MPix::BomberPixel::InitSnapshots(const Context& context)
{
   AssembledMagneticBase::InitSnapshots(context);
   IDynamic::InitSnapshots(context);
   time.InitSnapshots();
}

void MPix::BomberPixel::PushSnapshot(const Context& context)
{
   AssembledMagneticBase::PushSnapshot(context);
   IDynamic::PushSnapshot(context);
   time.PushSnapshot();
}

void MPix::BomberPixel::ClearSnapshots(const Context& context)
{
   AssembledMagneticBase::ClearSnapshots(context);
   IDynamic::ClearSnapshots(context);
   time.ClearSnapshots();
}

void MPix::BomberPixel::PopSnapshots(const Context& context, int n)
{
   AssembledMagneticBase::PopSnapshots(context, n);
   IDynamic::PopSnapshots(context, n);
   time.PopSnapshots(n);
   if (IsInAssembly() == false) {
      SetAliveBehavior("alivesimple");
   }
   context.PostEvent(PixelEvent::CHANGED_DATA, GetID());
}

bool MPix::BomberPixel::canStartAssembly(const Context& context)
{
   return false;
}

// Recursive explode
void RecursiveKill(const Context& context, BomberPixel& pp)
{
   for (auto d : EnumRanger<Direction>(DirectionType::ALL)) {
      auto p = pp.GetPos() + d;
      forward_list<shared_ptr<IAlive>> l;
      auto got = context.GetPixelsAt<IAlive>(l, p);
      if (got) {
         for (auto p : l) {
            if (p->IsAlive()) {
               p->Kill(IAlive::State::KILLED_BY_EXPLOSION);
               auto bb = dynamic_pointer_cast<BomberPixel>(p);
               if (bb && bb->IsInAssembly() == false) { // Wow got bomb here !
                  RecursiveKill(context, *bb);
               }
            }
         }
      }
   }
}

void MPix::BomberPixel::updateFinalize(const Context& context)
{
   if (IsInAssembly() || IsAlive() == false) return;
   time = time.GetValue() - 1;
   if (time > 0) {
      context.PostEvent(PixelEvent::CHANGED_DATA, GetID());
   }
   else {
      Kill(context, IAlive::State::KILLED_BY_EXPLOSION);
      RecursiveKill(context, *this);
   }
}



void MPix::BomberPixel::AddToAssembly(const Context& context)
{
   SetAliveBehavior("alivestandard");
   IAssembled::AddToAssembly(context);
}











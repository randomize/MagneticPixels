#include "BomberPixel.h"
#include "Context.h"

using namespace MPix;

// Allocating memory for EM_NODE_CHILD, see header
EM_NODE_CHILD_CPP(BomberPixel);


MPix::BomberPixel::BomberPixel( PixelColor color, int t ):
   AssembledMagneticBase(color)
{
   this->time = t;

   // Bomb cannot be killed by cactus once is not disposed
   SetAliveBehavior("alivesimple");

   // Blocked by any blocker
   SetStepperBehavior("steppermagnetic");

   // Simple blocker, blocks anythin on its point, and diagonals
   SetMoveBlockerBehavior("moveblockersimple");

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
   if (IsInAssembly() == false) 
      SetAliveBehavior("alivesimple");
   context.PostEvent(PixelEvent::CHANGED_DATA, GetID());
}

bool MPix::BomberPixel::canStartAssembly(const Context& context)
{
   return false;
}

void MPix::BomberPixel::updatePrelude(const Context& context)
{
   if (IsInAssembly()) return;
   if (time > 0) {
      time = time.GetValue() - 1;
      context.PostEvent(PixelEvent::CHANGED_DATA, GetID());
   }
}

bool MPix::BomberPixel::canLive(const Context& context)
{
   if (time == 0 && IsAlive()) {
      Kill(context, IAlive::State::KILLED_BY_EXPLOSION);
      return false;
   }
   
   return IAlive::canLive(context);

}

void MPix::BomberPixel::AddToAssembly(const Context& context)
{
   SetAliveBehavior("alivestandard");
   IAssembled::AddToAssembly(context);
}











#include "MutantPixel.h"
#include "Context.h"

using namespace MPix;

//====---------------------------------------------======//

// Allocating memory for ECNODE_CHILD, see header
ECNODE_CHILD_CPP(MutantPixel);

MPix::MutantPixel::MutantPixel( PixelColor colorA, PixelColor colorB ):
   MutantPixel(vector<PixelColor>{colorA, colorB})
{
}

MPix::MutantPixel::MutantPixel(const vector<PixelColor> colors):
   AssembledMagneticBase(PixelColor::RED)
{
   assert(colors.size() > 1);
   current_index = colors.size() - 1;
   current = colors.front();
   this->colors = colors;

   // Magnetic can be killed by any killer
   SetAliveBehavior("alivestandard");

   // Blocked by any blocker
   SetStepperBehavior("steppermagnetic");

   // Simple blocker, blocks anythin on its point, and diagonals
   SetMoveBlockerBehavior("moveblockersimple");

   // Grows in 4 dirs
   SetGrowBehavior("magneticgrowstandard");
}


MPix::MutantPixel::~MutantPixel()
{
}



MPix::PixelColor MPix::MutantPixel::GetColor() const
{
   return current;
}


void MPix::MutantPixel::updatePrelude(const Context& context)
{
   if (IsInAssembly()) {
      current = GetNextColor();
      IncrementColorIndex();
      context.PostEvent(PixelEvent::CHANGED_DATA, GetID());
      UpdateSmile(context);
   }
}

void MPix::MutantPixel::InitSnapshots(const Context& context)
{
   AssembledMagneticBase::InitSnapshots(context);
   IDynamic::InitSnapshots(context);
   current.InitSnapshots();
   current_index.InitSnapshots();
}

void MPix::MutantPixel::PushSnapshot(const Context& context)
{
   AssembledMagneticBase::PushSnapshot(context);
   IDynamic::PushSnapshot(context);
   current.PushSnapshot();
   current_index.PushSnapshot();
}

void MPix::MutantPixel::ClearSnapshots(const Context& context)
{
   AssembledMagneticBase::ClearSnapshots(context);
   IDynamic::ClearSnapshots(context);
   current.ClearSnapshots();
   current_index.ClearSnapshots();
}

void MPix::MutantPixel::PopSnapshots(const Context& context, int n)
{
   AssembledMagneticBase::PopSnapshots(context, n);
   IDynamic::PopSnapshots(context, n);
   current.PopSnapshots(n);
   current_index.PopSnapshots(n);
   context.PostEvent(PixelEvent::CHANGED_DATA, GetID());
}

MPix::PixelColor MPix::MutantPixel::GetNextColor() const
{
   return colors[current_index];
}

void MPix::MutantPixel::IncrementColorIndex()
{
   current_index = current_index + 1;
   if (current_index >= colors.size()) {
      current_index = 0;
   }
}

void MPix::MutantPixel::Move(const Context& context, Direction dir)
{
   IMovableStepper::Move(context, dir);

}




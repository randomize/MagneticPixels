#include "MutantPixel.h"
#include "Context.h"

using namespace MPix;

//====---------------------------------------------======//

// Allocating memory for EM_NODE_CHILD, see header
EM_NODE_CHILD_CPP(MutantPixel);

MPix::MutantPixel::MutantPixel( PixelColor colorA, PixelColor colorB ):
   AssembledMagneticBase(PixelColor::RED)
{
   colors.push_back(colorA);
   colors.push_back(colorB);
   current_index = colors.size() - 1;
   current = colorA;

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
   current = GetNextColor();
   IncrementColorIndex();
   context.PostEvent(PixelEvent::CHANGED_DATA, GetID());
   UpdateSmile(context);
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

MPix::PixelColor MPix::MutantPixel::GetNextColor()
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



#include "MagneticPixel.h"

using namespace MPix;

// Allocating memory for EM_NODE_CHILD, see header
EM_NODE_CHILD_CPP(MagneticPixel);


MPix::MagneticPixel::MagneticPixel( PixelColor color ):
   AssembledMagneticBase(color)
{

   // Magnetic can be killed by any killer
   SetAliveBehavior("alivestandard");

   // Blocked by any blocker
   SetStepperBehavior("steppermagnetic");

   // Simple blocker, blocks anythin on its point, and diagonals
   SetMoveBlockerBehavior("moveblockersimple");

   // Grows in 4 dirs
   SetGrowBehavior("magneticgrowstandard");
}

MPix::MagneticPixel::~MagneticPixel()
{
}



#include "MagneticDiagonalPixel.h"

using namespace MPix;

// Allocating memory for EM_NODE_CHILD, see header
EM_NODE_CHILD_CPP(MagneticDiagonalPixel);


MPix::MagneticDiagonalPixel::MagneticDiagonalPixel( PixelColor color ):
   AssembledMagneticBase(color)
{

   // Magnetic can be killed by any killer
   SetAliveBehavior("alivestandard");

   // Blocked by any blocker
   SetStepperBehavior("steppermagnetic");

   // Simple blocker, blocks anything on its point, and diagonals
   SetMoveBlockerBehavior("moveblockersimple");

   // Grows in 8 directions
   SetGrowBehavior("magneticgrowextended");
}

MPix::MagneticDiagonalPixel::~MagneticDiagonalPixel()
{
}



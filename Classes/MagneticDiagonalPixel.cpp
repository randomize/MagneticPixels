#include "MagneticDiagonalPixel.h"

using namespace MPix;

// Allocating memory for ECNODE_CHILD, see header
ECNODE_CHILD_CPP(MagneticDiagonalPixel);


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



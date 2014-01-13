#include "BadMagneticPixel.h"

using namespace MPix;

//====---------------------------------------------======//

EM_NODE_CHILD_CPP(BadMagneticPixel);

MPix::BadMagneticPixel::BadMagneticPixel( PixelColor color /*= PixelColor::RED*/ ) :
   AssembledMagneticBase(color)
{

   // Magnetic can be killed by any killer
   SetAliveBehavior("alivestandard");

   // Blocked by any blocker
   // TODO: hater stepper
   SetStepperBehavior("steppermagnetic");

   // Simple blocker, blocks anything on its point, and diagonals
   // TODO: hater blocker
   SetMoveBlockerBehavior("moveblockersimple");

   // Grows in 4 directions
   SetGrowBehavior("magneticgrowstandard");

}

MPix::BadMagneticPixel::~BadMagneticPixel()
{

}

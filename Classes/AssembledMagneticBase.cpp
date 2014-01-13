#include "AssembledMagneticBase.h"
#include "Context.h"

using namespace MPix;

//====---------------------------------------------======//

MPix::AssembledMagneticBase::AssembledMagneticBase( PixelColor color ) :
   IAssembled(color)
{
   // Do not setup any strategies, children must setup
}

void MPix::AssembledMagneticBase::InitSnapshots( const Context& context )
{
   Pixel::InitSnapshots(context);
   IMagnetic::InitSnapshots(context);
   IAssembled::InitSnapshots(context);
   IMoveBlocker::InitSnapshots(context);
}

void MPix::AssembledMagneticBase::PushSnapshot( const Context& context )
{
   Pixel::PushSnapshot(context);
   IMagnetic::PushSnapshot(context);
   IAssembled::PushSnapshot(context);
   IMoveBlocker::PushSnapshot(context);
}

void MPix::AssembledMagneticBase::ClearSnapshots( const Context& context )
{
   Pixel::ClearSnapshots(context);
   IMagnetic::ClearSnapshots(context);
   IAssembled::ClearSnapshots(context);
   IMoveBlocker::ClearSnapshots(context);
}

void MPix::AssembledMagneticBase::PopSnapshots( const Context& context, int n )
{
   Pixel::PopSnapshots(context, n);
   IMagnetic::PopSnapshots(context, n);
   IAssembled::PopSnapshots(context, n);
   IMoveBlocker::PopSnapshots(context, n);
}

bool MPix::AssembledMagneticBase::canStartAssembly( const Context& context )
{
   return true;
}

shared_ptr<IAssembled> MPix::AssembledMagneticBase::tryMagnetizeToMe( const Context& context, shared_ptr<Pixel> pix )
{
   // Allow to magnetize, return myself
   return dynamic_pointer_cast<IAssembled>(context.GetByID(GetID()));
}


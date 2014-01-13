#include "SokobanBase.h"
#include "Context.h"
#include "IAssembled.h"

using namespace MPix;

//====---------------------------------------------======//
// IColorful,        // Has color and smile
// IDynamic,         // Dynamics used to implement logic
// IAlive,           // Can be killed by pittrap
// IMovableStepper,  // Moves step by step
// IMoveBlocker      // Blocking used to implement logic

MPix::SokobanBase::SokobanBase():
   next_move(Direction::DIR_UNKNOWN)
{
}

void MPix::SokobanBase::InitSnapshots( const Context& context )
{
   Pixel::InitSnapshots(context);
   IAlive::InitSnapshots(context);
   IMovableStepper::InitSnapshots(context);
   IMoveBlocker::InitSnapshots(context);
}

void MPix::SokobanBase::PushSnapshot( const Context& context )
{
   Pixel::PushSnapshot(context);
   IAlive::PushSnapshot(context);
   IMovableStepper::PushSnapshot(context);
   IMoveBlocker::PushSnapshot(context);
}

void MPix::SokobanBase::ClearSnapshots( const Context& context )
{
   Pixel::ClearSnapshots(context);
   IAlive::ClearSnapshots(context);
   IMovableStepper::ClearSnapshots(context);
   IMoveBlocker::ClearSnapshots(context);

}

void MPix::SokobanBase::PopSnapshots( const Context& context, int n )
{
   Pixel::PopSnapshots(context, n);
   IAlive::PopSnapshots(context, n);
   IMovableStepper::PopSnapshots(context, n);
   IMoveBlocker::PopSnapshots(context, n);
}


void MPix::SokobanBase::preparePrelude( const Context& context, bool move_blocked )
{
   if (move_blocked == false && next_move != Direction::DIR_UNKNOWN) {
         Move(context, next_move);
   }
   next_move = Direction::DIR_UNKNOWN; // Reset
}

bool MPix::SokobanBase::blocksMoves( const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix /*= nullptr*/ )
{
   // For those not presented act as default blocker, for non diagoanl also
   if (pix == nullptr || DirectionIsMajor(to) == false) {
      return IMoveBlocker::blocksMoves(context, from, to, pix);
   }

   // Check who is pushing, allow assembly and other sokoban
   auto asm_pix = dynamic_pointer_cast<IAssembled>(pix);
   auto soko_pix = dynamic_pointer_cast<SokobanBase>(pix);
   if ( (asm_pix && asm_pix->IsInAssembly()) || (soko_pix) ) {

      if ( from + to == GetPos() ) {
         next_move = to;
         bool can = canMoveThis(context, next_move);
         if (!can) {
            next_move = Direction::DIR_UNKNOWN;
            return true;
         }
         return false;
      }
   }

   return IMoveBlocker::blocksMoves(context, from, to, pix);

}

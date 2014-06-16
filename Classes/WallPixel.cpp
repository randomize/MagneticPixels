#include "WallPixel.h"
#include "IKilling.h"
#include "IAlive.h"

//#include "Context.h"

using namespace MPix;

// Allocating memory for ECNODE_CHILD, see header
ECNODE_CHILD_CPP(WallPixel);


MPix::WallPixel::WallPixel( int sh ) :
   Pixel(10)
{
   SetShapeInt(sh);
   // Walls protect form cactuses
   //SetProtectorBehavior("cactusprotect");
}

bool MPix::WallPixel::isProtecting( const Context& context, shared_ptr<IAlive> victim, shared_ptr<IKilling> killer )
{
   auto kpos = killer->GetPos();

   auto d = GetDirectionVector(kpos, victim->GetPos());

   return blocksMoves(context, kpos, d, killer);

}

bool MPix::WallPixel::blocksMagnetizm( const Context& context, shared_ptr<Pixel> from, shared_ptr<Pixel> to )
{
   // Simple wall don't block
   return false;
}

bool MPix::WallPixel::blocksMoves( const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix )
{
   // Oh yes, it blocks, depending on shape
   // Standard wall don't cares Inside/Outside, blocks both
   // Standard wall dont care pixel type/color etc

   // Check if shares position, (means pixel tries to move from inside of wall to new pos like LEAVE
   if (from == GetPos()) {
      list<Direction> toCheck = SplitDirection(to);
      for (auto i : toCheck) {
         if (shape.find(i) != shape.end())
            return true;
      }
      return false;
   }

   // Calculate targeting point
   auto pixel_new_pos = from + to; 

   // If pixel is moving to me then reverse dir and same check
   if (pixel_new_pos == GetPos()) {
      to = ReverseDirection(to);
      list<Direction> toCheck = SplitDirection(to);
      for (auto i : toCheck) {
         if (shape.find(i) != shape.end())
            return true;
      }
      return false;
   }


   // Diagonal movements left
   if (!DirectionIsMajor(to)) {
      switch (to)
      {
      case Direction::DIR_UPRIGHT:
         if (pixel_new_pos == GetPos()+Direction::DIR_UP) {
            if (IsBlocking(Direction::DIR_UP) || IsBlocking(Direction::DIR_LEFT)) return true; 
         } else 
         if (pixel_new_pos == GetPos()+Direction::DIR_RIGHT){
            if (IsBlocking(Direction::DIR_DOWN) || IsBlocking(Direction::DIR_RIGHT)) return true; 
         }
         break;
      case Direction::DIR_UPLEFT:
         if (pixel_new_pos == GetPos()+Direction::DIR_UP) {
            if (IsBlocking(Direction::DIR_UP) || IsBlocking(Direction::DIR_RIGHT)) return true; 
         } else 
         if (pixel_new_pos == GetPos()+Direction::DIR_LEFT){
            if (IsBlocking(Direction::DIR_DOWN) || IsBlocking(Direction::DIR_LEFT)) return true; 
         }
         break;
      case Direction::DIR_DOWNRIGHT:
         if (pixel_new_pos == GetPos()+Direction::DIR_DOWN) {
            if (IsBlocking(Direction::DIR_DOWN) || IsBlocking(Direction::DIR_LEFT)) return true; 
         } else 
         if (pixel_new_pos == GetPos()+Direction::DIR_RIGHT){
            if (IsBlocking(Direction::DIR_UP) || IsBlocking(Direction::DIR_RIGHT)) return true; 
         }
         break;
      case Direction::DIR_DOWNLEFT:
         if (pixel_new_pos == GetPos()+Direction::DIR_DOWN) {
            if (IsBlocking(Direction::DIR_DOWN) || IsBlocking(Direction::DIR_RIGHT)) return true; 
         } else 
         if (pixel_new_pos == GetPos()+Direction::DIR_LEFT){
            if (IsBlocking(Direction::DIR_UP) || IsBlocking(Direction::DIR_LEFT)) return true; 
         }
         break;
      default:
         assert(false);
         break;
      }


   }

   return false;
}

void MPix::WallPixel::SetBlockAt( Direction dir )
{
   auto list = SplitDirection(dir);
   for (auto el : list) {
      shape.emplace(el);
   }
}


bool MPix::WallPixel::IsBlocking( Direction dir )
{
   return (shape.find(dir) != shape.end());
}


int MPix::WallPixel::GetShapeInt() const
{
   int k = 0;
   for (auto p : shape)
   {
      switch (p)
      {
      case Direction::DIR_UP:
         k += 1;
         break;
      case Direction::DIR_RIGHT:
         k += 2;
         break;
      case Direction::DIR_DOWN:
         k += 4;
         break;
      case Direction::DIR_LEFT:
         k += 8;
         break;
      default:
         assert(false);
         break;
      }
   }
   return k;
}


void MPix::WallPixel::SetShapeInt( int sh )
{
   shape.clear();
   assert(sh <= 16);

   // Get first
   auto p = EnumRanger<Direction>().begin();

   while (sh)
   {
      if ( sh & 1 ) SetBlockAt(*p);
      sh >>= 1;
      p++;
   }
}

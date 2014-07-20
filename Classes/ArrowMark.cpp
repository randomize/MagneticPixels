#include "ArrowMark.h"

using namespace MPix;

//====---------------------------------------------======//

ArrowMark* MPix::ArrowMark::create()
{
   auto fab = new ArrowMark();
   fab->autorelease();

   auto d = DrawNode::create();

   Vector2* verts = new Vector2[3];
   verts[0] = Vector2(MPIX_CELL_SIZE_HALF/2.0, 0);
   verts[1] = Vector2(-MPIX_CELL_SIZE_HALF/2.0, MPIX_CELL_SIZE_HALF/4.0);
   verts[2] = Vector2(-MPIX_CELL_SIZE_HALF/2.0, -MPIX_CELL_SIZE_HALF/4.0);

   d->drawPolygon(verts, 3, Color4F::RED, 0.0f, Color4F());

   fab->addChild(d);
   return fab;

}

void MPix::ArrowMark::SetDirection( Direction dir )
{
   switch (dir)
   {
   case MPix::Direction::DIR_UP:
      setRotation(-90);
      break;
   case MPix::Direction::DIR_RIGHT:
      setRotation(0);
      break;
   case MPix::Direction::DIR_DOWN:
      setRotation(90);
      break;
   case MPix::Direction::DIR_LEFT:
      setRotation(180);
      break;
   case MPix::Direction::DIR_UPRIGHT:
      setRotation(-45);
      break;
   case MPix::Direction::DIR_UPLEFT:
      setRotation(-90-45);
      break;
   case MPix::Direction::DIR_DOWNRIGHT:
      setRotation(45);
      break;
   case MPix::Direction::DIR_DOWNLEFT:
      setRotation(90+45);
      break;
   default:
      break;
   }
}


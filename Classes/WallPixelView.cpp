#include "WallPixelView.h"
#include "WallPixel.h"

#include "ColorBox.h"
#include "ContentManager.h"

using namespace MPix;

EM_NODE_CHILD_CPP(WallPixelView);

void MPix::WallPixelView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);

   pixel = std::dynamic_pointer_cast<WallPixel>(model);
   blocks_urdl.reserve(4);

   auto & borset = pixel->GetShape();
   for (auto a : borset)
   {
      auto pp = Sprite::createWithSpriteFrameName(("wallpixel_"+ToString(rand()%5+1)+".png").c_str());
      if (rand()%2) {
         pp->setFlippedX(true);
      }
      switch (a) 
      {
      case Direction::DIR_UP:
         break;
      case Direction::DIR_RIGHT:
         pp->setRotation(90.0f);
         break;
      case Direction::DIR_DOWN:
         pp->setRotation(180.0f);
         break;
      case Direction::DIR_LEFT:
         pp->setRotation(-90.0f);
         break;
      default:
         assert(false); // Only major directions in wall shape
         break;
      }
      pp->setPosition(LogicToScreen(pixel->GetPos()) + MPIX_CELL_SIZE_HALF_P);
      blocks_urdl.push_back(pp);
   }

}

bool MPix::WallPixelView::Update( CmdUIUpdatePixelView::Reason reason )
{
   return true;
}

void MPix::WallPixelView::BindContents( Node* target, int recommendedOrder /*= 0 */ )
{
   this->contents = nullptr; // Not using
   this->target = target;
   auto bn = ContentManager::getInstance().GetBatchNode(target, "wall_pixel");
   for (auto n : blocks_urdl) {
      bn->addChild(n, recommendedOrder);
   }
}

void MPix::WallPixelView::setVisible( bool visibility )
{
   for (auto n : blocks_urdl) {
      n->setVisible(visibility);
   }
}

void MPix::WallPixelView::setPosition( Point pos )
{
   for (auto n : blocks_urdl) {
      n->setPosition(LogicToScreen(pixel->GetPos()) + MPIX_CELL_SIZE_HALF_P + pos);
   }
}

MPix::WallPixelView::WallPixelView()
{

}

MPix::WallPixelView::~WallPixelView()
{
   auto bn = ContentManager::getInstance().GetBatchNode(target, "wall_pixel");
   for (auto n : blocks_urdl) {
      bn->removeChild(n, true);
   }
}


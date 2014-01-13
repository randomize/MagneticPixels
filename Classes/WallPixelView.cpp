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
   //blocks_urdl.reserve(4);

   auto & borset = pixel->GetShape();
   for (auto a : borset)
   {
      auto pp = ContentManager::getInstance().GetSimpleSprite("wall_pixel");
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
      contents->addChild(pp);
      //blocks_urdl.push_back(pp);
   }

}

bool MPix::WallPixelView::Update( CmdUIUpdatePixelView::Reason reason )
{
   return true;
}



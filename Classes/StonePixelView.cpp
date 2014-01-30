#include "StonePixelView.h"
#include "StonePixel.h"
#include "GameStateManager.h"
#include "ContentManager.h"

using namespace MPix;

//====---------------------------------------------======//

EM_NODE_CHILD_CPP(StonePixelView);




MPix::StonePixelView::StonePixelView()
{
   z_order = 11;

}

MPix::StonePixelView::~StonePixelView()
{

}

void MPix::StonePixelView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);

   pixel = std::dynamic_pointer_cast<StonePixel>(model);

   auto & cm = ContentManager::getInstance();

   auto cb = cm.GetSimpleSprite("stone_bg");

   contents->addChild(cb);
}

void MPix::StonePixelView::PixelMoved()
{
   PixelView::PixelMoved();

   GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("stone_move"));

}

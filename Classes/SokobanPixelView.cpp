#include "SokobanPixelView.h"
#include "SokobanPixel.h"


#include "HSVSprite.h"
#include "Shake.h"
#include "ContentManager.h"
#include "GameStateManager.h"

using namespace MPix;

//====---------------------------------------------======//

EM_NODE_CHILD_CPP(SokobanPixelView);

void MPix::SokobanPixelView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);
   pixel = std::dynamic_pointer_cast<SokobanPixel>(model);
   assert(pixel && "Pixel using this view must implement this interface");

   auto & cm = ContentManager::getInstance();

   body = cm.GetHSVSprite("magnetic_bg_norm");
   body->SetHSV(PixelColorToHSV(pixel->GetColor()));

   // Setup contents
   contents->addChild(body);
}

MPix::SokobanPixelView::SokobanPixelView() :
   body(nullptr)
{
   z_order = 10;
}

//////////////////////////////////////////////////////////////////////////
// Animations



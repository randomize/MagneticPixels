#include "PitfallView.h"
#include "Pitfall.h"
#include "ColorBox.h"

using namespace MPix;

//====---------------------------------------------======//

EM_NODE_CHILD_CPP(PitfallView);

MPix::PitfallView::PitfallView()
{
  z_order = 1;
}

MPix::PitfallView::~PitfallView()
{
}


void MPix::PitfallView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);

   pixel = std::dynamic_pointer_cast<Pitfall>(model);

   auto cb = ColorBox::create();
   cb->SetColor(Color4F(0,0,0,1));
   cb->setPosition(-MPIX_CELL_SIZE_HALF_P);

   contents->addChild(cb);
}



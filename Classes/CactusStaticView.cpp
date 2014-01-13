#include "CactusStaticView.h"
#include "CactusStatic.h"
#include "Shake.h"

using namespace MPix;

EM_NODE_CHILD_CPP(CactusStaticView);

void MPix::CactusStaticView::Build( shared_ptr<Pixel> model )
{
   CactusView::Build(model);
   pixel = std::dynamic_pointer_cast<CactusStatic>(model);
}

void MPix::CactusStaticView::PixelMoved()
{
   // Static not moving
   assert(false);
}

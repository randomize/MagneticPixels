#include "MagneticPixelView.h"
#include "MagneticPixel.h"


using namespace MPix;

EM_NODE_CHILD_CPP(MagneticPixelView);

void MagneticPixelView::Build( shared_ptr<Pixel> model )
{
   MagneticView::Build(model);

   pixel = std::dynamic_pointer_cast<MagneticPixel>(model);

}










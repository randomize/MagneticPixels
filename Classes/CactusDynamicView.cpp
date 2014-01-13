#include "CactusDynamicView.h"
#include "CactusDynamic.h"
#include "ArrowMark.h"

using namespace MPix;

EM_NODE_CHILD_CPP(CactusDynamicView);

void MPix::CactusDynamicView::Build( shared_ptr<Pixel> model )
{
   CactusView::Build(model);
   pixel = std::dynamic_pointer_cast<CactusDynamic>(model);

#ifdef MPIX_DEVELOPERS_BUILD
   mark = ArrowMark::create();
   mark->SetDirection(pixel->GetWay());
   contents->addChild(mark);
#endif

}

void MPix::CactusDynamicView::PixelMoved()
{
   // Moving pixel
   auto cont_act = Sequence::create(
      DelayTime::create(0.05f),
      MoveTo::create(0.08f, LogicToScreen(model->GetPos()) + MPIX_CELL_SIZE_HALF_P),
      nullptr
   );
   RunLockingAction(contents, cont_act);

   // Scaling BG
   auto m_act = Sequence::create(
      ScaleTo::create(0.05f, 0.70f),
      DelayTime::create(0.08f),
      ScaleTo::create(0.03f, 1.0f),
      nullptr
   );
   bg->runAction(m_act);
}

void MPix::CactusDynamicView::PixelChanged()
{
#ifdef MPIX_DEVELOPERS_BUILD
   mark->SetDirection(pixel->GetWay());
#endif
   // TODO: next move eyes show direction
}

#ifdef MPIX_DEVELOPERS_BUILD
void MPix::CactusDynamicView::PixelDied()
{
   CactusView::PixelDied(); 
   mark->setVisible(false);
}

void MPix::CactusDynamicView::PixelResurrect()
{
   CactusView::PixelResurrect(); 
   mark->setVisible(true);
}
#endif


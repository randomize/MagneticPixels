#include "CactusDynamicView.h"
#include "CactusDynamic.h"
#include "ArrowMark.h"
#include "Shake.h"
#include "ContentManager.h"

using namespace MPix;

ECNODE_CHILD_CPP(CactusDynamicView);

void MPix::CactusDynamicView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);

   bg = ContentManager::getInstance().GetSimpleSprite("cactus_bg");

   contents->addChild(bg,0);
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


MPix::CactusDynamicView::CactusDynamicView()
{
   z_order = 5;
}


void MPix::CactusDynamicView::PixelCreated()
{
   bg->setScale(0.01f);
   auto sq = Sequence::createWithTwoActions(
      ScaleTo::create(0.4f, 1.0f),
      DelayTime::create(rand() % 100 / 100.0f)
   );
   RunLockingAction(bg, sq);
}

void MPix::CactusDynamicView::PixelKilledSomeone()
{
   auto m_act = Shake::createWithStrength(0.8f, 0.0, 5.0f);
   RunLockingAction(bg, m_act);
}

void MPix::CactusDynamicView::PixelDied()
{
#ifdef MPIX_DEVELOPERS_BUILD
   mark->setVisible(false);
#endif
   auto ast = pixel->GetLiveState();
   switch (ast) {
   case IAlive::State::KILLED_BY_PITTRAP: {
      auto m_act = Spawn::create(
         FadeOut::create(0.9f),
         ScaleTo::create(0.7f, 0.001f),
         RotateTo::create(0.5f, 180.0f),
         nullptr
         );
      bg->runAction(m_act);
      break;
   }
   case  IAlive::State::KILLED_BY_EXPLOSION:{
      contents->setColor(Color3B::BLACK);
      contents->runAction(FadeOut::create(0.5f));
      break;
   }
   case IAlive::State::KILLED_BY_STONE: {
      auto m_act = Spawn::create(
         FadeOut::create(0.9f),
         ScaleTo::create(0.7f, 0.1f),
         RotateTo::create(0.5f, 180.0f),
         nullptr
         );
      bg->runAction(m_act);
      break;
   }
   default:
      break;
   }
}

void MPix::CactusDynamicView::PixelResurrect()
{

#ifdef MPIX_DEVELOPERS_BUILD
   mark->setVisible(true);
#endif
   contents->setOpacity(255);
   contents->setColor(Color3B::WHITE);
   bg->setRotation(0);
   auto m_act = Spawn::create( 
      FadeIn::create(0.5f),
      ScaleTo::create(0.5f, 1.0f),
      nullptr
   );
   bg->runAction(m_act);
}

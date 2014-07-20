#include "CactusStaticView.h"
#include "CactusStatic.h"
#include "Shake.h"
#include "ContentManager.h"

using namespace MPix;

ECNODE_CHILD_CPP(CactusStaticView);

void MPix::CactusStaticView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);

   bg = ContentManager::getInstance().GetSimpleSprite("cactus_bg");

   contents->addChild(bg,0);
   pixel = std::dynamic_pointer_cast<CactusStatic>(model);
}

void MPix::CactusStaticView::PixelMoved()
{
   // Static not moving
   assert(false);
}

MPix::CactusStaticView::CactusStaticView()
{
   z_order = 5;
}

void MPix::CactusStaticView::PixelCreated()
{
   bg->setScale(0.01f);
   auto sq = Sequence::createWithTwoActions(
      ScaleTo::create(0.4f, 1.0f),
      DelayTime::create(rand() % 100 / 100.0f)
   );
   RunLockingAction(bg, sq);
}

void MPix::CactusStaticView::PixelKilledSomeone()
{
   auto m_act = Shake::createWithStrength(0.8f, 0.0, 5.0f);
   RunLockingAction(bg, m_act);
}

void MPix::CactusStaticView::PixelDied()
{
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

void MPix::CactusStaticView::PixelResurrect()
{
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

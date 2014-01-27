#include "CactusView.h"
#include "CactusDynamic.h"
#include "Shake.h"
#include "ContentManager.h"

using namespace MPix;

MPix::CactusView::CactusView()
{
   z_order = 5;
}

void MPix::CactusView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);

   bg = ContentManager::getInstance().GetSimpleSprite("cactus_bg");

   contents->addChild(bg,0);
}


void MPix::CactusView::PixelCreated()
{
   bg->setScale(0.01f);
   auto m_act = ScaleTo::create(0.4f, 1.0f);
   auto pause = DelayTime::create(rand()%100 / 100.0f * 2);
   auto sq = Sequence::createWithTwoActions(pause, m_act);
   bg->runAction(sq);
}

void MPix::CactusView::PixelKilled()
{
   auto m_act = Shake::createWithStrength(0.8f, 0.0, 5.0f);
   RunLockingAction(bg, m_act);
}

void MPix::CactusView::PixelDied()
{
   auto m_act = Spawn::create( 
      FadeOut::create(0.5f),
      ScaleTo::create(0.5f, 0.001f),
      RotateTo::create(0.5f, 180.0f),
      nullptr
   );
   bg->runAction(m_act);
}

void MPix::CactusView::PixelResurrect()
{
   bg->setRotation(0);
   auto m_act = Spawn::create( 
      FadeIn::create(0.5f),
      ScaleTo::create(0.5f, 1.0f),
      nullptr
   );
   bg->runAction(m_act);
}

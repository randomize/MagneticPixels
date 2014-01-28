#include "PitfallView.h"
#include "Pitfall.h"
#include "ContentManager.h"

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

   auto cb = ContentManager::getInstance().GetSimpleSprite("pitfall_bg");
   cb->setOpacity(rand() % 56 + 200);
   //cb->setBlendFunc(BlendFunc::ADDITIVE);

   cb->runAction(
      RepeatForever::create(
      Sequence::create(
         DelayTime::create(rand() % 100 / 20.0f),
         FadeTo::create(rand() % 100 / 40.0f, rand() % 156 + 100),
         DelayTime::create(rand() % 100 / 20.0f),
         FadeTo::create(0.3f, 255),
         nullptr
      )
      )
   );

   contents->addChild(cb);
}



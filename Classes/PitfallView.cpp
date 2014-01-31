#include "PitfallView.h"
#include "Pitfall.h"
#include "ContentManager.h"

using namespace MPix;


//====---------------------------------------------======//

EM_NODE_CHILD_CPP(PitfallView);

MPix::PitfallView::PitfallView()
{
  z_order = 0;
}

MPix::PitfallView::~PitfallView()
{
}


void MPix::PitfallView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);

   pixel = std::dynamic_pointer_cast<Pitfall>(model);

   //for (int i = 0; i < 2; ++i) {

   auto cb = ContentManager::getInstance().GetSimpleSprite("pitfall_bg");
   cb->setOpacity(rand() % 56 + 200);

      cb->runAction(
         RepeatForever::create(
            Sequence::create(
               DelayTime::create(1.0 + rand() % 100 / 30.0f),
               FadeTo::create(0.4f, rand() % 156 + 100),
               DelayTime::create(1.0 + rand() % 100 / 30.0f),
               FadeTo::create(0.4f, 255),
               nullptr
            )
         )
      );

      contents->addChild(cb);
   //}
}



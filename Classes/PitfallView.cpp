#include "PitfallView.h"
#include "Pitfall.h"
#include "ContentManager.h"

using namespace MPix;


//====---------------------------------------------======//

ECNODE_CHILD_CPP(PitfallView);

MPix::PitfallView::PitfallView()
{
  z_order = 0;
}

MPix::PitfallView::~PitfallView()
{
}

void SetMeRandomColor(Node* s) {
   s->setColor(Color3B(235+rand()%20, 235+rand()%20, 235+rand()%20));
}

float RandomDelay() {
   return 1.0f + RandomFloat(3.0f);
}

void MPix::PitfallView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);

   pixel = std::dynamic_pointer_cast<Pitfall>(model);

   //for (int i = 0; i < 2; ++i) {

   auto cb = ContentManager::getInstance().GetSimpleSprite("pitfall_bg");
   cb->setOpacity(rand() % 56 + 200);
   //cb->setBlendFunc({ GL_DST_COLOR, GL_ONE });
   //cb->setOpacityModifyRGB(true);
   SetMeRandomColor(cb);
   auto dt = DelayTime::create(RandomDelay());

      cb->runAction(
         RepeatForever::create(
            Sequence::create(
               dt,
               CallFunc::create(
                  [=](){
                     auto chose = rand() % 3;
                     if (chose == 1 ) {
                        SetMeRandomColor(cb);
                     }
                     else if (chose == 2) {
                        cb->setOpacity(255);
                     }
                     else {
                        dt->setDuration(RandomDelay());
                     }
                  }
               ),
               FadeTo::create(0.8f, rand() % 56 + 200),
               nullptr
            )
         )
      );

      // TODO: bad lamp sound + flickering

      contents->addChild(cb);
   //}
}



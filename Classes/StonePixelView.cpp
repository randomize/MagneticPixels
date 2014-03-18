#include "StonePixelView.h"
#include "StonePixel.h"
#include "GameStateManager.h"
#include "ContentManager.h"

using namespace MPix;

//====---------------------------------------------======//

EM_NODE_CHILD_CPP(StonePixelView);

const Point l_pos = {-11.62f, 6.62f};
const Point r_pos = {11.67f, 6.62f};


MPix::StonePixelView::StonePixelView()
{
   z_order = 11;

}

MPix::StonePixelView::~StonePixelView()
{

}

void MPix::StonePixelView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);

   pixel = std::dynamic_pointer_cast<StonePixel>(model);

   auto & cm = ContentManager::getInstance();

   auto cb = cm.GetSimpleSprite("stone_bg");

   left_eye = cm.GetSimpleSprite("stone_eye");
   right_eye = cm.GetSimpleSprite("stone_eye");
   left_eye->setPosition(l_pos);
   right_eye->setPosition(r_pos);

   contents->addChild(cb, 1);
   contents->addChild(left_eye, 1);
   contents->addChild(right_eye, 1);
}

void MPix::StonePixelView::PixelMoved()
{
   PixelView::PixelMoved();

   GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("stone_move"));

}

void MPix::StonePixelView::PixelTapped()
{
   ResetEyes();

   auto acty = Sequence::create(
         ScaleTo::create(0.2f, 0.5f),
         ScaleTo::create(0.2f, 1.0f),
         nullptr
      );
   left_eye->runAction(acty);
   right_eye->runAction(acty->clone());

}

void MPix::StonePixelView::PixelIdleTrick()
{
   ResetEyes();

   left_eye->runAction(Sequence::create(
      MoveTo::create(0.3f, l_pos + Point(-RandomFloat(2.0),RandomFloat(-2.0, 2.0))),
      DelayTime::create(1.6f),
      MoveTo::create(0.3f, l_pos),
      nullptr
   ));

   right_eye->runAction(Sequence::create(
      MoveTo::create(0.3f, r_pos + Point(RandomFloat(2.0),RandomFloat(-2.0, 2.0))),
      DelayTime::create(1.6f),
      MoveTo::create(0.3f, r_pos),
      nullptr
   ));
}

void MPix::StonePixelView::ResetEyes()
{
   left_eye->stopAllActions();
   right_eye->stopAllActions();

   left_eye->setPosition(l_pos);
   right_eye->setPosition(r_pos);
   left_eye->setScale(1.0);
   right_eye->setScale(1.0);
}

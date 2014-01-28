#include "CuteBlocksLogo.h"
#include "ContentManager.h"

// For logo... 
#include "MagneticPixel.h"
#include "MagneticPixelView.h"

using namespace MPix;

//====---------------------------------------------======//

const float LOGO_PANE_FONT_SIZE = 76.0f;
const float LOGO_PANE_PIXEL_OFFSET = 60.0f;

//====---------------------------------------------======//

bool MPix::CuteBlocksLogo::init()
{
   auto & cm = ContentManager::getInstance();

   auto text = LabelTTF::create("Cute Blocks", cm.GetLogoFont(), LOGO_PANE_FONT_SIZE);
   text->setColor(Color3B::BLACK);
   addChild(text, 1);

   pixel_holder = Node::create();
   pixel_holder->setScale(0.52f);
   pixel_holder->setPosition(65, -7);
   //auto s = Sprite::create("mark.png");
   //pixel_holder->addChild(s, 2);
   addChild(pixel_holder, 2);

   auto red = make_shared<MagneticPixel>(PixelColor::RED);
   p = red;

   pixel = PixelView::create(p);
   pixel->Build(p);
   pixel->BindContents(pixel_holder);
   //pixel->Update(CmdUIUpdatePixelView::Reason::CREATED);
   pixel->setPosition(Point(0, 0));


   auto listener = EventListenerTouchOneByOne::create();
   listener->setSwallowTouches(true);
   listener->onTouchBegan = CC_CALLBACK_2(CuteBlocksLogo::onTouchBegan, this);

   _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
   touch_events = listener;

   runAction(
      RepeatForever::create(
         Sequence::createWithTwoActions(
            DelayTime::create(5.0),
            CallFunc::create(CC_CALLBACK_0(CuteBlocksLogo::UpdateIdle, this))
         )
      )
   );

   egg = 0;

   return true;
}

CuteBlocksLogo* MPix::CuteBlocksLogo::create()
{
   auto fab = new CuteBlocksLogo();
   if (fab->init()) {
      fab->autorelease();
      return fab;
   }
   delete fab;
   return nullptr;
}

void MPix::CuteBlocksLogo::UpdateIdle()
{
   pixel->Update(CmdUIUpdatePixelView::Reason::WAKE);

   if (rand() % 3 == 1) {
      pixel->Update(CmdUIUpdatePixelView::Reason::SMILED);
   }

}

bool MPix::CuteBlocksLogo::onTouchBegan(Touch *touch, Event *event)
{
   auto pt = this->convertTouchToNodeSpace(touch);

   if ((pt - pixel_holder->getPosition()).getLength() < 50) {
      egg++;
      Point rand_offset((rand() % 3 - 1) * 48, (rand() % 3 - 1) * 48);
      pixel_holder->runAction(
         EaseBounceOut::create(
            MoveTo::create(0.6f, pixel_holder->getPosition() + rand_offset)
         )
      );

      if (egg > 5) {
         this->stopAllActions();
         dynamic_pointer_cast<IAlive>(p)->Kill(IAlive::State::KILLED_BY_NEEDLE);
         pixel->Update(CmdUIUpdatePixelView::Reason::DIED);
         _eventDispatcher->removeEventListener(touch_events);
      }

   }



   return false;

}

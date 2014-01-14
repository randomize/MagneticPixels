#include "SokobanPixelView.h"
#include "SokobanPixel.h"
#include "EMAnimation.h"


#include "EMAnimation.h"
#include "Shake.h"
#include "ContentManager.h"
#include "GameStateManager.h"

using namespace MPix;

//====---------------------------------------------======//

EM_NODE_CHILD_CPP(SokobanPixelView);

void MPix::SokobanPixelView::Build( shared_ptr<Pixel> model )
{
   PixelView::Build(model);
   pixel = std::dynamic_pointer_cast<SokobanPixel>(model);
   assert(pixel && "Pixel using this view must implement this interface");

   // Animated body
   body = ContentManager::getInstance().GetAnimation("socoban_0");
   //body = EMAnimation::create("Pixel");
   //body->SetHSV(PixelColorToHSV(pixel->GetColor()));
   //body->setScale(0.25f);

   // Setup contents
   contents->addChild(body);
}

MPix::SokobanPixelView::SokobanPixelView() :
   body(nullptr)
{
   z_order = 10;
}

//////////////////////////////////////////////////////////////////////////
// Animations

// Dispatcher
bool MPix::SokobanPixelView::Update( CmdUIUpdatePixelView::Reason reason )
{

   switch (reason)
   {
   case MPix::CmdUIUpdatePixelView::Reason::SMILED:
      PixelSmiled();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::UNSMILED:
      PixelUnsmiled();
      break;
   default:
      return PixelView::Update(reason);
   }
   return true;

}

void MPix::SokobanPixelView::PixelCreated()
{
   body->setScale(0.001f);
   auto sq = Sequence::create(
      DelayTime::create(rand()%100 / 100.0f),
      CallFunc::create( [&]() { body->setScale(1.0f * 0.25f); body->PlayNow("create"); } ),
      nullptr
      );
   RunLockingAction(body, sq);
}

void MPix::SokobanPixelView::PixelAccepted()
{
   auto m_act = Sequence::create(
      Spawn::create(
         ScaleTo::create(0.5f, 0.5f), // FIXME scale later fix
         FadeOut::create(0.5f),
         nullptr
      ),
      nullptr
   );
   body->runAction(m_act);
}

void MPix::SokobanPixelView::PixelReset()
{
   PixelView::PixelReset();
   body->PlayNow("asleep");
}

void MPix::SokobanPixelView::PixelDied()
{
   auto ast = pixel->GetLiveState();
   if (ast == IAlive::State::KILLED_BY_NEEDLE || ast == IAlive::State::KILLED_BY_STONE ) 
   {
      body->Play("die");
      auto sq = Sequence::create(
         DelayTime::create(0.2f),
         CallFunc::create( [&]() {
            body->setScale(1.0f * 0.25f);
            body->PlayNow("blood");
            GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("pop_" + ToString(rand()%3+1)));
         }),
         nullptr 
         );
      body->runAction(sq);
   } 
   else if (ast == IAlive::State::KILLED_BY_PITTRAP ) 
   {
      auto m_act = Spawn::create( 
         FadeOut::create(0.5f),
         ScaleTo::create(0.5f, 0.001f),
         RotateTo::create(0.5f, 180.0f),
         nullptr
       );
      body->runAction(m_act);
   }
   else 
   {
      PixelView::PixelDied();
   }
}

void MPix::SokobanPixelView::PixelResurrect()
{
   body->Play("create");
   body->setOpacity(255);
   body->setScale(0.001f);
   body->setRotation(0);
   auto m_act = ScaleTo::create(0.4f, 0.25f);
   body->runAction(m_act);
   if ( pixel->IsSmiling() ) {
      body->Play("happy");
   }
}

void MPix::SokobanPixelView::PixelSmiled()
{
   body->Play("smile");
   body->Play("happy");
}

void MPix::SokobanPixelView::PixelUnsmiled()
{
   body->PlayNow("unsmile");
}


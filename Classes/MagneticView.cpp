#include "MagneticView.h"

#include "IAssembled.h"

#include "EMAnimation.h"
#include "Shake.h"
#include "ContentManager.h"
#include "GameStateManager.h"
#include "SleepingZets.h"

using namespace MPix;

//====---------------------------------------------======//

MPix::MagneticView::MagneticView():
   body(nullptr),
   zzz(nullptr)
{
   z_order = 10;
}

MPix::MagneticView::~MagneticView()
{
}

void MPix::MagneticView::Build( shared_ptr<Pixel> model )
{

   PixelView::Build(model);

   asm_interface = std::dynamic_pointer_cast<IAssembled>(model);
   assert(asm_interface && "Pixel using this view must implement this interface");

   // Z letters
   zzz = SleepingZets::create();

   // Animated body
   body = ContentManager::getInstance().GetAnimation("magnetic_0");
   //body = EMAnimation::create("Pixel");
   //body->SetHSV(PixelColorToHSV(asm_interface->GetColor()));
   //body->setScale(0.25f);

   // Setup contents
   contents->addChild( zzz, 5);
   contents->addChild(body);
}

//////////////////////////////////////////////////////////////////////////
// Animations

// Dispatcher
bool MPix::MagneticView::Update( CmdUIUpdatePixelView::Reason reason )
{

   switch (reason)
   {
   case MPix::CmdUIUpdatePixelView::Reason::WAKE:
      PixelWake();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::ASLEEP:
      PixelAsleep();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::SMILED:
      PixelSmiled();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::UNSMILED:
      PixelUnsmiled();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::BLOCKING:
      PixelBlocks();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::CANFALL:
      PixelCanFallOn();
      break;
   case MPix::CmdUIUpdatePixelView::Reason::CANTFALL:
      PixelCanFallOff();
      break;
   default:
      return PixelView::Update(reason);
   }
   return true;

}

void MPix::MagneticView::PixelCreated()
{
   body->setScale(0.001f);
   auto sq = Sequence::create(
      DelayTime::create(rand()%100 / 100.0f),
      CallFunc::create( [&]() { body->setScale(1.0f * 0.25f); body->PlayNow("create"); } ),
      nullptr
      );
   RunLockingAction(body, sq);
   zzz->Show();
}

void MPix::MagneticView::PixelAccepted()
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

void MPix::MagneticView::PixelReset()
{
   PixelView::PixelReset();
   body->PlayNow("asleep");
   zzz->Show();
}

void MPix::MagneticView::PixelDied()
{
   auto ast = asm_interface->GetLiveState();
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
   zzz->Hide();
}

void MPix::MagneticView::PixelResurrect()
{
   body->Play("create");
   body->setOpacity(255);
   body->setScale(0.001f);
   body->setRotation(0);
   auto m_act = ScaleTo::create(0.4f, 0.25f);
   body->runAction(m_act);
   if ( asm_interface->IsSmiling() ) {
      body->Play("happy");
   }
   if ( asm_interface->IsInAssembly() == false ) {
      zzz->Show();
   }
}

void MPix::MagneticView::PixelSmiled()
{
   body->Play("smile");
   body->Play("happy");
}

void MPix::MagneticView::PixelUnsmiled()
{
   if ( asm_interface->IsInAssembly() )
      body->PlayNow("unsmile");
}

void MPix::MagneticView::PixelWake()
{
   body->PlayLocked("wake");
   zzz->Hide();
}

void MPix::MagneticView::PixelAsleep()
{
   body->PlayNow("asleep");
   zzz->Show();
}

void MPix::MagneticView::PixelBlocks()
{
   GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("bump"));
   body->PlayNow("shocked");
   RunLockingAction(body,  Shake::create(0.2f, 2.0f));
   if ( asm_interface->IsSmiling() ) {
      body->Play("happy");
   }
}

void MPix::MagneticView::PixelCanFallOn()
{
   auto sq = RepeatForever::create(Sequence::create(
      ScaleTo::create(0.2f, 0.95f),
      ScaleTo::create(0.2f, 0.9f),
      nullptr
   ));
   sq->setTag(456);
   contents->runAction(sq);
}

void MPix::MagneticView::PixelCanFallOff()
{
   contents->stopActionByTag(456);
   auto sq = ScaleTo::create(0.2f,1.0f);
   contents->runAction(sq);
}


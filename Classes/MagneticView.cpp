#include "MagneticView.h"

#include "IAssembled.h"

#include "EMAnimation.h"
#include "Shake.h"
#include "ContentManager.h"
#include "GameStateManager.h"
#include "SleepingZets.h"
#include "HSVSprite.h"

using namespace MPix;

//====---------------------------------------------======//

MPix::MagneticView::MagneticView():
   mimics(nullptr),
   bg(nullptr),
   body(nullptr),
   smash(nullptr),
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

   auto& cm = ContentManager::getInstance();

   // Z letters
   zzz = SleepingZets::create();

   // Animated face
   mimics = cm.GetAnimation("magnetic_0");

   // Colored background
   bg = cm.GetHSVSprite("magnetic_bg_norm");
   bg->SetHSV(PixelColorToHSV(asm_interface->GetColor()));
   //bg->setScale(0.25f);

   // Dead state
   smash = cm.GetHSVSprite("magnetic_bg_smash");
   smash->SetHSV(PixelColorToHSV(asm_interface->GetColor()));

   // Body
   body = Node::create();
   body->addChild(bg, 1);
   body->addChild(mimics, 2);
   body->addChild(smash, 3);

   // Setup contents
   contents->addChild( zzz, 5);
   contents->addChild( body, 1);
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
   smash->setOpacity(0);
   bg->setVisible(true);
   bg->setScale(0.01f);
   auto sq = Sequence::create(
      DelayTime::create(rand()%100 / 100.0f),
      CallFunc::create([&]() { 
         bg->setScale(1.0f);
         mimics->PlayNow("create");
         mimics->Play("sleeping");
      }),
      nullptr
   );
   RunLockingAction(bg, sq);
   zzz->Show();
}

void MPix::MagneticView::PixelAccepted()
{
   auto m_act = Sequence::create(
      Spawn::create(
         ScaleTo::create(0.5f, 2.0f),
         FadeOut::create(0.5f),
         nullptr
      ),
      nullptr
   );
   mimics->runAction(m_act);
   bg->runAction(Sequence::create(
      Spawn::create(
         ScaleTo::create(0.5f, 2.0f),
         FadeOut::create(0.5f),
         nullptr
      ),
      nullptr
   ));
}

void MPix::MagneticView::PixelReset()
{
   PixelView::PixelReset();
   smash->setOpacity(0);
   bg->setVisible(true);
   bg->setScale(1.0f);
   mimics->PlayNow("asleep");
   zzz->Show();
}

void MPix::MagneticView::PixelDied()
{
   auto ast = asm_interface->GetLiveState();
   if (ast == IAlive::State::KILLED_BY_NEEDLE || ast == IAlive::State::KILLED_BY_STONE ) 
   {
      mimics->PlayNow("die");
      auto sq = Sequence::create(
         DelayTime::create(0.2f),
         CallFunc::create( [&]() {
            mimics->setScale(1.0f);
            body->setVisible(false);
            smash->setOpacity(255);
            smash->runAction(FadeOut::create(0.2f));
            GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("pop_" + ToString(rand()%3+1)));
         }),
         nullptr 
         );
      mimics->runAction(sq);
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
   mimics->Play("create");
   mimics->setOpacity(255);
   mimics->setScale(0.001f);
   mimics->setRotation(0);
   auto m_act = ScaleTo::create(0.4f, 1.0);
   mimics->runAction(m_act);
   if ( asm_interface->IsSmiling() ) {
      mimics->Play("happy");
   }
   if ( asm_interface->IsInAssembly() == false ) {
      zzz->Show();
   }
}

void MPix::MagneticView::PixelSmiled()
{
   mimics->PlayNow("smile");
   mimics->Play("happy");
}

void MPix::MagneticView::PixelUnsmiled()
{
   if ( asm_interface->IsInAssembly() )
      mimics->PlayNow("unsmile");
}

void MPix::MagneticView::PixelWake()
{
   mimics->PlayNow("wake");
   zzz->Hide();
   RunLockingAction(contents,  DelayTime::create(0.2f));
}

void MPix::MagneticView::PixelAsleep()
{
   mimics->PlayNow("asleep");
   zzz->Show();
}

void MPix::MagneticView::PixelBlocks()
{
   GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("bump"));
   mimics->PlayNow("shocked");
   RunLockingAction(mimics,  Shake::create(0.2f, 2.0f));
   if ( asm_interface->IsSmiling() ) {
      mimics->Play("happy");
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


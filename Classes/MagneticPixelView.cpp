#include "MagneticPixelView.h"
#include "MagneticPixel.h"

#include "EMAnimation.h"
#include "Shake.h"
#include "ContentManager.h"
#include "GameStateManager.h"
#include "SleepingZets.h"
#include "HSVSprite.h"

using namespace MPix;

EM_NODE_CHILD_CPP(MagneticPixelView);


MPix::MagneticPixelView::MagneticPixelView():
   zzz(nullptr),
   body(nullptr),
   mimics(nullptr),
   bg(nullptr),
   smash(nullptr)
{
   z_order = 10;
}

MPix::MagneticPixelView::~MagneticPixelView()
{
}

void MPix::MagneticPixelView::Build( shared_ptr<Pixel> model )
{

   PixelView::Build(model);

   pixel = std::dynamic_pointer_cast<MagneticPixel>(model);

   auto& cm = ContentManager::getInstance();

   // Z letters
   zzz = SleepingZets::create();

   // Animated face
   mimics = cm.GetAnimation("magnetic_mimics");

   // Colored background
   bg = cm.GetHSVSprite("magnetic_bg_norm");
   bg->SetHSV(PixelColorToHSV(pixel->GetColor()));
   //bg->setScale(0.25f);

   // Dead state
   smash = cm.GetHSVSprite("magnetic_bg_smash");
   smash->SetHSV(PixelColorToHSV(pixel->GetColor()));

   // Body
   body = Node::create();
   body->addChild(bg, 1);
   body->addChild(mimics, 2);
   body->addChild(smash, 3);
   body->setCascadeOpacityEnabled(true);

   // Setup contents
   contents->addChild( zzz, 5);
   contents->addChild( body, 1);

   // Initial 
   smash->setOpacity(0);
   mimics->PlayNow("asleep");
}

//////////////////////////////////////////////////////////////////////////
// Animations

// Dispatcher
bool MPix::MagneticPixelView::Update( CmdUIUpdatePixelView::Reason reason )
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

void MPix::MagneticPixelView::PixelCreated()
{
   zzz->Hide();
   smash->setOpacity(0);

   body->setScale(0.01f);
   body->setRotation(180.0f);
   mimics->setVisible(false);
   auto sq = Sequence::create(
      DelayTime::create(rand()%100 / 150.0f),
      Spawn::create(
         ScaleTo::create(0.4f, 1.0f),
         RotateTo::create(0.3f, 0),
         nullptr
      ),
      CallFunc::create([&]() { 
         mimics->setVisible(true);
         mimics->PlayNow("create");
         mimics->Play("sleeping");
         zzz->Show();
      }),
      nullptr
   );
   RunLockingAction(body, sq);
}

void MPix::MagneticPixelView::PixelAccepted()
{
   auto m_act = Sequence::create(
      Spawn::create(
         ScaleTo::create(0.5f, 2.0f),
         FadeOut::create(0.5f),
         nullptr
      ),
      nullptr
   );
   body->runAction(m_act);
}

void MPix::MagneticPixelView::PixelReset()
{
   PixelView::PixelReset();
   body->setOpacity(255);
   body->setScale(1.0);
   smash->setOpacity(0);
   bg->setVisible(true);
   bg->setScale(1.0f);
   mimics->PlayNow("asleep");
   zzz->Show();
}

void MPix::MagneticPixelView::PixelDied()
{
   auto ast = pixel->GetLiveState();
   if (ast == IAlive::State::KILLED_BY_NEEDLE) 
   {
      mimics->PlayNow("die");
      auto sq = Sequence::create(
         DelayTime::create(0.2f),
         Hide::create(),
         CallFunc::create( [&]() {
            bg->setVisible(false);
            smash->setOpacity(255);
            smash->runAction(FadeOut::create(0.5f));
            GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("pop_" + ToString(rand()%3+1)));
         }),
         nullptr 
         );
      mimics->runAction(sq);
   } 
   else if (ast == IAlive::State::KILLED_BY_STONE ) 
   {
      auto sq = Sequence::create(
         Hide::create(),
         CallFunc::create( [&]() {
            bg->setVisible(false);
            smash->setOpacity(255);
            smash->runAction(FadeOut::create(0.5f));
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
         ScaleTo::create(0.5f, 0.01f),
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

void MPix::MagneticPixelView::PixelResurrect()
{
   zzz->Hide();
   smash->setOpacity(0);
   bg->setVisible(true);

   body->setScale(0.01f);
   body->setRotation(180.0f);
   body->setOpacity(255);
   mimics->setVisible(false);
   auto sq = Sequence::create(
      Spawn::create(
         ScaleTo::create(0.3f, 1.0f),
         RotateTo::create(0.2f, 0),
         nullptr
      ),
      CallFunc::create([&]() { 
         mimics->setVisible(true);
         mimics->PlayNow("create");
         if ( pixel->IsSmiling() ) {
            mimics->Play("happy");
         }
         if ( pixel->IsInAssembly() == false ) {
            zzz->Show();
            mimics->Play("sleeping");
         } else {
            mimics->Play("wake");
         }
      }),
      nullptr
   );
   RunLockingAction(body, sq);

}

void MPix::MagneticPixelView::PixelSmiled()
{
   mimics->PlayNow("smile");
   mimics->Play("happy");
}

void MPix::MagneticPixelView::PixelUnsmiled()
{
   if ( pixel->IsInAssembly() )
      mimics->PlayNow("unsmile");
}

void MPix::MagneticPixelView::PixelWake()
{
   mimics->PlayNow("wake");
   zzz->Hide();
   RunLockingAction(contents,  DelayTime::create(0.2f));
}

void MPix::MagneticPixelView::PixelAsleep()
{
   mimics->PlayNow("asleep");
   mimics->Play("sleeping");
   zzz->Show();
}

void MPix::MagneticPixelView::PixelBlocks()
{
   GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("bump"));
   mimics->PlayNow("shocked");
   RunLockingAction(mimics,  Shake::create(0.2f, 2.0f));
   if ( pixel->IsSmiling() ) {
      mimics->Play("happy");
   }
}

void MPix::MagneticPixelView::PixelCanFallOn()
{
   auto sq = RepeatForever::create(Sequence::create(
      ScaleTo::create(0.2f, 0.95f),
      ScaleTo::create(0.2f, 0.9f),
      nullptr
   ));
   sq->setTag(456);
   contents->runAction(sq);
}

void MPix::MagneticPixelView::PixelCanFallOff()
{
   contents->stopActionByTag(456);
   auto sq = ScaleTo::create(0.2f,1.0f);
   contents->runAction(sq);
}

void MPix::MagneticPixelView::PixelIdleTrick()
{
   if (pixel->IsInAssembly() && pixel->IsAlive()) {
      string s("idle_" + ToString(rand() % 8));
      mimics->Play(s);
   }

}









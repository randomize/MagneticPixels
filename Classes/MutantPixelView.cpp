
#include "MutantPixelView.h"
#include "MutantPixel.h"

#include "ECAnimation.h"
#include "Shake.h"
#include "ContentManager.h"
#include "GameStateManager.h"
#include "SleepingZets.h"
#include "HSVSprite.h"

using namespace MPix;

ECNODE_CHILD_CPP(MutantPixelView);


MPix::MutantPixelView::MutantPixelView():
   zzz(nullptr),
   body(nullptr),
   mimics(nullptr),
   bg(nullptr),
   smash(nullptr)
{
   z_order = 10;
}

MPix::MutantPixelView::~MutantPixelView()
{
}

void MPix::MutantPixelView::Build( shared_ptr<Pixel> model )
{

   PixelView::Build(model);

   pixel = std::dynamic_pointer_cast<MutantPixel>(model);
   assert(pixel);

   auto& cm = ContentManager::getInstance();

   // Z letters
   zzz = SleepingZets::create();
   //zzz->setGlobalZOrder(-1);

   // Animated face
   mimics = cm.GetAnimation("magnetic_mimics");

   // Colored background
   bg = cm.GetHSVSprite("magnetic_bg_norm");

   // Dots
   dots = cm.GetHSVSprite("mutant_dots");

   // Dead state
   smash = cm.GetHSVSprite("magnetic_bg_smash");
   smash->SetHSV(PixelColorToHSV(pixel->GetColor()));

   // Body
   body = Node::create();
   body->addChild(bg, 1);
   body->addChild(dots, 2);
   body->addChild(mimics, 3);
   body->addChild(smash, 4);
   body->setCascadeOpacityEnabled(true);
   body->setCascadeColorEnabled(true);

   // Setup contents
   contents->addChild( zzz, 5);
   contents->addChild( body, 1);

   // Init color
   UpdateColors();

   // Initial 
   smash->setOpacity(0);
   mimics->PlayNow("asleep");
}

//////////////////////////////////////////////////////////////////////////
// Animations

// Dispatcher
bool MPix::MutantPixelView::Update( CmdUIUpdatePixelView::Reason reason )
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

void MPix::MutantPixelView::PixelCreated()
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
      CallFunc::create([=]() { 
         mimics->setVisible(true);
         mimics->PlayNow("create");
         mimics->Play("sleeping");
         zzz->Show();
      }),
      nullptr
   );
   RunLockingAction(body, sq);
}

void MPix::MutantPixelView::PixelAccepted()
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

void MPix::MutantPixelView::PixelReset()
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

void MPix::MutantPixelView::PixelDied()
{
   auto ast = pixel->GetLiveState();
   switch (ast) {
   case IAlive::State::KILLED_BY_NEEDLE:
   {
      mimics->PlayNow("die");
      auto sq = Sequence::create(
         DelayTime::create(0.2f),
         Hide::create(),
         CallFunc::create( [=]() {
            bg->setVisible(false);
            smash->setOpacity(255);
            smash->runAction(FadeOut::create(0.5f));
            dots->runAction(FadeOut::create(0.5f));
            GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("pop_" + ToString(rand()%3+1)));
         }),
         nullptr 
         );
      mimics->runAction(sq);
      break;
   } 
   case IAlive::State::KILLED_BY_STONE :
   {
      auto sq = Sequence::create(
         Hide::create(),
         CallFunc::create( [=]() {
            bg->setVisible(false);
            smash->setOpacity(255);
            smash->runAction(FadeOut::create(0.5f));
            GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("pop_" + ToString(rand()%3+1)));
         }),
         nullptr 
         );
      mimics->runAction(sq);
      break;
   }
   case IAlive::State::KILLED_BY_PITTRAP:
   {
      auto m_act = Spawn::create( 
         FadeOut::create(0.5f),
         ScaleTo::create(0.5f, 0.01f),
         RotateTo::create(0.5f, 180.0f),
         nullptr
       );
      body->runAction(m_act);
      break;
   }
   case IAlive::State::KILLED_BY_EXPLOSION: {
      contents->setColor(Color3B::BLACK);
      contents->runAction(FadeOut::create(0.5f));
      break;
   }
   default:
   {
      PixelView::PixelDied();
   }
   }
   zzz->Hide();
}

void MPix::MutantPixelView::PixelResurrect()
{
   contents->setOpacity(255);
   contents->setColor(Color3B::WHITE);
   zzz->Hide();
   smash->setOpacity(0);
   bg->setVisible(true);
   dots->setOpacity(255);

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
      CallFunc::create([=]() { 
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

void MPix::MutantPixelView::PixelSmiled()
{
   mimics->PlayNow("smile");
   mimics->Play("happy");
}

void MPix::MutantPixelView::PixelUnsmiled()
{
   if ( pixel->IsInAssembly() )
      mimics->PlayNow("unsmile");
}

void MPix::MutantPixelView::PixelWake()
{
   mimics->PlayNow("wake");
   zzz->Hide();
   RunLockingAction(contents,  DelayTime::create(0.2f));
}

void MPix::MutantPixelView::PixelAsleep()
{
   mimics->PlayNow("asleep");
   mimics->Play("sleeping");
   zzz->Show();
}

void MPix::MutantPixelView::PixelBlocks()
{
   GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("bump"));
   mimics->PlayNow("shocked");
   RunLockingAction(mimics,  Shake::create(0.2f, 2.0f));
   if ( pixel->IsSmiling() ) {
      mimics->Play("happy");
   }
}

void MPix::MutantPixelView::PixelCanFallOn()
{
   auto sq = RepeatForever::create(Sequence::create(
      ScaleTo::create(0.2f, 0.95f),
      ScaleTo::create(0.2f, 0.9f),
      nullptr
   ));
   sq->setTag(456);
   contents->runAction(sq);
}

void MPix::MutantPixelView::PixelCanFallOff()
{
   contents->stopActionByTag(456);
   auto sq = ScaleTo::create(0.2f,1.0f);
   contents->runAction(sq);
}

void MPix::MutantPixelView::PixelIdleTrick()
{
   if (pixel->IsInAssembly() && pixel->IsAlive() && pixel->IsSmiling() == false) {
      string s("idle_" + ToString(rand() % 8));
      mimics->Play(s);
   }

}

void MPix::MutantPixelView::PixelTapped()
{
   if ( pixel->IsInAssembly() && pixel->IsAlive() && pixel->IsSmiling() == false ) {
      mimics->Play("shocked"); // Shocked on click )
   }

}

void MPix::MutantPixelView::PixelChanged()
{
   bg->runAction(Sequence::create(
      FadeTo::create(0.2f, 50),
      CallFunc::create([this]() {UpdateColors(); }),
      FadeTo::create(0.2f, 255),
      nullptr
   ));
}

void MPix::MutantPixelView::UpdateColors()
{
   // Main color
   const float COLOR_VARIACE = 0.04f; // %
   auto color = PixelColorToHSV(pixel->GetColor());
   color.saturation += RandomFloat(-COLOR_VARIACE, COLOR_VARIACE);
   color.value      += RandomFloat(-COLOR_VARIACE, COLOR_VARIACE);
   color.hue        += RandomFloat(-COLOR_VARIACE, COLOR_VARIACE);
   bg->SetHSV(color);
   smash->SetHSV(PixelColorToHSV(pixel->GetColor()));

   // Dots color
   color = PixelColorToHSV(pixel->GetNextColor());
   color.saturation += RandomFloat(-COLOR_VARIACE, COLOR_VARIACE);
   color.value      += RandomFloat(-COLOR_VARIACE, COLOR_VARIACE);
   color.hue        += RandomFloat(-COLOR_VARIACE, COLOR_VARIACE);
   dots->SetHSV(color);
}












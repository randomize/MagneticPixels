#include "BomberPixelView.h"

#include "BomberPixel.h"

#include "EMAnimation.h"
#include "Shake.h"
#include "ContentManager.h"
#include "GameStateManager.h"
#include "SleepingZets.h"
#include "HSVSprite.h"

using namespace MPix;

EM_NODE_CHILD_CPP(BomberPixelView);


MPix::BomberPixelView::BomberPixelView():
   bomb(nullptr),
   body(nullptr),
   mimics(nullptr),
   bg(nullptr),
   smash(nullptr)
{
   z_order = 11;
}

MPix::BomberPixelView::~BomberPixelView()
{
}

void MPix::BomberPixelView::Build( shared_ptr<Pixel> model )
{

   PixelView::Build(model);

   pixel = std::dynamic_pointer_cast<BomberPixel>(model);

   auto& cm = ContentManager::getInstance();

   // Animated face
   mimics = cm.GetAnimation("magnetic_mimics");
   mimics->setVisible(false);

   // Colored background
   bg = cm.GetHSVSprite("magnetic_bg_norm");
   const float COLOR_VARIACE = 0.04f; // %
   auto color = PixelColorToHSV(pixel->GetColor());
   color.saturation += RandomFloat(-COLOR_VARIACE, COLOR_VARIACE);
   color.value      += RandomFloat(-COLOR_VARIACE, COLOR_VARIACE);
   color.hue        += RandomFloat(-COLOR_VARIACE, COLOR_VARIACE);
   bg->SetHSV(color);
   bg->setVisible(false);

   // Bomb
   bomb = cm.GetSimpleSprite("bomber_bg");
   bomb->setPosition(Vector2::ZERO);

   bomb_eyes = cm.GetHSVSprite("bomber_eyes");
   bomb_eyes->SetHSV(color);
   bomb_eyes->setPosition(Vector2::ZERO);

   // Label
   label = LabelTTF::create(ToString(pixel->GetTime()), cm.GetBaseFont(), 32.0f);
   label->setColor(Color3B::WHITE);
   label->setPosition(Vector2(0,-15));

   // Dead state
   smash = cm.GetHSVSprite("magnetic_bg_smash");
   smash->SetHSV(PixelColorToHSV(pixel->GetColor()));

   // Body
   body = Node::create();
   body->addChild(bg, 1);
   body->addChild(mimics, 2);
   body->addChild(smash, 3);
   body->addChild(bomb, 4);
   body->addChild(bomb_eyes, 5);
   body->addChild(label, 6);
   body->setCascadeOpacityEnabled(true);
   body->setCascadeColorEnabled(true);

   // Setup contents
   contents->addChild( body, 1);

   // Initial 
   smash->setOpacity(0);
   mimics->PlayNow("asleep");
}

//////////////////////////////////////////////////////////////////////////
// Animations

// Dispatcher
bool MPix::BomberPixelView::Update( CmdUIUpdatePixelView::Reason reason )
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

void MPix::BomberPixelView::PixelCreated()
{
   smash->setOpacity(0);

   body->setScale(0.01f);
   mimics->setVisible(false);
   bg->setVisible(false);

   auto sq = Sequence::create(
      DelayTime::create(rand()%100 / 150.0f),
      ScaleTo::create(0.4f, 1.0f),
      nullptr
   );
   RunLockingAction(body, sq);
}

void MPix::BomberPixelView::PixelAccepted()
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

void MPix::BomberPixelView::PixelReset()
{

   auto sq = Sequence::create(
      Shake::create(0.5,10), 
      MoveTo::create(0.1f, LogicToScreen(model->GetPos()) + MPIX_CELL_SIZE_HALF_P),
      CallFunc::create([this](){
         body->setOpacity(255);
         body->setScale(1.0);
         smash->setOpacity(0);

         bg->setVisible(false);
         mimics->setVertexZ(false);
         bomb->setOpacity(255);
         bomb_eyes->setVisible(true);
         label->setVisible(true);
      }),
      nullptr
      );
   RunLockingAction(contents, sq);

}

void MPix::BomberPixelView::PixelDied()
{
   auto ast = pixel->GetLiveState();
   if (ast == IAlive::State::KILLED_BY_NEEDLE) 
   {
      mimics->PlayNow("die");
      auto sq = Sequence::create(
         DelayTime::create(0.2f),
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
   } 
   else if (ast == IAlive::State::KILLED_BY_STONE ) 
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
   else if (ast == IAlive::State::KILLED_BY_EXPLOSION)
   {
      //TODO: particle effect
      body->runAction(FadeOut::create(0.5f));
      GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("boom_" + ToString(rand() % 3 + 1)));
   }
   else 
   {
      PixelView::PixelDied();
   }
}

void MPix::BomberPixelView::PixelResurrect()
{
   smash->setOpacity(0);
   mimics->setVisible(false);

   if (pixel->IsInAssembly() == false) {
      bg->setVisible(false);
      bomb->setOpacity(255);
      label->setVisible(true);
      bomb_eyes->setVisible(true);
   }
   else {
      bg->setVisible(true);
      bomb->setOpacity(0);
      label->setVisible(false);
      bomb_eyes->setVisible(false);
   }

   body->setScale(0.01f);
   body->setOpacity(255);

   auto sq = Sequence::create(
      ScaleTo::create(0.3f, 1.0f),
      CallFunc::create([=]() { 

      if (pixel->IsInAssembly() == true) {
         mimics->setVisible(true);
         if ( pixel->IsSmiling() ) {
            mimics->Play("happy");
         }
         mimics->Play("wake");
      }

      }),
      nullptr
   );
   RunLockingAction(body, sq);

}

void MPix::BomberPixelView::PixelSmiled()
{
   mimics->PlayNow("smile");
   mimics->Play("happy");
}

void MPix::BomberPixelView::PixelUnsmiled()
{
   if ( pixel->IsInAssembly() )
      mimics->PlayNow("unsmile");
}

void MPix::BomberPixelView::PixelWake()
{
   bomb->runAction(FadeOut::create(0.2f));
   bomb_eyes->setVisible(false);
   label->setVisible(false);
   bg->setVisible(true);
   mimics->setVisible(true);
   mimics->PlayNow("wake");

   RunLockingAction(contents,  DelayTime::create(0.2f));
}

void MPix::BomberPixelView::PixelAsleep()
{
   bomb->runAction(FadeIn::create(0.2f));
   bomb_eyes->setVisible(true);
   label->setVisible(true);
   bg->setVisible(false);
   mimics->setVisible(false);
}

void MPix::BomberPixelView::PixelBlocks()
{
   GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("bump"));
   mimics->PlayNow("shocked");
   RunLockingAction(mimics,  Shake::create(0.2f, 2.0f));
   if ( pixel->IsSmiling() ) {
      mimics->Play("happy");
   }
}

void MPix::BomberPixelView::PixelCanFallOn()
{
   auto sq = RepeatForever::create(Sequence::create(
      ScaleTo::create(0.2f, 0.95f),
      ScaleTo::create(0.2f, 0.9f),
      nullptr
   ));
   sq->setTag(456);
   contents->runAction(sq);
}

void MPix::BomberPixelView::PixelCanFallOff()
{
   contents->stopActionByTag(456);
   auto sq = ScaleTo::create(0.2f,1.0f);
   contents->runAction(sq);
}

void MPix::BomberPixelView::PixelIdleTrick()
{
   if (pixel->IsInAssembly() && pixel->IsAlive() && pixel->IsSmiling() == false) {
      string s("idle_" + ToString(rand() % 8));
      mimics->Play(s);
   }

}

void MPix::BomberPixelView::PixelTapped()
{
   if (pixel->IsInAssembly() == false) {
      label->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2f, 1)));
   } else if ( pixel->IsAlive() && pixel->IsSmiling() == false) {
      mimics->Play("idle_5"); // Blink
   }
}

void MPix::BomberPixelView::PixelChanged()
{
   label->setString(ToString(pixel->GetTime()));
}











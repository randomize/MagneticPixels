#include "SleepingZets.h"
#include "MPix.h"

using namespace MPix;

//====---------------------------------------------======//


SleepingZets* MPix::SleepingZets::create()
{
   auto fab = new SleepingZets();
   if (fab->init()) {
      fab->autorelease();
      return fab;
   }
   delete fab;
   return nullptr;
}

void MPix::SleepingZets::Hide()
{
   zzz->stopSystem();
   setVisible(false);
}

void MPix::SleepingZets::Show()
{
   zzz->resetSystem();
   setVisible(true);
}

bool MPix::SleepingZets::init()
{

   zzz = ParticleSmoke::create();
   auto texture = Director::getInstance()->getTextureCache()->addImage("zee.png");
      
   zzz->setTexture( texture );
   zzz->setEmitterMode( ParticleSystem::Mode::GRAVITY );
   zzz->setEmissionRate(1.0f);
  // zzz->setTotalParticles(250);
   zzz->setPositionType(ParticleSystem::PositionType::RELATIVE);
   zzz->setPosVar(Vector2::ZERO);
   zzz->setAngle(60.0f);
   zzz->setAngleVar(0);
   zzz->setLife(3);
   zzz->setLifeVar(0.25f);
   zzz->setStartSize(1.0f);
   zzz->setStartSizeVar(0);
   zzz->setEndSize(20.0f);
   zzz->setEndSizeVar(0);
   zzz->setSpeed(10);
   zzz->setSpeedVar(0);
   zzz->setPosition(MPIX_CELL_SIZE_HALF_P*0.5f );
   zzz->stopSystem();

   addChild( zzz, 5);

   return true;
}

#include "EMAnimation.h"

#include "HSVArmature.h"
#include "GameStateManager.h"

using namespace MPix;

//====---------------------------------------------======//


MPix::EMAnimation* MPix::EMAnimation::create( const char* arm_name )
{
   auto fab = new EMAnimation;
   if (fab->init(arm_name)) {
      fab->autorelease();
      return fab;
   }
   delete fab;
   return nullptr;
}

bool MPix::EMAnimation::init( const char* arm_name )
{
   armature = HSVArmature::create(arm_name);
   assert(armature);
   this->setCascadeOpacityEnabled(true);
   armature->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);
   armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(EMAnimation::animationNext));
   islocking = false;
   isplaying = true;
   addChild(armature);
   return true;
}

void MPix::EMAnimation::Play( const char* name )
{
   ani_queue.push_back(make_pair(false, string(name)));
   if (!isplaying)
      ProcessOne();
}

void MPix::EMAnimation::PlayLocked( const char* name )
{
   ani_queue.push_back(make_pair(true, string(name)));
   if (!isplaying)
      ProcessOne();
}

void MPix::EMAnimation::PlayNow( const char* name )
{
   if (islocking) {
      islocking = false;
      GameStateManager::getInstance().Busy(false);
   }

   ani_queue.clear();
   armature->getAnimation()->stop();
   isplaying = true;
   armature->getAnimation()->play(name);
}


void MPix::EMAnimation::animationNext( Armature *armature, MovementEventType movementType, const char *movementID )
{
    if (movementType == COMPLETE)
    {
       if (ani_queue.empty() == false) {
          ProcessOne();
       } else {
          isplaying = false;
          if (islocking) {
             islocking = false;
             GameStateManager::getInstance().Busy(false);
          }
       }
    }
}

void MPix::EMAnimation::SetHSV( HSVColor color )
{
   armature->SetHSV(color);
}

void MPix::EMAnimation::ProcessOne()
{
   auto pr = ani_queue.front();
   ani_queue.pop_front();

   if (islocking != pr.first) {
      if (islocking) {
         GameStateManager::getInstance().Busy(false);
      }
      else {
         GameStateManager::getInstance().Busy(true);
      }
      islocking = pr.first;
   }
 
   armature->getAnimation()->play(pr.second.c_str());
   isplaying = true;

}








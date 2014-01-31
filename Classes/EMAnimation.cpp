#include "EMAnimation.h"

#include "HSVArmature.h"
#include "GameStateManager.h"

using namespace MPix;

//====---------------------------------------------======//


MPix::EMAnimation* MPix::EMAnimation::create( const string& arm_name )
{
   auto fab = new EMAnimation;
   if (fab->initWithArmatureName(arm_name)) {
      fab->autorelease();
      return fab;
   }
   delete fab;
   return nullptr;
}

bool MPix::EMAnimation::initWithArmatureName( const string& arm_name )
{
   if (Node::init()) {

      armature = Armature::create(arm_name);
      if (armature == nullptr) {
         EM_LOG_ERROR("Cannot create " + arm_name + " armature ");
         return false;
      }
      armature->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);
      armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(EMAnimation::animationNext, this));
      addChild(armature);

      this->setCascadeOpacityEnabled(true);
      islocking = false;
      isplaying = true;
      return true;
   }
   return false;
}

void MPix::EMAnimation::Play( const string& name )
{
   ani_queue.push_back(make_pair(false, name));
   if (!isplaying)
      ProcessOne();
}

void MPix::EMAnimation::PlayLocked( const string& name )
{
   ani_queue.push_back(make_pair(true, name));
   if (!isplaying)
      ProcessOne();
}

void MPix::EMAnimation::PlayNow( const string& name )
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


void MPix::EMAnimation::animationNext( Armature *armature, MovementEventType movementType, const std::string& movementID )
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








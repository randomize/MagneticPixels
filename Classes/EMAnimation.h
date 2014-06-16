//===-- Animation.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/01
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Wrapper for cocos animation objects
//
//===---------------------------------------------------------===//

#pragma once
#ifndef ECANIMATION_H_
#define ECANIMATION_H_

#include "ECBase.h"

namespace MPix {

   // Animation
   class ECAnimation : public Node
   {
   public:

      static ECAnimation* create( const string& arm_name );

      // Plays or schedules to queue and play after current
      void Play( const string& name );

      // Plays forcedly this, resetting all locks
      void PlayNow( const string& name );

      // Plays or schedules but locks UI
      void PlayLocked( const string& name );

   protected:

      bool initWithArmatureName(const string& arm_name);

      void animationNext(Armature *armature, MovementEventType movementType, const std::string& movementID);

      list<std::pair<bool,string>> ani_queue;

      void ProcessOne();

      Armature* armature;

      bool isplaying;
      bool islocking;

   };

}


#endif // ECANIMATION_H_

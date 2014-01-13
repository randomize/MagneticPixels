//===-- SleepingZets.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/01
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Sleeping xets used by many pixels
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SLEEPINGZETS_H_
#define SLEEPINGZETS_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies


   // SleepingZets

   class SleepingZets : public Node
   {
   public:

      static SleepingZets* create();
      bool init();

      // Show particles
      void Hide();

      // Hide particles
      void Show();

   private:

      ParticleSystemQuad* zzz;

   };

}


#endif // SLEEPINGZETS_H_
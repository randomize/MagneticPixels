//===-- EMBaseMasterLoop.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/12/04
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: This is only one singleton that holds all parts of game
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EMBASEMASTERLOOP_H_
#define EMBASEMASTERLOOP_H_

#include "EMBase.h"

namespace MPix {

   // EMBaseMasterLoop

   class EMBaseMasterLoop : public Node
   {
   public:

      EMBaseMasterLoop();

      // Method instantiates all singletones in required order
      void InitGame();

      // Method called once the game started
      void RunGame();

      // Test
      static void PrintTime();
      static double GetTime();

   private:

      // Called on each frame
      void update(float t) override;

      // 
      void MasterTick(float t);

      // Timer
      static double time_runs;

   };

}


#endif // EMBASEMASTERLOOP_H_

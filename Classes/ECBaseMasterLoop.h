//===-- ECBaseMasterLoop.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/12/04
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: This is only one singleton that holds all parts of game
//
//===---------------------------------------------------------===//

#pragma once
#ifndef ECBASEMASTERLOOP_H_
#define ECBASEMASTERLOOP_H_

#include "ECBase.h"

namespace MPix {

   // ECBaseMasterLoop

   class ECBaseMasterLoop : public Node
   {
   public:

      ECBaseMasterLoop();

      // Method instantiates all singletones in required order
      void InitGame();

      // Method called once the game started
      void RunGame();

      // Test
      static void PrintTime();
      static double GetTime();

      // Called on each frame
      void update(float t) override;

   private:


      // 
      void MasterTick(float t);

      // Timer
      static double time_runs;

   };

}


#endif // ECBASEMASTERLOOP_H_

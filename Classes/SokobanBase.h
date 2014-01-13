//===-- SokobanBase.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/15
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Sokoban pixel can't be magnetized, but can be pushed by assembly
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SOKOBANBASE_H_
#define SOKOBANBASE_H_

#include "EMBase.h"
#include "IDynamic.h"
#include "IAlive.h"
#include "IMovable.h"
#include "IMoveBlocker.h"

namespace MPix {

   // SokobanBase
   class SokobanBase :
      public IDynamic,         // Dynamics used to implement logic
      public IAlive,           // Can be killed 
      public IMovableStepper,  // Moves step by step
      public IMoveBlocker      // Blocking used to implement logic
   {
   public:

      SokobanBase();


      // History methods
      void InitSnapshots( const Context& context ) override;
      void PushSnapshot( const Context& context ) override;
      void ClearSnapshots( const Context& context ) override;
      void PopSnapshots( const Context& context, int n ) override;

      // Inside update we make move
      void preparePrelude( const Context& context, bool move_blocked ) override;

      // Block method has main magic
      bool blocksMoves (const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix = nullptr);

   protected:

      // Holds direction, which is set in IBlocker and used in IDynamic
      Direction next_move;

   };

}


#endif // SOKOBANBASE_H_
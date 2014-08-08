//===-- IMoveBlocker.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/23
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Blocker blocks =)
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IMOVEBLOCKER_H_
#define IMOVEBLOCKER_H_

#include "ECBase.h"
#include "MPix.h"
#include "Pixel.h"
#include "AnyBehavior.h"

namespace MPix {

   // Forward dependencies
   class Context;
   class IMoveBlocker;

   class MoveBlockerBehavior : public AnyBehavior
   {
   public:

      virtual bool blocksMoves (
         shared_ptr<IMoveBlocker> subject,  
         const Context& context,
         Coordinates from,
         Direction to,
         shared_ptr<Pixel> pix 
      ) = 0;

   };

   // IMoveBlocker

   class IMoveBlocker : public virtual Pixel
   {
   public:

      // Called by other pixels to check if move possible
      //  pix - pixel, trying to move
      //  dir - where it wants to move
      virtual bool blocksMoves (const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix = nullptr);

      // Override specific history
      void InitSnapshots( const Context& context ) override; // Stores no history
      void PushSnapshot( const Context& context ) override; // Stores no history
      void ClearSnapshots( const Context& context ) override; // Stores no history
      void PopSnapshots( const Context& context, int n ) override; // Stores no history

      // Behavior interfaces
      void SetMoveBlockerBehavior(shared_ptr<MoveBlockerBehavior> new_behavior);
      void SetMoveBlockerBehavior(const string& name);

   private:

      shared_ptr<MoveBlockerBehavior> blocker;

   };

}


#endif // IMOVEBLOCKER_H_
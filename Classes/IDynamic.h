//===-- IDynamic.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/22
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Dynamics can move on scene
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IDYNAMIC_H_
#define IDYNAMIC_H_

#include "ECBase.h"
#include "Pixel.h"

namespace MPix {

   // Forward dependencies
   class Context;

   // IDynamic
   class IDynamic : public virtual Pixel
   {
   public:

      // Called immediately after player's move is performed, split into prelude/postum
      virtual void preparePrelude( const Context& context, bool move_blocked ) {}
      virtual void preparePostum( const Context& context, bool move_blocked ) {}

      // Called when it is time for world to update, split into prelude/postum
      virtual void updatePrelude(const Context& context) {}
      virtual void updateAction( const Context& context ) {}
      virtual void updateFinalize( const Context& context ) {}

      // Override specific history
      void InitSnapshots( const Context& context ) override {} // Stores no history
      void PushSnapshot( const Context& context ) override {} // Stores no history
      void ClearSnapshots( const Context& context ) override {} // Stores no history
      void PopSnapshots( const Context& context, int n ) override {} // Stores no history

   protected:

   private:

   };

}


#endif // IDYNAMIC_H_
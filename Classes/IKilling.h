//===-- IKilling.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/22
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Killing interface, used by pixels who can kill
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IKILLING_H_
#define IKILLING_H_

#include "ECBase.h"
#include "Pixel.h"
#include "AnyBehavior.h"

namespace MPix {

   // Forward dependencies
   class Context;
   class IKilling;

   // Strategy
   class KillingBehavior : public AnyBehavior {
   public:

      virtual bool tryKillThat(shared_ptr<IKilling> subject, const Context& context, int target) = 0;

   };

   // IKilling
   class IKilling : public virtual Pixel
   {
   public:

      IKilling();
      ~IKilling();

      // returns true if was killed
      virtual bool tryKillThat(const Context& context, int target);

      // killer rank
      virtual int getKillerRank() const { return rank; }

      // Override specific history
      void InitSnapshots( const Context& context ) override; // Stores no history
      void PushSnapshot( const Context& context ) override; // Stores no history
      void ClearSnapshots( const Context& context ) override; // Stores no history
      void PopSnapshots( const Context& context, int n ) override; // Stores no history

      // Behavior interfaces
      void SetKillingBehavior(shared_ptr<KillingBehavior> new_behavior);
      void SetKillingBehavior(const string& name);
      void SetKillingRank(int rank) { this->rank = rank; }

   private:

      shared_ptr<KillingBehavior> killing;
      int rank;

   };

}


#endif // IKILLING_H_
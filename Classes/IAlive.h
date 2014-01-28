//===-- IAlive.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/23
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Implemented by pixels that can be killed
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IALIVE_H_
#define IALIVE_H_

#include "EMBase.h"
#include "Historical.h"
#include "Pixel.h"
#include "AnyBehavior.h"

namespace MPix {

   // Forward dependencies
   class Context;
   class IAlive;
   class IKilling;

   // Strategy
   class AliveBehavior : public AnyBehavior {
   public:

      virtual bool canLive(shared_ptr<IAlive> subject, const Context& context) = 0;

   };

   // Interface IAlive
   class IAlive : public virtual Pixel
   {
   public:

      enum class State {
         ALIVE = 0,
         KILLED_BY_NEEDLE,
         KILLED_BY_PITTRAP,
         KILLED_BY_STONE,
         KILLED_BY_LASER,
         KILLED_BY_HUNGRY_WOLF
      };

      IAlive();

      // Life checking method can be overridden, or a strategy can be chosen
      virtual bool canLive(const Context& context);

      // Life and dead methods
      bool IsAlive() const;
      State GetLiveState() const { return life; }
      void Kill(State cause);
      void Kill(const Context& context, State cause);
      void Kill(const Context& context, const IKilling& killer);
      bool KilledBy( const IKilling& k ) const;

      // Save/restore
      void InitSnapshots( const Context& context ) override;
      void PushSnapshot( const Context& context ) override;
      void ClearSnapshots( const Context& context ) override;
      void PopSnapshots( const Context& context, int n ) override;

      // Behavior interfaces
      void SetAliveBehavior(shared_ptr<AliveBehavior> new_behavior);
      void SetAliveBehavior(const string& name);


   private:

      shared_ptr<AliveBehavior> alive;
      Historical<State> life;
      State GetStateByKiller(const IKilling& k) const;

   };


}


#endif // IALIVE_H_
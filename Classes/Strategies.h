//===-- Strategies.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/24
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Here are defined all pixel strategies, in one file for now
//
//===---------------------------------------------------------===//

#pragma once
#ifndef STRATEGIES_H_
#define STRATEGIES_H_

#include "EMBase.h"


// Interfaces
#include "IAlive.h"
#include "IAssembled.h"
#include "IKilling.h"
#include "IMoveBlocker.h"
#include "IMovable.h"
#include "IColorful.h"
#include "IDynamic.h"
#include "IMagnetic.h"
#include "IAssembled.h"
#include "IProtector.h"

namespace MPix {


   //////////////////////////////////////////////////////////////////////////
   // IAlive

   // Immortal never dies
   class AliveImmortal : public AliveBehavior {
   public:

      EM_NODE_CHILD(AliveImmortal);
      virtual bool canLive( shared_ptr<IAlive> subject, const Context& context ) override;

   };

   // Simple checks only current pos for killer
   class AliveSimple : public AliveBehavior {
   public:

      EM_NODE_CHILD(AliveSimple);
      virtual bool canLive( shared_ptr<IAlive> subject, const Context& context ) override;

   protected:

      // Checks pos for any killers, and calls can kill, if one can, return true, if alive return false
      bool GotAnyKillerAt(int subject,  const Context& context, Coordinates pos);

   };

   // Standard asks for killers on pos and 4 sides
   class AliveStandard : public AliveSimple {
   public:

      EM_NODE_CHILD(AliveStandard);
      virtual bool canLive( shared_ptr<IAlive> subject, const Context& context ) override;

   };

   // Extended asks for killers on pos and on 8 sides
   class AliveExtended : public AliveStandard {
   public:

      EM_NODE_CHILD(AliveExtended);
      virtual bool canLive( shared_ptr<IAlive> subject, const Context& context ) override;

   };

   //////////////////////////////////////////////////////////////////////////
   // IKilling

   // Super killer kills always (when is asked)
   class KillingBase : public KillingBehavior {
   public:

      EM_NODE_CHILD(KillingBase);
      virtual bool tryKillThat( shared_ptr<IKilling> subject, const Context& context, int target) override;
      void PerformKill( const IKilling& subject, const Context& context, IAlive& victim );

   };

   // Kind killer never kills 
   class KillingKind : public KillingBehavior {
   public:

      EM_NODE_CHILD(KillingKind);
      virtual bool tryKillThat(shared_ptr<IKilling> subject, const Context& context, int target) { return false; }

   };

   // Kills at itself point, based on rankings
   class KillingAtPoint : public KillingBase {
   public:

      EM_NODE_CHILD(KillingAtPoint);
      virtual bool tryKillThat( shared_ptr<IKilling> subject, const Context& context, int target) override;

   };

   // Pit killer kills when on same pos
   class KillingPitfall : public KillingBase {
   public:

      EM_NODE_CHILD(KillingPitfall);
      virtual bool tryKillThat( shared_ptr<IKilling> subject, const Context& context, int target) override;

   };

   // Spike kills 
   class KillingSpikes : public KillingBase {
   public:

      EM_NODE_CHILD(KillingSpikes);
      virtual bool tryKillThat( shared_ptr<IKilling> subject, const Context& context, int target) override;
      bool CheckProtectionAt(  const Context& context, Coordinates pos,shared_ptr<IAlive> victim, shared_ptr<IKilling> killer );

   };

   //////////////////////////////////////////////////////////////////////////
   // IMovable

   // Always blocked, can't move
   class StepperAlways : public StepperBehavor {
   public:
      EM_NODE_CHILD(StepperAlways);
      virtual bool canMoveThis( shared_ptr<IMovableStepper> subject, const Context& context, Direction dir) override;
   };

   // Moves if there is no blocker
   class StepperStandard : public StepperBehavor {
   public:
      EM_NODE_CHILD(StepperStandard);
      virtual bool canMoveThis( shared_ptr<IMovableStepper> subject, const Context& context, Direction dir) override;
   protected:
      virtual bool IsSomeoneBlockingMeAt( shared_ptr<IMovableStepper> subject, const Context& context, Coordinates pos, Direction dir);
   };

   // Moves if there is no wall blocker
   class StepperHard : public StepperStandard {
   public:
      EM_NODE_CHILD(StepperHard);
   protected:
      bool IsSomeoneBlockingMeAt( shared_ptr<IMovableStepper> subject, const Context& context, Coordinates pos, Direction dir) override;
   };

   // Moves if there is no hard blocker
   class StepperMagnetic : public StepperStandard {
   public:
      EM_NODE_CHILD(StepperMagnetic);
   protected:
      bool IsSomeoneBlockingMeAt( shared_ptr<IMovableStepper> subject, const Context& context, Coordinates pos, Direction dir) override;
   };

   // Moves anyway
   class StepperGhost : public StepperBehavor {
   public:
      EM_NODE_CHILD(StepperGhost);
      virtual bool canMoveThis( shared_ptr<IMovableStepper> subject, const Context& context, Direction dir) override;
   };

   //////////////////////////////////////////////////////////////////////////
   // IMoveBlocker

   // Simple blocker blocks only when someone targets it's pos, or pass by diagonal
   class MoveBlockerSimple : public MoveBlockerBehavior {
   public:
      EM_NODE_CHILD(MoveBlockerSimple);
      virtual bool blocksMoves (shared_ptr<IMoveBlocker> subject, const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix ) override;
   };

   // My type blocker blocks only pixels of its type
   class MoveBlockerMyType : public MoveBlockerSimple {
   public:
      EM_NODE_CHILD(MoveBlockerMyType);
      virtual bool blocksMoves (shared_ptr<IMoveBlocker> subject, const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix ) override;
   };

   //////////////////////////////////////////////////////////////////////////
   // Grow behaviors

   // Disabled grower
   class DisabledGrow : public GrowBehavior {
   public:
      EM_NODE_CHILD(DisabledGrow);
      virtual bool growThis(shared_ptr<IAssembled> subject, const Context& context);
   };

   // Grows in 4 directions
   class MagneticGrowStandard : public GrowBehavior {
   public:
      EM_NODE_CHILD(MagneticGrowStandard);
      virtual bool growThis(shared_ptr<IAssembled> subject, const Context& context);
      shared_ptr<IAssembled> PerformMagneticGrow(shared_ptr<IAssembled> subject,  const Context& context, Coordinates pos);
   };

   // Grow in 8 directions
   class MagneticGrowExtended : public MagneticGrowStandard {
   public:
      EM_NODE_CHILD(MagneticGrowExtended);
      virtual bool growThis(shared_ptr<IAssembled> subject, const Context& context);
   };

   //////////////////////////////////////////////////////////////////////////
   // Protecting

   // Protects from cactus
   class CactusWallProtector : public ProtectorBehavior {
   public:

      EM_NODE_CHILD(CactusWallProtector);
      bool isProtecting( shared_ptr<IProtector> subject, const Context& context, shared_ptr<IAlive> victim, shared_ptr<IKilling> killer );

   };

}


#endif // STRATEGIES_H_
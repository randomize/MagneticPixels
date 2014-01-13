//===-- IMovable.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/24
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Anything that moves
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IMOVEBLE_H_
#define IMOVEBLE_H_

#include "EMBase.h"
#include "Pixel.h"
#include "AnyBehavior.h"

namespace MPix {

   // Forward dependencies
   class Context;
   class IMovableStepper;

   // IMoveble
   /*class IMovable : public virtual Pixel
   {
   public:

      // Called to check that this pixel can move to position
      virtual bool canMoveThis(const Context& context, Coordinates pos);;

      // Override specific history // TODO : coordinates
      void InitSnapshots( const Context& context ) override; // Stores no history yet
      void PushSnapshot( const Context& context ) override; // Stores no history yet
      void ClearSnapshots( const Context& context ) override; // Stores no history
      void PopSnapshots( const Context& context, int n ) override; // Stores no history yet

   };*/

   class StepperBehavor : public AnyBehavior {
   public:

      virtual bool canMoveThis(shared_ptr<IMovableStepper> subject, const Context& context, Direction dir) = 0;

   };

   // Moves step by step only
   class IMovableStepper : public virtual Pixel
   {
   public:

      // Called to check that this pixel can move to given direction
      virtual bool canMoveThis(const Context& context, Direction dir); // TODO: blockers return

      // Called to perform move
      virtual void Move( const Context& context, Direction dir);

      // Override specific history // TODO : coordinates
      void InitSnapshots( const Context& context ) override; // Stores no history yet
      void PushSnapshot( const Context& context ) override; // Stores no history yet
      void ClearSnapshots( const Context& context ) override; // Stores no history
      void PopSnapshots( const Context& context, int n ) override; // Stores no history yet

      // Behavior interfaces
      void SetStepperBehavior(shared_ptr<StepperBehavor> new_behavior);
      void SetStepperBehavior(const string& name);

   private:

      shared_ptr<StepperBehavor> stepper;

   };

}


#endif // IMOVEBLE_H_
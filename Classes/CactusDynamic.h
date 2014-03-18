//===-- CactusDynamic.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/09
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: 
//
//===---------------------------------------------------------===//

#pragma once
#ifndef NEEDLEPIXEL_H_
#define NEEDLEPIXEL_H_

#include "EMBase.h"
#include "EMNode.h"

#include "IDynamic.h"
#include "IKilling.h"
#include "IMovable.h"
#include "IAlive.h"
#include "IMoveBlocker.h"

namespace MPix {

   // Forward dependencies
   class Context;

   // MagneticPixel
   class CactusDynamic : 
      public IDynamic,         // Updates every move
      public IKilling,         // Can kill
      public IMovableStepper,  // Moves step by step
      public IAlive,           // Can be killed(by pitfalls for example)
      public IMoveBlocker      // Blocks cactus es
   {
   public:

      // Register this pixel type to pixel factory
      EM_NODE_CHILD(CactusDynamic);

   public:

      enum class NeedleType {
         STEPPER = 0,
         MOVER,
         RANDOM,
         PREDATOR,
         Last,
         First = STEPPER
      };

      static const char* TypeToString(NeedleType t);

   public:

      CactusDynamic(NeedleType t = NeedleType::STEPPER, Direction way = Direction::DIR_RIGHT, bool sleeping = false);

      // ----- Implements interfaces  -----------------------------------------

      // Pixel
      PixelType GetType() const override { return Pixel::PixelType::CACTUS_PIX; }
      virtual void InitSnapshots( const Context& context ) override;
      virtual void PushSnapshot( const Context& context ) override;
      virtual void ClearSnapshots( const Context& context ) override;
      virtual void PopSnapshots( const Context& context, int n ) override;

      // Dynamic
      void updatePrelude( const Context& context ) override;
      void updateAction( const Context& context ) override;

      // ----- Interface for Views ---------------------------------------

      NeedleType GetNeedleType() const { return type; }
      Direction GetWay() const { return way; }


   private:

      NeedleType type;
      Historical<Direction> way;
      Historical<bool> sleeping;
      void UpdateMyWay( const Context& context );
      int srandy;

   };

}


#endif // NEEDLEPIXEL_H_
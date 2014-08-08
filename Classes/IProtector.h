//===-- IProtector.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/15
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Protectors protect from misc danger
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IPROTECTOR_H_
#define IPROTECTOR_H_

#include "ECBase.h"
#include "Pixel.h"
#include "AnyBehavior.h"

namespace MPix {

   // Forward dependencies
   class IAlive;
   class IKilling;
   class IProtector;

   // Strategy
   class ProtectorBehavior : public AnyBehavior {
   public:

      // One protection method
      virtual bool isProtecting(
         shared_ptr<IProtector> subject,
         const Context& context,
         shared_ptr<IAlive> victim,
         shared_ptr<IKilling> killer
      ) = 0;

   };

   // IProtector
   class IProtector : public virtual Pixel
   {
   public:

      virtual bool isProtecting( const Context& context, shared_ptr<IAlive> victim, shared_ptr<IKilling> killer);

      // History support
      void InitSnapshots( const Context& context )override; //Nothing
      void PushSnapshot( const Context& context )override; //Nothing
      void ClearSnapshots( const Context& context )override; //Nothing
      void PopSnapshots( const Context& context, int n )override; //Nothing

      // Strategy for protection
      void SetProtectorBehavior(shared_ptr<ProtectorBehavior> new_behavior);
      void SetProtectorBehavior(const string& name);

   private:

      shared_ptr<ProtectorBehavior> protector;

   };

}


#endif // IPROTECTOR_H_
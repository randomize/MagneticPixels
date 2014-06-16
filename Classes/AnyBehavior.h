//===-- AnyBehavior.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/24
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Class for generalizing behaviors
//
//===---------------------------------------------------------===//

#pragma once
#ifndef ANYBEHAVIOR_H_
#define ANYBEHAVIOR_H_

#include "ECBase.h"
#include "ECNode.h"

namespace MPix {

   // Forward dependencies


   // AnyBehavior

   class AnyBehavior : public ECNode
   {
   public:

      ECNODE_BASE(AnyBehavior);

      // By default all behaviors are shared, they are stateless
      // If there will be one with states, it should override and return false;
      virtual bool IsShared() { return true; }

   protected:

   private:

   };

}


#endif // ANYBEHAVIOR_H_
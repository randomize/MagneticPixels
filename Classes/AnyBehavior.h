//===-- AnyBehavior.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/24
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Class for generalizing behaviors
//
//===---------------------------------------------------------===//

#pragma once
#ifndef ANYBEHAVIOR_H_
#define ANYBEHAVIOR_H_

#include "EMBase.h"
#include "EMNode.h"

namespace MPix {

   // Forward dependencies


   // AnyBehavior

   class AnyBehavior : public EMNode
   {
   public:

      EM_NODE_BASE(AnyBehavior);

      // By default all behaviors are shared, they are stateless
      // If there will be one with states, it should override and return false;
      virtual bool IsShared() { return true; }

   protected:

   private:

   };

}


#endif // ANYBEHAVIOR_H_
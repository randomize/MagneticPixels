//===-- ArrowMark.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/01
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: 
//
//===---------------------------------------------------------===//

#pragma once
#ifndef ARROWMARK_H_
#define ARROWMARK_H_

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // ArrowMark
   class ArrowMark : public Node
   {
   public:

      static ArrowMark* create();

      void SetDirection(Direction d);

   };

}


#endif // ARROWMARK_H_

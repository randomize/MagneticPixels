//===-- RoundMark.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/26
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Simple UI element, digit label in circle
//
//===---------------------------------------------------------===//

#pragma once
#ifndef ROUNDMARK_H_
#define ROUNDMARK_H_

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // Forward dependencies


   // RoundMark

   class RoundMark : public Node
   {
   public:
      static RoundMark* create(const char* text);
   };

}


#endif // ROUNDMARK_H_
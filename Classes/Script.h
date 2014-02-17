//===-- Script.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/02/01
//  Author:      Mihailenco E. at Emboss Games, 2014
//  Description: Base class for game script
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SCRIPT_H_
#define SCRIPT_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies
   class PixelContext;

   // Script

   class Script
   {
   public:

      // Script functors return value type
      enum class Ret {
         YES,
         NO,
         OK,
         FAIL
      };

      Script();
      virtual ~Script();


   protected:

      string m_name;

      std::function<Ret(const PixelContext&)> on_load;
      std::function<Ret(const PixelContext&)> on_pre_tick;

   private:

   };

}


#endif // SCRIPT_H_


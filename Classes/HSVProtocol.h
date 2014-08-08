//===-- HSVProtocol.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/30
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Class for anything that can have HSV shift
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HSVPROTOCOL_H_
#define HSVPROTOCOL_H_

#include "ECBase.h"
#include "HSVShader.h"
#include "MPix.h"

namespace MPix {

   // HSVProtocol
   class HSVProtocol
   {
   public:

      void SetHSV( float hue, float saturation, float value )
      {
         HSVShader::GenHSVMatrix(xform, hue, saturation, value);
      }

      void SetHSV( const HSVColor& shift )
      {
         SetHSV(shift.hue, shift.saturation, shift.value);
      }

   protected:

      float* GetHSVMatrix() { return xform; }

   private:

      float xform[16];

   };

}


#endif // HSVPROTOCOL_H_
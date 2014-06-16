//===-- BadMagneticPixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/03
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Bad magnetic pixel disallows to step in its area
//
//===---------------------------------------------------------===//

#pragma once
#ifndef BADMAGNETICPIXEL_H_
#define BADMAGNETICPIXEL_H_

#include "EMBase.h"
#include "EMNode.h"
#include "MPix.h"

#include "AssembledMagneticBase.h"

namespace MPix {

   // BadMagneticPixel

   class BadMagneticPixel : public AssembledMagneticBase
   {
   public:

      // Register this pixel type to pixel factory
      EM_NODE_CHILD(BadMagneticPixel);

   public:

      BadMagneticPixel(PixelColor color = PixelColor::RED);
      ~BadMagneticPixel();

   };

}


#endif // BADMAGNETICPIXEL_H_
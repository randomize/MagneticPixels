//===-- MagneticPixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Magnetic pixel has magic abilities
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MAGNETICPIXEL_H_
#define MAGNETICPIXEL_H_

#include "EMBase.h"
#include "EMNode.h"
#include "MPix.h"

#include "AssembledMagneticBase.h"

namespace MPix {

   // MagneticPixel

   class MagneticPixel : public  AssembledMagneticBase
   {
   public:

      // Register this pixel type to pixel factory
      EM_NODE_CHILD(MagneticPixel);

   public:

      MagneticPixel(PixelColor color = PixelColor::RED);
      ~MagneticPixel();

   };

}


#endif // MAGNETICPIXEL_H_


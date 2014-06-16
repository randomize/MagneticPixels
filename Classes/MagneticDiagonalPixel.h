//===-- MagneticPixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Diagonal can stick in 8 directions
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MAGNETICDIAGONALPIXEL_H_
#define MAGNETICDIAGONALPIXEL_H_

#include "EMBase.h"
#include "EMNode.h"
#include "MPix.h"

#include "AssembledMagneticBase.h"

namespace MPix {

   // MagneticDiagonalPixel

   class MagneticDiagonalPixel : public  AssembledMagneticBase
   {
   public:

      // Register this pixel type to pixel factory
      EM_NODE_CHILD(MagneticDiagonalPixel);
      PixelType GetType() const override { return PixelType::ASSEMBLY_PIX; }

   public:

      MagneticDiagonalPixel(PixelColor color = PixelColor::RED);
      ~MagneticDiagonalPixel();

   };

}


#endif // MAGNETICDIAGONALPIXEL_H_


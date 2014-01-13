//===-- MagneticPixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/06
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Magnetic pixels view
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MAGNETICPIXELVIEW_H_
#define MAGNETICPIXELVIEW_H_

#include "EMBase.h"
#include "MagneticView.h"

namespace MPix {

   // Forward dependencies
   class MagneticPixel;

   // MagneticPixelView

   class MagneticPixelView : public MagneticView
   {
   public:

      EM_NODE_CHILD(MagneticPixelView);

      void Build( shared_ptr<Pixel> model ) override;

   protected:

   private:

      shared_ptr<MagneticPixel> pixel;

   };

}


#endif // MAGNETICPIXELVIEW_H_
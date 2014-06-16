//===-- SokobanPixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/15
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: View for sokoban pixel
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SOKOBANPIXELVIEW_H_
#define SOKOBANPIXELVIEW_H_

#include "EMBase.h"
#include "EMNode.h"
#include "MPix.h"

#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class SokobanPixel;
   class HSVSprite;

   // SokobanPixelView
   class SokobanPixelView : PixelView
   {
   public:

      EM_NODE_CHILD(SokobanPixelView);
      SokobanPixelView();

      void Build( shared_ptr<Pixel> model ) override;

   protected:

      HSVSprite*   body;

   private:

      shared_ptr<SokobanPixel> pixel;

   };

}


#endif // SOKOBANPIXELVIEW_H_

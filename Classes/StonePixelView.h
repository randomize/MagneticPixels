//===-- StonePixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/16
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Stone pixel view
//
//===---------------------------------------------------------===//

#pragma once
#ifndef STONEPIXELVIEW_H_
#define STONEPIXELVIEW_H_

#include "EMBase.h"
#include "EMNode.h"
#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class StonePixel;

   // StonePixelView
   class StonePixelView : public PixelView
   {
   public:

      StonePixelView();
      ~StonePixelView();

      EM_NODE_CHILD(StonePixelView);

      void Build( shared_ptr<Pixel> model ) override;

      void PixelMoved() override;
      void PixelTapped() override;
      void PixelIdleTrick() override;

   private:

      shared_ptr<StonePixel> pixel;

      Sprite* left_eye, *right_eye;

      void ResetEyes();

   };

}


#endif // STONEPIXELVIEW_H_
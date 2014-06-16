//===-- StonePixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/16
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Stone pixel view
//
//===---------------------------------------------------------===//

#pragma once
#ifndef STONEPIXELVIEW_H_
#define STONEPIXELVIEW_H_

#include "ECBase.h"
#include "ECNode.h"
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

      ECNODE_CHILD(StonePixelView);

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
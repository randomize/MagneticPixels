//===-- SokobanPixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/15
//  Author:      Mihailenco E. at Emboss Games, 2013
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
   class EMAnimation;

   // SokobanPixelView
   class SokobanPixelView : PixelView
   {
   public:

      EM_NODE_CHILD(SokobanPixelView);
      SokobanPixelView();

      bool Update( CmdUIUpdatePixelView::Reason reason ) override;
      void Build( shared_ptr<Pixel> model ) override;

      // Base update reasons implementation
      void PixelCreated() override;
      void PixelAccepted() override;
      void PixelReset() override;
      void PixelDied() override;
      void PixelResurrect() override;

      // Own update reasons
      virtual void PixelSmiled();
      virtual void PixelUnsmiled();

   protected:

      EMAnimation* body;

   private:

      shared_ptr<SokobanPixel> pixel;

   };

}


#endif // SOKOBANPIXELVIEW_H_

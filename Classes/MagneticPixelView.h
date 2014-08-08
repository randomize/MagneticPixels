//===-- MagneticPixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/06
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Magnetic pixels view
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MAGNETICPIXELVIEW_H_
#define MAGNETICPIXELVIEW_H_

#include "ECBase.h"
#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class MagneticPixel;
   class SleepingZets;
   class ECAnimation;
   class HSVSprite;

   // MagneticPixelView

   class MagneticPixelView : public PixelView
   {
   public:

      ECNODE_CHILD(MagneticPixelView);
      MagneticPixelView();
      ~MagneticPixelView();

      bool Update( CmdUIUpdatePixelView::Reason reason ) override;

      void Build( shared_ptr<Pixel> model ) override;

      // Base update reasons implementation
      virtual void PixelCreated() override;
      virtual void PixelAccepted() override;
      virtual void PixelReset() override;
      virtual void PixelDied() override;
      virtual void PixelResurrect() override;
      virtual void PixelIdleTrick() override;
      virtual void PixelTapped() override;

      // Own update reasons
      virtual void PixelCanFallOn();
      virtual void PixelCanFallOff();
      virtual void PixelSmiled();
      virtual void PixelUnsmiled();
      virtual void PixelWake() ;
      virtual void PixelAsleep() ;
      virtual void PixelBlocks() ;

   private:

     // contents
      SleepingZets* zzz;
      Node* body;
      ECAnimation* mimics;
      HSVSprite* bg;
      HSVSprite* smash;

      shared_ptr<MagneticPixel> pixel;

   };

}


#endif // MAGNETICPIXELVIEW_H_
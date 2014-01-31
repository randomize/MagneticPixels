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
#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class MagneticPixel;
   class SleepingZets;
   class EMAnimation;
   class HSVSprite;

   // MagneticPixelView

   class MagneticPixelView : public PixelView
   {
   public:

      EM_NODE_CHILD(MagneticPixelView);
      MagneticPixelView();
      ~MagneticPixelView();

      bool Update( CmdUIUpdatePixelView::Reason reason ) override;

      void Build( shared_ptr<Pixel> model ) override;

      // Base update reasons implementation
      void PixelCreated() override;
      void PixelAccepted() override;
      void PixelReset() override;
      void PixelDied() override;
      void PixelResurrect() override;

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
      EMAnimation* mimics;
      HSVSprite* bg;
      HSVSprite* smash;

      shared_ptr<MagneticPixel> pixel;

   };

}


#endif // MAGNETICPIXELVIEW_H_
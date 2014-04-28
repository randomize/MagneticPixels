//===-- BomberPixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/02/19
//  Author:      Mihailenco E. at Emboss Games, 2014
//  Description: Bomber explodes
//
//===---------------------------------------------------------===//

#pragma once
#ifndef BOMBERPIXELVIEW_H_
#define BOMBERPIXELVIEW_H_


#include "EMBase.h"
#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class BomberPixel;
   class SleepingZets;
   class EMAnimation;
   class HSVSprite;

   // MagneticPixelView

   class BomberPixelView : public PixelView
   {
   public:

      EM_NODE_CHILD(BomberPixelView);
      BomberPixelView();
      ~BomberPixelView();

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

      // Bomber
      virtual void PixelChanged() override;

   private:

     // contents
      Node* body;
      EMAnimation* mimics;
      HSVSprite* bg;
      HSVSprite* bomb_eyes;
      Sprite* bomb;
      HSVSprite* smash;
      Label* label;

      shared_ptr<BomberPixel> pixel;

   };

}

#endif // BOMBERPIXELVIEW_H_

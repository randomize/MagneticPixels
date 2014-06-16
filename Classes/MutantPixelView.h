//===-- MutantPixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/02/19
//  Author:      Mihailenco E. at TheEndlessCat Games, 2014
//  Description: Mutant pixel view, like simple, but with color dots
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MUTANTPIXELVIEW_H_
#define MUTANTPIXELVIEW_H_

#include "ECBase.h"
#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class MutantPixel;
   class SleepingZets;
   class ECAnimation;
   class HSVSprite;


   // MutantPixelView

   class MutantPixelView : public PixelView
   {
   public:

      ECNODE_CHILD(MutantPixelView);
      MutantPixelView();
      ~MutantPixelView();

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

      // Mutant update reasons
      virtual void PixelChanged();

   private:

     // contents
      SleepingZets* zzz;
      Node* body;
      ECAnimation* mimics;
      HSVSprite* bg;
      HSVSprite* smash;
      HSVSprite* dots;

      shared_ptr<MutantPixel> pixel;

      void UpdateColors();

   protected:

   private:

   };

}


#endif // MUTANTPIXELVIEW_H_
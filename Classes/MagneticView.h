//===-- MagneticView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/15
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Base view for magnetics
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MAGNETICVIEW_H_
#define MAGNETICVIEW_H_

#include "EMBase.h"
#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class SleepingZets;
   class EMAnimation;
   class IAssembled;

   // MagneticView

   class MagneticView : public PixelView
   {
   public:

      MagneticView();
      ~MagneticView();


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

   protected:

      SleepingZets* zzz;
      EMAnimation* body;

   private:

      shared_ptr<IAssembled> asm_interface;
      bool canFallOn;

   };

}


#endif // MAGNETICVIEW_H_

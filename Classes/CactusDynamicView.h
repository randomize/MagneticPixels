//===-- CactusDynamicView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/02
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: 
//
//===---------------------------------------------------------===//

#pragma once
#ifndef CACTUSDYNAMICVIEW_H_
#define CACTUSDYNAMICVIEW_H_

#include "EMBase.h"
#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class CactusDynamic;
   class ArrowMark;

   // CactusDynamicView
   class CactusDynamicView : public PixelView
   {
   public:

      EM_NODE_CHILD(CactusDynamicView);
      CactusDynamicView();

      void Build( shared_ptr<Pixel> model ) override;

      void PixelMoved() override;
      
      void PixelChanged() override;


      void PixelCreated() override;
      void PixelKilledSomeone() override;
      void PixelDied() override;
      void PixelResurrect() override;

   private:

      Sprite* bg;

      shared_ptr<CactusDynamic> pixel;

#ifdef MPIX_DEVELOPERS_BUILD
      ArrowMark* mark;
#endif

   };

}

#endif // CACTUSDYNAMICVIEW_H_


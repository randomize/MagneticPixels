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
#include "CactusView.h"

namespace MPix {

   // Forward dependencies
   class CactusDynamic;
   class ArrowMark;

   // CactusDynamicView
   class CactusDynamicView : public CactusView
   {
   public:

      EM_NODE_CHILD(CactusDynamicView);

      void Build( shared_ptr<Pixel> model ) override;

      void PixelMoved() override;
      
      void PixelChanged() override;


   protected:


   private:

      shared_ptr<CactusDynamic> pixel;

#ifdef MPIX_DEVELOPERS_BUILD
      ArrowMark* mark;
   public:
      void PixelDied() override;
      void PixelResurrect() override;
#endif

   };

}

#endif // CACTUSDYNAMICVIEW_H_


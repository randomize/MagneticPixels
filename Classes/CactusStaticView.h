//===-- CactusDynamicView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/11
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Represents static cactus pixel
//
//===---------------------------------------------------------===//

#pragma once
#ifndef CACTUS_STATIC_VIEW_H_
#define CACTUS_STATIC_VIEW_H_

#include "EMBase.h"
#include "CactusView.h"

namespace MPix {

   // Forward dependencies
   class CactusStatic;

   // CactusDynamicView
   class CactusStaticView : public CactusView
   {
   public:

      EM_NODE_CHILD(CactusStaticView);

      void Build( shared_ptr<Pixel> model ) override;

      void PixelMoved() override;

   protected:

   private:

      shared_ptr<CactusStatic> pixel;

   };

}


#endif // CACTUS_STATIC_VIEW_H_
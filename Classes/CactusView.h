//===-- CactusDynamicView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/11
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Represents base cactus pixel
//
//===---------------------------------------------------------===//

#pragma once
#ifndef CACTUS_VIEW_H_
#define CACTUS_VIEW_H_

#include "EMBase.h"
#include "PixelView.h"

namespace MPix {

   // CactusView
   class CactusView : public PixelView
   {
   public:

      CactusView();

      void Build( shared_ptr<Pixel> model ) override;

      void PixelCreated() override;
      void PixelKilledSomeone() override;
      void PixelDied() override;
      void PixelResurrect() override;

   protected:

      Sprite* bg;

   };

}


#endif // CACTUS_VIEW_H_
//===-- CuteBlocksLogo.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/01/28
//  Author:      Mihailenco E. at Emboss Games, 2014
//  Description: Logo class, used in main menu and credits
//
//===---------------------------------------------------------===//

#pragma once
#ifndef CUTEBLOCKSLOGO_H_
#define CUTEBLOCKSLOGO_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies
   class PixelView;
   class Pixel;


   // CuteBlocksLogo

   class CuteBlocksLogo : public Node
   {
   public:

      static CuteBlocksLogo* create();

      bool init() override;

   private:

      shared_ptr<Pixel> p;
      shared_ptr<PixelView> pixel;
      Node* pixel_holder;
      EventListener* touch_events;
      int egg;

      void UpdateIdle();

      bool onTouchBegan(Touch *touch, Event *event);

   };

}


#endif // CUTEBLOCKSLOGO_H_

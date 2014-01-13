//===-- HSVSprite.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/26
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Sprite with HSV parameters
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HSVSPRITE_H_
#define HSVSPRITE_H_

#include "EMBase.h"
#include "MPix.h"
#include "HSVProtocol.h"

namespace MPix {

   // Forward dependencies


   // HSVSprite

   class HSVSprite : public Sprite, public HSVProtocol
   {
   public:

      static HSVSprite* create(const char *filename);
      static HSVSprite* createWithSpriteFrameName(const char *spriteFrameName);

      bool initWithTexture(Texture2D* texture, const Rect&  rect) override;
      void draw() override;

   };

}


#endif // HSVSPRITE_H_
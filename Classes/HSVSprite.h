//===-- HSVSprite.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/26
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Sprite with HSV parameters
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HSVSPRITE_H_
#define HSVSPRITE_H_

#include "ECBase.h"
#include "MPix.h"
#include "HSVProtocol.h"

namespace MPix {

   // Forward dependencies


   // HSVSprite

   class HSVSprite : public Sprite, public HSVProtocol
   {
   public:

      static HSVSprite* create( const string& file_name );
      static HSVSprite* createWithSpriteFrameName( const string& frame_name );

      bool initWithTexture(Texture2D* texture, const Rect&  rect) override;
      virtual void draw(Renderer *renderer, const Matrix &transform, bool transformUpdated) override;

   protected:

      CustomCommand _renderCommand;
      void onDraw(const Matrix &transform, bool transformUpdated);

   };

}


#endif // HSVSPRITE_H_

//===-- ColorBox.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/06
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Color box is used for current pixel views,
//     letter Ill remove it because views will use Textures
//
//===---------------------------------------------------------===//

#pragma once
#ifndef COLORBOX_H_
#define COLORBOX_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies


   // ColorBox

   class ColorBox : public Node
   {
   public:

      enum Borders {
         BORDER_UP = 1,
         BORDER_RIGHT = 2,
         BORDER_DOWN = 4,
         BORDER_LEFT = 8
      };

      static ColorBox* create();
      virtual void draw(Renderer *renderer, const Matrix &transform, bool transformUpdated) override;

      void SetColor(Color4F col);
      void SetBorders(unsigned borders, Color4F col);
      void SetBorderWidth(float w) { width = w; }

   private:

      // Actual drawing happens here
      void onDraw(const Matrix &transform, bool transformUpdated);

      // Drawing command for new cocos renderer
      CustomCommand _customCommand;

      // Box color
      Color4F col;

      // Borderes color
      Color4F borders_col;

      // Borders configuration code(encoded in pows of 2)
      unsigned borders;

      // Width of the borders
      float width;

   };

}


#endif // COLORBOX_H_

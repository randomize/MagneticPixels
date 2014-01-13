//===-- ColorBox.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/06
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Color box is used for current pixel views,
//     letter Ill remove it because views will use Textures
//
//===---------------------------------------------------------===//

#pragma once
#ifndef COLORBOX_H_
#define COLORBOX_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies


   // ColorBox

   class ColorBox : public NodeRGBA
   {
   public:

      enum Borders {
         BORDER_UP = 1,
         BORDER_RIGHT = 2,
         BORDER_DOWN = 4,
         BORDER_LEFT = 8
      };

      static ColorBox* create();
      void draw() override;

      void SetColor(Color4F col);
      void SetBorders(unsigned borders, Color4F col);
      void SetBorderWidth(float w) { width = w; }

   private:
      Color4F col;
      Color4F borders_col;
      unsigned borders;
      float width;

   };

}


#endif // COLORBOX_H_
//===-- SokobanPixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/15
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Moved by pushing
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SOKOBANPIXEL_H_
#define SOKOBANPIXEL_H_

#include "ECBase.h"
#include "ECNode.h"
#include "MPix.h"

#include "SokobanBase.h"
#include "IColorful.h"

namespace MPix {

   // Forward dependencies


   // SokobanPixel

   class SokobanPixel : 
      public SokobanBase, 
      public IColorful
   {
   public:

      ECNODE_CHILD(SokobanPixel);

   public:

      SokobanPixel(PixelColor color = PixelColor::RED);
      ~SokobanPixel();

      // Pixel base 
      PixelType GetType() const override { return PixelType::SOKOBAN_PIX; }

      // History methods
      void InitSnapshots( const Context& context ) override;
      void PushSnapshot( const Context& context ) override;
      void ClearSnapshots( const Context& context ) override;
      void PopSnapshots( const Context& context, int n ) override;

      // Movable Move is overridden to update smile
      void Move(const Context& context, Direction dir) override;

   private:

      // When pixel moves smile must be updated
      void UpdateSmile( const Context& context );


   protected:

   private:

   };

}


#endif // SOKOBANPIXEL_H_
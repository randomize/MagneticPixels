//===-- MutantPixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/02/19
//  Author:      Mihailenco E. at TheEndlessCat Games, 2014
//  Description: Mutant pixel changes color
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MUTANTPIXEL_H_
#define MUTANTPIXEL_H_

#include "ECBase.h"
#include "ECNode.h"
#include "MPix.h"

#include "AssembledMagneticBase.h"
#include "IDynamic.h"

namespace MPix {

   // Forward dependencies

   // MutantPixel

   class MutantPixel : 
      public  AssembledMagneticBase,
      public  IDynamic
   {
   public:

      // Register this pixel type to pixel factory
      ECNODE_CHILD(MutantPixel);
      PixelType GetType() const override { return PixelType::ASSEMBLY_PIX; }

      // Return color
      PixelColor GetColor() const override;

      // Dynamism
      void updatePrelude(const Context& context) override;

      // History methods
      void InitSnapshots( const Context& context ) override;
      void PushSnapshot( const Context& context ) override;
      void ClearSnapshots( const Context& context ) override;
      void PopSnapshots( const Context& context, int n ) override;

      // Self
      PixelColor GetNextColor() const;
      void Move(const Context& context, Direction dir) override;

   public:

      MutantPixel(PixelColor colorA = PixelColor::RED, PixelColor colorB = PixelColor::BLUE);
      MutantPixel(const vector<PixelColor> colors);
      ~MutantPixel();
      void IncrementColorIndex();
      const vector<PixelColor>& GetColors() const { return colors;  }
      void SetColors(vector<PixelColor> &colors);

   protected:

      Historical<PixelColor> current;
      vector<PixelColor> colors;
      Historical<size_t> current_index;

   };

}


#endif // MUTANTPIXEL_H_


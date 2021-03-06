//===-- WallPixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Wall pixels restrict movements
//
//===---------------------------------------------------------===//

#pragma once
#ifndef WALLPIXEL_H_
#define WALLPIXEL_H_

#include "ECBase.h"
#include "MPix.h"

#include "Pixel.h"
#include "IWallPixel.h"

// For auto factory support
#include "ECNode.h"

namespace MPix {

   // Forward dependencies

   typedef unordered_set<Direction> WallShape;

   // WallPixel

   class WallPixel : public IWallPixel
   {
   public:

      // Register this pixel type to pixel factory
      ECNODE_CHILD(WallPixel);

   public:

      PixelType GetType() const override { return PixelType::WALL_PIX; }

      // ------- Creations interface ----------------------------------

      // Default value for shape is taken from random generator :)
      // Shape is an integer sum of POT directions:
      // 1 + 2 + 4 + 8
      WallPixel(int shape = 14);

      // ----- Implements IWallInterface ------------------------------------------

      bool isProtecting(const Context& context, shared_ptr<IAlive> victim, shared_ptr<IKilling> killer) override;
      bool blocksMoves( const Context& context, Coordinates from, Direction to, shared_ptr<Pixel> pix = nullptr ) override;
      bool blocksMagnetizm(const Context& context, shared_ptr<Pixel> from, shared_ptr<Pixel> to) override;

      // ----- Interface for Views ---------------------------------------

      const WallShape & GetShape() const { return shape; }
      int GetShapeInt() const ;

   protected:

      // wall can have different forms like L,|,_,U,O, etc
      WallShape shape;

      // Helper for checking
      bool IsBlocking(Direction dir);

      // Helper for defining shape
      void SetBlockAt(Direction dir);

      // Helper for shape
      void SetShapeInt(int sh);

   };

}


#endif // WALLPIXEL_H_
//===-- PixelsLayer.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/04
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Pixels layer displays and manages pixel views
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELSLAYER_H_
#define PIXELSLAYER_H_

#include "EMBase.h"
#include "MPix.h"
#include "Context.h"

namespace MPix {

   // Forward dependencies
   class PixelView;
   class GoalView;
   class Pixel;
   class Level;
   class AssemblyView;


   // PixelsLayer

   class PixelsLayer : public Layer
   {
   public:

      bool init() override;  
      CREATE_FUNC(PixelsLayer);

   public:

      PixelsLayer();
      ~PixelsLayer();

      void Reset();

   private: // Binded to commands

      ErrorCode CreateAllViews();

   private: // Storage

      // Store context
      const Context* activeContext;

   private: // Helpers

      ErrorCode CreateViewForPixel(shared_ptr<Pixel> p);
      void Clear();

   };

}


#endif // PIXELSLAYER_H_
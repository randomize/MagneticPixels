//===-- PixelsLayer.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/04
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Pixels layer displays and manages pixel views
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELSLAYER_H_
#define PIXELSLAYER_H_

#include "ECBase.h"
#include "MPix.h"
#include "Context.h"

namespace MPix {

   // Forward dependencies
   class PixelView;
   class GoalView;
   class Pixel;
   class Goal;
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

      // Create pixel view to this pixel and put to view manager
      ErrorCode CreateViewForPixel(shared_ptr<Pixel> p);

      // Create gloa view to this gloa and put to view manager
      ErrorCode CreateViewForGoal(shared_ptr<Goal> g);

      void Clear();

   };

}


#endif // PIXELSLAYER_H_
//===-- Context.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/05
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: This class holds all necessary info for a pixel logic
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELCONTEXT_H_
#define PIXELCONTEXT_H_

#include "EMBase.h"
#include "MPix.h"
#include "Events.h"

namespace MPix {

   // Forward dependencies
   class Field;
   class Goals;
   class Assembly;
   class EventProcessor;

   // Pixel interfaces
   class Pixel;
   class IMagnetic;
   class IWallPixel;
   class IKilling;
   class IColorful;
   class IMoveBlocker;
   class IProtector;

}

namespace MPix {

   // Context
   class Context
   {
   public:

      Context();

      // Current field, pixel can get info about neighbors
      shared_ptr<Field> field;

      // Current goal set, pixel can check them
      shared_ptr<Goals> goals;

      // Current assembly (if any), pixel can look in it
      shared_ptr<Assembly> assembly;

      // Events processor
      shared_ptr<EventProcessor> pixel_events;

      // Current move number, counting from last assembly, zero if not assembling
      int moveNumber;

      // Number of solving phases
      int solutionPhases;

   public:

      // Saves context
      void PushContextSnapshots();

      // Restores context
      void PopContextSnapshots(size_t number_of = 1);

   public: 

      // ================= Field helpers ======================

      // Return pixel by ID, null if not found
      shared_ptr<Pixel> GetByID( int id ) const;

      // Get top magnetic pixel at pos, null if no pixel
      shared_ptr<IMagnetic> GetTopMagenticAt(Coordinates pos) const;

      // Get pixel with color
      shared_ptr<IColorful> GetColorfulAt( Coordinates pos ) const;

      // Group of methods returning list of all pixels at position with given interface
      template<class T> bool GetPixelsAt(forward_list<shared_ptr<T>> &list, Coordinates pos) const;

      // Return pixel by its tag
      shared_ptr<Pixel> GetPixelByTag(int tag) const;

      // ================= Assembly helpers ======================

      // Adds pixel to assembly
      ErrorCode AddToAssembly( shared_ptr<Pixel> pix ) const;

      // Checks if all pixels of assembly stay on their colors goals
      bool IsAssemblySmiling() const;

      // Check is assembly is empty 
      bool IsAssemblyEmpty() const;

      // Get list of coordinates of all pixels
      std::forward_list<std::pair<int, Coordinates>> & GetAssemblyPositions() const;

      // Get coordinates of assembly center
      Vector2 GetAssemblyCenter() const;

      // ================= Assembly helpers ======================

      // Checks that a goal with given color is on that positions
      bool ExistsMatchingGoal(Coordinates pos, PixelColor color) const;

      // ================= Assembly helpers ======================

      // Pixel event post, by ID
      void PostEvent(PixelEvent e, int id, int priority = 0) const;

      // Pixel event post, by shared_ptr
      void PostEvent(PixelEvent e, shared_ptr<Pixel> p, int priority = 0) const;

      // Goal event post
      void PostEvent(GoalEvent e, int id, Coordinates pos, int priority = 0) const;

   private:

      Context(const Context&) {}// Do not copy me, please!

   };

}



#endif // PIXELCONTEXT_H_


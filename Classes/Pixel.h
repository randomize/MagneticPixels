//===-- Pixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Yes, this is simple pixel base class
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXEL_H_
#define PIXEL_H_

#include "EMBase.h"
#include "MPix.h"
#include "EMNode.h"
#include "Historical.h"

namespace MPix {

   // Forward dependencies
   class Context;

   // Pixel base class
   class Pixel : public EMNode
   {
   public:

      // Registering Pixel as base node, and create pixel factory
      EM_NODE_BASE(Pixel);

   public:

      // State of pixel, changes
      enum class State { 
         ACTIVE,      // Interacts 
         HIDDEN,      // Placed under other pixel, inactive but can be activated
         INACTIVE,    // Dead or played out
         Last,
         First = ACTIVE
      };

      // Type of pixels, constant, are more general than classes itself
      enum class PixelType { 
         WALL_PIX,     // Walls : SimpleWall, ValveWall
         PITFALL_PIX,  // Pitfall
         CACTUS_PIX,   // Cactus es : CactusDynamic, CactusStatic
         ASSEMBLY_PIX, // Assembly pixels
         SOKOBAN_PIX,  // SokobanPixel
         STONE_PIX,    // StonePixel
         Last,
         First = WALL_PIX
      };

      // ------- Creations interface ----------------------------------

      Pixel(int z = 0);
      virtual ~Pixel();
      virtual shared_ptr<Pixel> Dublicate();

      // -------- Setters ------------------------------------------

      inline void SetPos(Coordinates p) { pos = p; }
      inline void SetState(State s) { st = s; }
      inline void SetID(int id) { this->id = id; }
      inline void SetZ(int z) { this->z = z; }

      // -------- Informations -----------------------------------

      inline const int GetID() const { return id; }
      inline Point GetPosAsPoint() const  { return Point(GetPos().x, GetPos().y); }
      inline Coordinates GetPos() const { return pos; }
      //inline Coordinates GetOriginalPos() const  { return original_pos; }
      inline State GetState() const  { return st; }
      inline int GetZ() const { return z; }

      virtual PixelType GetType() const = 0;

      // -------- Undo/Redo system -----------------------------------

      // Init history
      virtual void InitSnapshots(const Context& context);

      // Store current changeable data
      virtual void PushSnapshot(const Context& context);

      // Clear snapshots, leaving current state
      virtual void ClearSnapshots(const Context& context);

      // Restore N snapshots
      virtual void PopSnapshots(const Context& context, int n);


   private:

      friend bool operator< (const shared_ptr<Pixel>& lhs, const shared_ptr<Pixel>& rhs);

      // Current position in field, can be changed only through field
      //Coordinates pos;
      Historical<Coordinates> pos;

      // Original position in field, cannot be changed
      //Coordinates original_pos; 

      // Current state
      Historical<State> st;

      // Z order is used to sort pixels
      int z;

      // ID will be set only by field in which pixels are put, shown to children
      int id;

   };

   bool operator< (const shared_ptr<Pixel>& lhs, const shared_ptr<Pixel>& rhs);

}

namespace std
{
   template <> struct hash<MPix::Pixel::State> {
      size_t operator()(const MPix::Pixel::State& v) const
      {
         return static_cast<unsigned>(v) ;
      }
   };
   template <> struct hash<MPix::Pixel::PixelType> {
      size_t operator()(const MPix::Pixel::PixelType& v) const
      {
         return static_cast<unsigned>(v) ;
      }
   };
}

#endif // PIXEL_H_
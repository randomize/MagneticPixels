//===-- Field.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/04
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Field represents 2D map of pixels
//
//===---------------------------------------------------------===//

#pragma once
#ifndef FIELD_H_
#define FIELD_H_

#include "EMBase.h"
#include "MPix.h"
#include "Pixel.h"


namespace MPix {

   // Forward dependencies
   class IMagnetic;
   class IWallPixel;
   class IAssembled;
   class IKilling;
   class IMoveBlocker;
   class IColorful;
   class Context;


   // Field

   class Field
   {
   public:

      Field();
      Field(const Field& oth);
      virtual ~Field();
      shared_ptr<Field> Dublicate();

      // =========== History support ==================================================//

      ErrorCode PushSnapshot( const Context& context );
      ErrorCode ClearSnapshots( const Context& context );
      ErrorCode PopSnapshots(const Context& context, int n = 1);
      ErrorCode InitSnapshots( const Context& context );

      // =========== World update methods =============================================//

      // Called on startup to send CREATE for active pixel views
      ErrorCode SendCreateEvents(const Context& context);

      // This happens immediately after player move is performed
      ErrorCode WorldAfterPlayersMove(const Context& context, bool blocked = false);

      // This happens after player move is performed and animated
      ErrorCode WorldMove(const Context& context);

      // Check lost
      ErrorCode WorldCheckForLost(const Context& context);

      // =========== Reading methods ================================================= //

      bool IsEmpty() const;

      shared_ptr<Pixel> GetPixelByID(int id);
      shared_ptr<Pixel> GetActivePixelByID(int id);
      shared_ptr<Pixel> GetPixelAt(int x, int y);
      shared_ptr<Pixel> GetPixelAt(Coordinates pos);
      shared_ptr<Pixel> GetActivePixelAt(int x, int y);
      shared_ptr<Pixel> GetActivePixelAt(Coordinates pos);
      list<shared_ptr<Pixel>> GetAllPixelsAt(int x, int y);
      list<shared_ptr<Pixel>> GetAllPixelsAt(Coordinates pos);
      list<shared_ptr<Pixel>> GetAllActivePixelsAt(int x, int y);
      list<shared_ptr<Pixel>> GetAllActivePixelsAt(Coordinates pos);

      shared_ptr<IMagnetic> GetTopMagenticAt(Coordinates pos);
      shared_ptr<IAssembled> GetTopAssembleAt(Coordinates pos);
      shared_ptr<IKilling> GetTopKillingAt(Coordinates pos);
      shared_ptr<IColorful> GetTopColorful(Coordinates pos);
      shared_ptr<IWallPixel> GetWallAt(Coordinates pos);

      shared_ptr<Pixel> GetPixelByTag(int tag);

      template<class T> bool GetPixelsAt( forward_list<shared_ptr<T>> &list, Coordinates pos ) ;


      // =========== Changing methods =========================================================== //

      ErrorCode MovePixelByID(int id, Coordinates pos);
      ErrorCode MovePixel(shared_ptr<Pixel> px, Coordinates pos);
      ErrorCode MovePixel(shared_ptr<Pixel> px, Direction d);
      ErrorCode SetPixelState(int id, Pixel::State st);
      ErrorCode SetPixelState(shared_ptr<Pixel> px, Pixel::State st);

      // =========== Creation methods, used by generator only ==================================== //

      ErrorCode InsertPixel(Pixel* p, Coordinates pos);
      ErrorCode InsertPixel(Pixel* p, int x, int y);
      ErrorCode InsertPixel(shared_ptr<Pixel> p);

      // ========== Low level getters ========================================================= //

      const vector<shared_ptr<Pixel>> & GetPixelData() const { return id_map; }

   private:

      // Main unique identification is made by id inside pixel and this searching map
      vector<shared_ptr<Pixel>> id_map;

      // Also possible to search by coordinates, can have collisions
      unordered_map<Coordinates,forward_list<int>> map;

      // Tag map
      unordered_map<int, unsigned> tags;

      //////////////////////////////////////////////////////////////////////////
      // Optimizations:

      enum class PixelKind {
         MOVE_BLOCKER,
         ALIVE,
         DYNAMIC,
         KILLER,
         Last,
         First = MOVE_BLOCKER
      };

      struct PixelKindHash
      {
         std::size_t operator()(const PixelKind &p) const
         {
            return static_cast<std::size_t>(p);
         }
      };

      unordered_map<PixelKind, unordered_set<int>, PixelKindHash > kind_map;
      unordered_map<Pixel::State, unordered_set<int>> state_map;
      unordered_map<Pixel::PixelType, unordered_set<int>> type_map;

   private:

      // =========== Internal helpers ==================================== //

      // Removes from map at was and places at now
      void UpdatePosInMap(int id, Coordinates was, Coordinates now);

      // Updates states maps
      void UpdateStateMap(Pixel::State was, Pixel::State now, int id);

      // Puts to map to where
      void PlacePosInMap(int id, Coordinates pos);

      // Remove from map at pos (and erase list if was last)
      void RemovePosFormMap(int id, Coordinates pos);

   };

}
/*
namespace std
{
   template <> struct std::hash<MPix::Field::PixelKind> {
      size_t operator()(const MPix::Field::PixelKind& v) const
      {
         return static_cast<unsigned>(v) ;
      }
   };
}*/

#endif // FIELD_H_

//===-- MPix.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Generic types and constants for game Magnetic Pixels
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MPIX_H_
#define MPIX_H_

   //== ------ Initial setup ----------------------------------------------

// This indicates that currently build is for developers, it hasbuggy editor,
// level saving, etc
#define MPIX_DEVELOPERS_BUILD

// Version string
#define MPIX_VERSION "v0.2"



// Include master header
#include "ECBase.h"



   //== ------ Constants ----------------------------------------------

   // For FG/BG notification in managers
   #define MPIX_DID_ENTER_BG  "Mpix::DidEnterBg"
   #define MPIX_WILL_ENTER_FG "Mpix::WillEnterFg"

   //== ------ Direction --------------------------------------

namespace MPix {

   struct Coordinates;

   // Direction type
   enum class Direction {

      // ATTENTION: Should never reorder this! order used in EnumRanger below

      // Major URDL 4 directions
      DIR_UP = 0,
      DIR_RIGHT,
      DIR_DOWN,
      DIR_LEFT,
      // Minor 4 directions
      DIR_UPRIGHT,
      DIR_UPLEFT,
      DIR_DOWNRIGHT,
      DIR_DOWNLEFT,
      // Special case - unknown
      DIR_UNKNOWN,
      Last,
      First = DIR_UP
   };

   // Get string representation of direction
   const char* DirectionsToString(Direction dir);

   // Yous you can feed streams with Direction
   ostream& operator<<(ostream& oStream, const Direction& dir);

   // URDL are major, others are secondary
   bool DirectionIsMajor(Direction dir);

   // If is major, just return, otherwise split to two
   list<Direction> SplitDirection(Direction dir);

   // Returns opposite direction
   Direction ReverseDirection(Direction dir);

   // Returns random direction
   Direction RandomDirection();

   // Return direction from Start to End
   Direction GetDirectionVector(Coordinates s, Coordinates e);
   Direction GetDirectionVector(Vector2 s, Vector2 e);

   // Helper enum for direction classification
   enum class DirectionType {
      EVERY = 0,  // All 8 Dir + unknown
      ALL,        // All 8 Dir
      MAJOR,      // Major URDL 4 Dir
      MINOR       // Minor 4 Dir
   };

}

namespace std
{
   template <> struct hash<MPix::Direction>
   {
      size_t operator()(const MPix::Direction& v) const {
         return static_cast<int>(v);
      }
   };
}

template<> struct EnumRanger<MPix::Direction> {
   EnumRanger(MPix::DirectionType t = MPix::DirectionType::ALL) {
      type = t;
      // Using simple order-dependent implementation, be careful here
      // FIXME: Replace with more error-prone method
      dd[0][0] = MPix::Direction::First; dd[0][1] = MPix::Direction::Last;
      dd[1][0] = MPix::Direction::First; dd[1][1] = MPix::Direction::DIR_UNKNOWN;
      dd[2][0] = MPix::Direction::First; dd[2][1] = MPix::Direction::DIR_UPRIGHT;
      dd[3][0] = MPix::Direction::DIR_UPRIGHT; dd[3][1] = MPix::Direction::DIR_UNKNOWN;
   }
   enum_iterator<MPix::Direction> begin() { return enum_iterator<MPix::Direction>( dd[static_cast<int>(type)][0] ); }
   enum_iterator<MPix::Direction> end() { return enum_iterator<MPix::Direction>( dd[static_cast<int>(type)][1] ); }
private:
   MPix::DirectionType type;
   MPix::Direction dd[4][2];
};
   //== ------ Coordinates point --------------------------------------

namespace MPix {
   struct Coordinates {
      int x;
      int y;
      Coordinates(int x = 0, int y = 0);
      bool operator == (const Coordinates& rhs) const;
      bool operator != (const Coordinates& rhs) const;
      friend ostream& operator<<(ostream& os, const Coordinates& b);
      void MoveTo(Direction dir);
      Coordinates operator+(const Direction& d);
      Coordinates operator+(const Coordinates& c);
      static Coordinates ZERO;
   };
   ostream& operator<<(ostream& oStream, const Coordinates& coords);
}

namespace std
{
   template <> struct hash<MPix::Coordinates>
   {
      size_t operator()(const MPix::Coordinates& v) const {
         return v.x << 16 | ( v.y & 0x0000FFFF ); // As 0xXXXXYYYY
      }
   };
}

   //== ------ Rectangle class --------------------------------------

namespace MPix {
   struct Rectangle {
      Coordinates BL; // Bottom Left
      Coordinates TR; // Top Right
      bool operator== (const Rectangle& rhs) const;
      bool operator!= (const Rectangle& rhs) const;
      friend ostream& operator<<(ostream& os, const Rectangle& b);
      void MoveTo(Direction dir);
      void MoveTo(Direction dir, int len);
   };
   ostream& operator<<(ostream& oStream, const Rectangle& coords);
}
   //== ------ For cocos classes --------------------------------------

namespace cocos2d {
   namespace math {
   ostream& operator<<( ostream& os, const Vector2& b );
   }
}

//== ------ HSV color --------------------------------------
struct HSVColor {

   HSVColor(float hue = 0.0f,float saturation = 1.0f,float value = 1.0f ) :
      hue(hue), saturation(saturation), value(value) {}

   float hue;
   float saturation;
   float value;

};


//////////////////////////////////////////////////////////////////////////
// Constants

namespace MPix {
   class Pixel;


   // Defines cell size in design resolution coordinates
   extern const float MPIX_CELL_SIZE;
   extern const float MPIX_CELL_SIZE_HALF;
   extern const Vector2 MPIX_CELL_SIZE_P;
   extern const Vector2 MPIX_CELL_SIZE_HALF_P;

   // Converts screen to logic
   Coordinates ScreenToLogic(Vector2 p);

   // Converts logic to screen
   Vector2 LogicToScreen(Coordinates c);
   Vector2 LogicToScreen(int x, int y);

}

namespace MPix {

   //////////////////////////////////////////////////////////////////////////
   // Pixel color

   enum class PixelColor {
      WHITE = 0,
      GRAY,
      RED,
      GREEN,
      BLUE,
      YELLOW,
      VIOLET,
      CYAN,
      PINK,
      Last,
      First=WHITE
   };

   const char* PixelColorToStr(PixelColor pc);
   HSVColor PixelColorToHSV(PixelColor pc);
   Color3B PixelColorToRGB(PixelColor pc);
   ostream& operator<<(ostream& oStream, const PixelColor& color);

}



// Localized string or resource name/directory shorthand


const char* LocalUTF8Char(const char* key);
const string& LocalUTF8Str(const string& key);


// Core universal things like Localization, PlatformDependent;
using namespace ECCore;

#endif // MPIX_H_

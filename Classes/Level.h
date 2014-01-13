//===-- Level.h -----------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Level stores map, info, score etc, it is only container
//
//===---------------------------------------------------------===//

#pragma once
#ifndef LEVEL_H_
#define LEVEL_H_

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // Forward dependencies
   class Goals;
   class Field;
   class Solution;

   // Level class
   class Level
   {
   public:

      Level();
      ~Level();
      shared_ptr<Level> Dublicate();

      enum class State {
         IS_LOCKED = 0,
         IS_OPEN,
         IS_SOLVED,
         IS_PLAYING
      };

      // G/S for world, setts when loaded from XML
      const int GetWorld() const { return world; }
      void SetWorld(int w) { world = w; }

      // G/S for state, sets when loaded profile data, and played by GPM
      State GetState() const { return st; }
      void SetState(const State st) { this->st = st; }

      // Getters for field and goals, used bu GPM
      shared_ptr<Field> GetField() const { return field; }
      shared_ptr<Goals> GetGoals() const { return goals; }

      // Getter for auto pan, used by GPM
      bool AutoPan() { return auto_pan; }
      void SetAutopan(bool v) { auto_pan = v; }

      // Getter for viewport, used by GPM
      const Rectangle & GetViewport() const { return viewport; }

      // Getter for ID, used by LM
      unsigned int GetID() const { return levelID; }

      // Name
      const string & GetName() const { return name; }
      void SetName(const string& n) { name = n; }

      // Stars
      int GetStars() const { return starz; }

   private: 

      //////////////////////////////////////////////////////////////////////////
      // Data set outside
      
      // State defines level 
      State st;

      unsigned int world;

      // Loaded from profile, can be 0 1 2 3
      int starz;

      //////////////////////////////////////////////////////////////////////////
      // Static internal data

      // Unique identifier 32 bits, random number
      unsigned int levelID;

      // Name is simple string
      string name;

      // Use or not auto pan, set to true for levels > 8x12
      bool auto_pan;

      // Viewport defines level startup zoom and pan
      Rectangle viewport;

      // Field and goal
      shared_ptr<Field> field;
      shared_ptr<Goals> goals;

      // Solutions
      shared_ptr<Solution> solution;

   private:

      friend class LevelGenerator;
      friend class LevelXML;

   };

}


#endif // LEVEL_H_

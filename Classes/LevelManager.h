//===-- LevelManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Level manager holds all levels and worlds of the game
//
//===---------------------------------------------------------===//

#pragma once
#ifndef LEVELMANAGER_H_
#define LEVELMANAGER_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies
   class Level;
   class World;

   // LevelManager

   class LevelManager
   {
   public:

      enum class State {
         EMPTY = 0,
         LOADED
      };

      //////////////////////////////////////////////////////////////////////////
      // Level acquire methods, return NEW level objects, ready to be played

      // Returns empty new level
      shared_ptr<Level> GetEmpty();

      // Debugging method, returns test level
      shared_ptr<Level> GetPlayableTestLevel();

      // Returns level object by ID, new one! but holding old copy or info for quick build again
      shared_ptr<Level> GetPlayableLevelByID(unsigned int levelID);

      // Returns here last returned level =)
      shared_ptr<Level> GetPlayableLastLevel();

      // Store level for editing between sessions/test runs
      shared_ptr<Level> GetPlayableEditorsLevel();

      // Return level next to last ;) returns nullptr on game end
      shared_ptr<Level> GetPlayableNextLevel();

      //////////////////////////////////////////////////////////////////////////
      // Misc info about levels

      // Stars 0 - 3 int
      int GetStarsByLevelID(unsigned int levelID);

      // Is it locked or not
      bool GetLockedByLevelID(unsigned int levelID);

      // Returns levels name
      const string& GetNameByLevelID(unsigned int levelID);

      // Return level by id, WARNING: it will be unplayable and it is alive lm's level
      shared_ptr<Level> GetLevelByID(unsigned int levelID);


      //////////////////////////////////////////////////////////////////////////
      // Worlds

      // Get all worlds ids
      const vector<int>& GetWorldIDs(void) ;

      // Get concrete world
      shared_ptr<World> GetWorldByID(int wID);

      // Get worlds name
      const string& GetNameByWorldID(int wID);

      // Get worlds level count
      int GetLevelCountByWorldID(int wID);

      // Get worlds first level id
      unsigned GetFirstLevelByWorldID(int wID);

      // Get worlds last level id
      unsigned GetLastLevelByWorldID(int wID);

      // Get worlds next level id
      unsigned GetNextLevelByWorldID(int wID, unsigned levelID);

      // Get all levels in world
      vector<unsigned int> GetLevelsByWorldID(int wID);


      //////////////////////////////////////////////////////////////////////////
      // For level editor

      void SetEditorsLevel( shared_ptr<Level> l );
      shared_ptr<Level> EditorGetCurrentLevel();
      void SaveEditorsLevel();

      //////////////////////////////////////////////////////////////////////////
      // Storage manipulation

      // Load levels from default storage
      ErrorCode LoadData();

      // Level manipulation
      void StoreLevel(shared_ptr<Level> level);

      // Delete given level
      void DeleteLevelByID(unsigned int levelID);

      // Shorthand for common procedure of level preparing
      shared_ptr<Level> DupeLastAndMakePlayable();

   private:

      //////////////////////////////////////////////////////////////////////////
      // Data

      // Map holds all levels with assigned ID's
      unordered_map<unsigned int, shared_ptr<Level>> levels;

      // Ordered list of worlds
      list<shared_ptr<World>> worlds;

      // State of level manager
      State state;

      // Holds null or currently editing level
      shared_ptr<Level> ed_lvl;

      // Holds last level given
      shared_ptr<Level> last_lvl;

      //////////////////////////////////////////////////////////////////////////
      // Helpers


      // Clear levels and worlds maps
      ErrorCode ResetData();

      //////////////////////////////////////////////////////////////////////////
      // Caches and optimizations

      // Searchmap for mapping ID => world
      unordered_map<int,shared_ptr<World>> worlds_map;

      // Cached list of worlds id's
      vector<int> worlds_ids;

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 

      static LevelManager& getInstance() {
         static LevelManager theSingleInstance;
         return theSingleInstance;
      }

   private:

      LevelManager();
      LevelManager(LevelManager& root){}
      LevelManager& operator=(LevelManager&){return *this;}
   };

}


#endif // LEVELMANAGER_H_

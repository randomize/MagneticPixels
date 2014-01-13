//===-- LevelManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Level manager holds all levels and worlds of the game
//
//===---------------------------------------------------------===//

#pragma once
#ifndef LEVELMANAGER_H_
#define LEVELMANAGER_H_

#include "EMBase.h"

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
      // Level acquire methods

      // Returns empty new level
      shared_ptr<Level> GetEmpty();

      // Debugging method, returns test level
      shared_ptr<Level> GetTestLevel();

      // Returns level object by ID, new one! but holding old copy or info for quick build again
      shared_ptr<Level> GetLevelByID(unsigned int levelID);

      // Returns here last returned level =)
      shared_ptr<Level> GetLastLevel();

      // Store level for editing between sessions/test runs
      shared_ptr<Level> GetEditorsLevel();

      // Misc info about levels
      int GetLevelStarsByID(unsigned int levelID);
      const string& GetLevelNameByID(unsigned int levelID);

      //////////////////////////////////////////////////////////////////////////
      // Worlds

      // Get all worlds ids
      vector<int> GetWorldsIDs(void) ;

      // Get all levels in world
      vector<unsigned int> GetLevelsInWorld(int wID);

      // Get concrete world
      shared_ptr<World> GetWorldByID(int wID);

      // Misc info about worlds
      const string& GetWorldNameByID(int wID);
      int GetWorldLevelCountByID(int wID);

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

   private:

      //////////////////////////////////////////////////////////////////////////
      // Data

      // Map holds all levels with assigned ID's
      unordered_map<unsigned int, shared_ptr<Level>> levels;
      list<shared_ptr<World>> worlds;
      unordered_map<int,shared_ptr<World>> worlds_map;

      // State of level manager
      State st;

      // Holds null or currently editing level
      shared_ptr<Level> ed_lvl;

      // Holds last level given
      shared_ptr<Level> last_lvl;

      //////////////////////////////////////////////////////////////////////////
      // Helpers

      // Return level by id
      shared_ptr<Level> FindLevel(unsigned int levelID);

      // Clear levels and worlds maps
      ErrorCode ResetData();


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

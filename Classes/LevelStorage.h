//===-- LevelStorage.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/03
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: XML loader can load data from levels.xml file
//
//===---------------------------------------------------------===//

#pragma once
#ifndef XMLLOADER_H_
#define XMLLOADER_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies
   class World;
   class Level;

   // LevelStorage
   class CC_DLL LevelStorage
   {
   public:
      ~LevelStorage();

      void SetMap(const string & levelMap);

      ErrorCode GetLevels( 
         list<shared_ptr<World>> &worlds, 
         unordered_map<unsigned int, shared_ptr<Level>> &levels
      );

      void SaveLevel( shared_ptr<Level> level );

      void DeleteLevel( unsigned id );

      ErrorCode ExportMap();
      const char* GetExportDir();

   private:

      string levelMap;
      string exportDir;

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 
   
      static LevelStorage* getInstance() {
         static LevelStorage theSingleInstance;
         return &theSingleInstance;
      }
   
   private:
   
      LevelStorage();
      LevelStorage(LevelStorage& root){}
      LevelStorage& operator=(LevelStorage&){return *this;}

   };

}


#endif // XMLLOADER_H_
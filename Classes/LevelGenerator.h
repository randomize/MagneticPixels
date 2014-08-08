//===-- LevelGenerator.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Level generator creates or loads levels
//
//===---------------------------------------------------------===//

#pragma once
#ifndef LEVELGENERATOR_H_
#define LEVELGENERATOR_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies
   class Level;
   class EditorData;
   class TiXmlElement;

   // LevelGenerator

   class LevelGenerator
   {
   public:

      shared_ptr<Level> CreateEmpty();
      shared_ptr<Level> CreateTestLevel();
      shared_ptr<Level> CreateFromEditorData(EditorData* source, shared_ptr<Level> target);
      EditorData* SaveToEditorData(shared_ptr<Level> source);

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 
   
      static LevelGenerator* getInstance() {
         static LevelGenerator theSingleInstance;
         return &theSingleInstance;
      }
   
   private:
   
      LevelGenerator();
      LevelGenerator(LevelGenerator& root){}
      LevelGenerator& operator=(LevelGenerator&){return *this;}
   
   
   };

}


#endif // LEVELGENERATOR_H_
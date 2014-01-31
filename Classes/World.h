//===-- World.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Worlds are groups in which levels are stored
//
//===---------------------------------------------------------===//

#pragma once
#ifndef WORLD_H_
#define WORLD_H_

#include "EMBase.h"

namespace MPix {

   // World

   class World
   {
   public:

      World(int id);
      World(int id, const char* name);

      const string& GetName();

      int GetID();
      int GetLevelCount() { return levels.size(); }

      void AddLevel(unsigned id);
      void EraseLevel(unsigned id);

      const list<unsigned> & GetLevels() { return levels; }

   private:

      int id;
      string name;

      list<unsigned> levels;

   };

}


#endif // WORLD_H_
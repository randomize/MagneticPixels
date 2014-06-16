//===-- EditorData.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/20
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Editor data stores raw and _mutable_ information
//               of levels field and goal
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EDITORDATA_H_
#define EDITORDATA_H_

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // Forward dependencies
   class Pixel;
   class LevelGenerator;
   class Goal;
   enum class PixelColor;


   // EditorData

   class EditorData
   {
   public:

      EditorData();

      // Pixels
      void InsertPixelAt(Coordinates pos, shared_ptr<Pixel> p);
      int NumberOfPixelsAt(Coordinates pos);
      bool RemoveTopPixelAt(Coordinates pos);
      bool RemoveAllPixelsAt(Coordinates pos);

      // Goals
      bool InsertTask(int index, Coordinates pos, PixelColor col);
      bool RemoveTask(Coordinates pos);

      // Wal;s
      void InsertWallAt(Coordinates pos, shared_ptr<Pixel> p);
      bool RemoveWallAt(Coordinates pos);

      // Common
      bool CheckConsistency();
      std::vector<shared_ptr<Pixel>> GetAllWalls();
      std::vector<shared_ptr<Pixel>> GetAllPixels();
      std::vector<std::pair<Coordinates, PixelColor>> GetAllGoals();

      // Name
      void SetName(const string& s) { name = s; }
      const string& GetName() const { return name; }

   private:

      friend LevelGenerator;
      unordered_map<Coordinates, vector<shared_ptr<Pixel>>> field;
      unordered_map<Coordinates, shared_ptr<Pixel>> walls;
      vector<shared_ptr<Goal>> goals;
      string name;

   };

}


#endif // EDITORDATA_H_

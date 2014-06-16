//===-- Ranking.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/17
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Ranking holds ranks for killers, blockers etc
//
//===---------------------------------------------------------===//

#pragma once
#ifndef RANKING_H_
#define RANKING_H_

#include "EMBase.h"

namespace MPix {

   // Killers ranking
   namespace KillersRanking
   {
      static const int PITFALL = 50;
      static const int BOMB = 30;
      static const int STONE = 25;
      static const int CACTUS = 10;
      static const int FIRE = 5;
   };

   // Blockers ranking
   namespace BlockersRanking
   {
      static const int WALL = 50;
      static const int STONE = 25;
      static const int PIXEL = 5;
   };

   // Depth ranking
   namespace DepthRanking
   {
      static const int WALL = 50;
      static const int STONE = 25;
      static const int PIXEL = 5;

      //static constexpr float TEST_CPP11 = 42.f; // VS2012 Sucks... Microsoft F* you!
   };
}


#endif // RANKING_H_
//===-- LevelSelector.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/04
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Level selector state displays grid of levels/wordls
//
//===---------------------------------------------------------===//

#pragma once
#ifndef LEVELSELECTOR_H_
#define LEVELSELECTOR_H_

#include "EMBase.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies


   // LevelSelector

   class LevelSelector : public GameState
   {
   public: //  === State interface =====================

      EM_GAME_STATE(LevelSelector);
      LevelSelector();
      ~LevelSelector();
      bool init() override;

      void onEnter() override;
      void onExit() override;

   private: // Helpers

      void SelectedLevel(unsigned int id);
      void SelectedWorld(int id);
      void BackToWorlds();
      void BackToMainMenu();

      // Data
      Menu* world_m; // World menu
      Menu* level_m; // Level menu



   };

}


#endif // LEVELSELECTOR_H_
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

   private: 

      // Handlers
      void SelectedLevel(unsigned int id);
      void SelectedWorld(int id);
      void BackToWorlds();
      void BackToMainMenu();

      // Data
      Menu* world_m; // World menu
      Menu* level_m; // Level menu

      // Touch handlers
      bool     onTouchBegan( Touch *touch, Event *event);
      void onTouchCancelled( Touch *touch, Event *event);
      void     onTouchEnded( Touch *touch, Event *event);
      void     onTouchMoved( Touch *touch, Event *event);

      // Current state
      enum class State {
         WAIT,
         SCROLL
      } state;

      // Scrollable layer with worlds
      Layer* worlds_layer;
      Point initial_pos;

      // UI misc helper constants
      unordered_map<int,Node*> title_lables; // ID -> label
      unordered_map<int, int>   ids_indexes; // ID -> index
      vector<int> indexed_ids;               // Index -> ID
      int current_index;                     // Selected world index

   };

}


#endif // LEVELSELECTOR_H_
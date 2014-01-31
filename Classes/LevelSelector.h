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
#include "MPix.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies
   class LevelView;


   // LevelSelector

   class LevelSelector : public GameState
   {
   public: //  === State interface =====================

      EM_GAME_STATE(LevelSelector);

      void onEnter() override;
      void onExit() override;

   protected:

      bool init() override;

   private: 

      // Handlers

      // Loads level with given id does to game main sate
      void SelectedLevel(unsigned int id);

      // Returns to main menu state
      void BackToMainMenu();

      // Current state
      enum class State {
         WAIT,
         BUTTON,
         IGNORING,
         SCROLL,
         ANIMATING
      } state;

      // Scrollable layer with worlds
      Layer* worlds_layer;
      Point initial_pos, initial_touch;
      void ElasticBounceToCurrentWorld();
      Point NormalizePozition(Point pos);

      // Worlds
      unordered_map<int,Node*> title_lables; // ID -> label
      unordered_map<int, int>   ids_indexes; // ID -> index
      vector<int> indexed_ids;               // Index -> ID
      vector<float> indexed_positions;       // Index -> Worlds Layer X coord

      int current_index;                     // Selected world index
      int world_count;                       // Number of worlds

      // Levels
      vector<vector<LevelView*>> indexed_views; // World ID -> level index -> Button(LevelView)

      // Flip world actions
      void NextWorld();
      void PrewWorld();

      // Arrow Buttons
      void UpdateButtons();
      Node* prew_button;
      Node* next_button;

      // Saved geometry
      Size fullSize, halfSize, visibleSize;
      Point lowerLeft, lowerRight, centerPoint, upperLeft, upperRight;

      //////////////// TOUCH HANDLING

      // Touch callbacks
      bool     onTouchBegan( Touch *touch, Event *event);
      void onTouchCancelled( Touch *touch, Event *event);
      void     onTouchEnded( Touch *touch, Event *event);
      void     onTouchMoved( Touch *touch, Event *event);

      // Idling helps not to recalculate to much of data on each on move
      int indling_couner;

      enum class Gesture {
         TO_NEXT,
         TO_PREW,
         SAME
      } gesture_action;

      /////////////// LEVEL BUTTONS

      // Current clicked level button
      LevelView* m_cur_button;

      // Helper method, searches for tap match, nullptr if not found
      LevelView* GetViewAtPoint(Point touch_pos);

#ifdef MPIX_DEVELOPERS_BUILD

      // Super open on triple click
      unsigned last_locked, pre_last_locked;

#endif

      ///////////// Init Helpers
      void CreateLevelButtons();
      void CreateArrowButtons();

   };

}


#endif // LEVELSELECTOR_H_
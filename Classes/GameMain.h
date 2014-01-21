//===-- GameMain.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Main game state is active when playing
//
//===---------------------------------------------------------===//

#pragma once
#ifndef GAMEMAIN_H_
#define GAMEMAIN_H_

#include "EMBase.h"
#include "MPix.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies
   class TouchLayer;
   class PixelsLayer;
   class Command;

   // Game scene
   class GameMain: public GameState
   {
   public:

      EM_GAME_STATE(GameMain);
      GameMain();
      ~GameMain();
      bool init() override;

      void onEnter() override;
      void onExit() override;

      ErrorCode Tick( float t ) override;

   protected:

      // Layers, constructing the content
      PixelsLayer* pixels;
      TouchLayer* touch;
      Layer * bg;
       
      // Content holds pixels and touch layers, making easy to scroll/zoom
      Node* content;

   protected:

      // Command to display results on game finish
      virtual void CreateButtons();
      virtual ErrorCode FinishedGame();

      void BtnHnadler(Object* sender);

   private: // Commands

      // Handles CmdGameFinish
      ErrorCode FinishGame();

   public: // General UI game constants

      // Defines logic tick ratio
      static int const GAMEPLAY_TICK_RATIO;

      // UI Elements Z order constants
      static int const Z_WALLS;
      static int const Z_PIXEL_BG;
      static int const Z_PIXEL_MIMICS;

   };

}

#endif // GAMEMAIN_H_

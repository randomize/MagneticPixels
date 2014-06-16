//===-- GameStateManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/01
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: This singleton manages game states and switches
//
//===---------------------------------------------------------===//

#pragma once
#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "EMBase.h"
#include "GameState.h"

namespace MPix {

   class GameStateManager
   {

   public: ////// States switching ///////////////////////////////////////////////////////////////////

      // Entry point 
      void SwitchToStart(void);

      // "Exit point" - termination of game
      void SwitchToExit(void);

      // Starts normal game
      void SwitchToGame(void);

      // Starts test game
      void SwitchToTestGame(void);

      // Shows main menu
      void SwitchToMenu(void);

      // Shows level selector
      void SwitchToSelector(void);

      // Shows settings
      void SwitchToSettings(void);

      // Shows editor
      void SwitchToEditor(void);

      // Shows results
      void SwitchToResults(void);

      // Editor Menu
      void SwitchToEditorMenu(void);

      // Credits scene
      void SwitchToCredits();

   public: ////// Common interface ////////////////////////////////////////////////////////////////////////

      // Informations
      const GameState* CurrentState(void) const { return currentState; }

      // Lock/Unlock GUI called when animating
      void Busy(bool state);

      // Update current state
      void Tick( float t );

      // Returns current time
      double GetElapsedTime();

   public: ////// Singleton ///////////////////////////////////////////////////////////////////

      static GameStateManager& getInstance() {
         static GameStateManager theSingleInstance;
         return theSingleInstance;
      }

      //////////////////////////////////////////////////////////////////////////

   private: 

      GameStateManager();
      GameStateManager(GameStateManager& root){}
      GameStateManager& operator=(GameStateManager&){return *this;}

      GameState* currentState;
      bool SwitchScene( GameState* newState );
      double timer;

   };


}

#endif // UIMANAGER_H_

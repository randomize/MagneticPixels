//===-- GameplayManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: This class implements game rules and logic
//
//===---------------------------------------------------------===//

#pragma once
#ifndef GAMEPLAYMANAGER_H_
#define GAMEPLAYMANAGER_H_

#include "ECBase.h"
#include "MPix.h"

#include "Context.h"


namespace MPix {

   // Forwards dependencies
   class Level;
   class Assembly;
   class Command;
   class IAssembled;
   class Goal;

   class GameplayManager
   {
   public:

      enum class State {
         IDLE,       // Idle state means no level is loaded
         READY,      // Level is loaded, waiting to begin
         PLAYING,    // Game is playing, pixels sleep
         PAUSED,    // Game is paused by ingame menu
         FINISHED   // Game is finished, shown results
      };

      // Current state getter
      State GetState(void) const;

      // Active context getter
      const Context & GetContext() const;

   public:  // Global controllers - take effect immediately

      // Loads new level
      ErrorCode LoadLevel(shared_ptr<Level> target);

      // Reset should be called when before new level loaded
      // Cleanups manager and sets state to Idle, returns RET_OK
      ErrorCode Reset();

      // Inits context and starts game with loaded level
      ErrorCode Play();

   public:

      // Command system: two schedulers and ProcessOneCommand executor
      ErrorCode ProcessOneCommand();
      ErrorCode PostCommand(Command* cmd);
      ErrorCode PostPriorityCommand(Command* cmd);

   private: // Commands handlers

      ErrorCode ClickAtPoint(Coordinates position);
      ErrorCode MoveAssembly(Direction whereMove);
      ErrorCode AfterMoveAssembly(Direction whereMove);
      ErrorCode MoveAssemblyFast(Direction whereMove);
      ErrorCode GrowAssembly();
      ErrorCode UndoMove();
      ErrorCode CancelAssembly();

      // Occurs once after Play called and all views created and played animations
      ErrorCode FirstMove();

   private: // Helper funs

      ErrorCode StartAssembling(shared_ptr<IAssembled> pixel);
      ErrorCode EndAssembling();
      ErrorCode UpdateUI();

      ErrorCode CheckForSolution();

      // Checking lose status
      ErrorCode CheckLoseStatus();

      // UI messages from Manager itself
      ErrorCode SendUINeedWakePixelFirst(); // "Please wake a pixel" message
      ErrorCode SendUIPleaseUndoOrRestart(); // "You can't win now" message
      ErrorCode SendUIFullSolution(); // Some winning sound and message
      ErrorCode SendUIPartSolution(); // Some winning sound and message
      ErrorCode SendUIAcceptSolution(); // Again some sound and message

   private: // Data and low level helpers

      // Holds currently loaded level
      shared_ptr<Level> level;

      // Holds level and assembly shortcuts as context
      Context context;

      // Holds state
      State st;

      // Holds commands, directed to GameplayManager
      list<Command* > commands;

      // State helpers
      ErrorCode SwitchState(State to);
      const char * StateString(State s);

   ////// Singleton ///////////////////////////////////////////////////////////////////

   public: 

      static GameplayManager& getInstance() {
         static GameplayManager theSingleInstance;
         return theSingleInstance;
      }

   private:

      GameplayManager();
      GameplayManager(GameplayManager& root) {}
      GameplayManager& operator=(GameplayManager&){return *this;}
   };

}


#endif // GAMEPLAYMANAGER_H_

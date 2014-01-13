//===-- GameState.h --------------------------------*- C++ -*-===//
//
//  Created:     2013/09/01
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Base class for any state in game 
//
//===---------------------------------------------------------===//

#pragma once
#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies
   class Command;

   // Game state controls game in given moment of time
   class GameState: public Scene
   {
   public:

      GameState();
      virtual ~GameState();

      // Each state has string name
      virtual const char* GetName() const = 0;

      // Each state can execute commands immediately
      virtual ErrorCode Execute(Command* cmd) const;

      // Each state can execute commands delayed
      virtual ErrorCode ExecuteAsync(Command* cmd);

      // Tick executed on update, default one just processes commands
      virtual ErrorCode Tick(float deltha);

      // Busyness is used to make game wait until certain animation finishes 
      bool IsBusy();

      // When called, state should become busy
      void Lock();

      // Control user interface availability
      void Unlock();

   protected:

      // Children cant acces semaphore, but cen reset locks
      void ResetLocks();

      // For async commanding
      vector<Command*> scheduled_commands;

   private:

      int semaphore;

   };

   // GAME_STATE is shorthand for managing states
   /* Usage:
         class MyGameState : public GameState {
            GAME_STATE(MyGameState);
            ..
         };
   */

   #define EM_GAME_STATE(Name)                                     \
      const char* GetName() const override { return #Name; } \
      CREATE_FUNC(Name)                                         \
   // GAME_STATE

}

#endif // GAMESTATE_H_

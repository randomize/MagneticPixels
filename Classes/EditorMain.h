//===-- EditorMain.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: State of editing
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EDITORMAIN_H_
#define EDITORMAIN_H_

#include "EMBase.h"
#include "MPix.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies
   class EditorLayer;
   class Command;
   class Level;
   class EditorToolbox;

   // Game scene
   class EditorMain: public GameState
   {
   public:

      EM_GAME_STATE(EditorMain);
      EditorMain();
      ~EditorMain();
      bool init() override;

      void onEnter() override;
      void onExit() override;

      ErrorCode ExecuteAsync( Command* cmd ) override;
      ErrorCode Tick( float t ) override;

   private:

      EditorLayer* editor;
      EditorToolbox* tb;
      Layer * bg;
      LayerColor * tb_bg;
      float contentScale;
      Label* lvl_name;
       
      // For async commanding
      vector<Command*> scheduled_commands;

   private: // Internal helpers

      void BtnHnadler(Ref* sender);
      void Renamer(string lname);
      void TestLevel ();
      void ShowHint(const string& text);
      void SaveLevel();

   public: // Actions

      void ShowToolBox();
      void HideToolBox();
      void ReuseLastTool();

   };

}

#endif // EDITORMAIN_H_
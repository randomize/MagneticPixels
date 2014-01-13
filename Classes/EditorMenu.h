//===-- EditorMenu.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/15
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Menu alows to cerate, delete and edit levels
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EDITORMENU_H_
#define EDITORMENU_H_

#include "EMBase.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies


   // EditorMenu

   class EditorMenu : public GameState
   {
   public:

      EM_GAME_STATE(EditorMenu);
      EditorMenu();
      ~EditorMenu();
      bool init() override;

      void onEnter() override;
      void onExit() override;

   private:

      void SelectedLevel(unsigned int id);
      void SelectedWorld(int id);
      void BackToWorlds();
      void BackToEditorMenu();
      void BackToMainMenu();

      void SelectedNewLevel();
      void SelectedLast();
      void SelectedEdit();
      void SelectedDelete();
      void SelectedExportMap();

      void BuildWorldsMenu();
      void BuildLevelsMenu(int w);
      void BuildEditorsMenu();

      // Data
      Menu* edit_m; // Editors menu
      Menu* world_m; // World menu
      Menu* level_m; // Level menu

      // Mode
      enum class Mode {
         IDLE,
         DELETE,
         EDIT,
         NEWFAB
      } mode;
      unsigned confirmed;

   };

}


#endif // EDITORMENU_H_
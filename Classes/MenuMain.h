//===-- MenuMain.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/20
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Main menu, simple version for now
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MENUMAIN_H_
#define MENUMAIN_H_

#include "EMBase.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies


   // MenuMain

   class MenuMain : public GameState
   {
   public: //  === State interface =====================

      EM_GAME_STATE(MenuMain);

      bool init() override;

      void onEnter() override;

   private: // Helpers

      void ToEditor();
      void ToTest();
      void ToSelector();
      void ToExit();
      void ToCredits();

   };

}


#endif // MENUMAIN_H_
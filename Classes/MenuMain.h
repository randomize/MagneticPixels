//===-- MenuMain.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/20
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Main menu, simple version for now
//
//===---------------------------------------------------------===//

#pragma once
#ifndef MENUMAIN_H_
#define MENUMAIN_H_

#include "ECBase.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies


   // MenuMain

   class MenuMain : public GameState
   {
   public: //  === State interface =====================

      ECGAME_STATE(MenuMain);

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
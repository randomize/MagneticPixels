//===-- CreditsState.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/01/29
//  Author:      Mihailenco E. at TheEndlessCat Games, 2014
//  Description: Credits state
//
//===---------------------------------------------------------===//

#pragma once
#ifndef CREDITSSTATE_H_
#define CREDITSSTATE_H_

#include "EMBase.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies


   // CreditsState

   class CreditsState : public GameState
   {
   public:

      EM_GAME_STATE(CreditsState);
      CreditsState();
      ~CreditsState();

      virtual bool init() override;

      virtual void onEnter() override;

   protected:

   private:

   };

}


#endif // CREDITSSTATE_H_



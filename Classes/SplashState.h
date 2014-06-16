//===-- SplashState.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/01/29
//  Author:      Mihailenco E. at TheEndlessCat Games, 2014
//  Description: 
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SPLASHSTATE_H_
#define SPLASHSTATE_H_

#include "EMBase.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies

   // SplashState

   class SplashState : public GameState
   {
   public:

      EM_GAME_STATE(SplashState);
      SplashState();
      ~SplashState();

      virtual bool init() override;

      virtual void onEnter() override;

   protected:

   private:

   };

}


#endif // SPLASHSTATE_H_


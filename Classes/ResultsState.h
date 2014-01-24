//===-- ResultsState.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/07
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: 
//
//===---------------------------------------------------------===//

#pragma once
#ifndef RESULTSSTATE_H_
#define RESULTSSTATE_H_

#include "EMBase.h"
#include "GameState.h"

namespace MPix {

   // Forward dependencies


   // ResultsState

   class ResultsState : public GameState
   {
   public: //  === State interface =====================

      EM_GAME_STATE(ResultsState);
      ResultsState();
      ~ResultsState();

      bool init() override;

   private:

      void ToReplay();
      void ToNext();

   };

}


#endif // RESULTSSTATE_H_


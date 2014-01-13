//===-- Solution.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/03
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Solution of a level
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SOLUTION_H_
#define SOLUTION_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies
   enum class SolutionAction {
      CLICK,
      MOVE,
      FAST_MOVE
   };


   // Solution

   class Solution
   {
   public:

      // TODO: history for undoos
      // TODO: gameplay manager stores it in editor test only
      Solution();
      shared_ptr<Solution> Dublicate();

   private:

      vector<int> numbers_solution; // IDs of pixels ordered in assembly appearance

   };

}


#endif // SOLUTION_H_
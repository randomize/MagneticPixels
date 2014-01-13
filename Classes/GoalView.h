//===-- GoalView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/03
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: This class represents Pixel Shape in UI
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELSHAPEVIEW_H_
#define PIXELSHAPEVIEW_H_

#include "EMBase.h"
#include "MPix.h"
#include "Cmd.h"
#include "View.h"

namespace MPix {

   // Forward dependencies
   class Goal;

   // GoalView

   class GoalView : public View
   {
   public:
      //static GoalView* create(shared_ptr<Goal> goal);

      GoalView();
      ~GoalView();

   public: // ===== Views interface ====================

      // Build creates contents, putting it as children of contents node...
      virtual void Build(shared_ptr<Goal> model);

      // Update method doing animations and view changes
      virtual bool Update(CmdUIUpdateGoalView::Reason reason, Coordinates pos = Coordinates::ZERO);

   private:

      unordered_map<Coordinates, Node*> tasks;
      shared_ptr<Goal> goal;

   private:

      Node* GetTaskAt(Coordinates pos);

   };

}


#endif // PIXELSHAPEVIEW_H_

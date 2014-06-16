//===-- LevelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/01/29
//  Author:      Mihailenco E. at TheEndlessCat Games, 2014
//  Description: Level button in level selector
//
//===---------------------------------------------------------===//

#pragma once
#ifndef LEVELVIEW_H_
#define LEVELVIEW_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies
   //class Level;
   class GoalView;


   // LevelView

   class LevelView : public Node
   {
   public:

      static LevelView* create(int unsigned, int label_number);

      unsigned GetLevelID() const;

      void PlayLockedAnimation();

   protected:

      bool initWithId(int unsigned, int label_number);

   private:

      // Associated level id
      unsigned level_id;

      // Presentation parts
      shared_ptr<GoalView> m_goal_view;
      Sprite* m_lock;

   };

}


#endif // LEVELVIEW_H_

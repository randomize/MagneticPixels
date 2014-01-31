//===-- LevelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/01/29
//  Author:      Mihailenco E. at Emboss Games, 2014
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

      static LevelView* create(int lvl_id, int label_number);

      bool initWithId(int lvl_id, int label_number);

      unsigned GetLevelID() const;

   protected:

   private:

      //shared_ptr<Level> level;
      shared_ptr<GoalView> gv;
      unsigned level_id;

   };

}


#endif // LEVELVIEW_H_

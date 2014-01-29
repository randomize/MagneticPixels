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


   // LevelView

   class LevelView : public Node
   {
   public:

      static LevelView* create(int lvl_id);

      bool initWithId(int lvl_id);

   protected:

   private:

   };

}


#endif // LEVELVIEW_H_
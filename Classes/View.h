//===-- View.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/11
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Views are presentations for models
//
//===---------------------------------------------------------===//

#pragma once
#ifndef VIEW_H_
#define VIEW_H_

#include "MPix.h"

namespace MPix {

   // Views try to behave much like Node, but more high level
   class View
   {
   public:

      View();
      virtual ~View();

      // Makes view content visible/invisible
      virtual void setVisible(bool visibility);

      // Change position of view contents
      virtual void setPosition(Point pos);

      // This method called most likely by target itself makes this view contents displayed on target
      // this can be done direct by childing, or through a BatchNode medium
      virtual void BindContents( Node* target, int recommendedOrder );
      virtual void BindContents( Node* target );

      // Contents may be acquired with this method
      virtual Node* GetContents();

   protected:

      // Parent 
      Node* target;

      // Contents
      Node* contents;

      // Zet oder
      int z_order;

   };

}


#endif // VIEW_H_
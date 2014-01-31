//===-- ViewsConstants.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/18
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: All constants like tags, zorders etc are here
//
//===---------------------------------------------------------===//

#pragma once
#ifndef VIEWSCONSTANTS_H_
#define VIEWSCONSTANTS_H_

#include "EMBase.h"

namespace MPix {

   namespace ViewsConstants
   {
      namespace ActionTags {

         const int GOAL_HIGHLIGHT_PULSE = 777;
         const int PIXEL_CANFALL_PULSE  = 778;

      }

      namespace Tags {

         const float p = 4.7f;

      }

      namespace Zorders {

      // UI Elements Z order constants
      static int const WALLS = 10;
      static int const PIXEL_BG = 4;
      static int const PIXEL_MIMICS = 5;

      }




   };

}


#endif // VIEWSCONSTANTS_H_
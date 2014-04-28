//===-- ImageView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/19
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Image views used ti display icons in UI
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IMAGEVIEW_H_
#define IMAGEVIEW_H_

#include "MPix.h"
#include "View.h"

namespace MPix {

   // Forward dependencies


   // ImageView

   class ImageView : public View
   {
   public:

      ImageView ();

      void setVisible( bool visibility ) override;
      void setPosition( Vector2 pos ) override;

   private:

   };

}


#endif // IMAGEVIEW_H_
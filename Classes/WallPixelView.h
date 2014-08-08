//===-- WallPixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/06
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Wall pixel view
//
//===---------------------------------------------------------===//

#pragma once
#ifndef WALLVIEW_H_
#define WALLVIEW_H_

#include "ECBase.h"
#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class WallPixel;


   // WallPixelView

   class WallPixelView : public PixelView
   {

   public:

      ECNODE_CHILD(WallPixelView);

      void Build( shared_ptr<Pixel> model ) override;

      bool Update( CmdUIUpdatePixelView::Reason reason ) override;

   private:

      shared_ptr<WallPixel> pixel;
      //vector<Node*>  blocks_urdl;

   };

}


#endif // WALLVIEW_H_
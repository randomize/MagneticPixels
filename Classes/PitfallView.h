//===-- PitfallView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: View for pitfall pixel
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PITFALLVIEW_H_
#define PITFALLVIEW_H_

#include "EMBase.h"
#include "PixelView.h"

namespace MPix {

   // Forward dependencies
   class Pitfall;


   // PitfallView

   class PitfallView : public PixelView
   {
   public:

      EM_NODE_CHILD(PitfallView);

      PitfallView();
      ~PitfallView();

      void Build( shared_ptr<Pixel> model ) override;

   protected:

   private:

      shared_ptr<Pitfall> pixel;

   };

}


#endif // PITFALLVIEW_H_
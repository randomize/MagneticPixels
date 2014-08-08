//===-- PixelView.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/03
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Pixel view represents a pixel on screen
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELVIEW_H_
#define PIXELVIEW_H_

#include "ECBase.h"
#include "ECNode.h"
#include "Cmd.h"
#include "View.h"

namespace MPix {

   // Forward dependencies
   class Pixel;


   // PixelView
   class PixelView : public ECNode, public View
   {
   public:

      PixelView();
      virtual ~PixelView();
      ECNODE_BASE(PixelView);
      static shared_ptr<PixelView> create(shared_ptr<Pixel> px);

   public:

      // Build creates contents, putting it as children of contents node...
      virtual void Build(shared_ptr<Pixel> model);

      // Update method doing animations and view changes
      virtual bool Update(CmdUIUpdatePixelView::Reason reason);

   protected:

      // Helper method allowing to lock UI take action and unlock back
      void RunLockingAction( Node* model, FiniteTimeAction* action );

      // Standard pixel behavior
      virtual void PixelCreated();
      virtual void PixelMoved();
      virtual void PixelReset();
      virtual void PixelAccepted();
      virtual void PixelDied();
      virtual void PixelResurrect();
      virtual void PixelChanged();
      virtual void PixelKilledSomeone();
      virtual void PixelIdleTrick();
      virtual void PixelTapped();

      shared_ptr<Pixel> model;

   };

}


#endif // PIXELVIEW_H_
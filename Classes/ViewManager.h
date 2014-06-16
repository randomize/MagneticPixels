//===-- ViewManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/09
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: View manager stores all views in scene
//
//===---------------------------------------------------------===//

#pragma once
#ifndef VIEWMANAGER_H_
#define VIEWMANAGER_H_

#include "EMBase.h"
#include "Cmd.h"

namespace MPix {

   // Forward dependencies
   class PixelView;
   class GoalView;

   // ViewManager

   class ViewManager
   {
   public:

      ~ViewManager();

      // Clear all views
      void Clear();

      // Add Pixel view
      void AddPixelView(int id, shared_ptr<PixelView> pview);

      // Add Goal view
      void AddGoalView(int id, shared_ptr<GoalView> g);

      // Get Goal view
      shared_ptr<GoalView> GetGoalViewByID(int id);

      // Get Pixel view
      shared_ptr<PixelView> GetPixelViewByID(int id);

      // Performing idle actions
      void RunIdleUpdateOnRandomPixel();

   private:

      // Update that view
      ErrorCode UpdatePixelView(int id, CmdUIUpdatePixelView::Reason reason);
      ErrorCode UpdateGoalView(int id, Coordinates task, CmdUIUpdateGoalView::Reason reason);

      // Store views
      unordered_map<int,shared_ptr<PixelView>> pixels;
      unordered_map<int,shared_ptr<GoalView>>  goals;

      ////// Singleton ///////////////////////////////////////////////////////////////////

   public: 

      static ViewManager& getInstance() {
         static ViewManager theSingleInstance;
         return theSingleInstance;
      }

   private:

      ViewManager();
      ViewManager(ViewManager& root){}
      ViewManager& operator=(ViewManager&){return *this;}

   };

}


#endif // VIEWMANAGER_H_

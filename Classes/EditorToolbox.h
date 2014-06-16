//===-- EditorToolbox.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/18
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Toolbox holds tools
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EDITORTOOLBOX_H_
#define EDITORTOOLBOX_H_

#include "ECBase.h"
#include "MPix.h"

namespace MPix {

   // Forward dependencies
   class EditorTool;
   class EditorLayer;


   // EditorToolbox

   class EditorToolbox : public Layer
   {
   public:

      bool init() override;

      CREATE_FUNC(EditorToolbox);

      void PrepareTools();
      void ReuseLastTool();

      EditorLayer* p_editor_layer;

      // Touch control for toolbox
      void TouchEnable();
      void TouchDisable();

   private:

      void CloseToolbox();
      void GotClick(Coordinates p);
      void LoadTool(shared_ptr<EditorTool> tool, int id);

      vector<shared_ptr<EditorTool>> tools;

      shared_ptr<EditorTool> current_tool;
      shared_ptr<EditorTool> root_tool;
      int root_tool_size;
      int last_used;

      Label* tool_name;

      // Touch system
      bool onTouchBegan( Touch *touch, Event *event ) override;
      EventListener* touch_events;

   };

}


#endif // EDITORTOOLBOX_H_

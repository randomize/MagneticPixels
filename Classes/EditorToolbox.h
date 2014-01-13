//===-- EditorToolbox.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/18
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Toolbox holds tools
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EDITORTOOLBOX_H_
#define EDITORTOOLBOX_H_

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // Forward dependencies
   class EditorTool;
   class EditorLayer;


   // EditorToolbox

   class EditorToolbox : public Layer
   {
   public:

      EditorToolbox();
      ~EditorToolbox();

      CREATE_FUNC(EditorToolbox);

      bool init() override;

      void onShow();

      void BuildAllTools();

      EditorLayer* Editor;

      bool onTouchBegan( Touch *touch, Event *event) override;

   private:

      EditorTool* current_tool;
      EditorTool* root_tool;
      int root_tool_size;
      int last_used;
      void GotClick(Coordinates p);
      void LoadTool(EditorTool* too, int id);
      void CloseToolbox();
      vector<EditorTool*> tools;
      LabelTTF* tool_name;

   };

}


#endif // EDITORTOOLBOX_H_
//===-- EditorToolGenerator.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/18
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Generates default map of tools
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EDITORTOOLGENERATOR_H_
#define EDITORTOOLGENERATOR_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies
   class EditorTool;

   // EditorToolGenerator

   class EditorToolGenerator
   {
   public:

   // Generates all default tools
   static vector<shared_ptr<EditorTool>> GenerateDefaultSet();

   };

}


#endif // EDITORTOOLGENERATOR_H_
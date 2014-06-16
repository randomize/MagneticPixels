
//===-- Scripts.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/02/01
//  Author:      Mihailenco E. at TheEndlessCat Games, 2014
//  Description: Script fabrics
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SCRIPTS_H_
#define SCRIPTS_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies
   class Script;

   // Hardcoded C++ Scripts version
   class CppScripts
   {
   public:

      static unordered_map<unsigned, shared_ptr<Script>> GetScripts();

   protected:

   private:

   };

   // Lua scripts
   // TODO: implement
   class LuaScripts
   {

      static unordered_map<unsigned, shared_ptr<Script>> GetScripts();

   protected:

   private:

   };

}


#endif // SCRIPTS_H_

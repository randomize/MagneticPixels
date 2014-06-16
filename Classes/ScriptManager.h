
//===-- ScriptManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/02/01
//  Author:      Mihailenco E. at TheEndlessCat Games, 2014
//  Description: Manages scripts
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SCRIPTMANAGER_H_
#define SCRIPTMANAGER_H_

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // Forward dependencies
   class Script;
   class Context;

   // ScriptManager

   class ScriptManager
   {
   public:


      // Searches for script for given level id, sets sefault if not found
      void LoadScript(unsigned id);

      // Callbacks

      // Called when level first loaded
      void OnFirstMove( const Context& context );

      // Called when player clicked
      bool OnPlayerClicked( const Context& context, Coordinates where );

      // Called when player tries to move assembly
      bool OnPlayerMove( const Context& context, Direction where );

      // Called after successfully last grow
      void OnLastGrow(  const Context& context );

   private:

      shared_ptr<Script> m_current;
      unordered_map<unsigned, shared_ptr<Script>> m_scripts;

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 
   
      static ScriptManager& getInstance() {
         static ScriptManager theSingleInstance;
         return theSingleInstance;
      }
   
   private:
   
      ScriptManager();
      ~ScriptManager();
      ScriptManager(ScriptManager& root){}
      ScriptManager& operator=(ScriptManager&){return *this;}
   

   };

}


#endif // SCRIPTMANAGER_H_

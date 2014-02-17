
//===-- ScriptManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/02/01
//  Author:      Mihailenco E. at Emboss Games, 2014
//  Description: Manages scripts
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SCRIPTMANAGER_H_
#define SCRIPTMANAGER_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies
   class Script;

   // ScriptManager

   class ScriptManager
   {
   public:

      // Gameplay manager calls it on level start
      void LoadScript(unsigned id);

      // Callbacks

      bool OnLoad();
      bool OnUnload();
      bool OnRestart();


   private:

      shared_ptr<Script> m_current;
      unordered_map<unsigned, shared_ptr<Script>> m_scripts;

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 
   
      static ScriptManager* getInstance() {
         static ScriptManager theSingleInstance;
         return &theSingleInstance;
      }
   
   private:
   
      ScriptManager();
      ScriptManager(ScriptManager& root){}
      ScriptManager& operator=(ScriptManager&){return *this;}
   

   };

}


#endif // SCRIPTMANAGER_H_

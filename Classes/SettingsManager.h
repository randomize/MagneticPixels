//===-- SettingsManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Store user profile, achievements and other settings
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies


   // SettingsManager

   class SettingsManager
   {
   public:
      // TODO: implement
      // Get/Set levels progress
      // Get/Set music on/off from settings state
      // Get/Set ads on/off from iOS in APP 
      // Get/Set language from settings state
      // Get/Set if was shown promotion and when it was shown

   protected:

   private:
      // TODO: Some kind of DB, maybe cocos, maybe xml, maybe platform dependent
      //CCUserDefault::

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 

      static SettingsManager& getInstance() {
         static SettingsManager theSingleInstance;
         return theSingleInstance;
      }

   private:

      SettingsManager();
      SettingsManager(SettingsManager& root){}
      SettingsManager& operator=(SettingsManager&){return *this;}

   };

}


#endif // SETTINGSMANAGER_H_

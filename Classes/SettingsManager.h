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

      // Tries to read profile, or generates default one
      void LoadSettings();

      // Flushes data to user directory if any update
      void SaveSettings();

      int GetLastWorldIndex();

   protected:

   private:

      // Fortunately cocos provides this :)
      UserDefault* db;

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

//===-- SettingsManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Store user profile, achievements and other settings
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies


   // SettingsManager

   class SettingsManager
   {
   public:

      enum class Key {
         LAST_WORLD,
         SOUND_ON,
         MUSIC_ON,
         CURRENT_LANG,
         N_OF_RUNS
      };

      // Tries to read profile, or generates default one
      void LoadSettings();

      // Flushes data to user directory if any update
      void SaveSettings();

      // General values - integers only
      int GetKey(Key key);
      void SetKey(Key key, int value);

      // Level specific -1 = locked, 0 open, n - solved with n
      void SetLevelStatus(unsigned int id, int state);
      int GetLevelStatus(unsigned int id);

   private:

      // Fortunately cocos provides this :)
      // Unfortunately it is stupid enough not to use caching :(
      UserDefault* db;

   ////// Internal implementation ///////////////////////////////////////////////////////////////////

      struct KeyHash
      {
         std::size_t operator()(const Key &p) const
         {
            return static_cast<std::size_t>(p);
         }
      };

      struct SettingValue {
         SettingValue(bool is_dirty, int value, const string& db_key) :
            is_dirty(is_dirty),
            value(value),
            db_key(db_key)
         {}
         bool is_dirty;    // Cached value needs to be saved
         int value;        // Current value
         string db_key;    // String key for cocos db
      };

      unordered_map<Key, SettingValue, KeyHash> m_key_vals;

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

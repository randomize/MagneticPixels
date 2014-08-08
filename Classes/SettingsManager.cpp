#include "SettingsManager.h"


using namespace MPix;

MPix::SettingsManager::SettingsManager() :
m_key_vals({
   { Key::CURRENT_LANG, { false, 0, "lang" } },
   { Key::LAST_WORLD, { false, 0, "lwi" } },
   { Key::MUSIC_ON, { false, 1, "mus" } },
   { Key::N_OF_RUNS, { false, 0, "nrun" } },
   { Key::SOUND_ON, { false, 0, "snd" } }
})
{
   ECLOG_INFO("[ SettingsManager initialized ]");
}

void MPix::SettingsManager::LoadSettings()
{
   ECLOG_INFO("Loading data...");

   // Init db cocosy
   db = UserDefault::getInstance();
   assert(db);

   ECLOG_INFO("XML File: " + db->getXMLFilePath());

   // Now load from cocos db existing values
   // inexistent will have default value
   for (auto& a : m_key_vals) {
      a.second.value = db->getIntegerForKey(a.second.db_key.c_str(), a.second.value);
      ECLOG_DEBUG("Got : " + a.second.db_key + "=" + a.second.value);
   }

}

void MPix::SettingsManager::SaveSettings()
{
   ECLOG_INFO("Saving data...");
   for (auto& v : m_key_vals) {
      if (v.second.is_dirty) {
         db->setIntegerForKey(v.second.db_key.c_str(), v.second.value);
      }
   }
}

int MPix::SettingsManager::GetKey(Key key)
{
   // Search cache
   auto it = m_key_vals.find(key);
   assert(it != m_key_vals.end());

   // Return
   return  it->second.value;
}

void MPix::SettingsManager::SetKey(Key key, int value)
{
   auto it = m_key_vals.find(key);
   assert(it != m_key_vals.end());
   ECLOG_DEBUG("Setting " + value + " => " + it->second.db_key);
   it->second.value = value;
   it->second.is_dirty = true;
}

void MPix::SettingsManager::SetLevelStatus(unsigned int id, int state)
{
   // TODO:
}

int MPix::SettingsManager::GetLevelStatus(unsigned int id)
{
   // TODO:
   return 0;
}

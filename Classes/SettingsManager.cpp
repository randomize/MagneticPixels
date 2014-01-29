#include "SettingsManager.h"

using namespace MPix;


MPix::SettingsManager::SettingsManager()
{
   EM_LOG_INFO("[ SettingsManager initialized ]");
}

void MPix::SettingsManager::LoadSettings()
{
   EM_LOG_INFO("Loading data...");
   db = UserDefault::getInstance();
   assert(db);
}

void MPix::SettingsManager::SaveSettings()
{
   EM_LOG_INFO("Saving data...");
}

int MPix::SettingsManager::GetLastWorldIndex()
{
   return db->getIntegerForKey("last_world_index", 0);
}

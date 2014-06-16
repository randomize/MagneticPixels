#include "LevelManager.h"

#include "Level.h"
#include "World.h"
#include "LevelGenerator.h"

#include "LevelStorage.h"
#include "SettingsManager.h"

using namespace MPix;

LevelManager::LevelManager()
{
   state = State::EMPTY;
   ECLOG_INFO("[ LevelManager initialized ]");
}

EndlessCatLib::ErrorCode MPix::LevelManager::ResetData()
{
   ECLOG_INFO("LevelManager cleaning... ");
   levels.clear();
   worlds.clear();
   worlds_map.clear();
   last_lvl = nullptr;
   ed_lvl = GetEmpty();
   state = State::EMPTY;
   return ErrorCode::RET_OK;
}

ErrorCode LevelManager::LoadData()
{

   if (state == State::LOADED)
   {
      ResetData();
   }

   ECLOG_INFO("LevelManager loading ... ");

   auto ret = LevelStorage::getInstance()->GetLevels(worlds, levels); 

   auto ws = worlds.size();
   auto ls = levels.size();

   if (ret != ErrorCode::RET_OK || ws == 0 || ls == 0 ) {
      ECLOG_ERROR("LevelManager failed levels load, resetting");
      ResetData();
      return ErrorCode::RET_FAIL;
   }

   ECLOG_INFO("LevelManager loaded " + worlds.size() + " worlds with " + levels.size() + " levels");

   // Setting up searchmap
   worlds_map.reserve(worlds.size());
   worlds_ids.reserve(worlds.size());
   for (auto w : worlds) {

      int id = w->GetID();

#ifndef MPIX_DEVELOPERS_BUILD
      if (id == 0) 
         continue; // Skip editors world in release
#endif
      worlds_map.emplace(id, w);
      worlds_ids.push_back(id); 
   }

   // Filling in data about levels using settings

   // Set defaults
   for (auto & l : levels) {
      l.second->SetState(Level::State::IS_LOCKED);
   }

   // Open first level of first world
   auto first_lvl_id = GetFirstLevelByWorldID(worlds_ids.front());
   auto it = levels.find(first_lvl_id);
   assert(it != levels.end());
   auto first_lvl = it->second;
   assert(first_lvl);
   first_lvl->SetState(Level::State::IS_OPEN);
   
   // TODO: Load user data of solved levels atars etc from st;
   //auto & st = SettingsManager::getInstance();

   return ErrorCode::RET_OK;
}

//////////// LEVELS MANAGEMENT ////////////////////////////////////////////////////////////////////

shared_ptr<Level> MPix::LevelManager::GetEmpty()
{
   return LevelGenerator::getInstance()->CreateEmpty();
}

shared_ptr<Level> LevelManager::GetPlayableLevelByID(unsigned  int levelID )
{
   auto lvl = GetLevelByID(levelID);

   if (lvl == nullptr) { 
      ECLOG_ERROR("> Not found Level " + levelID);
      return nullptr;
   }

   ECLOG_DEBUG("> Requested level " + levelID);
   last_lvl = lvl;
   return DupeLastAndMakePlayable();
}

shared_ptr<Level> LevelManager::GetPlayableTestLevel()
{
   last_lvl = LevelGenerator::getInstance()->CreateTestLevel();
   return DupeLastAndMakePlayable();
}

shared_ptr<Level> MPix::LevelManager::GetPlayableEditorsLevel()
{
   if (ed_lvl == nullptr) {
      return nullptr;
   }

   last_lvl = ed_lvl;
   return DupeLastAndMakePlayable();
}

shared_ptr<Level> MPix::LevelManager::GetPlayableLastLevel()
{
   if (last_lvl == nullptr) {
      return nullptr;
   }

   return DupeLastAndMakePlayable();
}

shared_ptr<Level> MPix::LevelManager::GetPlayableNextLevel()
{
   assert(worlds_ids.empty() == false);

   if (last_lvl == nullptr) // Return first (fallback) if no recent level
   {
      auto lvl = GetLevelByID(GetFirstLevelByWorldID(worlds_ids.front()));
      assert(lvl);
      last_lvl = lvl;
      return DupeLastAndMakePlayable();
   }

   auto last_id = last_lvl->GetID();
   auto w_id = last_lvl->GetWorld();
   unsigned next_id;

   if (last_id == GetLastLevelByWorldID(w_id)) // last level was the last one in its world
   {
      // Find next world if exists
      auto it = find(worlds_ids.begin(), worlds_ids.end(), w_id);
      assert(it != worlds_ids.end());
      ++it;
      if (it == worlds_ids.end()) // No next
      {
         return nullptr;
      }
      next_id = GetFirstLevelByWorldID(*it);
   }
   else
   {
      next_id = GetNextLevelByWorldID(w_id, last_id);
   }

   auto lvl = GetLevelByID(next_id);
   assert(lvl);
   last_lvl = lvl;
   return DupeLastAndMakePlayable();
}

/////////////////////////////////////////////////////////////////////////////////


void MPix::LevelManager::SetEditorsLevel( shared_ptr<Level> l )
{
   assert(l);
   ed_lvl = l;
}

shared_ptr<Level> MPix::LevelManager::EditorGetCurrentLevel()
{
   return ed_lvl;
}

int MPix::LevelManager::GetStarsByLevelID( unsigned int levelID )
{
   return GetLevelByID(levelID)->GetStars();
}

const string& MPix::LevelManager::GetNameByLevelID( unsigned int levelID )
{
   return GetLevelByID(levelID)->GetName();
}

bool MPix::LevelManager::GetLockedByLevelID(unsigned int levelID)
{
   return GetLevelByID(levelID)->GetState() == Level::State::IS_LOCKED ;
}

//////////// WORLDS MANAGEMENT ////////////////////////////////////////////////////////////////////

const vector<int>& MPix::LevelManager::GetWorldIDs( void )
{
   return worlds_ids;
}

shared_ptr<World> MPix::LevelManager::GetWorldByID( int wID )
{
   auto p = worlds_map.find(wID);
   if (p == end(worlds_map)) {
      ECLOG_ERROR("> Requested world " + wID + " not exists");
      return nullptr;
   }
   ECLOG_DEBUG("> Requested world " + wID);
   return p->second;
}

vector<unsigned int> MPix::LevelManager::GetLevelsByWorldID( int wID )
{
   auto w = GetWorldByID(wID);
   assert(w);
   vector<unsigned int> fab;
   fab.reserve(w->GetLevelCount());
   for (auto l : w->GetLevels() ) {
      fab.push_back(l);
   }
   return fab;
}

const string& MPix::LevelManager::GetNameByWorldID( int wID )
{
   auto w = GetWorldByID(wID);
   assert(w);
   return w->GetName();
}

int MPix::LevelManager::GetLevelCountByWorldID( int wID )
{
   auto w = GetWorldByID(wID);
   assert(w);
   return w->GetLevelCount();
}

unsigned MPix::LevelManager::GetFirstLevelByWorldID(int wID)
{
   auto w = GetWorldByID(wID);
   assert(w && w->GetLevels().empty() == false);
   return w->GetLevels().front();
}

unsigned MPix::LevelManager::GetLastLevelByWorldID(int wID)
{
   auto w = GetWorldByID(wID);
   assert(w && w->GetLevels().empty() == false);
   return w->GetLevels().back();
}

unsigned MPix::LevelManager::GetNextLevelByWorldID(int wID, unsigned levelID)
{
   auto w = GetWorldByID(wID);
   assert(w);

   auto & levels = w->GetLevels();
   assert(levels.empty() == false);
   
   auto it = find(levels.begin(), levels.end(), levelID);
   assert(it != levels.end());
   ++it;
   assert(it != levels.end());
   return *it;
}

//////////////////////// INTERNAL //////////////////////////////////////////////

shared_ptr<Level> MPix::LevelManager::GetLevelByID( unsigned int levelID )
{
   auto p = levels.find(levelID);
   if( p != end(levels) ) {
      return p->second;
   }
   return nullptr;
}

shared_ptr<Level> MPix::LevelManager::DupeLastAndMakePlayable()
{
   auto fab = last_lvl->Dublicate();
   fab->SetState(Level::State::IS_PLAYABLE);
   return fab;
}

/////////////////////////// EDITOR FUNCTIONS ////////////////////////////////////

void MPix::LevelManager::StoreLevel( shared_ptr<Level> level )
{
   auto levelID = level->GetID(); // New or existing
   auto worldID = level->GetWorld();

   auto it = worlds_map.find(worldID); // Check worlds
   if (it == worlds_map.end()) { // Bad
      ECLOG_ERROR("World ID="+worldID+" not found, level not stored");
      return;
   }

   ECLOG_DEBUG("Saving level id=" + levelID);

   // Delete if exists
   DeleteLevelByID(levelID);

   // Inject
   levels.emplace(levelID, level);
   it->second->AddLevel(levelID);

   // Save to XML
   LevelStorage::getInstance()->SaveLevel(level);
}

void MPix::LevelManager::DeleteLevelByID( unsigned int levelID )
{
   auto p = levels.find(levelID);
   if( p == end(levels) ) { // Nothing to delete
      return;
   }

   auto it = worlds_map.find(p->second->GetWorld()); // Check worlds
   if (it != worlds_map.end()) { 
      it->second->EraseLevel(levelID);
   }

   ECLOG_DEBUG("Erasing level id=" + levelID);
   levels.erase(p);

   LevelStorage::getInstance()->DeleteLevel(levelID);
}

void MPix::LevelManager::SaveEditorsLevel()
{
   assert(ed_lvl);
   StoreLevel(ed_lvl);
}








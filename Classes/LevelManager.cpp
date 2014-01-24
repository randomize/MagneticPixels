#include "LevelManager.h"

#include "Level.h"
#include "World.h"
#include "LevelGenerator.h"

#include "LevelStorage.h"

using namespace MPix;

LevelManager::LevelManager()
{
   st = State::EMPTY;
   EM_LOG_INFO("[ LevelManager initialized ]");
}

EmbossLib::ErrorCode MPix::LevelManager::ResetData()
{
   EM_LOG_INFO("LevelManager cleaning... ");
   levels.clear();
   worlds.clear();
   worlds_map.clear();
   last_lvl = nullptr;
   ed_lvl = GetEmpty();
   st = State::EMPTY;
   return ErrorCode::RET_OK;
}

ErrorCode LevelManager::LoadData()
{

   if (st == State::LOADED)
   {
      ResetData();
   }

   EM_LOG_INFO("LevelManager loading ... ");

   auto ret = LevelStorage::getInstance()->GetLevels(worlds, levels); 

   if (ret == ErrorCode::RET_OK ) {
      worlds_map.reserve(worlds.size());
      for (auto w : worlds) {
         worlds_map.emplace(w->GetID(), w);
      }
      EM_LOG_INFO("LevelManager loaded " + worlds.size() + " worlds with " + levels.size() + " levels");
   } else {
      EM_LOG_ERROR("LevelManager failed levels load, resetting");
      ResetData();
   }

   return ret;
}

shared_ptr<Level> MPix::LevelManager::GetEmpty()
{
   return LevelGenerator::getInstance()->CreateEmpty();
}

shared_ptr<Level> LevelManager::GetLevelByID(unsigned  int levelID )
{
   auto l = FindLevel(levelID);

   if (l == nullptr) { 
      EM_LOG_ERROR("> Not found Level " + levelID);
      return nullptr;
   }

   EM_LOG_DEBUG("> Requested level " + levelID);
   last_lvl = l;
   return last_lvl->Dublicate();
}

shared_ptr<Level> LevelManager::GetTestLevel()
{
   last_lvl = LevelGenerator::getInstance()->CreateTestLevel();
   return last_lvl->Dublicate();
}

shared_ptr<Level> MPix::LevelManager::GetEditorsLevel()
{
   if (ed_lvl) {
      last_lvl = ed_lvl;
      return last_lvl->Dublicate();
   }
   else
      return nullptr;
}

void MPix::LevelManager::SetEditorsLevel( shared_ptr<Level> l )
{
   assert(l);
   ed_lvl = l;
}

shared_ptr<Level> MPix::LevelManager::EditorGetCurrentLevel()
{
   return ed_lvl;
}

shared_ptr<Level> MPix::LevelManager::GetLastLevel()
{
   if (last_lvl)
      return last_lvl->Dublicate();
   else 
      return nullptr;
}

int MPix::LevelManager::GetLevelStarsByID( unsigned int levelID )
{
   return FindLevel(levelID)->GetStars();
}

const string& MPix::LevelManager::GetLevelNameByID( unsigned int levelID )
{
   return FindLevel(levelID)->GetName();
}

vector<int> MPix::LevelManager::GetWorldsIDs( void )
{
   vector<int> fab;
   fab.reserve(worlds_map.size());
   for (auto w : worlds_map ) {
#ifndef MPIX_DEVELOPERS_BUILD
      if (w.first == 0) 
         continue; // Skip editors world in release
#endif
      fab.push_back(w.first);
   }
   return fab;
}

shared_ptr<World> MPix::LevelManager::GetWorldByID( int wID )
{
   auto p = worlds_map.find(wID);
   if (p == end(worlds_map)) {
      EM_LOG_ERROR("> Requested world " + wID + " not exists");
      return nullptr;
   }
   EM_LOG_DEBUG("> Requested world " + wID);
   return p->second;
}

vector<unsigned int> MPix::LevelManager::GetLevelsInWorld( int wID )
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

const string& MPix::LevelManager::GetWorldNameByID( int wID )
{
   return GetWorldByID(wID)->GetName();
}

int MPix::LevelManager::GetWorldLevelCountByID( int wID )
{
   return GetWorldByID(wID)->GetLevelCount();
}

shared_ptr<Level> MPix::LevelManager::FindLevel( unsigned int levelID )
{
   auto p = levels.find(levelID);
   if( p != end(levels) ) {
      return p->second;
   }
   return nullptr;
}

void MPix::LevelManager::StoreLevel( shared_ptr<Level> level )
{
   auto levelID = level->GetID(); // New or existing
   auto worldID = level->GetWorld();

   auto it = worlds_map.find(worldID); // Check worlds
   if (it == worlds_map.end()) { // Bad
      EM_LOG_ERROR("World ID="+worldID+" not found, level not stored");
      return;
   }

   EM_LOG_DEBUG("Saving level id=" + levelID);

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

   EM_LOG_DEBUG("Erasing level id=" + levelID);
   levels.erase(p);

   LevelStorage::getInstance()->DeleteLevel(levelID);
}

void MPix::LevelManager::SaveEditorsLevel()
{
   assert(ed_lvl);
   StoreLevel(ed_lvl);
}

shared_ptr<Level> MPix::LevelManager::GetNextLevel()
{
   auto l = GetLastLevel();
   // TODO: get id and find next, or next world
   return GetLastLevel();
}



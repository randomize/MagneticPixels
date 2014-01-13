#include "HistoryManager.h"

using namespace MPix;

//====---------------------------------------------======//


void MPix::HistoryManager::Unregister( int id )
{
   ReturnID(id);
   auto p = map_main.find(id);
   assert( p != map_main.end());
   delete p->second;
   map_main.erase(p);
}

MPix::HistoryManager::HistoryManager()
{
   // reserving 100 places for data
   map_main.reserve(PLACES);
   free_ids.reserve(PLACES);
   ExpandIDs();
   EM_LOG_INFO("============= HistoryManager =============");
}

int MPix::HistoryManager::GetFreeID()
{
   if ( free_ids.empty() ) {
      ExpandIDs();
   }
   int id = *free_ids.begin();
   free_ids.erase(id);
   return id;
}

void MPix::HistoryManager::ReturnID( int id )
{
   //assert(free_ids.empty() || free_ids.find(id) == free_ids.end());
   free_ids.emplace(id);
}

void MPix::HistoryManager::ExpandIDs()
{
   // TODO: random??
   static int max = 0;
   int last = max + 100;
   EM_LOG_WARNING(" Expanding history ids :" + last );
   for (int i = max; i < last; ++i)
      free_ids.emplace(i);
   max = last;
}

MPix::HistoryManager::~HistoryManager()
{
   EM_LOG_WARNING("History closing");
}




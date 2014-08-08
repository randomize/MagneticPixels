//===-- HistoryManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/24
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: History manager is universal storage for history
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HISTORYMANAGER_H_
#define HISTORYMANAGER_H_

#include "ECBase.h"
#include "HistoryStorage.h"


namespace MPix {

   // Forward dependencies
   class BaseStorage;


   // HistoryManager is singleton, that can store any data for
   // undo/redo system

   // Main advantages and features:
   //  + Less memory fragmentation
   //  + Universal types, stores everything
   //  + Strips out duplicates
   //  + Restore last state
   //  + Restore

   class HistoryManager
   {
   public:

      // Store value of any type to stack
      template<class T> void PushSnapshotFor(int id, const T& value) {
         auto p = map_main.find(id);
         assert( p != map_main.end());
         auto hh = static_cast<HistoryStorage<T>*> (p->second);
         hh->PushValue(value);
      }

      // Pop N snapshots and set value to N-th, N starts from 1, 0 means all, value set to first
      template<class T> void PopSnapshotTo(int id, T& value, unsigned n = 1) {
         auto p = map_main.find(id);
         assert( p != map_main.end());
         auto hh = static_cast<HistoryStorage<T>*> (p->second);
         hh->PopValue(n, value);
      }

      // Remove all snapshots, no impact on value
      void ClearSnapshots(int id) {
         auto p = map_main.find(id);
         assert( p != map_main.end());
         auto hh = (p->second);
         hh->Clear();
      }

      // Register, returns id
      template<class T> int Register(const T& value) {
         //ECLOG_DEBUG(" Registered hist " + typeid(T).name());
         int id = GetFreeID();
         auto storage = new HistoryStorage<T>();
         map_main.emplace(id, storage);
         return id;
      }

      // Unregister id
      void Unregister( int id );

   private:

      unordered_map<int, BaseStorage*> map_main;

      unordered_set<int> free_ids;

      int GetFreeID();
      void ReturnID(int id);
      void ExpandIDs();

   ////// Singleton ///////////////////////////////////////////////////////////////////

   public:

      static HistoryManager& getInstance() {
         static HistoryManager theSingleInstance;
         return theSingleInstance;
      }

   private:

      HistoryManager();
      ~HistoryManager();
      HistoryManager(HistoryManager& root){}
      HistoryManager& operator=(HistoryManager&){return *this;}
      // 100 places for object id's
      static const int PLACES = 100;

      //

   };

}


#endif // HISTORYMANAGER_H_

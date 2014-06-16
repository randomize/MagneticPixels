//===-- Historical.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/24
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Historical class is wrapper for any type needs to be historized
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HISTORICAL_H_
#define HISTORICAL_H_

#include "ECBase.h"
#include "HistoryManager.h"

namespace MPix {

   // Forward dependencies


   // Historical

   template<typename T> class Historical
   {
   public:

      Historical() {
         id = -1;
      }

      Historical(const Historical& rhs) {
         id = -1;
         value = rhs.value;
      }

      Historical(const T& rhs) {
         id = -1;
         value = rhs;
      }

      ~Historical() {
         if ( id != -1 )
            HistoryManager::getInstance().Unregister(id);
      }

      void InitSnapshots() {
         assert( id = -1);
         id = HistoryManager::getInstance().Register(value);
      }

      void PushSnapshot() {
         HistoryManager::getInstance().PushSnapshotFor(id, value);
      }

      void PopSnapshots(unsigned n = 1) {
         HistoryManager::getInstance().PopSnapshotTo(id, value, n);
      }

      void ClearSnapshots() {
         HistoryManager::getInstance().ClearSnapshots(id);
      }

      /*operator T() {
         return value;
      }

      operator T() const {
         return value;
      }*/

      T& GetValue() { return value; }
      const T& GetValue() const { return value; }

      operator T&() {
         return value;
      }

      operator const T&() const {
         return value;
      }

      bool operator==(const T& rhs) const {
         return value == rhs;
      }

      bool operator!=(const T& rhs) const {
         return value != rhs;
      }

      const T& operator=(const T& v) {
         value = v;
         return v;
      }

   private:

      T value;
      int id;

   };

}

#endif // HISTORICAL_H_

//===-- HistoryStorage.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/24
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: 
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HISTORYSTORAGE_H_
#define HISTORYSTORAGE_H_

#include "EMBase.h"

namespace MPix {


   // Base
   class BaseStorage {
   public:
      virtual void Clear() {}
   };

   // Each storage for one type
   template<class T> class HistoryStorage : public BaseStorage {
   public:

      void PopValue(unsigned n, T& v) { 
         if (n == 1) {
            // Pop one
            v = values.back();
            values.pop_back();
         } else if (n > 1) {
            // 2 3 or more
            assert(n <= values.size());
            values.resize(values.size() - (n-1));
            v = values.back();
            values.pop_back();
         } else { // n==0 
            // Pop all
            v = values.front();
            values.clear();
         } 
      }

      void PushValue(const T& v) {
         values.push_back(v);
      }

      void Clear() override {
         values.clear();
      }

   private:

      // map of data
      vector<T> values; // TODO : replace with distributed compact diffs
   
   };

}


#endif // HISTORYSTORAGE_H_
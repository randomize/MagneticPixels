//===-- StatisticsManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/07
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Universal statistics in games
//
//===---------------------------------------------------------===//

#pragma once
#ifndef STATISTICSMANAGER_H_
#define STATISTICSMANAGER_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies


   // StatisticsManager

   class StatisticsManager
   {
   public:


      // Game play stats
      void PostGameResult();

      // Developers info
      void RaportBug(const string& message);

   protected:

   private:

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 
   
      static StatisticsManager& getInstance() {
         static StatisticsManager theSingleInstance;
         return theSingleInstance;
      }
   
   private:
   
      StatisticsManager();
      ~StatisticsManager();
      StatisticsManager(StatisticsManager& root){}
      StatisticsManager& operator=(StatisticsManager&){return *this;}

   };

}


#endif // STATISTICSMANAGER_H_

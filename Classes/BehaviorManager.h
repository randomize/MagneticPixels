//===-- BehaviorManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/24
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Pixel strategies realization
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELBEHAVIOR_H_
#define PIXELBEHAVIOR_H_

#include "EMBase.h"


namespace MPix {

   // Forward dependencies
   class AnyBehavior;


   // BehaviorManager

   class BehaviorManager
   {
   public:

      shared_ptr<AnyBehavior> GetPixelBehavior(const string& name);
      shared_ptr<AnyBehavior> GetSharedBehaviour(const string& name);

   private:

      unordered_map<string,shared_ptr<AnyBehavior>> shared;

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 
   
      static BehaviorManager& getInstance() {
         static BehaviorManager theSingleInstance;
         return theSingleInstance;
      }
   
   private:
   
      BehaviorManager();
      BehaviorManager(BehaviorManager& root){}
      BehaviorManager& operator=(BehaviorManager&){return *this;}
   

   };

}


#endif // PIXELBEHAVIOR_H_

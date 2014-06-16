//===-- GoalXML.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/03
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Sores/Loads Goal to/from XML
//
//===---------------------------------------------------------===//

#pragma once
#ifndef GOALXML_H_
#define GOALXML_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies
   class Goal;
   class Goals;

   // GoalXML
   class CC_DLL GoalXML
   {
   public:

      // Goal
      static shared_ptr<Goal> GenerateGoal( tinyxml2::XMLElement* src);
      static bool Store(shared_ptr<Goal> src, tinyxml2::XMLPrinter* dst);

      // Goals
      static shared_ptr<Goals> GenerateGoals( tinyxml2::XMLElement* src);
      static bool Store(shared_ptr<Goals> src, tinyxml2::XMLPrinter* dst);

   };

}


#endif // GOALXML_H_
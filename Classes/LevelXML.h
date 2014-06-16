//===-- LevelXML.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/17
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: This static class holds methods for saving/loading XML
//
//===---------------------------------------------------------===//

#pragma once
#ifndef LEVELXML_H_
#define LEVELXML_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies
   class Level;

   // LevelXML

   class CC_DLL LevelXML
   {
   public:

      //Level
      static shared_ptr<Level> Generate(tinyxml2::XMLElement* src);
      static bool Store(shared_ptr<Level> src, tinyxml2::XMLPrinter* dst);

   };

}


#endif // LEVELXML_H_
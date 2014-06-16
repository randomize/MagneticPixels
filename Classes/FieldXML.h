//===-- FieldXML.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/03
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Loads/Stores field from/to XML
//
//===---------------------------------------------------------===//

#pragma once
#ifndef FIELDXML_H_
#define FIELDXML_H_

#include "ECBase.h"

namespace MPix {

   // Forward dependencies
   class Field;

   // FieldXML
   class CC_DLL FieldXML 
   {
   public:

      // Field 
      static shared_ptr<Field> Generate( tinyxml2::XMLElement* src );
      static bool Store(shared_ptr<Field> src, tinyxml2::XMLPrinter * dst);

   };

}


#endif // FIELDXML_H_
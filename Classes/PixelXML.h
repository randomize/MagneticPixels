//===-- PixelXML.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/17
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Saves and loads XML data from/to pixels
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELXML_H_
#define PIXELXML_H_

#include "EMBase.h"

namespace MPix {

   // Forward dependencies
   class Pixel;


   // PixelXML
   class CC_DLL PixelXML
   {
   public:

      static shared_ptr<Pixel> Generate(tinyxml2::XMLElement* src);
      static bool Store( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst );

   private:
      static bool StoreMagneticPixelToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst);
      static bool StoreWallPixelToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst);
      static bool StoreDynamicCactusToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst);
      static bool StorePitfallToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst);
      static bool StoreSokobanPixelToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst);
      static bool StoreStonePixelToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst);
      static bool StoreStaticCactusToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst);
      static bool StoreBomberToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst);
      static bool StoreMutantToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst);

      static shared_ptr<Pixel> GenMagneticPixelFromXML(tinyxml2::XMLElement* src);
      static shared_ptr<Pixel> GenWallPixelFromXML(tinyxml2::XMLElement* src);
      static shared_ptr<Pixel> GenDynamicCactusFromXML(tinyxml2::XMLElement* src);
      static shared_ptr<Pixel> GenPitfallFromXML(tinyxml2::XMLElement* src);
      static shared_ptr<Pixel> GenSokobanPixelFromXML(tinyxml2::XMLElement* src);
      static shared_ptr<Pixel> GenStonePixelFromXML(tinyxml2::XMLElement* src);
      static shared_ptr<Pixel> GenStaticCactusFromXML(tinyxml2::XMLElement* src);
      static shared_ptr<Pixel> GenBomberFromXML(tinyxml2::XMLElement* src);
      static shared_ptr<Pixel> GenMutantFromXML(tinyxml2::XMLElement* src);

   };

}


#endif // PIXELXML_H_
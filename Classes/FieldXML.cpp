#include "FieldXML.h"
#include "PixelXML.h"
#include "Field.h"

using namespace MPix;
using namespace tinyxml2;

//====---------------------------------------------======//


bool MPix::FieldXML::Store( shared_ptr<Field> src, tinyxml2::XMLPrinter * dst )
{
   dst->OpenElement("fld");

   for (auto px : src->GetPixelData()) {

      if (PixelXML::Store( px, dst ) == false) {
         ECLOG_WARNING("Field couldn't save pixel id=" + px->GetID() );
      }

   }

   dst->CloseElement(); // fld

   return true;
}

shared_ptr<Field> MPix::FieldXML::Generate( tinyxml2::XMLElement* el )
{
   auto f = make_shared<Field>();
   int pc = 0; // pix count
   int e_pc = 0; // error pix count
   auto px = el->FirstChildElement();

   while (px)
   {
      auto pixel = PixelXML::Generate(px);
      if (pixel == nullptr) {
         ECLOG_WARNING("Field got bad pixel, skipping..");
         e_pc++;
         px = px->NextSiblingElement();
         continue;
      }
      pc++;
      f->InsertPixel(pixel);
      px = px->NextSiblingElement();
   }

   if (e_pc) {
      ECLOG_WARNING("Field loading got " + e_pc + " pixel errors ");
   }

   if (pc == 0) { // If empty field, bad
      ECLOG_ERROR("Field has no pixels");
      return nullptr;
   }

   ECLOG_DEBUG("Field loaded, got " + pc + " pixels ");

   return f;
}

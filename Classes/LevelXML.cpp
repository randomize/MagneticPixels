#include "LevelXML.h"
#include "Level.h"

#include "FieldXML.h"
#include "GoalXML.h"
#include "LevelGenerator.h"


using namespace MPix;
using namespace tinyxml2;

//====---------------------------------------------======//


shared_ptr<Level> MPix::LevelXML::Generate( tinyxml2::XMLElement* src )
{
   auto fab = LevelGenerator::getInstance()->CreateEmpty();

   // <lvl id="12344" w="0" name="Training 1" >

   // Identifier - required
   if ( src->QueryUnsignedAttribute("id", &fab->levelID) != XML_NO_ERROR ) return nullptr;

   // World - required
   if ( src->QueryUnsignedAttribute("w", &fab->world) != XML_NO_ERROR ) return nullptr;

   // Name - optional, default = "Noname"
   auto n =  src->Attribute("name");
   if (n) {
      fab->name = n;
   }

   // Loading solution
   // TODO: implemen loading/saving

   // Loading pixel field
   auto fld = src->FirstChildElement("fld");
   if ( fld == nullptr ) {
      EM_LOG_ERROR("No field `fld` tag, level " + fab->GetID() + " load failed ");
      return nullptr;
   }
   auto f = FieldXML::Generate(fld);
   if (f == nullptr) {
      EM_LOG_ERROR("Field loading failed, level " + fab->GetID() + " load failed ");
      return nullptr;
   }
   fab->field = f;

   // Loading goals
   auto gl = fld->NextSiblingElement("gl");
   if ( fld == nullptr ) {
      EM_LOG_ERROR("No goal `gl` tag, level " + fab->GetID() + " load failed ");
      return nullptr;
   }
   auto goals = GoalXML::GenerateGoals(gl);
   if (!goals) {
      EM_LOG_ERROR("Goals loading failed, level " + fab->GetID() + " load failed ");
      return nullptr;
   }
   fab->goals = goals;

   EM_LOG_DEBUG("Level `"  + fab->GetName() + "` id="+ fab->GetID() + " loaded ");

   return fab;

}


bool MPix::LevelXML::Store( shared_ptr<Level> src, tinyxml2::XMLPrinter* dst )
{
   dst->OpenElement("lvl");

   // Basic attributes(id, name)
   dst->PushAttribute("id", src->GetID());
   dst->PushAttribute("w", src->GetWorld());
   dst->PushAttribute("name", src->GetName().c_str());

   // TODO: Solution if any

   // Field
   FieldXML::Store(src->GetField(), dst);

   // Goals
   GoalXML::Store(src->GetGoals(), dst);

   dst->CloseElement(); // lvl
   return true;

}

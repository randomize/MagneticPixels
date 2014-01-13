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

   // <lvl id="12344" w="0" name="Training 1" pan="0" >

   // Identifier - required
   if ( src->QueryUnsignedAttribute("id", &fab->levelID) != XML_NO_ERROR ) return nullptr;

   // World - required
   if ( src->QueryUnsignedAttribute("w", &fab->world) != XML_NO_ERROR ) return nullptr;

   // autopan optional, default = false
   int i = 0;
   src->QueryIntAttribute("pan", &i);
   if (i) {
      fab->auto_pan = true;
   } else {
      fab->auto_pan = false;
   }

   // Name - optional, default = "Noname"
   auto n =  src->Attribute("name");
   if (n) {
      fab->name = n;
   }

   // Loading viewport
   auto vp = src->FirstChildElement("vp");
   assert(vp);
   //  <vp tx="3" ty="5" bx="-4" by="-6" />

   int tx = 3;
   int ty = 5;
   vp->QueryIntAttribute("tx", &tx);
   vp->QueryIntAttribute("ty", &ty);
   fab->viewport.TR = Coordinates(tx,ty);
   int bx = -4;
   int by = -6;
   vp->QueryIntAttribute("bx", &bx);
   vp->QueryIntAttribute("by", &by);
   fab->viewport.BL = Coordinates(bx,by);

   // Loading solution
   // TODO: implemen loading/saving
   //auto sol = vp->NextSiblingElement("sol");

   // Loading pixel field
   auto fld = vp->NextSiblingElement("fld");
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

   // Basic attributes(id, name, pan)
   dst->PushAttribute("id", src->GetID());
   dst->PushAttribute("w", src->GetWorld());
   dst->PushAttribute("name", src->GetName().c_str());
   dst->PushAttribute("pan",  src->AutoPan() ? 1 : 0);

   //  Viewport
   dst->OpenElement("vp");
   auto vp = src->GetViewport();
   dst->PushAttribute("tx", vp.TR.x);
   dst->PushAttribute("ty", vp.TR.y);
   dst->PushAttribute("bx", vp.BL.x);
   dst->PushAttribute("by", vp.BL.y);
   dst->CloseElement(); // vp

   // Field
   FieldXML::Store(src->GetField(), dst);

   // Goals
   GoalXML::Store(src->GetGoals(), dst);

   dst->CloseElement(); // lvl
   return true;

}

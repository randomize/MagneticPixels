#include "GoalXML.h"
#include "Goal.h"
#include "MPix.h"

using namespace MPix;
using namespace tinyxml2;

//====---------------------------------------------======//


bool MPix::GoalXML::Store( shared_ptr<Goal> src, tinyxml2::XMLPrinter* dst )
{
   dst->OpenElement("g");
   for (auto t : src->GetTasks()) {
      dst->OpenElement("p");
      auto p = t.second.GetPos();
      dst->PushAttribute("x",p.x);
      dst->PushAttribute("y",p.y);
      dst->PushAttribute("c", static_cast<int>(t.second.GetColor()));
      dst->CloseElement(); //p
   }
   dst->CloseElement(); // g
   return true;
}

bool MPix::GoalXML::Store( shared_ptr<Goals> src, tinyxml2::XMLPrinter* dst )
{
   dst->OpenElement("gl");
   for (auto g : src->GetGoals()) {
      Store(g, dst);
   }
   dst->CloseElement(); // gl
   return true;
}

shared_ptr<Goal> MPix::GoalXML::GenerateGoal( tinyxml2::XMLElement* src )
{

   auto fab = make_shared<Goal>();
   auto gtask = src->FirstChildElement("p");
   int tc = 0;
   while (gtask) {

      do { // Not a loop, hidden GOTO...

         int x = 0;
         if ( gtask->QueryIntAttribute("x", &x) != XML_NO_ERROR ) {
            EM_LOG_WARNING("Goal task has no x, skipping ");
            break; // skip
         }

         int y = 0;
         if ( gtask->QueryIntAttribute("y", &y) != XML_NO_ERROR ) {
            EM_LOG_WARNING("Goal task has no y, skipping ");
            break; // skip
         }

         Coordinates pos(x,y);

         int i = 0;
         if ( gtask->QueryIntAttribute("c", &i) != XML_NO_ERROR ) {
            EM_LOG_WARNING("Goal task has no color, skipping ");
            break; // skip
         }

         PixelColor color = static_cast<PixelColor>(i);
         fab->AddTask(pos, color);
         tc++;

      } while (false);

      gtask = gtask->NextSiblingElement("p");
   }

   if (tc == 0) {
      EM_LOG_WARNING("No `p` tag in goal, load goal failed");
      return nullptr;
   }

   return fab;

}

shared_ptr<Goals> MPix::GoalXML::GenerateGoals( tinyxml2::XMLElement* src )
{
   auto fab = make_shared<Goals>();
   int gl_cnt = 0;

   auto g = src->FirstChildElement("g");

   while (g) {

      auto  goal = GenerateGoal(g);
      if (goal == nullptr ) {
         EM_LOG_WARNING("Goal load failed, skipping");
         g = g->NextSiblingElement();
         continue;
      }

      fab->InsertGoal(goal);
      gl_cnt++;
      g = g->NextSiblingElement();

   }

   if (gl_cnt == 0) {
      EM_LOG_ERROR("No goals loaded");
      return nullptr;
   }

   EM_LOG_DEBUG("Loaded " + gl_cnt + " goals ");
   return fab;
}

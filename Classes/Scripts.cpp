
#include "Scripts.h"
#include "Script.h"
#include "Context.h"
#include "Cmd.h"
#include "MPix.h"


#include "IColorful.h"
#include "IAssembled.h"

using namespace MPix;

//====---------------------------------------------======//

// Helpers

void ShowGlobalNotification(const string& message) {
   auto c = new CmdUIShowNotification(message);
   c->Execute();
}

void ShowNotificationOnPixel(const string& message, const Pixel& px) {
   auto c = new CmdUIShowNotification(message, true, LogicToScreen(px.GetPos()));
   c->Execute();
}


// Scripts

class ScriptTutorial1 : public Script {

public:

   const int TAG_RED = 1;

   ScriptTutorial1() :
      Script("Tutorial 1")
   {
      step = 0;

      on_first_move = [this]( const Context& context ) {
         step = 0;
         ShowGlobalNotification(LocalUTF8Str("Tap on red sleepy block to wake it!"));
      };

      on_last_grow = [this](const Context& context) {
         switch (step)
         {
         case 0:
         {
            auto px = context.GetPixelByTag(TAG_RED);
            auto pp = dynamic_pointer_cast<IAssembled>(px);
            if (pp && pp->IsInAssembly() == true) {
               step++;
               ShowGlobalNotification(LocalUTF8Str("Now use swipe gestures\n to move it to the goal."));
            }
            break;
         }
         case 1:
         {
            auto px = context.GetPixelByTag(TAG_RED);
            auto pp = dynamic_pointer_cast<IAssembled>(px);
            if (pp && pp->IsSmiling() == true) {
               step++;
               ShowGlobalNotification(LocalUTF8Str("Great! Tap happy block to accept\n it and finish the level."));
            }
            break;
         }
         default:
            break;
         }

      };
   }

   int step;

};

class ScriptTutorial2 : public Script {

public:

   ScriptTutorial2() :
      Script("Tutorial 2")
   {
      // TODO:
   }

};

unordered_map<unsigned, shared_ptr<Script>> MPix::CppScripts::GetScripts()
{
 /*  return{
      { 9911, make_shared<ScriptTutorial1>()},
      { 2, make_shared<ScriptTutorial2>()}
   };*/ // God forgive Visual Studio... initializers are so buggy

   unordered_map<unsigned, shared_ptr<Script>> ret;
   ret.emplace(9911, make_shared<ScriptTutorial1>());
   ret.emplace(2, make_shared<ScriptTutorial2>());
   return ret;
}

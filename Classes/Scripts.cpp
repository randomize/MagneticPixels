
#include "Scripts.h"
#include "Script.h"
#include "Context.h"
#include "Cmd.h"
#include "MPix.h"

#include "ContentManager.h"


#include "IColorful.h"
#include "IAssembled.h"

using namespace MPix;

//====---------------------------------------------======//

// Helpers

void ShowSimpleNotification(const string& message, Point pos = Point::ZERO) {
   auto notification_content = Node::create();
   notification_content->setCascadeOpacityEnabled(true);
   notification_content->setOpacityModifyRGB(true);
   notification_content->retain();

   LabelTTF* label;
   label = LabelTTF::create(message, ContentManager::getInstance().GetBaseFont(), 32.0f);
   label->setPosition(pos);
   label->setColor(Color3B::WHITE);
   notification_content->addChild(label);

   auto c = new CmdUIShowNotification(notification_content);
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
         ShowSimpleNotification(LocalUTF8Str("Tutorial 1.1"));
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
               ShowSimpleNotification(LocalUTF8Str("Tutorial 1.2"));
            }
            break;
         }
         case 1:
         {
            auto px = context.GetPixelByTag(TAG_RED);
            auto pp = dynamic_pointer_cast<IAssembled>(px);
            if (pp && pp->IsSmiling() == true) {
               step++;
               ShowSimpleNotification(LocalUTF8Str("Tutorial 1.3"));
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

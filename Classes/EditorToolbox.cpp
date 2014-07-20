#include "EditorToolbox.h"
#include "EditorTool.h"
#include "ColorBox.h"
#include "EditorLayer.h"
#include "EditorToolGenerator.h"

#include "GameStateManager.h"
#include "ContentManager.h"
#include "Cmd.h"
#include "MPix.h"

using namespace MPix;


bool EditorToolbox::init()
{
   touch_events = nullptr;

   tools = EditorToolGenerator::GenerateDefaultSet();

   for (auto t : tools) {
      t->BindContents(this);
      t->Hide();
   }

   tool_name = Label::createWithTTF("No tool", "Arial", 32.0f*2);
   //tool_name->getTexture()->setAliasTexParameters();
   tool_name->setScale(0.5f);
   tool_name->setColor(Color3B(0,0,0));
   tool_name->setPositionY( Director::getInstance()->getVisibleSize().height/2.0f );
   //tool_name->setPositionY( Director::getInstance()->getVisibleSize().height/2.0f - 24.0f );
   tool_name->setPositionX( MPIX_CELL_SIZE_HALF );
   addChild(tool_name);

   root_tool = tools[0];
   root_tool_size = root_tool->Childs()->size();
   current_tool = nullptr;
   last_used = 0;

   return true;
}

bool EditorToolbox::onTouchBegan( Touch *touch, Event *event )
{
   auto p = ScreenToLogic(this->convertTouchToNodeSpace(touch)) ;
   GotClick(p);
   return false;
}

void MPix::EditorToolbox::PrepareTools()
{
   if (last_used) {
      auto n = dynamic_pointer_cast<EditorFolderTool>(root_tool);
      n->Insert(last_used);
      if (n->Count() == 16) {
         int sz = 16 - root_tool_size;
         list<int> p;
         for (int x = 0; x < sz; ++x) {
            p.push_front(n->RemoveLast());
         }
         p.pop_front();
         for (int x = 1; x < sz; ++x) { // sz-1 times
            n->Insert(p.front());
            p.pop_front();
         }
      }

   }
   LoadTool( root_tool, 0);
}

void MPix::EditorToolbox::GotClick(Coordinates p)
{
   auto map = current_tool->Childs();
   assert(map); // current tool must have childs

   // search for any tool at pos
   auto it = map->find(p);

   if ( it == map->end())
      CloseToolbox();
   else
      LoadTool(tools[it->second], it->second);
}

void MPix::EditorToolbox::LoadTool( shared_ptr<EditorTool> tool, int id)
{
   tool->Chosen();
   tool_name->setString(tool->GetName());

   if (current_tool) {
      auto c = current_tool->Childs();
      if (c) {
         for (auto t : *c ) {
            auto tt = tools[t.second];
            tt->Hide();
         }
      }
   }

   current_tool = tool;

   auto c = current_tool->Childs();
   if (c) {
      for (auto t : *c ) {
         auto tt = tools[t.second];
         tt->Show(t.first);
      }
   }

   if (current_tool->onSelected(p_editor_layer)) {
      CloseToolbox();
      last_used = id;
   }

}

void MPix::EditorToolbox::ReuseLastTool()
{
   if (current_tool && current_tool->Childs() == nullptr) {
      current_tool->onSelected(p_editor_layer);
   }
}


void MPix::EditorToolbox::CloseToolbox()
{
   GameStateManager::getInstance().CurrentState()->Execute(new CmdEditorAction(CmdEditorAction::EditorAction::ED_HIDE_TOOLS));
}

void MPix::EditorToolbox::TouchDisable()
{
   _eventDispatcher->removeEventListener(touch_events);
   touch_events = nullptr;
}

void MPix::EditorToolbox::TouchEnable()
{
   ECLOG_DEBUG("Editor toolbox layer touches on");

   assert(touch_events == nullptr); // Ensures Enable/Disable matching

   auto listener = EventListenerTouchOneByOne::create();
   listener->setSwallowTouches(true);
   listener->onTouchBegan     = CC_CALLBACK_2(EditorToolbox::onTouchBegan, this);

   _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
   touch_events = listener;
}




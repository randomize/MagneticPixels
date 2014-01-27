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

MPix::EditorToolbox::EditorToolbox()
{
   // Create all tools(Allocates memory, returns RAW pointer array)
   tools = EditorToolGenerator::GenerateDefaultSet();
}

MPix::EditorToolbox::~EditorToolbox()
{
   // Deallocate memory for tools, and remove all tools
   for (auto t : tools) {
      delete t;
   }
   tools.clear();
}

bool EditorToolbox::init()
{

   // Markup
  /* for (int x = -2; x <= 2; x+=2 )
      for (int y = -4; y <= 4; y+=2 ) {
         auto cb = ColorBox::create();
         cb->SetColor(Color4F(1,1,1,0.6f));
         cb->setPosition(LogicToScreen(x, y));
         addChild(cb);
      }*/


   for (auto t : tools) {
      t->BindContents(this);
      t->Hide();
   }

   tool_name = LabelTTF::create("No tool", "Arial", 32.0f*2);
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

void MPix::EditorToolbox::onShow()
{
   if (last_used) {
      auto n = static_cast<EditorFolderTool*>(root_tool);
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

void MPix::EditorToolbox::LoadTool( EditorTool* tool, int id)
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

   if (current_tool->onSelected(Editor)) {
      CloseToolbox();
      last_used = id;
   }

}


void MPix::EditorToolbox::CloseToolbox()
{
   GameStateManager::getInstance().CurrentState()->Execute(new CmdEditorAction(CmdEditorAction::EditorAction::ED_HIDE_TOOLS));
}

void MPix::EditorToolbox::TouchDisable()
{
   _eventDispatcher->removeEventListeners(EventListener::Type::TOUCH_ONE_BY_ONE);
}

void MPix::EditorToolbox::TouchEnable()
{
   EM_LOG_DEBUG("Editor toolbox layer touches on");
   auto listener = EventListenerTouchOneByOne::create();
   listener->setSwallowTouches(false);
   listener->onTouchBegan     = CC_CALLBACK_2(EditorToolbox::onTouchBegan, this);
   _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}



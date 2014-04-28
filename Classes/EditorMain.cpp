#include "EditorMain.h"
#include "Cmd.h"

#include "EditorLayer.h"
#include "EditorToolbox.h"

#include "GameplayManager.h"
#include "GameStateManager.h"
#include "LevelManager.h"
#include "Level.h"
#include "SimpleInputDialog.h"

using namespace MPix;
using namespace std::placeholders;

// Static local method for handling
/* Use if static methods like this allows to not include cmd.h to EditorMain.h
*/
ErrorCode EditorActionHandler(EditorMain* that, CmdEditorAction::EditorAction act) {
   switch (act)
   {
   case MPix::CmdEditorAction::EditorAction::ED_SHOW_TOOLS:
      that->ShowToolBox();
      break;
   case MPix::CmdEditorAction::EditorAction::ED_HIDE_TOOLS:
      that->HideToolBox();
      break;
   case CmdEditorAction::EditorAction::ED_DRAW_WITH_LAST_TOOL:
      that->ReuseLastTool();
      break;
   default:
      break;
   }
   return ErrorCode::RET_OK;
}

MPix::EditorMain::EditorMain()
{
   CmdEditorAction::listners["EditorMain"] = std::bind( EditorActionHandler, this, std::placeholders::_1);
}

MPix::EditorMain::~EditorMain()
{
   CmdEditorAction::listners.erase("EditorMain");
}

bool EditorMain::init()
{

   // 1. super init first
   if ( !Scene::init() )
   {
      return false;
   }


   return true;
}

void EditorMain::onEnter()
{
   Scene::onEnter();

   Size fullSize = Director::getInstance()->getWinSize();
   Size halfSize =  fullSize / 2.0f;
   Size visibleSize = Director::getInstance()->getVisibleSize();
   Vector2 origin = Director::getInstance()->getVisibleOrigin();

   contentScale = visibleSize.height/fullSize.height * visibleSize.width/fullSize.width;

   bg = LayerColor::create(Color4B::GRAY);
   bg->setScale(2.0f);
   addChild(bg, 0);

   editor = EditorLayer::create();
   assert(editor);
   editor->setPosition(halfSize.width, halfSize.height);
   editor->setScale(contentScale);
   editor->TouchEnable();
   addChild(editor, 1);

   const unsigned char f = 190;
   tb_bg = LayerColor::create(Color4B(f,f,f,255));
   tb_bg->setVisible(false);
   addChild(tb_bg, 2);

   this->tb = EditorToolbox::create();
   tb->setPosition(halfSize.width-MPIX_CELL_SIZE_HALF, halfSize.height-MPIX_CELL_SIZE_HALF);
   tb->setScale(contentScale);
   tb->setVisible(false);
   tb->p_editor_layer = editor;
   addChild(tb, 3);


   // Create menu
   auto menu = Menu::create();
   menu->setPosition(Vector2::ZERO);
   int base_tag = 101;
   const char* names[] = {
      "play.png", // 101
      "save.png",   // 102
      "shake.png",  // 103
      "menu.png",    // 104
      "rename.png" // 105
   };
   for (int i = 0; i < sizeof(names)/sizeof(names[0]); ++i) {
      auto btn = MenuItemImage::create( names[i], names[i], CC_CALLBACK_1(EditorMain::BtnHnadler, this));
      btn->setTag(base_tag + i);
      btn->setPosition(Vector2(
         origin.x + visibleSize.width - btn->getContentSize().width/2 * (2*i+1) ,
         origin.y + btn->getContentSize().height/2)
         );
      menu->addChild(btn);
   }
   this->addChild(menu, 4);
   menu->setAnchorPoint(Vector2(1, 0));
   menu->setScale(Director::getInstance()->getContentScaleFactor()); // FIXME

   lvl_name = Label::createWithTTF(editor->GetLevelName().c_str(), "Arial", 36);
   lvl_name->setColor(Color3B::BLACK);
   auto sx = lvl_name->getContentSize();
   lvl_name->setPosition(origin +Point(sx.width, sx.height));
   addChild(lvl_name, 5);

}

void EditorMain::onExit()
{
   Scene::onExit();

   editor->TouchDisable();
}

ErrorCode EditorMain::Tick( float t )
{

   //static int cnt = 0;

   // Check if async commands available
   if (!scheduled_commands.empty()) {
      for ( auto c : scheduled_commands )
         c->Execute();
   }

   return ErrorCode::RET_OK;
}

ErrorCode EditorMain::ExecuteAsync( Command* cmd )
{
   scheduled_commands.push_back(cmd);
   return ErrorCode::RET_OK;
}

void MPix::EditorMain::BtnHnadler(Ref* sender)
{
   auto id = (dynamic_cast<Node*>(sender))->getTag();

   switch(id) {
      case 101: {
         TestLevel();
         return;
      }
      case 102: {
         SaveLevel();
         return;
      }
      case 103: {
         ShowHint("REMOVED FUNCTION");
         return;
      }
      case 104: {
         GameStateManager::getInstance().SwitchToEditorMenu();
         return;
      }
      case 105: {

         auto smd = SimpleInputDialog::create("Enter new level name", editor->GetLevelName().c_str());
         smd->SetCallback( std::bind(&EditorMain::Renamer, this, std::placeholders::_1));
         addChild(smd, 5);

      }
   }
}

void MPix::EditorMain::TestLevel()
{
   auto lvl = editor->CreateLevel();
   if (lvl) {
      LevelManager::getInstance().SetEditorsLevel(lvl);
      auto l = LevelManager::getInstance().GetPlayableEditorsLevel();
      GameplayManager::getInstance().LoadLevel(l);
      GameStateManager::getInstance().SwitchToTestGame();
   } else {
      ShowHint("Level must have at least\n one Pixel and one Goal");
   }
}

void MPix::EditorMain::SaveLevel()
{
   auto lvl = editor->CreateLevel();
   if (lvl) {
      LevelManager::getInstance().SetEditorsLevel(lvl);
      LevelManager::getInstance().SaveEditorsLevel();
      ShowHint("Saved level\n id="+ ToString(lvl->GetID()) + " ("+ lvl->GetName()+")");
   } else {
      ShowHint("Level must have at least\n one Pixel and one Goal");
   }
}

void MPix::EditorMain::ShowToolBox()
{
   tb_bg->setVisible(true);
   tb_bg->stopAllActions();
   tb_bg->runAction(FadeTo::create(0.4f, 200));
   editor->TouchDisable();
   tb->TouchEnable();
   tb->setVisible(true);
   tb->PrepareTools();
}

void MPix::EditorMain::HideToolBox()
{
   tb_bg->stopAllActions();
   tb_bg->runAction(Sequence::create(FadeTo::create(0.4f, 0), Hide::create(), nullptr));
   tb->setVisible(true);
   tb->TouchDisable();
   editor->TouchEnable();
   tb->setVisible(false);
}

void MPix::EditorMain::ShowHint( const string& text )
{
   auto hint = Label::createWithTTF(text.c_str(), "Arial", 48.0f*2);
   hint->setScale(0.5f);
   hint->setColor(Color3B::RED);
   hint->setPositionY( Director::getInstance()->getVisibleSize().height - 54.0f );
   hint->setPositionX( Director::getInstance()->getVisibleSize().width/2.0);
   addChild(hint, 100);
   hint->runAction( Sequence::create( FadeOut::create(2.5f), RemoveSelf::create(), nullptr ));

}

void MPix::EditorMain::Renamer( string lname )
{
   editor->RenameLevel(lname);
   lvl_name->setString(lname.c_str());
}

void MPix::EditorMain::ReuseLastTool()
{
   tb->ReuseLastTool();
}




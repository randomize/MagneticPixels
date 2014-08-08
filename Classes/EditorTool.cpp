#include "EditorTool.h"

#include "ContentManager.h"
#include "EditorLayer.h"

#include "Pixel.h"
#include "Goal.h"

#include "PixelView.h"
#include "GoalView.h"

#include "ColorBox.h"

using namespace MPix;

//////////////////////////////////////////////////////////////////////////
// Base tool class

MPix::EditorTool::EditorTool( const char* name ) :
   name(name)
{
}

ToolMap* MPix::EditorTool::Childs()
{
   return nullptr;
}

void MPix::EditorTool::Chosen()
{
   // Does nothing
}

void MPix::EditorTool::Hide()
{
   name_l->setVisible(false);
}

void MPix::EditorTool::Show( Coordinates where )
{
   name_l->setVisible(true);
   auto p = LogicToScreen(where) ;
   p.x += MPIX_CELL_SIZE_HALF;
   p.y -= MPIX_CELL_SIZE_HALF/2;
   name_l->setPosition(p);
}

void MPix::EditorTool::BindContents( Node * target )
{
   name_l = Label::createWithTTF(name.c_str(), ContentManager::getInstance().GetEditorFont(), 20.0f*2);
   name_l->setScale(0.5f);
   name_l->setColor(Color3B(0,0,0));
   target->addChild(name_l);
}

const char* MPix::EditorTool::GetName()
{
   return name.c_str();
}

//////////////////////////////////////////////////////////////////////////
// Folder

MPix::EditorFolderTool::EditorFolderTool( const char* name, const char* icon_name /*= nullptr*/ ) :
   EditorTool(name),
   cur(-2,4),
   icon_frame(icon_name),
   icon(nullptr)
{
}

void MPix::EditorFolderTool::AddCur()
{
   cur.x += 2;
   if (cur.x > 2) {
      cur.y -= 2;
      cur.x = -2;
   }
}

void MPix::EditorFolderTool::SubCur()
{
   cur.x -= 2;
   if (cur.x < -2) {
      cur.y += 2;
      cur.x = 2;
   }
}


void MPix::EditorFolderTool::Insert( int id )
{
   if ( map_inv.find(id) != map_inv.end()) 
      return;
   assert(map.find(cur) == map.end());
   map.emplace(cur, id);
   map_inv.emplace(id, cur);
   AddCur();
}

int MPix::EditorFolderTool::RemoveLast()
{
   assert(!map.empty());
   SubCur();
   auto it = map.find(cur);
   auto id = it->second;
   map_inv.erase(id);
   map.erase(it);
   return id;
}

size_t MPix::EditorFolderTool::Count() const
{
   return map.size();
}


ToolMap* MPix::EditorFolderTool::Childs()
{
   return &map;
}

bool MPix::EditorFolderTool::onSelected( EditorLayer* target )
{
   // Does nothing, false means - don't close
   return false; 
}

void MPix::EditorFolderTool::Hide()
{
   EditorTool::Hide();
   icon->setVisible(false);
}

void MPix::EditorFolderTool::Show( Coordinates where )
{
   EditorTool::Show(where);
   icon->setPosition(LogicToScreen(where) + MPIX_CELL_SIZE_HALF_P);
   icon->setVisible(true);
}

void MPix::EditorFolderTool::BindContents( Node * target )
{
   EditorTool::BindContents(target);

   if (icon_frame) // Specified logo
   {
      icon = Node::create();

      if (string("Goal") == icon_frame) // Oh dear, so bad code.. goal as logo
      {
         auto shape = make_shared<Goal>();
         shape->AddTask(Coordinates(0, 0), PixelColor::RED);
         auto gv = make_shared<GoalView>();
         gv->Build(shape);
         gv->BindContents(icon);
         gv->setPosition(-MPIX_CELL_SIZE_HALF_P);
         icon_view = gv;
      }
      else // Pixel name as logo
      {
         auto px = shared_ptr<Pixel>(Pixel::create(icon_frame));
         auto view = PixelView::create(px);
         view->Build(px);
         view->BindContents(icon);
         view->setPosition(Vector2(0,0));
         icon_view = view;
      }

   }
   else // No logo
   { 
      auto b = ColorBox::create();
      b->setAnchorPoint(Vector2(0.5f,0.5f));
      b->SetColor(Color4F(0.8f,0.8f,0.8f,0.8f));
      b->SetBorders(15,Color4F(0.0f,0.0f,0.0f,0.8f));
      b->SetBorderWidth(1.0f);
      icon = b;
   }
   target->addChild(icon);
}

MPix::EditorFolderTool::~EditorFolderTool()
{
   // To avoid memory corruption if icon_view was used:
   if (icon_view) {
      icon_view.reset(); // View will be destroyed, destructor will clenup contents form icon node which is still present
   }
   // Then it is safe to kill icon's parent, later it will die
   if (icon) {
      icon->removeFromParent();
   }
}


//////////////////////////////////////////////////////////////////////////
// Pixel

MPix::EditorToolPixel::EditorToolPixel( const char* name, shared_ptr<Pixel> pixel ) :
   EditorTool(name),
   px(pixel->Dublicate())
{
}

void MPix::EditorToolPixel::BindContents( Node * target )
{
   EditorTool::BindContents(target);
   string name = px->GetTypeName() + "view";
   shared_ptr<PixelView> view(dynamic_cast<PixelView*>(PixelView::Factory()->NewNodeByName(name))); // TODO: think an move cast to factory itself!
   this->view = view;
   assert(view);
   view->Build(px);
   view->BindContents(target, 1);
   view->Update(CmdUIUpdatePixelView::Reason::CREATED); 
}

bool MPix::EditorToolPixel::onSelected( EditorLayer* target )
{
   target->InsertPixel( px->Dublicate() );
   return true;
}

void MPix::EditorToolPixel::Hide()
{
   EditorTool::Hide();
   view->setVisible(false);
}

void MPix::EditorToolPixel::Show( Coordinates where )
{
   EditorTool::Show(where);
   view->setPosition(LogicToScreen(where) + MPIX_CELL_SIZE_HALF_P);
   view->setVisible(true);
   view->Update(CmdUIUpdatePixelView::Reason::CREATED); 
}

//////////////////////////////////////////////////////////////////////////
// Goal

MPix::EditorGoalTool::EditorGoalTool( const char* name, PixelColor color, int goaln ) :
   EditorTool(name),
   color(color),
   goalindex(goaln)
{

}

bool MPix::EditorGoalTool::onSelected( EditorLayer* target )
{
   target->InsertGoal(goalindex, color);
   return true;
}

void MPix::EditorGoalTool::Hide()
{
   EditorTool::Hide();
   gv->setVisible(false);
}

void MPix::EditorGoalTool::Show( Coordinates where )
{
   EditorTool::Show(where);
   gv->setPosition(LogicToScreen(where));
   gv->setVisible(true);
   gv->Update(CmdUIUpdateGoalView::Reason::CREATED);
}

void MPix::EditorGoalTool::BindContents( Node * target )
{
   EditorTool::BindContents(target);
   g = make_shared<Goal>();
   g->AddTask(Coordinates(0,0), color);
   gv = make_shared<GoalView>();
   gv->Build(g);
   gv->BindContents(target, 0);
   gv->Update(CmdUIUpdateGoalView::Reason::CREATED);
}

//////////////////////////////////////////////////////////////////////////
// Eraser

const char * MakeEraserName(MPix::EditorToolEraser::Type t) {
   switch (t)
   {
   case MPix::EditorToolEraser::Type::ALL:
      return "Eraser All";
   case MPix::EditorToolEraser::Type::TOP:
      return "Eraser Top";
   case MPix::EditorToolEraser::Type::GOAL:
      return "Eraser Goal";
   case MPix::EditorToolEraser::Type::PIXEL:
      return "Eraser Pixel";
   case EditorToolEraser::Type::WALL:
      return "Wall Eraser";
   default:
      return "Eraser Unknown";
   }
   return "WTF";
}

MPix::EditorToolEraser::EditorToolEraser( Type type ):
   EditorTool(MakeEraserName(type)), icon(nullptr), t(type)
{
}

bool MPix::EditorToolEraser::onSelected( EditorLayer* target )
{
   switch (t)
   {
   case Type::ALL:
      target->EraseEverything();
      break;
   case Type::TOP:
      target->EraseTopPixel();
      break;
   case Type::GOAL:
      target->EraseGoal();
      break;
   case Type::PIXEL:
      target->ErasePixels();
      break;
   case Type::WALL:
      target->EraseWall();
      break;
   default:
      assert(false);
   }
   return true;
}

void MPix::EditorToolEraser::Hide()
{
   EditorTool::Hide();
   icon->setVisible(false);
}

void MPix::EditorToolEraser::Show( Coordinates where )
{
   EditorTool::Show(where);
   icon->setPosition(LogicToScreen(where) + MPIX_CELL_SIZE_HALF_P);
   icon->setVisible(true);
}

void MPix::EditorToolEraser::BindContents( Node * target )
{
   EditorTool::BindContents(target);
   icon = ContentManager::getInstance().GetSimpleSprite("trash_can");
   target->addChild(icon);
}

MPix::EditorToolEraser::~EditorToolEraser()
{
   if (icon) {
      icon->removeFromParent();
   }

}

//////////////////////////////////////////////////////////////////////////
// Wall

bool MPix::EditorToolWall::onSelected( EditorLayer* target )
{
   target->InsertWall( px->Dublicate() );
   return true;
}

void MPix::EditorToolWall::Show( Coordinates where )
{
   EditorTool::Show(where);
   view->setPosition(LogicToScreen(where) + MPIX_CELL_SIZE_HALF_P);
   view->setVisible(true);

}

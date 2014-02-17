#include "PixelsLayer.h"

#include "Cmd.h"
#include "GameplayManager.h"
#include "ContentManager.h"
#include "ViewManager.h"

#include "Context.h"
//#include "Pixel.h"
#include "Field.h"
#include "Goal.h"

#include "PixelView.h"
#include "GoalView.h"


using namespace MPix;

MPix::PixelsLayer::PixelsLayer()
{
   // Register command executors for pixel field
   CmdUICreateViews::listners["PixelsLayer"] = std::bind(&PixelsLayer::CreateAllViews, this);
}

bool MPix::PixelsLayer::init()
{
   assert(GameplayManager::getInstance().GetState() == GameplayManager::State::READY);

   // Store context reference
   activeContext = &GameplayManager::getInstance().GetContext();

   return true;
}

MPix::PixelsLayer::~PixelsLayer()
{
   // Removing all views
   Clear();

   // Unregister
   CmdUICreateViews::listners.erase("PixelsLayer");
}

void MPix::PixelsLayer::Reset()
{
   Clear();
   init();
}


////////// Views ////////////////////////////////////////////////////////////////

EmbossLib::ErrorCode MPix::PixelsLayer::CreateAllViews()
{
   EM_LOG_DEBUG("Creating all views in PixelsLayer");
   assert(activeContext->goals);
   assert(activeContext->field);

   // Only one goal per layer in current version of game
   assert(activeContext->goals->GoalsCount() == 1);
   auto g = activeContext->goals->GetGoalByID(1);
   CreateViewForGoal(g);

   // TODO: can use field map instead of ID's to determine pixel stacks and put them to StackView
   // Create also pixel views
   const auto & pxs = activeContext->field->GetPixelData();
   for (auto px : pxs) {
      CreateViewForPixel(px);
   }

   return ErrorCode::RET_OK;

}

void MPix::PixelsLayer::Clear()
{
   // Cleaning views, they should remove everything from BatchNodes
   ViewManager::getInstance().Clear();

   // Ensure that no one left a node on pixels layer
   auto n = this->getChildrenCount();
   if (n != 0) {
      removeAllChildren();
      EM_LOG_WARNING(" Pixels layer was not cleared fully by views, removing " + ToString(n) + " children");
   }
}

EmbossLib::ErrorCode MPix::PixelsLayer::CreateViewForPixel( shared_ptr<Pixel> p )
{
   auto view = PixelView::create(p);
   assert(view);
   view->Build(p);
   ViewManager::getInstance().AddPixelView(p->GetID(), view);
   view->BindContents(this);
   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::PixelsLayer::CreateViewForGoal(shared_ptr<Goal> g)
{
   auto gv = make_shared<GoalView>();
   gv->Build(g);
   ViewManager::getInstance().AddGoalView(g->GetID(), gv);
   gv->BindContents(this);
   return ErrorCode::RET_OK;
}


////////// Assembly ////////////////////////////////////////////////////////////////




#include "ViewManager.h"

#include "PixelView.h"
#include "GoalView.h"

using namespace MPix;

MPix::ViewManager::ViewManager()
{
   EM_LOG_INFO("[ ViewManager initialized ]");
   CmdUIUpdatePixelView::listners["ViewManager"] = std::bind(&ViewManager::UpdatePixelView, this, std::placeholders::_1, std::placeholders::_2);
   CmdUIUpdateGoalView::listners["ViewManager"] = std::bind(&ViewManager::UpdateGoalView, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

MPix::ViewManager::~ViewManager()
{
   CmdUIUpdatePixelView::listners.erase("ViewManager");
   CmdUIUpdateGoalView::listners.erase("ViewManager");
}

void MPix::ViewManager::AddPixelView(int id, shared_ptr<PixelView> pview )
{
   assert(pview);
   auto vit = pixels.find(id);
   assert(vit == pixels.end());
   this->pixels.emplace(id, pview);
}

void MPix::ViewManager::AddGoalView( int id, shared_ptr<GoalView> g )
{
   goals.emplace(id, g);
}

shared_ptr<PixelView> MPix::ViewManager::GetPixelViewByID( int id )
{
   auto vit = pixels.find(id);
   assert(vit != pixels.end());
   assert(vit->second);
   return vit->second;
}

shared_ptr<GoalView> MPix::ViewManager::GetGoalViewByID( int id )
{
   auto vit = goals.find(id);
   assert(vit != goals.end());
   assert(vit->second);
   return vit->second;
}

EmbossLib::ErrorCode MPix::ViewManager::UpdatePixelView( int id, CmdUIUpdatePixelView::Reason reason )
{
   if ( GetPixelViewByID(id)->Update(reason) == false ) {
      EM_LOG_WARNING("Pixel View #" + id + " not handled update reason #" + static_cast<int>(reason)) ;
   }
   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::ViewManager::UpdateGoalView( int id, Coordinates task, CmdUIUpdateGoalView::Reason reason )
{
   if (GetGoalViewByID(id)->Update(reason, task) == false ) {
      EM_LOG_WARNING("Goal View #" + id + " Task# " + task + " not handled update reason #" + static_cast<int>(reason)) ;
   };
   return ErrorCode::RET_OK;
}

void MPix::ViewManager::Clear()
{
   this->pixels.clear();
   this->goals.clear();
}


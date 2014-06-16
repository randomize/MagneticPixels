#include "ViewManager.h"

#include "PixelView.h"
#include "GoalView.h"

using namespace MPix;

MPix::ViewManager::ViewManager()
{
   ECLOG_INFO("[ ViewManager initialized ]");
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

EndlessCatLib::ErrorCode MPix::ViewManager::UpdatePixelView( int id, CmdUIUpdatePixelView::Reason reason )
{
   if ( GetPixelViewByID(id)->Update(reason) == false ) {
      ECLOG_WARNING("Pixel View #" + id + " not handled update reason #" + static_cast<int>(reason)) ;
   }
   return ErrorCode::RET_OK;
}

EndlessCatLib::ErrorCode MPix::ViewManager::UpdateGoalView( int id, Coordinates task, CmdUIUpdateGoalView::Reason reason )
{
   if (GetGoalViewByID(id)->Update(reason, task) == false ) {
      ECLOG_WARNING("Goal View #" + id + " Task# " + task + " not handled update reason #" + static_cast<int>(reason)) ;
   };
   return ErrorCode::RET_OK;
}

void MPix::ViewManager::Clear()
{
   this->pixels.clear();
   this->goals.clear();
}

void MPix::ViewManager::RunIdleUpdateOnRandomPixel()
{

   for (auto& p : pixels) {
      float dice = rand() % 1000 / 1000.0;
      if (dice < 0.2) { // %20 percent of pixels
         auto reason = CmdUIUpdatePixelView::Reason::IDLE_TRICK;
         if ( p.second->Update(reason) == false ) {
            ECLOG_WARNING("Pixel View #" + p.first + " not handled update reason #" + static_cast<int>(reason)) ;
         }
      }
   }

}



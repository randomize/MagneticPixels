#include "EditorData.h"

#include "Pixel.h"
#include "Goal.h"

using namespace MPix;

//====---------------------------------------------======//


MPix::EditorData::EditorData()
{
   // Let's have some nullptr goals =)
   goals.resize(4);
   viewport.BL = Coordinates(-4,-6);
   viewport.TR = Coordinates(3,5);
   name = "Unnamed";
   autopan = true;
}


void MPix::EditorData::InsertPixelAt( Coordinates pos, shared_ptr<Pixel> p )
{
   p->SetPos(pos);
   auto it = field.find(pos);
   if (it != field.end()) {
      it->second.push_back(p);
   } else {
      vector<shared_ptr<Pixel>> v;
      v.reserve(5);
      v.push_back(p);
      field.emplace(pos, v);
   }
}

int MPix::EditorData::NumberOfPixelsAt( Coordinates pos )
{
   auto it = field.find(pos);
   if (it != field.end()) { 
      return it->second.size();
   }
   else{
      return 0;
   }
}

bool MPix::EditorData::RemoveTopPixelAt( Coordinates pos )
{
   auto it = field.find(pos);
   if (it != field.end()) { 
      it->second.pop_back();
      if (it->second.empty()) // LAst
         field.erase(it);
      return true;
   }
   else{
      return false;
   }
}

bool MPix::EditorData::RemoveAllPixelsAt( Coordinates pos )
{
   auto it = field.find(pos);
   if (it != field.end()) { 
      field.erase(pos);
      return true;
   }
   else{
      return false;
   }
}

bool MPix::EditorData::InsertTask( int index, Coordinates pos, PixelColor col )
{
   assert(index>=0 && index<3);
   auto & g = goals[index];
   if (  g == nullptr ) {
      g = make_shared<Goal>();
      g->AddTask(pos, col);
      return true;
   }
   if (g->ContainsPoint(pos)) {
      return false;
   }
   g->AddTask(pos, col);
   return true;

}

bool MPix::EditorData::RemoveTask( Coordinates pos )
{
   bool was = false;
   for ( auto g : goals ) {
      if ( g == nullptr ) continue;
      if (g->ContainsPoint(pos)) {
         was = true;
         g->RemoveTask(pos);
         if ( g->TaskCount() == 0 ) {
            g = nullptr;
         }
      }
   }

   return was ? true : false;
}

bool MPix::EditorData::CheckConsistency()
{
   // Check if there is field with one pixel and task with one goal
   if (field.empty()) return false;
   for (auto f : goals ) {
      if (f) {
         return true;
      }
   }
   return false;
}

void MPix::EditorData::InsertWallAt( Coordinates pos, shared_ptr<Pixel> p )
{
   p->SetPos(pos);
   walls[pos] = p;
}

bool MPix::EditorData::RemoveWallAt( Coordinates pos )
{
   auto it = walls.find(pos);
   if (it != walls.end()) { 
      walls.erase(pos);
      return true;
   }
   else{
      return false;
   }
}

std::vector<shared_ptr<Pixel>> MPix::EditorData::GetAllWalls()
{
   std::vector<shared_ptr<Pixel>> fab;
   fab.reserve(walls.size());
   for ( auto w : walls ) {
      fab.push_back( w.second );
   }
   return fab;
}

std::vector<shared_ptr<Pixel>> MPix::EditorData::GetAllPixels()
{
   std::vector<shared_ptr<Pixel>> fab;
   fab.reserve(field.size()); // at least

   for ( auto pos : field ) {
      for ( auto px : pos.second )
         fab.push_back( px );
   }
   return fab;
}

std::vector<std::pair<Coordinates, PixelColor>> MPix::EditorData::GetAllGoals()
{
   std::vector<std::pair<Coordinates, PixelColor>> fab;

   for (auto g : goals ) {
      if (g) {
         for ( auto p : g->GetTasks() ) {
            fab.push_back(std::make_pair(p.first, p.second.GetColor()));
         }
      }
   }
   return fab;
}

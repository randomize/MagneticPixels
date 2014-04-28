#include "Assembly.h"

#include "IAssembled.h"
#include "Pixel.h"
#include "Context.h"
#include "Goal.h"
#include "Field.h"
#include "IMagnetic.h"

using namespace MPix;

Assembly::Assembly()
{
   move_number = 0;
   last_dir = Direction::DIR_UNKNOWN;
}

ErrorCode Assembly::AddPixel( const Context& context, shared_ptr<IAssembled> pixel )
{
   // Add to contents
   contents.GetValue().push_front(pixel);

   // Set internal state
   pixel->AddToAssembly(context);

   EM_LOG_DEBUG(" Pixel "+ pixel->GetID() + " added to assembly");

   return ErrorCode::RET_OK;
}

ErrorCode MPix::Assembly::CheckMove( const Context& context, Direction d )
{
   bool moving = true;
   for (auto pix : contents.GetValue()) {
      if (pix->canMoveThis(context, d) == false) {
         context.PostEvent(PixelEvent::BLOCKED_MOVE, pix);
         moving = false;
      }
   }
   return moving ? ErrorCode::RET_YES : ErrorCode::RET_NO;
}


ErrorCode Assembly::Move( const Context& context, Direction d )
{
   for (auto pix : contents.GetValue()) {
      pix->Move(context, d);
   }
   last_dir = d;
   return ErrorCode::RET_OK;
}


ErrorCode Assembly::Grow( const Context& cont )
{

   bool growHappend = false;

   // Copy current contents of assembly
   auto growers = contents.GetValue();

   // Performing only one wave of grow
   for (auto pix : growers) {
      if (pix->growThis(cont)) {
         growHappend = true;
      }
   }

   return growHappend ? ErrorCode::RET_YES : ErrorCode::RET_NO;

}


ErrorCode Assembly::AcceptedSolution( const Context& context )
{
   for (auto pix : contents.GetValue()) {
      pix->Accept(context);
   }
   contents.GetValue().clear();
   return ErrorCode::RET_OK;
}

ErrorCode Assembly::CheckForLost( const Context& context )
{
   auto & contents = this->contents.GetValue();

   if (contents.empty()) 
      return ErrorCode::RET_NO;

   bool lost = false;

   // phase 1 - remove dead pixels from assembly
   contents.remove_if( 
      [&](shared_ptr<IAssembled> px) {
         if (!px->canLive(context)) {
            px->RemoveFromAssembly(context);
            context.field->SetPixelState(px, Pixel::State::INACTIVE);
            lost = true;
            return true;
         }
         return false;
      }
   );

   // phase 2 - check now collective life(pitfall test for assembly);

   // 2.1 - build classes of connectivity
   // one pixel standing on the ground can hold any number of aliases
   list<unordered_set<shared_ptr<IAssembled>>> classes;
   unordered_set<shared_ptr<IAssembled>> toProcess(begin(contents), end(contents));
   list<shared_ptr<IAssembled>> stack;

   while (toProcess.empty() == false) // while have to process
   {
      // Create new class
      classes.emplace_back(10);
      auto & cur_class = classes.back();

      // Remove one from toProcess
      auto cur = *toProcess.begin();
      toProcess.erase(cur);
      cur_class.insert(cur);
      stack.push_back(cur);

      while (stack.empty() == false)
      {
         auto px = stack.front(); 
         stack.pop_front();
         auto pos = px->GetPos();

         // Check 4 directions
         for (auto d : EnumRanger<Direction>(DirectionType::MAJOR)) 
         {
            for ( auto p : toProcess )
            {
               if ( p->GetPos() == pos + d )
               {
                  stack.push_back(p);
                  cur_class.insert(p);
                  toProcess.erase(p);
                  break;
               }
            }
         }
      }
   }

   // Test for debug 
   /*EM_LOG_ERROR(" Clsss split: got " + classes.size() );
   for ( auto & a : classes ) {
      EM_LOG_ERROR(" - Class = " + a.size() );
   }*/

   // 2.2 - Check each class, form dead pixel set in toProcess set
   for ( auto & c : classes ) 
   {
      bool fall_all = true;
      for (auto & pix : c) 
      {
         if (pix->CheckCanFall(context) == false ) 
         {
            fall_all = false;
         }
      }

      if (fall_all) 
      {
         toProcess.insert(c.begin(), c.end());
      }
   }


   // 2.3 - Finally kill those fallen
   if ( toProcess.empty() == false ) {
      lost = true;

      contents.remove_if( 
         [&](shared_ptr<IAssembled> px) {
            if (toProcess.find(px) != end(toProcess)) {
               px->Kill(context, IAlive::State::KILLED_BY_PITTRAP);
               px->RemoveFromAssembly(context);
               context.field->SetPixelState(px, Pixel::State::INACTIVE);
               return true;
            }
            return false;
         }
      );
   }

   return lost ? ErrorCode::RET_YES : ErrorCode::RET_NO;
}

//////////////////////////////////////////////////////////////////////////
// History

ErrorCode MPix::Assembly::InitSnapshots( const Context& context )
{
   contents.InitSnapshots();
   last_dir.InitSnapshots();
   move_number.InitSnapshots();
   return ErrorCode::RET_OK;
}

ErrorCode MPix::Assembly::PopSnapshots( const Context& context, int count /*1*/, bool shake /*false*/ )
{

   if (shake) {
      for (auto p: contents.GetValue()) {
         context.PostEvent(PixelEvent::RESET, p->GetID(), 10);
      }
   }

   contents.PopSnapshots(count);
   last_dir.PopSnapshots(count);
   move_number.PopSnapshots(count);

   return ErrorCode::RET_OK;
}

ErrorCode MPix::Assembly::PushSnapshot()
{
   contents.PushSnapshot();
   last_dir.PushSnapshot();
   move_number.PushSnapshot();
   return ErrorCode::RET_OK;
}

//////////////////////////////////////////////////////////////////////////

MPix::Rectangle MPix::Assembly::GetContentsRect() const
{
   auto fr = contents.GetValue().front();
   auto pp = (*contents.GetValue().begin())->GetPos();
   int u = pp.y; int d = pp.y;
   int r = pp.x; int l = pp.x;

   // Searching boundary
   for (auto pix : contents.GetValue()) {
      auto pos = pix->GetPos();
      u = MAX(u, pos.y);
      r = MAX(r, pos.x);
      d = MIN(d, pos.y);
      l = MIN(l, pos.x);
   }
   Rectangle fab;
   fab.BL = Coordinates(l,d);
   fab.TR = Coordinates(r,u);
   return fab;
}

std::forward_list<std::pair<int, Coordinates>> & MPix::Assembly::GetAssemblyPositions() const
{
   lst_coords.clear();
   for (auto i : contents.GetValue()) {
      lst_coords.push_front(std::make_pair(i->GetID(), i->GetPos()));
   }
   return lst_coords;
}

Point MPix::Assembly::GetCenter()
{
   UpdateCenter(); // For now no caching
   return center;
}

void MPix::Assembly::UpdateCenter()
{

   if (contents.GetValue().empty()) { 
      center = Vector2::ZERO;
      return;
   }

   float xes = 0; float yes = 0; int l = 0;
   for (auto px : contents.GetValue()) {
      auto p = px->GetPos();
      xes += p.x;
      yes += p.y;
      l++;
   }

   center = Vector2(xes/l, yes/l);
}

bool MPix::Assembly::IsEmpty()
{
   return contents.GetValue().empty();
}

bool MPix::Assembly::AreAllSmiling()
{
   if (IsEmpty()) 
      return false;

   for (auto p : contents.GetValue()) {
      if (p->IsSmiling() == false) {
         return false;
      }
   }
   return true;
}





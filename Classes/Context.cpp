#include "Context.h"


#include "Field.h"
#include "Assembly.h"
#include "Goal.h"
#include "EventProcessor.h"


#include "Pixel.h"
#include "IAssembled.h"

using namespace MPix;

MPix::Context::Context()
{
}

//////////////////////////////////////////////////////////////////////////
// Getters part

shared_ptr<IMagnetic> MPix::Context::GetTopMagenticAt( Coordinates pos ) const
{
   assert(field);
   return field->GetTopMagenticAt(pos);
}

template<class T>
bool MPix::Context::GetPixelsAt( forward_list<shared_ptr<T>> &list, Coordinates pos ) const
{
   return field->GetPixelsAt( list, pos);
}

shared_ptr<Pixel> MPix::Context::GetByID( int id ) const
{
   assert(field);
   return field->GetPixelByID(id);

}

//////////////////////////////////////////////////////////////////////////
// Events part

void MPix::Context::PostEvent( PixelEvent e, int id, int priority /*= 0*/ ) const
{
   pixel_events->PostEvent(make_shared<PixelEventPacket>(e, id, priority));
}

void MPix::Context::PostEvent( PixelEvent e, shared_ptr<Pixel> p, int priority /*= 0*/ ) const
{
   pixel_events->PostEvent(make_shared<PixelEventPacket>(e, p->GetID(), priority));
}

void MPix::Context::PostEvent( GoalEvent e, int id, Coordinates pos, int priority /*= 0*/ ) const
{
   pixel_events->PostEvent(make_shared<GoalEventPacket>(e, id, pos, priority));
}

//////////////////////////////////////////////////////////////////////////
// Assembly part

Point MPix::Context::GetAssemblyCenter() const
{
   assert(assembly);
   return assembly->GetCenter();
}

ErrorCode Context::AddToAssembly( shared_ptr<Pixel> pix ) const
{
   auto ia_pix = std::dynamic_pointer_cast<IAssembled>(pix);
   assert(ia_pix); 
   return assembly->AddPixel(*this, ia_pix);
}

std::forward_list<std::pair<int, Coordinates>> & MPix::Context::GetAssemblyPositions() const
{
   assert(assembly);
   return assembly->GetAssemblyPositions();
}

bool MPix::Context::IsAssemblySmiling() const
{
   assert(assembly);
   return assembly->AreAllSmiling();
}

//////////////////////////////////////////////////////////////////////////
// Goals part

bool MPix::Context::ExistsMatchingGoal( Coordinates pos, PixelColor color ) const
{
   return goals->ExistsMatchingGoal(pos, color);
}

bool MPix::Context::IsAssemblyEmpty() const
{
   return assembly->IsEmpty();
}

shared_ptr<IColorful> MPix::Context::GetColorfulAt( Coordinates pos ) const
{
   auto pix = field->GetTopAssembleAt(pos);
   if (pix) {
      if (pix->IsInAssembly()) {
         return pix;
      } else {
         return nullptr;
      }
   } else {
      auto pix = field->GetTopColorful(pos);
      if (pix) {
         return pix;
      } else {
         return nullptr;
      }
   }
   return nullptr;
}




//////////////////////////////////////////////////////////////////////////
// Stupid C++ j
// 1) move templlate impl. to header -- Fuck placing implementation in header! I don't want Field.h dependency in header...
// 2) Monkey copy paste this.. well at least works
template bool MPix::Context::GetPixelsAt( forward_list<shared_ptr<IKilling>> &list, Coordinates pos ) const;
template bool MPix::Context::GetPixelsAt( forward_list<shared_ptr<IMoveBlocker>> &list, Coordinates pos ) const;
template bool MPix::Context::GetPixelsAt( forward_list<shared_ptr<IProtector>> &list, Coordinates pos ) const;
template bool MPix::Context::GetPixelsAt( forward_list<shared_ptr<IAssembled>> &list, Coordinates pos ) const;
template bool MPix::Context::GetPixelsAt( forward_list<shared_ptr<IAlive>> &list, Coordinates pos ) const;


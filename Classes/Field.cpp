#include "Field.h"


#include "Pixel.h"
#include "Context.h"

#include "IDynamic.h"
#include "IWallPixel.h"
#include "IMagnetic.h"
#include "IAssembled.h"
#include "IKilling.h"
#include "IMoveBlocker.h"
#include "IColorful.h"

using namespace MPix;

Field::Field()
{
   // Enjoy some premature optimization
   map.reserve(64);
   id_map.reserve(64);
   state_map.reserve(5);
   type_map.reserve(5);
   kind_map.reserve(6);

   // Fill states map
   for (auto st : EnumRanger<Pixel::State>() ) {
      state_map.emplace(st, unordered_set<int>());
   }

   // Fill type map
   for (auto st : EnumRanger<Pixel::PixelType>() ) {
      type_map.emplace(st, unordered_set<int>());
   }

   // Fill with empty kinds
   for (auto k : EnumRanger<PixelKind>() ) {
      kind_map.emplace(k, unordered_set<int>());
   }

}

MPix::Field::Field( const Field& oth ) :
   map(oth.map),
   tags(oth.tags),
   kind_map(oth.kind_map),
   state_map(oth.state_map),
   type_map(oth.type_map)
{
   id_map.reserve(oth.id_map.size());
   for (auto p : oth.id_map) {
      id_map.push_back(p->Dublicate()); // make Own shared_ptr
   }
}

shared_ptr<Field> MPix::Field::Dublicate()
{
   auto fab = new Field(*this);
   return shared_ptr<Field> (fab);
}


Field::~Field()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// General methods

EndlessCatLib::ErrorCode MPix::Field::SendCreateEvents( const Context& context )
{
   for (auto px : id_map ) {
      if (px->GetState() == Pixel::State::ACTIVE) {
         context.PostEvent(PixelEvent::CREATED, px->GetID());
      }
   }
   return ErrorCode::RET_OK;
}

ErrorCode MPix::Field::WorldAfterPlayersMove( const Context& context, bool blocked )
{
   auto & dyn_ids = kind_map[PixelKind::DYNAMIC];

   for (auto id : dyn_ids) {
      auto p = GetActivePixelByID(id);
      if (p) {
         dynamic_pointer_cast<IDynamic>(p)->preparePrelude(context, blocked);
      }
   }

   for (auto id : dyn_ids) {
      auto p = GetActivePixelByID(id);
      if (p) {
         dynamic_pointer_cast<IDynamic>(p)->preparePostum(context, blocked);
      }
   }

   return ErrorCode::RET_OK;
}

ErrorCode MPix::Field::WorldMove( const Context& context )
{

   auto & dyn_ids = kind_map[PixelKind::DYNAMIC];

   // Prelude:
   // Cactuses make their decisions
   for (auto id : dyn_ids) {
      auto p = GetActivePixelByID(id);
      if (p) {
         dynamic_pointer_cast<IDynamic>(p)->updatePrelude(context);
      }
   }

   // Axtion:
   // Cactuses move here
   for (auto id : dyn_ids) {
      auto p = GetActivePixelByID(id);
      if (p) {
         dynamic_pointer_cast<IDynamic>(p)->updateAction(context);
      }
   }

   // Finalize:
   // Bombs explode here
   for (auto id : dyn_ids) {
      auto p = GetActivePixelByID(id);
      if (p) {
         dynamic_pointer_cast<IDynamic>(p)->updateFinalize(context);
      }
   }

   return ErrorCode::RET_OK;
}

EndlessCatLib::ErrorCode MPix::Field::WorldCheckForLost( const Context& context )
{
   // Prepare list
   auto & world_alive = kind_map[PixelKind::ALIVE];
   auto & world_killers = kind_map[PixelKind::KILLER];
   auto & active = state_map[Pixel::State::ACTIVE];

   // Take killers
   auto ranked_k_list = std::list<int>(world_killers.begin(), world_killers.end());

   // Only alive one
   ranked_k_list.remove_if(
      [=](const int& v){ 
         bool not_alive = world_alive.find(v) == world_alive.end();
         bool not_active = active.find(v) == active.end();
         return not_active || not_alive;
      });

   // Sort by rank
   ranked_k_list.sort([this](const int& l, const int& r) {
      auto pl = dynamic_pointer_cast<IKilling>(GetActivePixelByID(l));
      auto pr = dynamic_pointer_cast<IKilling>(GetActivePixelByID(r));
      return pl->getKillerRank() > pr->getKillerRank();
   });

   // First check killers(cactus actually)
   for (auto id : ranked_k_list) {
      auto p = GetActivePixelByID(id);
      if (p) {

         auto as = std::dynamic_pointer_cast<IKilling>(p);
         if (!as)
            continue; // skip anything except killers

         auto alv = std::dynamic_pointer_cast<IAlive> (p);
         if (alv->canLive(context) == false) {
            context.PostEvent(PixelEvent::LOST_FROM_WORLD, p);
            SetPixelState(alv, Pixel::State::INACTIVE);
         }
      }
   }

   // Then check others
   for (auto id : world_alive) {
      auto p = GetActivePixelByID(id);
      if (p) {

         auto as = std::dynamic_pointer_cast<IAssembled>(p);
         if (as && as->IsInAssembly())
            continue; // assembly is not my business

         auto alv = std::dynamic_pointer_cast<IAlive> (p);
         if (!alv->canLive(context)) {
            context.PostEvent(PixelEvent::LOST_FROM_WORLD, p);
            SetPixelState(alv, Pixel::State::INACTIVE);
         }
      }
   }

   return ErrorCode::RET_OK;
}


///////// informers /////////////////////////////////////////////////////////////////

bool Field::IsEmpty() const
{
   return id_map.empty();
}

/////////// Pixel creators ///////////////////////////////////////////////////////////////


ErrorCode MPix::Field::InsertPixel( shared_ptr<Pixel> p )
{
   assert(p);

   // Assign ID
   auto id = (int)id_map.size() + 1;
   p->SetID(id);

   // Put to list
   id_map.push_back(p);

   // Put to map
   PlacePosInMap(id, p->GetPos());

   // State map
   state_map[p->GetState()].emplace(id);

   // Type map
   type_map[p->GetType()].emplace(id);

   // Tag map
   if (p->GetTag() != -1)
      tags.emplace(p->GetTag(), id);

   // Kind map

   if (dynamic_pointer_cast<IDynamic>(p))     kind_map[PixelKind::DYNAMIC].emplace(id);
   if (dynamic_pointer_cast<IAlive>(p))       kind_map[PixelKind::ALIVE].emplace(id);
   if (dynamic_pointer_cast<IMoveBlocker>(p)) kind_map[PixelKind::MOVE_BLOCKER].emplace(id);
   if (dynamic_pointer_cast<IKilling>(p))     kind_map[PixelKind::KILLER].emplace(id);

   return ErrorCode::RET_OK;
}


ErrorCode Field::InsertPixel( Pixel* p, Coordinates pos)
{
   assert (p);
   p->SetPos(pos);
   return InsertPixel(shared_ptr<Pixel>(p));
}

EndlessCatLib::ErrorCode MPix::Field::InsertPixel( Pixel* p, int x, int y )
{
   return InsertPixel(p, Coordinates(x,y));
}

//////// pixel getters //////////////////////////////////////////////////////////////////

shared_ptr<Pixel> Field::GetPixelByID( int id )
{
   assert (id > 0 && id <= (int)id_map.size() );
   return id_map[id-1];
}

shared_ptr<Pixel> MPix::Field::GetActivePixelByID( int id )
{
   auto p = GetPixelByID(id);
   return  p->GetState()==Pixel::State::ACTIVE ? p : nullptr;
}


shared_ptr<Pixel> Field::GetPixelAt( int x, int y )
{
   return GetPixelAt(Coordinates(x,y));
}

shared_ptr<Pixel> Field::GetPixelAt( Coordinates pos )
{
   auto pair = map.find(pos);
   if (pair == map.end()) return nullptr;

   auto & lst = pair->second; // If list exists means it has at least one

   assert(lst.empty() == false);


   auto px = GetPixelByID(lst.front());

   return px;
}

shared_ptr<Pixel> Field::GetActivePixelAt( int x, int y )
{
   return GetActivePixelAt(Coordinates(x,y));
}

shared_ptr<Pixel> Field::GetActivePixelAt( Coordinates pos )
{
   auto pair = map.find(pos);
   if (pair == map.end()) return nullptr;

   auto lst = pair->second; // If list exists means it has at least one

   assert(lst.empty() == false);

   for (auto pi : lst ) {
      auto p = GetPixelByID(pi);
      if (p->GetState() == Pixel::State::ACTIVE) {
         return p;
      }
   }

   return nullptr;
}

list<shared_ptr<Pixel>> Field::GetAllPixelsAt( int x, int y )
{
   return GetAllPixelsAt(Coordinates(x,y));
}

list<shared_ptr<Pixel>> Field::GetAllPixelsAt( Coordinates pos )
{
   list<shared_ptr<Pixel>> result;

   auto pair = map.find(pos);
   if (pair == map.end()) return result;

   auto lst = pair->second; 

   assert(lst.empty() == false);

   for (auto pi : lst ) {
      result.push_back(GetPixelByID(pi));
   }

   return result;
}

list<shared_ptr<Pixel>> Field::GetAllActivePixelsAt( int x, int y )
{
   return GetAllActivePixelsAt(Coordinates(x,y));
}

list<shared_ptr<Pixel>> Field::GetAllActivePixelsAt( Coordinates pos )
{
   list<shared_ptr<Pixel>> result;

   auto pair = map.find(pos);
   if (pair == map.end()) return result;

   auto lst = pair->second; 

   assert(lst.empty() == false);

   for (auto pi : lst ) {
      auto px = GetPixelByID(pi);
      if (px->GetState() == Pixel::State::ACTIVE) 
         result.push_back(px);
   }

   return result;
}

shared_ptr<IColorful> MPix::Field::GetTopColorful( Coordinates pos )
{
   return dynamic_pointer_cast<IColorful>(GetActivePixelAt(pos));
}

shared_ptr<IMagnetic> MPix::Field::GetTopMagenticAt( Coordinates pos )
{
   return dynamic_pointer_cast<IMagnetic>(GetActivePixelAt(pos));
}

shared_ptr<IAssembled> MPix::Field::GetTopAssembleAt( Coordinates pos )
{
   return dynamic_pointer_cast<IAssembled>(GetActivePixelAt(pos));
}

shared_ptr<IKilling> MPix::Field::GetTopKillingAt( Coordinates pos )
{
   return dynamic_pointer_cast<IKilling>(GetActivePixelAt(pos));
}


template<class T>
bool MPix::Field::GetPixelsAt( forward_list<shared_ptr<T>> &list, Coordinates pos )
{
   bool found = false;
   auto stack = GetAllActivePixelsAt(pos);

   for (auto px : stack) {
      auto pix = dynamic_pointer_cast<T> (px);
      if (pix) {
         list.push_front(pix);
         found = true;
      }
   }
   return found;
}


//////////////////////////////////////////////////////////////////////////
// Undo / Redo system

ErrorCode MPix::Field::InitSnapshots( const Context& context )
{
   for (auto p : this->id_map) {
      p->InitSnapshots(context);
   }
   return ErrorCode::RET_OK;
}

ErrorCode Field::PushSnapshot(const Context& context)
{
   for (auto p : this->id_map) {
      p->PushSnapshot(context);
   }
   return ErrorCode::RET_OK;
}

ErrorCode MPix::Field::PopSnapshots( const Context& context, int n )
{
   for (auto p : this->id_map) {

      auto pwas = p->GetPos();
      auto swas = p->GetState();

      p->PopSnapshots(context, n);

      auto pnow = p->GetPos();
      auto snow = p->GetState();
      auto id = p->GetID();

      if (pnow != pwas) {
         UpdatePosInMap(id, pwas, pnow);
      }
      if (snow != swas) {
         UpdateStateMap(swas, snow, id);
      }
   }
   return ErrorCode::RET_OK;
}

ErrorCode Field::ClearSnapshots( const Context& context )
{
   for (auto p : this->id_map) {
      p->ClearSnapshots(context);
   }
   return ErrorCode::RET_OK;
}

//////////////////////////////////////////////////////////////////////////
// Manipulations

ErrorCode Field::MovePixelByID( int id, Coordinates pos )
{
   auto px = GetPixelByID(id);
   assert(px);
   return MovePixel(px, pos);
}

ErrorCode Field::MovePixel( shared_ptr<Pixel> px, Coordinates pos )
{
   assert(px);

   // If not really moving
   if (px->GetPos() == pos) return ErrorCode::RET_OK;

   UpdatePosInMap(px->GetID(), px->GetPos(), pos);
   px->SetPos(pos);

   // TODO: event?

   return ErrorCode::RET_OK;
}

ErrorCode Field::MovePixel( shared_ptr<Pixel> px, Direction d )
{
   assert(px);
   auto p = px->GetPos() + d; // Perform move to d
   return MovePixel(px, p);
}

ErrorCode Field::SetPixelState( int id, Pixel::State st )
{
   auto px = GetPixelByID(id);
   assert(px);
   return SetPixelState(px, st);
}

ErrorCode Field::SetPixelState( shared_ptr<Pixel> px, Pixel::State st )
{
   int id = px->GetID();
   state_map[px->GetState()].erase(id);
   px->SetState(st);
   state_map[st].emplace(id);
   return ErrorCode::RET_OK;
}

void MPix::Field::UpdatePosInMap( int id, Coordinates was, Coordinates now )
{
   RemovePosFormMap(id, was);
   PlacePosInMap(id, now);
}

void MPix::Field::PlacePosInMap( int id, Coordinates pos )
{
   // Put id to pos map
   auto lst = map.find(pos);
   if ( lst == map.end()) {
      std::forward_list<int> l;
      l.push_front(id);
      map.emplace(pos, l);
   } else {
      lst->second.push_front(id);
      lst->second.sort(
         [&](const int & a, const int & b)
         { 
            return GetPixelByID(a) < GetPixelByID(b);
         }
      );
   }
}

void MPix::Field::RemovePosFormMap( int id, Coordinates pos )
{
   auto pair = map.find(pos);
   assert ( pair != map.end());
   auto & lst = pair->second;

   // Check that actually id is there for more error prone
   assert(std::find(lst.begin(), lst.end(), id) != lst.end());

   // Remove id from list
   lst.remove_if([&](int x){ return x == id; });

   // If removed last one
   if (lst.empty()) {
      map.erase(pos);
   }

}


void MPix::Field::UpdateStateMap( Pixel::State was, Pixel::State now, int id )
{
   state_map[was].erase(id);
   state_map[now].emplace(id);
}

shared_ptr<Pixel> MPix::Field::GetPixelByTag(int tag)
{
   auto it = tags.find(tag);
   if (it != tags.end())
      return GetPixelByID(it->second);
   else
      return nullptr;
}



//////////////////////////////////////////////////////////////////////////

template bool MPix::Field::GetPixelsAt( forward_list<shared_ptr<IKilling>> &list, Coordinates pos ) ;
template bool MPix::Field::GetPixelsAt( forward_list<shared_ptr<IMoveBlocker>> &list, Coordinates pos ) ;
template bool MPix::Field::GetPixelsAt( forward_list<shared_ptr<IProtector>> &list, Coordinates pos ) ;
template bool MPix::Field::GetPixelsAt( forward_list<shared_ptr<IAlive>> &list, Coordinates pos ) ;
template bool MPix::Field::GetPixelsAt( forward_list<shared_ptr<IAssembled>> &list, Coordinates pos ) ;


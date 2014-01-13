#include "Pixel.h"
#include "HistoryManager.h"
#include "Context.h"

using namespace MPix;


Pixel::Pixel(int Z)
{
   // Setting invalid ID to standalone pixel
   id = -1;
   st = State::ACTIVE;
   pos = Coordinates(0, 0);
   z = Z;
}

Pixel::~Pixel()
{
}

void MPix::Pixel::InitSnapshots( const Context& context )
{
   pos.InitSnapshots();
   st.InitSnapshots();
}

void MPix::Pixel::PushSnapshot( const Context& context )
{
   pos.PushSnapshot();
   st.PushSnapshot();
}

void MPix::Pixel::ClearSnapshots( const Context& context )
{
   pos.ClearSnapshots();
   st.ClearSnapshots();
}

void MPix::Pixel::PopSnapshots( const Context& context, int n )
{
   Coordinates was = pos;
   pos.PopSnapshots(n);
   st.PopSnapshots(n);
   if (was != pos) {
      context.PostEvent(PixelEvent::UNDO_MOVED, id);
   }
}

shared_ptr<Pixel> MPix::Pixel::Dublicate()
{
   auto fab = static_cast<Pixel*>(this->Clone());
   return shared_ptr<Pixel>(fab);
}

bool MPix::operator<( const shared_ptr<Pixel>& lhs, const shared_ptr<Pixel>& rhs )
{
   return lhs->z < rhs->z;
}



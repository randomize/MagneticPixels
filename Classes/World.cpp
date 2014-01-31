#include "World.h"

using namespace MPix;

MPix::World::World( int id ) :
   id(id),
   name("unnamed")
{
}

MPix::World::World( int id, const char* name ) :
   id(id),
   name(name)
{
}

void MPix::World::AddLevel( unsigned lvl_id )
{
   levels.remove(lvl_id);
   levels.push_back(lvl_id);
}

void MPix::World::EraseLevel( unsigned lvl_id )
{
   levels.remove(lvl_id);
}

const string& MPix::World::GetName()
{
   return name;
}

int MPix::World::GetID()
{
   return id;
}

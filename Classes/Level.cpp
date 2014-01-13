#include "Level.h"

#include "Field.h"
#include "Goal.h"
#include "Solution.h"

using namespace MPix;

MPix::Level::Level()
{
   field = make_shared<Field>();
   goals = make_shared<Goals>();
   solution = make_shared<Solution>();
   auto_pan = true;
   viewport.BL = Coordinates(-4,-6);
   viewport.TR = Coordinates(3,5);
   world = 0;
   starz = 0;

   // Assign a non-null random ID
   do {
   levelID = (unsigned)(rand());
   } while (levelID == 0); // For that happy case

   st = State::IS_OPEN;
   name = "Noname";
}

MPix::Level::~Level()
{
}

shared_ptr<Level> MPix::Level::Dublicate()
{
   assert(st != State::IS_PLAYING); // NEVER copy a loaded in GameplayManager level
   if (st == State::IS_PLAYING) return nullptr; // JIC

   Level* fab = new Level();

   fab->field = field->Dublicate();
   fab->goals = goals->Dublicate();
   fab->solution = solution->Dublicate();
   fab->auto_pan = auto_pan;
   fab->viewport = viewport;
   fab->starz = starz;
   fab->world = world;
   fab->levelID = levelID;
   fab->st = st;
   fab->name = name;

   return shared_ptr<Level>(fab);
}



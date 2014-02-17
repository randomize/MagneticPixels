
#include "Script.h"

using namespace MPix;

//====---------------------------------------------======//


MPix::Script::Script(const string& name):
 m_name(name)
{
   on_first_move = [](const Context&){};
   on_clicked = [](const Context&,Coordinates){return true; };
   on_moved = [](const Context&,Direction){return true; };
   on_last_grow = [](const Context&){};
}

MPix::Script::~Script()
{

}

void MPix::Script::OnFirstMove(const Context& context)
{
   on_first_move(context);
}

bool MPix::Script::OnPlayerClicked(const Context& context, Coordinates where)
{
   return on_clicked(context, where);
}

bool MPix::Script::OnPlayerMove(const Context& context, Direction where)
{
   return on_moved(context, where);
}

void MPix::Script::OnLastGrow(const Context& context)
{
   on_last_grow(context);
}

const string& MPix::Script::GetName() const
{
   return m_name;
}

#include "IColorful.h"
#include "Context.h"

using namespace MPix;

//====---------------------------------------------======//

MPix::IColorful::IColorful( PixelColor color /*= PixelColor::RED*/ ) :
  color(color)
{
   smile = false;
   accepted_by_goal = false;
}


void MPix::IColorful::SetSmile( const Context & cont, bool is_smiling )
{
   DiffSmile(cont, smile, is_smiling);
   smile = is_smiling;
}

bool MPix::IColorful::IsSmiling() const
{
   return smile;
}

MPix::PixelColor MPix::IColorful::GetColor() const
{
   return color;
}

string MPix::IColorful::GetColorAsString() const
{
   return PixelColorToStr(color);
}

void MPix::IColorful::Accept( const Context& context )
{
   accepted_by_goal = true;
   context.PostEvent(PixelEvent::ACCEPTED_GOAL, GetID());
}

void MPix::IColorful::PopSnapshots( const Context& context, int n )
{
   bool is_smiling = smile;
   smile.PopSnapshots(n);
   DiffSmile(context, is_smiling, smile);

   bool accepted_by_goal_was = accepted_by_goal;
   accepted_by_goal.PopSnapshots(n);
   if (accepted_by_goal_was && !accepted_by_goal )
      context.PostEvent(PixelEvent::UNDO_ACCEPTED_GOAL, GetID());

}

void MPix::IColorful::ClearSnapshots( const Context& context )
{
   smile.ClearSnapshots();
   accepted_by_goal.ClearSnapshots();
}

void MPix::IColorful::PushSnapshot( const Context& context )
{
   smile.PushSnapshot();
   accepted_by_goal.PushSnapshot();
}

void MPix::IColorful::InitSnapshots( const Context& context )
{
   smile.InitSnapshots();
   accepted_by_goal.InitSnapshots();
}

void MPix::IColorful::DiffSmile( const Context& context, bool from, bool to )
{
   if ( from != to ) {
      if (from) {
         context.PostEvent(PixelEvent::UNSMILED, GetID());
      } else {
         context.PostEvent(PixelEvent::SMILED, GetID());
      }
   }
}

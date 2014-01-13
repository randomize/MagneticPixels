#include "View.h"

using namespace MPix;

void MPix::View::setVisible( bool visibility )
{
   assert(contents);
   contents->setVisible(visibility);
}

void MPix::View::setPosition( Point pos )
{
   assert(contents);
   contents->setPosition(pos);
}

Node* MPix::View::GetContents()
{
   assert(contents); // Must call Build first
   return contents;
}

void MPix::View::BindContents( Node* target, int z )
{
   assert(target);
   assert(contents);
   this->target = target;
   this->z_order = z;
   target->addChild(contents, z);
}

void MPix::View::BindContents( Node* target )
{
   BindContents(target, z_order);
}

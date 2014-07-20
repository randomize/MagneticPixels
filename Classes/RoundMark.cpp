#include "RoundMark.h"

using namespace MPix;

//====---------------------------------------------======//

RoundMark* MPix::RoundMark::create( const char* text )
{
   auto fab = new RoundMark();
   fab->autorelease();
   const int RES = 4;
   auto digit = Label::createWithTTF(text, "Arial", 24.0f * RES,
                              Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
   digit->setScale(1.0f/RES);
   //fab->digit->setColor(Color3B(255,255,0));
   fab->addChild(digit, 1);
   digit->setCascadeOpacityEnabled(true);

   auto d = DrawNode::create();
   const int segs = 16;
   const float rad = MPIX_CELL_SIZE_HALF * 0.2f;

   Vector2* verts = new Vector2[segs];
   float drads = 2.0f * float(M_PI) / segs;
   for ( int k = 0; k < segs; ++k) {
      verts[k] = Vector2(cosf(k*drads)*rad, sinf(k*drads)*rad);
   }

   d->drawPolygon(verts, segs, Color4F::RED, 0.0f, Color4F());
   delete[] verts;

   fab->addChild(d);
   return fab;

}


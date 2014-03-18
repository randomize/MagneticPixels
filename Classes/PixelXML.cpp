#include "PixelXML.h"
#include "Pixel.h"

#include "MagneticPixel.h"
#include "CactusDynamic.h"
#include "CactusStatic.h"
#include "WallPixel.h"
#include "Pitfall.h"
#include "SokobanPixel.h"
#include "StonePixel.h"
#include "MutantPixel.h"
#include "BomberPixel.h"

using namespace MPix;
using namespace tinyxml2;


// ======================= Two general dispatcher methods ==================


shared_ptr<Pixel> MPix::PixelXML::Generate( tinyxml2::XMLElement* src )
{
   const char* t = src->Name();

   if (strcmp(t,"w") == 0) {
      return GenWallPixelFromXML(src);
   }

   if (strcmp(t,"c") == 0) {
      return GenDynamicCactusFromXML(src);
   }

   if (strcmp(t,"C") == 0) {
      return GenStaticCactusFromXML(src);
   }

   if (strcmp(t,"m") == 0) {
      return GenMagneticPixelFromXML(src);
   }

   if (strcmp(t,"p") == 0) {
      return GenPitfallFromXML(src);
   }

   if (strcmp(t,"s") == 0) {
      return GenSokobanPixelFromXML(src);
   }

   if (strcmp(t,"S") == 0) {
      return GenStonePixelFromXML(src);
   }

   if (strcmp(t,"M") == 0) {
      return GenMutantFromXML(src);
   }

   if (strcmp(t,"b") == 0) {
      return GenBomberFromXML(src);
   }

   EM_LOG_ERROR("Pixel loading from XML failed, unknown type =`" + t + "`");
   return nullptr;

}

bool MPix::PixelXML::Store( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst )
{

   auto t = src->GetTypeID();

   if (t == WallPixel::GetClassTypeID()) {
      return StoreWallPixelToXML(src, dst);
   }

   if (t == MagneticPixel::GetClassTypeID()) {
      return StoreMagneticPixelToXML(src, dst);
   }

   if (t == CactusDynamic::GetClassTypeID()) {
      return StoreDynamicCactusToXML(src, dst);
   }

   if (t == CactusStatic::GetClassTypeID()) {
      return StoreStaticCactusToXML(src, dst);
   }

   if (t == Pitfall::GetClassTypeID()) {
      return StorePitfallToXML(src, dst);
   }

   if (t == SokobanPixel::GetClassTypeID()) {
      return StoreSokobanPixelToXML(src, dst);
   }

   if (t == StonePixel::GetClassTypeID()) {
      return StoreStonePixelToXML(src, dst);
   }

   if (t == BomberPixel::GetClassTypeID()) {
      return StoreBomberToXML(src, dst);
   }

   if (t == MutantPixel::GetClassTypeID()) {
      return StoreMutantToXML(src, dst);
   }

   EM_LOG_ERROR("Unknown pixel type " + t);
   return false;

}


// ======================= Fuck polymorphism : For here usage only special methods ==================

void PushCommon(shared_ptr<Pixel> src, tinyxml2::XMLPrinter* dst) {
   dst->PushAttribute("x", src->GetPos().x);
   dst->PushAttribute("y", src->GetPos().y);
   if (src->GetTag() != -1) {
      dst->PushAttribute("g", src->GetTag());
   }
}


bool MPix::PixelXML::StoreMagneticPixelToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst)
{
   auto pix = dynamic_pointer_cast<MagneticPixel>(src);
   dst->OpenElement("m");
   PushCommon(src, dst);
   dst->PushAttribute("c", static_cast<int>(pix->GetColor()));
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreWallPixelToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst)
{
   auto pix = dynamic_pointer_cast<WallPixel>(src);
   dst->OpenElement("w");
   PushCommon(src, dst);
   dst->PushAttribute("s", pix->GetShapeInt());
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreDynamicCactusToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst)
{
   auto pix = dynamic_pointer_cast<CactusDynamic>(src);
   dst->OpenElement("c");
   PushCommon(src, dst);
   dst->PushAttribute("t", static_cast<int>(pix->GetNeedleType()));
   dst->PushAttribute("d", static_cast<int>(pix->GetWay()));
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreStaticCactusToXML( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst )
{
   auto pix = dynamic_pointer_cast<CactusStatic>(src);
   dst->OpenElement("C");
   PushCommon(src, dst);
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StorePitfallToXML( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst )
{
   auto pix = dynamic_pointer_cast<Pitfall>(src);
   dst->OpenElement("p");
   PushCommon(src, dst);
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreSokobanPixelToXML( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst )
{
   auto pix = dynamic_pointer_cast<SokobanPixel>(src);
   dst->OpenElement("s");
   PushCommon(src, dst);
   dst->PushAttribute("c", static_cast<int>(pix->GetColor()));
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreStonePixelToXML( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst )
{
   auto pix = dynamic_pointer_cast<StonePixel>(src);
   dst->OpenElement("S");
   PushCommon(src, dst);
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreBomberToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst)
{
   auto pix = dynamic_pointer_cast<BomberPixel>(src);
   dst->OpenElement("b");
   PushCommon(src, dst);
   dst->PushAttribute("c", static_cast<int>(pix->GetColor()));
   dst->PushAttribute("t", static_cast<int>(pix->GetTime()));
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreMutantToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst)
{
   auto pix = dynamic_pointer_cast<MutantPixel>(src);
   dst->OpenElement("M");
   PushCommon(src, dst);
   auto& cc = pix->GetColors();
   size_t s = cc.size();
   if (s <= 0 || s >= 20) return false;
   string fab;
   fab.resize(s, ' ');
   for (size_t i = 0; i < s; i++) {
      fab[i] = '0' + static_cast<int>(cc[i]);
   }
   dst->PushAttribute("c", fab.c_str());
   dst->CloseElement();
   return true;
}


// ======================= Fuck polymorphism : For here usage only special methods ==================


void QueryCommon(Pixel& px, tinyxml2::XMLElement* src) {

   int x; 
   
   if (src->QueryIntAttribute("x", &x) != XML_NO_ERROR) {
      EM_LOG_ERROR("Coordinateless pixel detected ");
   }

   int y; 
   
   if (src->QueryIntAttribute("y", &y) != XML_NO_ERROR) {
      EM_LOG_ERROR("Coordinateless pixel detected ");
   }

   int t; 
   
   if (src->QueryIntAttribute("g", &t) == XML_NO_ERROR) {
      px.SetTag(t);
   }

   px.SetPos(Coordinates(x,y));

}

shared_ptr<Pixel> MPix::PixelXML::GenMagneticPixelFromXML( tinyxml2::XMLElement* src )
{
   int c; if (src->QueryIntAttribute("c", &c) != XML_NO_ERROR) return nullptr;
   auto px = make_shared<MagneticPixel>( static_cast<PixelColor>(c) );
   QueryCommon(*px, src);
   return px;
}

shared_ptr<Pixel> MPix::PixelXML::GenWallPixelFromXML( tinyxml2::XMLElement* src )
{

   int s; if (src->QueryIntAttribute("s", &s) != XML_NO_ERROR) return nullptr;

   auto px = make_shared<WallPixel>(s);
   QueryCommon(*px, src);
   return px;

}

shared_ptr<Pixel> MPix::PixelXML::GenDynamicCactusFromXML( tinyxml2::XMLElement* src )
{

   int t; if (src->QueryIntAttribute("t", &t) != XML_NO_ERROR) return nullptr;
   int d; if (src->QueryIntAttribute("d", &d) != XML_NO_ERROR) return nullptr;

   auto px = make_shared<CactusDynamic>(static_cast<CactusDynamic::NeedleType>(t), static_cast<Direction>(d));
   QueryCommon(*px, src);
   return px;

}

shared_ptr<Pixel> MPix::PixelXML::GenStaticCactusFromXML( tinyxml2::XMLElement* src )
{
   auto px = make_shared<CactusStatic>();
   QueryCommon(*px, src);
   return px;
}

shared_ptr<Pixel> MPix::PixelXML::GenPitfallFromXML( tinyxml2::XMLElement* src )
{
   auto px = make_shared<Pitfall>();
   QueryCommon(*px, src);
   return px;
}

shared_ptr<Pixel> MPix::PixelXML::GenSokobanPixelFromXML( tinyxml2::XMLElement* src )
{
   int c; if (src->QueryIntAttribute("c", &c) != XML_NO_ERROR) return nullptr;
   auto px = make_shared<SokobanPixel>( static_cast<PixelColor>(c) );
   QueryCommon(*px, src);
   return px;
}

shared_ptr<Pixel> MPix::PixelXML::GenStonePixelFromXML( tinyxml2::XMLElement* src )
{
   auto px = make_shared<StonePixel>();
   QueryCommon(*px, src);
   return px;
}

shared_ptr<Pixel> MPix::PixelXML::GenBomberFromXML(tinyxml2::XMLElement* src)
{
   int c; if (src->QueryIntAttribute("c", &c) != XML_NO_ERROR) return nullptr;
   int t; if (src->QueryIntAttribute("t", &t) != XML_NO_ERROR) return nullptr;
   auto px = make_shared<BomberPixel>( static_cast<PixelColor>(c), t );
   QueryCommon(*px, src);
   return px;
}

shared_ptr<Pixel> MPix::PixelXML::GenMutantFromXML(tinyxml2::XMLElement* src)
{
   auto c = src->Attribute("c"); if (c==nullptr) return nullptr;
   vector<PixelColor> vv;
   while (*c != 0) {
      char b = *c - '0';
      assert(b >= 0 && b <= 9);
      vv.push_back(static_cast<PixelColor>(b));
      c++;
   }
   auto px = make_shared<MutantPixel>(vv);
   QueryCommon(*px, src);
   return px;
}








#include "PixelXML.h"
#include "Pixel.h"

#include "MagneticPixel.h"
#include "CactusDynamic.h"
#include "CactusStatic.h"
#include "WallPixel.h"
#include "Pitfall.h"
#include "SokobanPixel.h"
#include "StonePixel.h"

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

   EM_LOG_ERROR("Unknown pixel type " + t);
   return false;

}


// ======================= Fuck polymorphism : For here usage only special methods ==================


bool MPix::PixelXML::StoreMagneticPixelToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst)
{
   auto pix = dynamic_pointer_cast<MagneticPixel>(src);
   dst->OpenElement("m");
   dst->PushAttribute("x", pix->GetPos().x);
   dst->PushAttribute("y", pix->GetPos().y);
   dst->PushAttribute("c", static_cast<int>(pix->GetColor()));
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreWallPixelToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst)
{
   auto pix = dynamic_pointer_cast<WallPixel>(src);
   dst->OpenElement("w");
   dst->PushAttribute("x", pix->GetPos().x);
   dst->PushAttribute("y", pix->GetPos().y);
   dst->PushAttribute("s", pix->GetShapeInt());
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreDynamicCactusToXML(shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst)
{
   auto pix = dynamic_pointer_cast<CactusDynamic>(src);
   dst->OpenElement("c");
   dst->PushAttribute("x", pix->GetPos().x);
   dst->PushAttribute("y", pix->GetPos().y);
   dst->PushAttribute("t", static_cast<int>(pix->GetNeedleType()));
   dst->PushAttribute("d", static_cast<int>(pix->GetWay()));
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreStaticCactusToXML( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst )
{
   auto pix = dynamic_pointer_cast<CactusStatic>(src);
   dst->OpenElement("C");
   dst->PushAttribute("x", pix->GetPos().x);
   dst->PushAttribute("y", pix->GetPos().y);
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StorePitfallToXML( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst )
{
   auto pix = dynamic_pointer_cast<Pitfall>(src);
   dst->OpenElement("p");
   dst->PushAttribute("x", pix->GetPos().x);
   dst->PushAttribute("y", pix->GetPos().y);
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreSokobanPixelToXML( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst )
{
   auto pix = dynamic_pointer_cast<SokobanPixel>(src);
   dst->OpenElement("s");
   dst->PushAttribute("x", pix->GetPos().x);
   dst->PushAttribute("y", pix->GetPos().y);
   dst->PushAttribute("c", static_cast<int>(pix->GetColor()));
   dst->CloseElement();
   return true;
}

bool MPix::PixelXML::StoreStonePixelToXML( shared_ptr<Pixel> src, tinyxml2::XMLPrinter * dst )
{
   auto pix = dynamic_pointer_cast<StonePixel>(src);
   dst->OpenElement("S");
   dst->PushAttribute("x", pix->GetPos().x);
   dst->PushAttribute("y", pix->GetPos().y);
   dst->CloseElement();
   return true;
}


// ======================= Fuck polymorphism : For here usage only special methods ==================

shared_ptr<Pixel> MPix::PixelXML::GenMagneticPixelFromXML( tinyxml2::XMLElement* src )
{

   int x; if (src->QueryIntAttribute("x", &x) != XML_NO_ERROR) return nullptr;
   int y; if (src->QueryIntAttribute("y", &y) != XML_NO_ERROR) return nullptr;
   int c; if (src->QueryIntAttribute("c", &c) != XML_NO_ERROR) return nullptr;

   auto px = make_shared<MagneticPixel>( static_cast<PixelColor>(c) );
   px->SetPos(Coordinates(x,y));
   return px;

}

shared_ptr<Pixel> MPix::PixelXML::GenWallPixelFromXML( tinyxml2::XMLElement* src )
{

   int x; if (src->QueryIntAttribute("x", &x) != XML_NO_ERROR) return nullptr;
   int y; if (src->QueryIntAttribute("y", &y) != XML_NO_ERROR) return nullptr;
   int s; if (src->QueryIntAttribute("s", &s) != XML_NO_ERROR) return nullptr;

   auto px = make_shared<WallPixel>(s);
   px->SetPos(Coordinates(x,y));
   return px;

}

shared_ptr<Pixel> MPix::PixelXML::GenDynamicCactusFromXML( tinyxml2::XMLElement* src )
{

   int x; if (src->QueryIntAttribute("x", &x) != XML_NO_ERROR) return nullptr;
   int y; if (src->QueryIntAttribute("y", &y) != XML_NO_ERROR) return nullptr;
   int t; if (src->QueryIntAttribute("t", &t) != XML_NO_ERROR) return nullptr;
   int d; if (src->QueryIntAttribute("d", &d) != XML_NO_ERROR) return nullptr;

   auto px = make_shared<CactusDynamic>(static_cast<CactusDynamic::NeedleType>(t), static_cast<Direction>(d));
   px->SetPos(Coordinates(x,y));
   return px;

}

shared_ptr<Pixel> MPix::PixelXML::GenStaticCactusFromXML( tinyxml2::XMLElement* src )
{
   int x; if (src->QueryIntAttribute("x", &x) != XML_NO_ERROR) return nullptr;
   int y; if (src->QueryIntAttribute("y", &y) != XML_NO_ERROR) return nullptr;

   auto px = make_shared<CactusStatic>();
   px->SetPos(Coordinates(x,y));
   return px;
}

shared_ptr<Pixel> MPix::PixelXML::GenPitfallFromXML( tinyxml2::XMLElement* src )
{
   int x; if (src->QueryIntAttribute("x", &x) != XML_NO_ERROR) return nullptr;
   int y; if (src->QueryIntAttribute("y", &y) != XML_NO_ERROR) return nullptr;

   auto px = make_shared<Pitfall>();
   px->SetPos(Coordinates(x,y));
   return px;

}

shared_ptr<Pixel> MPix::PixelXML::GenSokobanPixelFromXML( tinyxml2::XMLElement* src )
{
   int x; if (src->QueryIntAttribute("x", &x) != XML_NO_ERROR) return nullptr;
   int y; if (src->QueryIntAttribute("y", &y) != XML_NO_ERROR) return nullptr;
   int c; if (src->QueryIntAttribute("c", &c) != XML_NO_ERROR) return nullptr;

   auto px = make_shared<SokobanPixel>( static_cast<PixelColor>(c) );
   px->SetPos(Coordinates(x,y));
   return px;

}

shared_ptr<Pixel> MPix::PixelXML::GenStonePixelFromXML( tinyxml2::XMLElement* src )
{
   int x; if (src->QueryIntAttribute("x", &x) != XML_NO_ERROR) return nullptr;
   int y; if (src->QueryIntAttribute("y", &y) != XML_NO_ERROR) return nullptr;

   auto px = make_shared<StonePixel>();
   px->SetPos(Coordinates(x,y));
   return px;
}




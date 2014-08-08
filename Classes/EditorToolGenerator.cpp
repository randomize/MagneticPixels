#include "EditorToolGenerator.h"

#include "EditorTool.h"

#include "MagneticPixel.h"
//#include "MagneticPixelView.h"

#include "WallPixel.h"
#include "CactusDynamic.h"
#include "CactusStatic.h"
#include "Pitfall.h"
#include "SokobanPixel.h"
#include "StonePixel.h"
#include "MutantPixel.h"
#include "BomberPixel.h"

using namespace MPix;

#define PUT_TO(A, B) (A)->Insert(v.size()); v.emplace_back(B)

vector<shared_ptr<EditorTool>> MPix::EditorToolGenerator::GenerateDefaultSet()
{
   vector<shared_ptr<EditorTool>> v;
   EditorTool* fab;

   auto root = new EditorFolderTool("Choose category");
   v.emplace_back(root);

   //////////////////////////////////////////////////////////////////////////
   // Core category

   auto mag = new EditorFolderTool("Magnetic Pixel", "magneticpixel");
   PUT_TO(root, mag);

   auto needl = new EditorFolderTool("Cactus Pixel", "cactusstatic");
   PUT_TO(root, needl);

   auto wall = new EditorFolderTool("Wall Pixel", "wallpixel");
   PUT_TO(root, wall);

   auto goal = new EditorFolderTool("Goal", "Goal");
   PUT_TO(root, goal);


   auto eraser = new EditorFolderTool("Erase");
   PUT_TO(root, eraser);

   auto stn = new EditorFolderTool("Sokoban pixels", "sokobanpixel");
   PUT_TO(root, stn);

   auto msc = new EditorFolderTool("Misc pixels");
   PUT_TO(root, msc);

   //////////////////////////////////////////////////////////////////////////
   // Magnetics category
   for (auto col : EnumRanger<PixelColor>() ) {
      auto pm = make_shared<MagneticPixel>(col);
      fab = new EditorToolPixel((string("Magnetic ")+PixelColorToStr(col)).c_str(), pm);
      PUT_TO(mag, fab);
   }

   //////////////////////////////////////////////////////////////////////////
   // Needle category

   // Static
   fab = new EditorToolPixel("Static", make_shared<CactusStatic>());
   PUT_TO(needl, fab);

   // Dynamics
   for (auto typ : EnumRanger<CactusDynamic::NeedleType>() ) {
      auto name = CactusDynamic::TypeToString(typ);
      auto ndyn = new EditorFolderTool(name, "cactusdynamic");
      PUT_TO(needl, ndyn);
      for (auto d : EnumRanger<Direction>(DirectionType::ALL)) {
         auto pn = make_shared<CactusDynamic>(typ, d);
         fab = new EditorToolPixel((DirectionsToString(d) + string(" ") + name).c_str() , pn);
         PUT_TO(ndyn, fab);
      }
   }

   //////////////////////////////////////////////////////////////////////////
   // Wall category

   for (int i = 1; i < 16; ++i) {
      auto pw = make_shared<WallPixel>(i);
      fab = new EditorToolWall(("Wall " + ToString(i)).c_str(), pw);
      PUT_TO(wall, fab);
   }

   //////////////////////////////////////////////////////////////////////////
   // Goals
   for (auto col : EnumRanger<PixelColor>() ) {
      string name = "Goal ";
      name += PixelColorToStr(col);
      fab = new EditorGoalTool(name.c_str(), col, 0);
      PUT_TO(goal, fab);
   }

   //////////////////////////////////////////////////////////////////////////
   // Erasers
   for (auto t : EnumRanger<EditorToolEraser::Type>() ) {
      auto e = new EditorToolEraser(t);
      PUT_TO(eraser, e);
   }

   //////////////////////////////////////////////////////////////////////////
   // Sokoban pixel category
   for (auto col : EnumRanger<PixelColor>() ) {
      auto pm = make_shared<SokobanPixel>(col);
      fab = new EditorToolPixel((string("Sok ")+PixelColorToStr(col)).c_str(), pm);
      PUT_TO(stn, fab);
   }

   //////////////////////////////////////////////////////////////////////////
   // Misc

   // Pitfall
   auto pf = make_shared<Pitfall>();
   fab = new EditorToolPixel( "Pitfall" , pf);
   PUT_TO(msc, fab);

   // Stone
   auto stp = make_shared<StonePixel>();
   fab = new EditorToolPixel( "Stone" , stp);
   PUT_TO(msc, fab);

   // Mutants
   auto muts = new EditorFolderTool("Mutants", "mutantpixel");
   PUT_TO(msc, muts);

   // Mutant
   auto stm = make_shared<MutantPixel>();
   fab = new EditorToolPixel( "Mutant def" , stm);
   PUT_TO(muts, fab);
   stm = make_shared<MutantPixel>(PixelColor::GREEN,PixelColor::RED); fab = new EditorToolPixel( "Mutant 1" , stm); PUT_TO(muts, fab);
   stm = make_shared<MutantPixel>(PixelColor::YELLOW,PixelColor::BLUE); fab = new EditorToolPixel( "Mutant 1" , stm); PUT_TO(muts, fab);
   stm = make_shared<MutantPixel>(PixelColor::VIOLET,PixelColor::RED); fab = new EditorToolPixel( "Mutant 1" , stm); PUT_TO(muts, fab);
   stm = make_shared<MutantPixel>(PixelColor::CYAN,PixelColor::YELLOW); fab = new EditorToolPixel( "Mutant 1" , stm); PUT_TO(muts, fab);

   // Bombs
   auto boms = new EditorFolderTool("Bombers", "bomberpixel");
   PUT_TO(msc, boms);

   auto stb = make_shared<BomberPixel>();
   fab = new EditorToolPixel( "Bomb 5 r" , stb);
   PUT_TO(boms, fab);
   stb = make_shared<BomberPixel>(PixelColor::YELLOW, 2);
   fab = new EditorToolPixel( "Bomb 2 y" , stb);
   PUT_TO(boms, fab);
   stb = make_shared<BomberPixel>(PixelColor::YELLOW, 8);
   fab = new EditorToolPixel( "Bomb 8 y" , stb);
   PUT_TO(boms, fab);

   return v;

}


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

   auto mag = new EditorFolderTool("Magnetic Pixel", "ed_magnet.png");
   PUT_TO(root, mag);

   auto needl = new EditorFolderTool("Cactus Pixel", "ed_needle.png");
   PUT_TO(root, needl);

   auto wall = new EditorFolderTool("Wall Pixel", "ed_walll.png");
   PUT_TO(root, wall);

   auto goal = new EditorFolderTool("Goal", "ed_goal.png");
   PUT_TO(root, goal);


   auto eraser = new EditorFolderTool("Erase", "ed_erase.png" );
   PUT_TO(root, eraser);

   auto stn = new EditorFolderTool("Sokoban pixels");
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
      auto ndyn = new EditorFolderTool(name, "ed_needle.png");
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

   const int n = 3;
   EditorFolderTool  **goals = new EditorFolderTool*[n];

   for (int k = 0; k < 3; ++k) {
      goals[k] = new EditorFolderTool( ("Additional Goal" + ToString(k+1)).c_str(), "ed_goal.png");
      PUT_TO(goal, goals[k]);
      for (auto col : EnumRanger<PixelColor>() ) {
         string name = "Goal ";
         name += PixelColorToStr(col);
         fab = new EditorGoalTool(name.c_str(), col, k+1);
         PUT_TO(goals[k], fab);
      }
   }
   delete [] goals;

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

   // Pitfall
   auto stp = make_shared<StonePixel>();
   fab = new EditorToolPixel( "Stone" , stp);
   PUT_TO(msc, fab);

   return v;

}


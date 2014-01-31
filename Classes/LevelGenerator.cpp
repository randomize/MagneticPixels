#include "LevelGenerator.h"

// Levels definition
#include "Level.h"

// Levels components
#include "Pixel.h"
#include "Field.h"
#include "Goal.h"

// Editor levels data
#include "EditorData.h"

// This class knows every type of pixel
#include "MagneticPixel.h"
#include "WallPixel.h"
#include "CactusDynamic.h"
#include "CactusStatic.h"
#include "Pitfall.h"
#include "SokobanPixel.h"
#include "StonePixel.h"

using namespace MPix;

LevelGenerator::LevelGenerator() {
   EM_LOG_INFO("[ LevelGenerator initialized ]");
}

WallPixel* GetRandomWall() {
   auto w = new WallPixel( rand()%16 );
   return w;
}

shared_ptr<Level> LevelGenerator::CreateTestLevel()
{
   auto fab = CreateEmpty();
   auto f = fab->field;

   // Make walls perimeter -- square 9x9 centered in [0,0]
   for (int x = -4; x<=3; ++x) {
      f->InsertPixel(GetRandomWall(), x, 5);
      f->InsertPixel(GetRandomWall(), x, -6);
   }
   for (int y = -3; y<=3; ++y) {
      f->InsertPixel(GetRandomWall(), -4, y);
      f->InsertPixel(GetRandomWall(), 3, y);
   }

   // And wall column in center
   f->InsertPixel(GetRandomWall(), 0, 0);

   Pixel* n = new CactusDynamic(CactusDynamic::NeedleType::STEPPER, Direction::DIR_UP);
   f->InsertPixel(n, 0, 1);
   n = new CactusDynamic(CactusDynamic::NeedleType::MOVER, Direction::DIR_UP);
   f->InsertPixel(n, -3, -5);
   n = new CactusStatic();
   f->InsertPixel(n, 0, -5);
   n = new CactusDynamic(CactusDynamic::NeedleType::MOVER, Direction::DIR_RIGHT);
   f->InsertPixel(n, -5, -5);

   // Sokobans
   n = new SokobanPixel(PixelColor::BLUE);
   f->InsertPixel(n, 3, 3);
   n = new SokobanPixel(PixelColor::BLUE);
   f->InsertPixel(n, 2, 2);

   // Pitfalls
   n = new Pitfall();
   f->InsertPixel(n, -3, 2);
   n = new Pitfall();
   f->InsertPixel(n, -4, 2);

   n = new StonePixel();
   f->InsertPixel(n, -2, 1);
   n = new StonePixel();
   f->InsertPixel(n, -2, 0);
   n = new StonePixel();
   f->InsertPixel(n, -2, -1);

   // Magnetics
   n = new MagneticPixel(PixelColor::GREEN);
   f->InsertPixel(n, -2, -2);
   // And 4 random placed pixels
   int x, y, i=0;
   while (i<4) {
      x = rand()%7 - 3; // -3 -2 -1 0 1 2 3
      y = rand()%7 - 3;
      if ( ! f->GetPixelAt(x, y) ) {
         ++i;
         f->InsertPixel(new MagneticPixel, x, y);
      }
   }

   // Shape will be like tetris T-shape
   auto shape = make_shared<Goal>();
   shape->AddTask(Coordinates(0, -1), PixelColor::RED);
   shape->AddTask(Coordinates(1, -1), PixelColor::RED);
   shape->AddTask(Coordinates(1, 0), PixelColor::RED);
   shape->AddTask(Coordinates(2, -1), PixelColor::GREEN);
   shape->AddTask(Coordinates(2, 1), PixelColor::BLUE);
   fab->goals->InsertGoal(shape);

   return fab;
}


shared_ptr<Level> LevelGenerator::CreateEmpty()
{
   // Empty level has no world(default=0) and is open
   auto fab = shared_ptr<Level>( new Level );
   return fab;
}


shared_ptr<Level> MPix::LevelGenerator::CreateFromEditorData( EditorData* source, shared_ptr<Level> target )
{

   shared_ptr<Level> fab;
   if (target) {
      fab = target->Dublicate();
      fab->field = make_shared<Field>();
      fab->goals = make_shared<Goals>();
   } else {
      fab = CreateEmpty();
   }

   for (auto pos : source->field ) {
      for ( auto px : pos.second ) {
         fab->field->InsertPixel(px->Dublicate());
      }
   }
   for (auto px : source->walls ) {
      fab->field->InsertPixel(px.second->Dublicate());
   }
   for (auto g : source->goals ) {
      if (g) {
         fab->goals->InsertGoal(g);
      }
   }

   fab->SetName(source->GetName());
   return fab;
}


EditorData* MPix::LevelGenerator::SaveToEditorData( shared_ptr<Level> source )
{
   if (!source)
      return nullptr;

   auto fab = new EditorData();
   for (auto p : source->field->GetPixelData()) {
      if (p->GetType() == Pixel::PixelType::WALL_PIX) {
      fab->InsertWallAt(p->GetPos(), p);
      } else {
      fab->InsertPixelAt(p->GetPos(), p);
      }
   }
   int sz = source->goals->GoalsCount();
   for (int p = 0; p < sz; ++p ) {
      for (auto t : source->goals->GetGoalByID(p+1)->GetTasks()) {
         fab->InsertTask(p, t.second.GetPos(), t.second.GetColor());
      }
   }
   fab->SetName(source->GetName());
   return fab;
}


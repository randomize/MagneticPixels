#include "test.h"

// Testing modules
#include "Field.h"
#include "Pixel.h"
#include "MagneticPixel.h"


#define TEST(p) if (!(p)) { EM_LOG_ERROR("Failed test : " + #p); } else {EM_LOG_INFO("Passed test : "+ #p );}

void MPix::TestClass::PerformTests()
{
   EM_LOG_ERROR("==== Running tests ====");
   //testField::Test();

   EM_LOG_ERROR("==== END ====");
}


void MPix::testField::Test()
{
   /*Field* f = new Field();

   TEST(f->GetWidth() == 0);
   TEST(f->GetHeight() == 0);
   TEST(f->IsEmpty() == true);

   for (int x = 0; x < 500; ++x) {
      f->InsertPixel(new MagneticPixel, rand()%11-5, rand()%11-5);
   }
   TEST(f->GetWidth() == 11);
   TEST(f->GetHeight() == 11);
   TEST(f->IsEmpty() == false);

   delete f;


    f = new Field();
   for (int x = 0; x < 10; x++)
   {
      f->InsertPixel(new MagneticPixel, 42, 42);
   }
   auto p = f->GetPixelByID(4);
   TEST ( p );
   TEST ( !f->GetPixelByID(400) );
   TEST ( f->GetPixelAt(42,42) );
   TEST ( !f->GetPixelAt(0,0) );
   TEST( f->GetPixelAt(Coordinates(42,42))->GetID() == 1 );
   TEST( ! f->GetAllPixelsAt(Coordinates(42,42)).empty() );
   TEST( f->GetAllPixelsAt(Coordinates(43,42)).empty() );
   TEST( f->GetAllPixelsAt(Coordinates(42,42)).size() == 10 );
   auto list = f->GetAllPixelsAt(Coordinates(42,42));
   string res;
   for (auto a : list) { res.append(std::to_string(a->GetID())) ;}
   TEST( res=="12345678910" );
   f->InsertPixel(new MagneticPixel, 42, 42);
   f->RemovePixelByID(7);
   list = f->GetAllPixelsAt(Coordinates(42,42));
   res = "";
   for (auto a : list) { res.append(std::to_string(a->GetID())) ;}
   TEST( res=="123456891011" );
   f->RemoveAllPixelsAt(42,42);
   TEST( f->GetAllPixelsAt(Coordinates(42,42)).empty() );
   TEST ( !f->GetPixelByID(4) );
   f->InsertPixel(new MagneticPixel, 0, 0);
   f->InsertPixel(new MagneticPixel, 41, 42);
   TEST ( f->GetPixelByID(13) );
   f->MovePixel(13, DIR_RIGHT);
   TEST( f->GetPixelAt(Coordinates(42,42))->GetID() == 13 );
   TEST( ! f->GetAllPixelsAt(Coordinates(42,42)).empty() );
   TEST( f->GetAllPixelsAt(Coordinates(41,42)).empty() );
   f->MovePixel(13, DIR_RIGHT);
   TEST( f->GetPixelAt(Coordinates(43,42))->GetID() == 13 );
   f->MovePixel(13, DIR_LEFT);
   TEST( f->GetPixelAt(Coordinates(42,42))->GetID() == 13 );
   */






}

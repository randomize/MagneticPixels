#include "MPix.h"
#include "LocaleManager.h"

using namespace MPix;

const float MPix::MPIX_CELL_SIZE = 80.0f;
const float MPix::MPIX_CELL_SIZE_HALF = MPIX_CELL_SIZE/2.0f;
const Point MPix::MPIX_CELL_SIZE_P = Point(MPIX_CELL_SIZE, MPIX_CELL_SIZE);
const Point MPix::MPIX_CELL_SIZE_HALF_P = Point(MPIX_CELL_SIZE/2.0, MPIX_CELL_SIZE/2.0);


// ------ Coordinates class implementation ----------------------------------

Coordinates::Coordinates( int x /*= 0*/, int y /*= 0*/ ) : 
   x(x),
   y(y)
{
}

bool Coordinates::operator==( const Coordinates& rhs ) const
{
   return ( x == rhs.x && y == rhs.y );
}

bool Coordinates::operator!=( const Coordinates& rhs ) const
{
   return !(*this == rhs);
}

void MPix::Coordinates::MoveTo( Direction dir )
{
   /*
   // Not sure if it is cooler then commented switch..
   // at least it uses lambdas
   auto inc = [](int& a) -> void {++a;};
   auto dec = [](int& a) -> void {--a;};
   auto nop = [](int& a) -> void {;};

   std::function<void(int& a)> xes[8] = {
      nop, inc, nop, dec, inc, dec, inc, dec  
   };

   std::function<void(int& a)> yes[8] = {
      inc, nop, dec, nop, inc, inc, dec, dec
   };

   // Now super magic
   yes[dir](y);
   xes[dir](x);
*/
   switch (dir)
   {
   case Direction::DIR_UP:
      y++;
      break;
   case Direction::DIR_RIGHT:
      x++;
      break;
   case Direction::DIR_DOWN:
      y--;
      break;
   case Direction::DIR_LEFT:
      x--;
      break;
   case Direction::DIR_UPRIGHT:
      x++; y++;
      break;
   case Direction::DIR_UPLEFT:
      x--; y++;
      break;
   case Direction::DIR_DOWNRIGHT:
      x++; y--;
      break;
   case Direction::DIR_DOWNLEFT:
      x--; y--;
      break;
   default:
      assert(false);// WTF case
      break;
   }
}

MPix::Coordinates MPix::Coordinates::operator+( const Direction& d )
{
   Coordinates tmp = *this;
   tmp.MoveTo(d);
   return tmp;
}

MPix::Coordinates MPix::Coordinates::operator+( const Coordinates& c )
{
   Coordinates tmp = *this;
   tmp.x += c.x;
   tmp.y += c.y;
   return tmp;
}

MPix::Coordinates MPix::Coordinates::ZERO(0,0);


// ------ Free members in MPix ---------------------------------------------

ostream& MPix::operator<<( ostream& os, const Coordinates& b )
{
   os << " [ " << b.x << ", " << b.y << " ] ";
   return os;
}

const char* MPix::DirectionsToString( Direction d )
{
   switch (d)
   {
   case MPix::Direction::DIR_UP:
      return "Up";
   case MPix::Direction::DIR_RIGHT:
      return "Right";
   case MPix::Direction::DIR_DOWN:
      return "Down";
   case MPix::Direction::DIR_LEFT:
      return "Left";
   case MPix::Direction::DIR_UPRIGHT:
      return "UpRight";
   case MPix::Direction::DIR_UPLEFT:
      return "UpLeft";
   case MPix::Direction::DIR_DOWNRIGHT:
      return "DownRight";
   case MPix::Direction::DIR_DOWNLEFT:
      return "DownLeft";
   case MPix::Direction::DIR_UNKNOWN:
      return "Unknown";
   default:
      assert(false);
   }
   return "WTF";
}

ostream& MPix::operator<<( ostream& os, const Direction& d )
{
   os << DirectionsToString(d);
   return os;
}

ostream& MPix::operator<<( ostream& os, const Point& b )
{
   size_t save_prec = os.precision();
   os << " ccp [ " << std::setprecision(4) << std::fixed << b.x << ", " << b.y << " ] " << std::scientific;
   os.precision(save_prec);
   return os;
}

ostream& MPix::operator<<( ostream& os, const Rectangle& d )
{
   os << "Rect [ " << d.BL << d.TR << " ] ";
   return os;
}

ostream& MPix::operator<<( ostream& os, const PixelColor& color )
{
   os << PixelColorToStr(color);
   return os;
}


bool MPix::DirectionIsMajor( Direction dir )
{
   switch (dir)
   {
   case Direction::DIR_UP:
   case Direction::DIR_RIGHT:
   case Direction::DIR_DOWN:
   case Direction::DIR_LEFT:
      return true;
   case Direction::DIR_UPRIGHT:
   case Direction::DIR_UPLEFT:
   case Direction::DIR_DOWNRIGHT:
   case Direction::DIR_DOWNLEFT:
      return false;
   default:
      assert(false); // WTF case
      return false;
   }
}

list<Direction> MPix::SplitDirection( Direction dir )
{

   list<Direction> result;

   switch (dir)
   {
   case Direction::DIR_UP:
   case Direction::DIR_RIGHT:
   case Direction::DIR_DOWN:
   case Direction::DIR_LEFT:
      result.push_back(dir);
      break;
   case Direction::DIR_UPRIGHT:
      result.push_back(Direction::DIR_UP);
      result.push_back(Direction::DIR_RIGHT);
      break;
   case Direction::DIR_UPLEFT:
      result.push_back(Direction::DIR_UP);
      result.push_back(Direction::DIR_LEFT);
      break;
   case Direction::DIR_DOWNRIGHT:
      result.push_back(Direction::DIR_DOWN);
      result.push_back(Direction::DIR_RIGHT);
      break;
   case Direction::DIR_DOWNLEFT:
      result.push_back(Direction::DIR_DOWN);
      result.push_back(Direction::DIR_LEFT);
      break;
   default:
      assert(false); // WTF case
      break;
   }

   return result;
}

MPix::Direction MPix::ReverseDirection( Direction dir )
{
   switch (dir)
   {
   case Direction::DIR_UP:
      return Direction::DIR_DOWN;
   case Direction::DIR_RIGHT:
      return Direction::DIR_LEFT;
   case Direction::DIR_DOWN:
      return Direction::DIR_UP;
   case Direction::DIR_LEFT:
      return Direction::DIR_RIGHT;
   case Direction::DIR_UPRIGHT:
      return Direction::DIR_DOWNLEFT;
   case Direction::DIR_UPLEFT:
      return Direction::DIR_DOWNRIGHT;
   case Direction::DIR_DOWNRIGHT:
      return Direction::DIR_UPLEFT;
   case Direction::DIR_DOWNLEFT:
      return Direction::DIR_UPRIGHT;
   case Direction::DIR_UNKNOWN:
      return dir;
   default:
      assert(false);
      break;
   }

   return dir;
}

MPix::Direction MPix::RandomDirection()
{
   static const Direction map[8] = {
      Direction::DIR_UP,
      Direction::DIR_RIGHT,
      Direction::DIR_DOWN,
      Direction::DIR_LEFT,
      Direction::DIR_UPRIGHT,
      Direction::DIR_UPLEFT,
      Direction::DIR_DOWNLEFT,
      Direction::DIR_DOWNRIGHT
   };
   return map[rand()%8];
}


MPix::Direction MPix::GetDirectionVector( Point s, Point e )
{
   float ang = (e-s).getAngle() * 4 / M_PI + 9/2.0f;
   int p = (int)floor(ang) % 8; // 0 .. 7 
   static const Direction d8[8] = {
      Direction::DIR_LEFT, // = 0
      Direction::DIR_DOWNLEFT,
      Direction::DIR_DOWN,
      Direction::DIR_DOWNRIGHT,
      Direction::DIR_RIGHT,
      Direction::DIR_UPRIGHT,
      Direction::DIR_UP,
      Direction::DIR_UPLEFT // = 7
   };
   return d8[p];
}

MPix::Direction MPix::GetDirectionVector( Coordinates s, Coordinates e )
{
   Point sp(s.x, s.y);
   Point se(e.x, e.y);
   return GetDirectionVector(sp, se);
}


MPix::Coordinates MPix::ScreenToLogic( Point p )
{
   return Coordinates(floor(p.x / MPIX_CELL_SIZE), floor(p.y/MPIX_CELL_SIZE));

}

Point MPix::LogicToScreen( Coordinates c )
{
   return Point(c.x * MPIX_CELL_SIZE, c.y * MPIX_CELL_SIZE);
}

cocos2d::Point MPix::LogicToScreen( int x, int y )
{
   return LogicToScreen(Coordinates(x,y));

}

bool MPix::Rectangle::operator==( const Rectangle& rhs ) const
{
   return TR == rhs.TR && BL == rhs.BL;
}

bool MPix::Rectangle::operator!=( const Rectangle& rhs ) const
{
   return !(*this == rhs);
}

void MPix::Rectangle::MoveTo( Direction dir )
{
   TR.MoveTo(dir);
   BL.MoveTo(dir);
}

void MPix::Rectangle::MoveTo( Direction dir, int len )
{
   for (int i = 0; i < len; ++i)
      MoveTo(dir);
}

const char* MPix::PixelColorToStr( PixelColor pc )
{
   switch (pc)
   {
   case PixelColor::BLACK:
      return "Black";
   case PixelColor::GRAY:
      return "Gray";
   case PixelColor::RED:
      return "Red";
   case PixelColor::GREEN:
      return "Green";
   case PixelColor::BLUE:
      return "Blue";
   case PixelColor::YELLOW:
      return "Yellow";
   default:
      return "Unknown";
   }
}

HSVColor MPix::PixelColorToHSV( PixelColor pc )
{
   // Stored in HSV-shift format from red
   switch (pc) {
   case PixelColor::BLACK:
      return HSVColor(0, 0, 0);
   case PixelColor::GRAY:
      return HSVColor(0, 0, 1.0f);
   case PixelColor::RED:
      return HSVColor(0, 1.0f, 1.0f);
   case PixelColor::GREEN:
      return HSVColor(-1.47f, 1.0f, 0.98f); // 60
   case PixelColor::BLUE:
      return HSVColor(-3.43f, 1.0f, 0.98f);
   case PixelColor::YELLOW:
      return HSVColor(-0.5f, 1.1f, 1.0f);
   default:
      return HSVColor(0.24f, 1.5f, 1.0f); // Fuchsia
   }
   return HSVColor(0.24f, 1.5f, 1.0f); // Fuchsia
}

//////////////////////////////////////////////////////////////////////////
// Localization support shorthands

const char* LocalUTF8Char( const char* key )
{
   return LocaleManager::getInstance().GetLocalisedChar(key);
}

const string& LocalUTF8Str( const string& key )
{
   return LocaleManager::getInstance().GetLocalisedString(key);
}


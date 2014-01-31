#include "TouchLayer.h"
#include "GameStateManager.h"
#include "GameplayManager.h"
#include "Cmd.h"
#include "EMBaseMasterLoop.h"

using namespace MPix;

// constants
namespace TouchConstants {

   const float MAX_GESTURE_TIMEOUT = 1.0f;

   // Tapping ===============================================================

   // If all paints of gesture lay in this circle gesture is considered a tap
   const float TAP_RADIUS_MAX = 10.0f;

   // Gesture recognition works only if gesture boundary is at least of this size
   const float GESTURE_BOUNDARY_MIN = 40.0f;


   // Shaking ==============================================================

   // Sets maximum angle that is considered acute when looking for shake
   const float ACUTE_ANGLE_MAX = PI_FLOAT / 5;

   // Max len to be counted as acute
   const float ACUTE_COUNTER_VECTOR_MAX_LEN = 0.1f;

   // Sets number of acute angles in stroke to be considered as shake
   const int MIN_ACUTE_TO_BE_SHAKE = 4;

   // Noise threshold, min points in stroke to be recognized as shake
   const int MIN_SHAKE_ACCEPT_SAMPLES = 10;


   // Circle ===============================================================

   // Min radius of circle
   const float CIRCLE_RAD = 150.0f; 

   // Threshold of fitness (percent of radius)
   const float CIRCLE_THRESH = 25.0f; 

   // Threshold for fitness of angles
   const float ANGLE_THRESH = PI_FLOAT / 4;// 45 degree

   // Percent of fitted points required to be circle
   const float CIRCLENESS_CRITERIA = 65.0f; 


   // Slipping touches
   const int IDLING_RATE = 2; // Use only each 2rd

}


MPix::TouchLayer::TouchLayer():
   ps(Point::ZERO),
   pe(Point::ZERO),
   n_acute_angles(0)
{
    CmdDidEnterBG::listners["TouchLayer"] = std::bind( &TouchLayer::onBGFG, this );
    CmdWillEnterFG::listners["TouchLayer"] = std::bind( &TouchLayer::onBGFG, this );
}

MPix::TouchLayer::~TouchLayer()
{
    CmdDidEnterBG::listners.erase("TouchLayer");
    CmdWillEnterFG::listners.erase("TouchLayer");
}

bool TouchLayer::init()
{


   // Draw a grid and gray pixel

   auto dn = DrawNode::create();
   auto cl = Color4F(0.5f, 0.5f, 0.5f, 0.5f);
   const int size = 90;
   for (int i=0; i < size*2+1; ++i){
      dn->drawSegment( 
         LogicToScreen(Coordinates(-size, i-size)),
         LogicToScreen(Coordinates(size, i-size)),
         0.5f,
         cl);
      dn->drawSegment( 
         LogicToScreen(Coordinates(i-size, -size)),
         LogicToScreen(Coordinates(i-size, size)),
         0.5f,
         cl);
   }
   addChild(dn);

   // Jic
   ResetState();

   return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool TouchLayer::onTouchBegan( Touch *touch, Event *event )
{

   auto posOnScreen = touch->getLocationInView();
   //EM_LOG_DEBUG("== BEGAN == " + posOnScreen);

   switch (st)
   {
   case MPix::TouchLayer::WAITING_TOUCH:

      ps = this->convertTouchToNodeSpace(touch);

      st = ONE_TOUCH_RECORDING;
      sequence.clear();
      n_acute_angles = 0;

      sequence.push_back(posOnScreen);

      timestamp = EMBaseMasterLoop::GetTime();

      idling_counter = 0;

      return true;

   case MPix::TouchLayer::ONE_TOUCH_RECORDING:

      sequence.clear();
      st = ZOOMING_AND_PANING;
      // TODO: Fix parameters
      return true;

   case MPix::TouchLayer::ZOOMING_AND_PANING:
      return false;

   default:
      return false;

   }

   return false;

}

void TouchLayer::onTouchMoved( Touch *touch, Event *event )
{

   auto posOnScreen = touch->getLocationInView();

   switch (st)
   {
   case MPix::TouchLayer::WAITING_TOUCH:
      assert(false);
      break;

   case MPix::TouchLayer::ONE_TOUCH_RECORDING:
      {
         if (idling_counter != 0) {
            idling_counter--;
            assert(idling_counter >= 0);
            return;
         }

         idling_counter = TouchConstants::IDLING_RATE;

         pe = this->convertTouchToNodeSpace(touch);
         sequence.push_back(posOnScreen);

         auto sz = sequence.size();
         if ( sz >= 3 ) {
            // Taking two vectors
            Point v1 = sequence[sz-3] - sequence[sz-2];
            Point v2 = sequence[sz-1] - sequence[sz-2];

            // Assure that vectors have enough length
            if (v1.getLength() < TouchConstants::ACUTE_COUNTER_VECTOR_MAX_LEN
               || v2.getLength() < TouchConstants::ACUTE_COUNTER_VECTOR_MAX_LEN)
               return;


            // Calculating angle between
            float angle = v1.getAngle(v2);
            if (fabs(angle) <= TouchConstants::ACUTE_ANGLE_MAX) {
               n_acute_angles++;
               //EM_LOG_DEBUG("== MOVE == " + n_acute_angles + " sm " + sz);
            }

            if (EMBaseMasterLoop::GetTime() - timestamp < TouchConstants::MAX_GESTURE_TIMEOUT * 2) {
               if (n_acute_angles >= TouchConstants::MIN_ACUTE_TO_BE_SHAKE && sz >= TouchConstants::MIN_SHAKE_ACCEPT_SAMPLES) {
                  GestureShake();
                  st = IGNORING;
               }
               //EM_LOG_DEBUG("========= " + posOnScreen);
            }

         }

      }
      break;
   case MPix::TouchLayer::ZOOMING_AND_PANING:

      // TODO: make zoom and pan, based on fixed parameters and current delthas
      break;

   case MPix::TouchLayer::IGNORING:
      break;

   default:
      break;

   }

}

void TouchLayer::onTouchCancelled( Touch *touch, Event *event )
{
   // React same as End
   onTouchEnded(touch, event);
}

void TouchLayer::onTouchEnded( Touch *touch, Event *event )
{

   auto posOnScreen = touch->getLocationInView();
   //EM_LOG_DEBUG("== END == " + posOnScreen);

   switch (st)
   {
   case MPix::TouchLayer::WAITING_TOUCH:
      assert(false);
      break;
   case MPix::TouchLayer::ONE_TOUCH_RECORDING:
      assert(touch->getID() == 0); // must be that one
      if (EMBaseMasterLoop::GetTime() - timestamp < TouchConstants::MAX_GESTURE_TIMEOUT) {
         sequence.push_back(posOnScreen);
         pe = this->convertTouchToNodeSpace(touch);
         AnalyseSequence();
      }
      sequence.clear();
      st = WAITING_TOUCH;
      break;
   case MPix::TouchLayer::ZOOMING_AND_PANING:
      // here are two touches,  but user released only one
      st = IGNORING;
      break;
   case MPix::TouchLayer::IGNORING:
      // Well now we can switch to waiting
      st = WAITING_TOUCH;
      break;
   default:
      break;
   }

}

/////////////////////////////////////////////////////////////////////////////////


void MPix::TouchLayer::GestureTapPoint( Point p )
{
   p = Director::getInstance()->convertToGL(p);
   p = this->convertToNodeSpace(p);
   Coordinates pos = ScreenToLogic(p);
   EM_LOG_DEBUG("Got a tap at: " + pos);

   GameplayManager::getInstance().PostCommand(new CmdGameplayClick(pos));
}

void MPix::TouchLayer::GestureSwipe( Direction dir )
{
   EM_LOG_DEBUG("Got a swype to " + dir);
   GameplayManager::getInstance().PostCommand(new CmdGameplayMove(dir));
}

void MPix::TouchLayer::GestureLongSwipe( Direction dir )
{
   EM_LOG_DEBUG("Got a long swype to " + dir);
}

void MPix::TouchLayer::GestureShake()
{
   EM_LOG_DEBUG("Got a shake");
   GameplayManager::getInstance().PostCommand(new CmdGameplayRestartAssembly);
}

void MPix::TouchLayer::GestureRotateCW()
{
   EM_LOG_DEBUG("Got a CW circle");
   GameplayManager::getInstance().PostCommand(new CmdGameplayRedoMove);
}

void MPix::TouchLayer::GestureRotateCCW()
{
   EM_LOG_DEBUG("Got a CCW circle");
   GameplayManager::getInstance().PostCommand(new CmdGameplayUndoMove);
}


////////////////////////////////////////////////////////////////////////////////////////////

void MPix::TouchLayer::ResetState()
{
   sequence.clear();
   st = WAITING_TOUCH;
   n_acute_angles = 0;
}

void TouchLayer::onEnter()
{
   Layer::onEnter();

   // Register touch
   TouchEnable();

}

ErrorCode TouchLayer::onBGFG() {
   ResetState();
   return ErrorCode::RET_OK;
}


EmbossLib::ErrorCode MPix::TouchLayer::TouchEnable()
{
   EM_LOG_DEBUG("Touch layer touches on");
   auto listener = EventListenerTouchOneByOne::create();
   listener->setSwallowTouches(true);
   listener->onTouchBegan     = CC_CALLBACK_2(TouchLayer::onTouchBegan, this);
   listener->onTouchMoved     = CC_CALLBACK_2(TouchLayer::onTouchMoved, this);
   listener->onTouchEnded     = CC_CALLBACK_2(TouchLayer::onTouchEnded, this);
   listener->onTouchCancelled = CC_CALLBACK_2(TouchLayer::onTouchCancelled, this);

   _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

   ResetState();

   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::TouchLayer::TouchDisable()
{
   EM_LOG_DEBUG("Touch layer touches off");

   _eventDispatcher->removeEventListeners(EventListener::Type::TOUCH_ONE_BY_ONE);

   ResetState();
   return ErrorCode::RET_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////

void MPix::TouchLayer::AnalyseSequence()
{
   // Good to have nonempty sequence 
   if (sequence.empty())
      return;

   auto & p_start = sequence.front();

   // Try the simplest, gesture : TAP
   if (sequence.size() <= 3) {
      GestureTapPoint( p_start );
      return;
   }

   // Collecting info
   auto n = sequence.size();

   // Max len, boundary, center
   float maxLen = 0;
   float avgx = 0;
   float avgy = 0;
   float xmin, xmax, ymin, ymax;
   xmin = xmax = p_start.x;
   ymin = ymax = p_start.y;
   for (auto p : sequence) {
      auto len = (p_start - p).getLengthSq();
      float x = p.x; float y = p.y;
      avgx += x; avgy += y;
      if ( len > maxLen ) maxLen = len;
      if ( x > xmax ) xmax = x;
      if ( x < xmin ) xmin = x;
      if ( y > ymax ) ymax = y;
      if ( y < ymin ) ymin = y;
   }
   avgx /= n;
   avgy /= n;
   maxLen = sqrt(maxLen);

   // Additional parameters
   auto center = Point(xmax+xmin, ymax+ymin) / 2.0f;
   auto avgCenter = Point(avgx, avgy);
   auto rectangle = Point(xmax-xmin, ymax-ymin);

   EM_LOG_DEBUG( " BBox is : " + rectangle.x + " x " + rectangle.y );
   EM_LOG_DEBUG( " BBox center = " + center);
   EM_LOG_DEBUG( " BBox avg center = " + avgCenter);

   // ok maybe it is still TAP
   if ( maxLen < TouchConstants::TAP_RADIUS_MAX  ) {
      //GestureTapPoint( mid / sequence.size() );
      GestureTapPoint( p_start );
      return;
   }

   // Filter out too small gestures to be gesture but too large to be tap
   if ( rectangle.x < TouchConstants::GESTURE_BOUNDARY_MIN 
     && rectangle.y < TouchConstants::GESTURE_BOUNDARY_MIN ) {
      return;
   }

   // Calculate:
   //   - number of acute angles in stroke
   //   - and average radius
   //   - list of all angles
   //   - avg speed (distance between subsequent)
   auto s_end = end(sequence);
   auto p1 = begin(sequence);
   auto p2 = p1 + 1;
   float avgRadius = (avgCenter-(*p1)).getLength();
   float avgAngle = 0;
   vector<float> allAngles;
   allAngles.reserve(sequence.size());
   // p1 p2 p3 .... end_it;
   for (; p2 != s_end; ++p2, ++p1)
   {
      Point v1 = *p2 - *p1;

      float nangle = fabs(v1.getAngle());
      // float nlen = v1.getLength();
      allAngles.push_back(nangle);

      avgRadius += (avgCenter-(*p2)).getLength() ;
      avgAngle += nangle;
   }
   avgRadius /= n;
   avgAngle /= allAngles.size();
   EM_LOG_DEBUG( " A radus = " + avgRadius);
   EM_LOG_DEBUG( " Angle avg " + avgAngle );

   // Trying to recognize circle
   if ( avgRadius >= TouchConstants::CIRCLE_RAD ) {
      int fitness = 0;
      float div = TouchConstants::CIRCLE_THRESH / 100.0f * avgRadius;
      for ( auto p : sequence ) {
         float rad = (p-avgCenter).getLength();
         if ( fabs(rad-avgRadius) < div ) 
            fitness++;
      }
      float fitnessPercent = fitness/float(n) * 100.0f;
      EM_LOG_DEBUG( " Fitness circle " + fitnessPercent );
      if ( fitnessPercent >= TouchConstants::CIRCLENESS_CRITERIA ) {
         //GestureRotateCCW(); // FIXME : circle is to unstable
         return;
      }
   }

   // Falling back to Linear gesture

   // Calculate angle 
   auto &p_end = sequence.back();
   auto pdeltha = p_end - p_start;
   float leadAng = pdeltha.getAngle();
   EM_LOG_DEBUG( " Lead ang " + leadAng );


   // Calculate deviation of angle
   int angle_fitness = 0;
   for ( auto a : allAngles )
   {
      //EM_LOG_DEBUG(a);
      if ( fabs(a-fabs(leadAng)) < TouchConstants::ANGLE_THRESH ) {
         angle_fitness++;
      }
   }
   float angle_fitnessPercent = float(angle_fitness) / allAngles.size() * 100.0f;
   EM_LOG_DEBUG( " Fitness line " + angle_fitnessPercent );
   if ( angle_fitnessPercent <= 40.0 ) {
      return;
   }

   // Getting direction from angle
   float ang = leadAng * 4 / M_PI + 9/2.0f;
   int p = (int)floor(ang) % 8; // 0 .. 7 

   Direction di = Direction::DIR_UNKNOWN;
   switch (p)
   {
   case 0:
      di = Direction::DIR_LEFT;
      break;
   case 1:
      di = Direction::DIR_UPLEFT;
      break;
   case 2:
      di = Direction::DIR_UP;
      break;
   case 3:
      di = Direction::DIR_UPRIGHT;
      break;
   case 4:
      di = Direction::DIR_RIGHT;
      break;
   case 5:
      di = Direction::DIR_DOWNRIGHT;
      break;
   case 6:
      di = Direction::DIR_DOWN;
      break;
   case 7:
      di = Direction::DIR_DOWNLEFT;
      break;
   default:
      assert(false);
      break;
   }

   //EM_LOG_DEBUG("Gesture len: "+ maxLen);
   /*if (maxLen > 300) {
      GestureLongSwipe(di);
   } else {
      GestureSwipe(di);
   }*/
   GestureSwipe(di);

}



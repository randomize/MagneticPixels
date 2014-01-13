#include "TouchLayer.h"
#include "GameStateManager.h"
#include "GameplayManager.h"
#include "Cmd.h"

using namespace MPix;

// constants
namespace TouchConstants {

   // If all paints of gesture lay in this circle gesture is considered a tap
   const float TAP_RADIUS_MAX = 10.0f;

   // Gesture recognition works only if gesture boundary is at least of this size
   const float GESTURE_BOUNDARY_MIN = 40.0f;

   // Min radius of circle
   const float CIRCLE_RAD = 150.0f; 

   // Min ratio of circle
   const float MINRATIO = 0.5f;

   // Sets maximum angle to be considered acute
   const float ACUTE_ANGLE_MAX = PI_FLOAT / 5;

   // Shake speed limit
   const float SHAKE_SPEED = 40.0f;

   // Threshold of fitness (percent of radius)
   const float CIRCLE_THRESH = 25.0f; 

   // Threshold for fitness of angles
   const float ANGLE_THRESH = PI_FLOAT / 4;// 45 degree

   // Percent of fitted points required to be circle
   const float CIRCLENESS_CRITERIA = 65.0f; 

}


MPix::TouchLayer::TouchLayer()
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
   st = TouchLayer::WAITING_TOUCH; 
//   setKeypadEnabled(true);

   auto dn = DrawNode::create();
   // Draw a grid and gray pixel
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

   return true;
}

ErrorCode TouchLayer::onBGFG() {
   ResetState();
   return ErrorCode::RET_OK;
}

bool TouchLayer::onTouchBegan( Touch *touch, Event *event )
{

   auto posOnScreen = touch->getLocationInView();

   switch (st)
   {
   case MPix::TouchLayer::WAITING_TOUCH:
      ps = this->convertTouchToNodeSpace(touch);
      st = ONE_TOUCH_RECORDING;
      sequence.push_back(posOnScreen);
      n_acute_angles = 0;
      unscheduleAllSelectors();
      scheduleOnce(schedule_selector(TouchLayer::onTimeoutElapsed), 1.5f);
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
         pe = this->convertTouchToNodeSpace(touch);
         sequence.push_back(posOnScreen);

         auto sz = sequence.size();
         if ( sequence.size() >= 3 ) {
            // Taking two vectors
            Point v1 = sequence[sz-3] - sequence[sz-2];
            Point v2 = sequence[sz-1] - sequence[sz-2];

            // Calculating angle between
            float angle = v1.getAngle(v2);
            if ( fabs(angle) <= TouchConstants::ACUTE_ANGLE_MAX )
               n_acute_angles++;

            if ( n_acute_angles >= 3 ) {
               GestureShake();
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

   switch (st)
   {
   case MPix::TouchLayer::WAITING_TOUCH:
      assert(false);
      break;
   case MPix::TouchLayer::ONE_TOUCH_RECORDING:
      assert(touch->getID() == 0); // must be that one
      sequence.push_back(posOnScreen);
      pe = this->convertTouchToNodeSpace(touch);
      AnalyseSequence();
      sequence.clear();
      st = WAITING_TOUCH;
      break;
   case MPix::TouchLayer::ZOOMING_AND_PANING:
      // here are two touches,  but user released only one
      st = IGNORING;
      break;
   case MPix::TouchLayer::IGNORING:
      // Well noooow we can switch to waiting
      st = WAITING_TOUCH;
      break;
   default:
      break;
   }

}

void MPix::TouchLayer::draw()
{

#ifdef MPIX_DEVELOPERS_BUILD

   if (st == ONE_TOUCH_RECORDING && sequence.size() >= 2) {
      DrawPrimitives::setDrawColor4F(1.0f, 0.0f, 1.0f, 1.0f);
      DrawPrimitives::setPointSize(10);
      DrawPrimitives::drawLine(ps,pe);
      DrawPrimitives::setDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);
      DrawPrimitives::drawPoint(ps);
      DrawPrimitives::drawPoint(pe);
   }

#endif

}

void MPix::TouchLayer::GestureTapPoint( Point p )
{
   p = Director::getInstance()->convertToGL(p);
   p = this->convertToNodeSpace(p);
   Coordinates pos = ScreenToLogic(p);
   EM_LOG_DEBUG("Sending a tap at: " + pos);

   GameplayManager::getInstance().PostCommand(new CmdGameplayClick(pos));
}

void MPix::TouchLayer::GestureSwipe( Direction dir )
{
   EM_LOG_DEBUG("Sending a swype to " + dir);
   GameplayManager::getInstance().PostCommand(new CmdGameplayMove(dir));
}

void MPix::TouchLayer::GestureLongSwipe( Direction dir )
{
   EM_LOG_DEBUG("Sending a long swype to " + dir);
   GameplayManager::getInstance().PostCommand(new CmdGameplayMoveFast(dir));
}

void MPix::TouchLayer::GestureShake()
{
   EM_LOG_DEBUG("Sending restart assembly");
   GameplayManager::getInstance().PostCommand(new CmdGameplayRestartAssembly);
}

void MPix::TouchLayer::GestureRotateCW()
{
   EM_LOG_DEBUG("Sending redo move in assembly");
   GameplayManager::getInstance().PostCommand(new CmdGameplayRedoMove);
}

void MPix::TouchLayer::GestureRotateCCW()
{
   EM_LOG_DEBUG("Sending undo move in assembly");
   GameplayManager::getInstance().PostCommand(new CmdGameplayUndoMove);
}

EmbossLib::ErrorCode MPix::TouchLayer::TouchEnable()
{
   this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
   this->setTouchEnabled(true);
   ResetState();
   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::TouchLayer::TouchDisable()
{
   ResetState();
   this->setTouchEnabled(false);
   return ErrorCode::RET_OK;
}

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
      float nlen = v1.getLength();
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
         GestureRotateCCW();
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

   Direction di;
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

void MPix::TouchLayer::ResetState()
{
   sequence.clear();
   st = WAITING_TOUCH;
}

void MPix::TouchLayer::onTimeoutElapsed( float )
{
   if ( st == ONE_TOUCH_RECORDING ) {
      sequence.clear();
      st = IGNORING;
   }
}


/*
void MPix::TouchLayer::onBackClicked()
{
   GameStateManager::getInstance()->SwitchToExit();
}*/



void TouchLayer::onEnter()
{
   Layer::onEnter();
   // Register touch
   TouchEnable();
}

void TouchLayer::onExit()
{
   Layer::onExit();
   // Unregister touch
   TouchDisable();
}


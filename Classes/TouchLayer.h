//===-- TouchLayer.h ------------------------------------*- C++ -*-===//
//
//  Created:     2013/09/01
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: TouchLayer works with touches, displays grid
//
//===---------------------------------------------------------===//

#pragma once
#ifndef TOUCHLAYER_H__
#define TOUCHLAYER_H__

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // Forward dependencies
   //

   // TouchLayer class
   class TouchLayer : public Layer
   {
   public:

      bool init() override;
      CREATE_FUNC(TouchLayer);

   public:

      enum FieldState {
         WAITING_TOUCH = 0,
         ONE_TOUCH_RECORDING,
         ZOOMING_AND_PANING,
         IGNORING,
      };

      TouchLayer();
      ~TouchLayer();

      void onEnter() override;

      // -------- Delegates -------------------------------------------------

      bool     onTouchBegan( Touch *touch, Event *event);
      void onTouchCancelled( Touch *touch, Event *event);
      void     onTouchEnded( Touch *touch, Event *event);
      void     onTouchMoved( Touch *touch, Event *event);

      void onBackClicked();

      // ---------- listners ----------------------------------------------
       ErrorCode onBGFG();
       void onTimeoutElapsed(float);

   private:

      FieldState st;

      // -------- Gesture with one touch ------------------------------------------

      // Here is gesture stored (SCREEN coordinates)
      vector<Point> sequence;

      void AnalyseSequence();

      Point ps, pe;
      int n_acute_angles;

      // --------- Zooming and panning ----------------------------------- // TODO
      // p1, p2, ps1, ps2 ... etc

   private: // Recognized gestures

      // Called when tap received
      void GestureTapPoint(Point pos);
      void GestureSwipe(Direction dir);
      void GestureLongSwipe(Direction dir);
      void GestureShake();
      void GestureRotateCW();
      void GestureRotateCCW();


   private: // Touch on/off commands use semaphore technique

      // Helpers, always do what said
      ErrorCode TouchEnable();
      ErrorCode TouchDisable();

      // Clear touch queue and set to waiting
      void ResetState();

   };

}

#endif // TOUCHLAYER_H__



//===-- EditorLayer.h ------------------------------------*- C++ -*-===//
//
//  Created:     2013/09/01
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: EditorLayer works with touches, displays grid
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EDITOR_LAYER_H__
#define EDITOR_LAYER_H__

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // Forward dependencies
   class Level;
   class Pixel;
   enum class PixelColor;
   class EditorData;
   class PixelView;
   class GoalView;

   // EditorLayer class
   class EditorLayer : public Layer
   {
   public:

      bool init() override;  
      CREATE_FUNC(EditorLayer);

   public:

      EditorLayer();
      ~EditorLayer();

      // Creates new level, based on current EditorData in me
      shared_ptr<Level> CreateLevel();

      // -------- Touch delegates -------------------------------------------------

      bool     onTouchBegan( Touch *touch, Event *event) override;
      void onTouchCancelled( Touch *touch, Event *event) override;
      void     onTouchEnded( Touch *touch, Event *event) override;
      void     onTouchMoved( Touch *touch, Event *event) override;
      ErrorCode onBGFG();

      // ---------- Pixel works ---------------------------------------------------

      // Methods affect current pos defined by cursor
      void InsertPixel( shared_ptr<Pixel> px );
      void InsertWall( shared_ptr<Pixel> px );
      void InsertGoal( int n, PixelColor color );
      void EraseWall();
      void ErasePixels();
      void EraseTopPixel();
      void EraseGoal();
      void EraseEverything();
      // Views
      void InsertPixelView( shared_ptr<Pixel> px );
      void InsertWallView( shared_ptr<Pixel> px );
      void InsertGoalView( PixelColor color );

      // UpdateViewport
      void UpdateViewport();
      void FixPosition();

      // Control
      void RenameLevel(string newname);
      const string& GetLevelName();
      bool TogglePan();
       
   private:

      // ----------- Logical Storage -----------------------------------------------------------

      // Current working point
      Coordinates cursor;

      // Raw data of level
      EditorData* editor_data;

      // ----------- Visual Storage -----------------------------------------------------------

      // Multiple views can share same point
      Node* cursor_node;
      unordered_map<Coordinates, vector<shared_ptr<PixelView>>> views;
      unordered_map<Coordinates, shared_ptr<PixelView>> walls;
      unordered_map<Coordinates, shared_ptr<GoalView>> tasks;
      unordered_map<Coordinates, Node*> marks;
      Node* vp_marks[4];

      // --------- Touching ------------------------------------------------------------

      enum class TouchState {
         WAITING_TOUCH = 0,
         ONE_TOUCH,
         ZOOMING,
         SELECTING_TOOL,
         IGNORING,
      };

      TouchState st;
      Touch *first_touch, *second_touch; 
      Point pos;
      float scale;
      bool dragging;

      // ---------- Viewprt ------------------------------------------------------------
      Point initial_pos;

   private: // Internal

      // Helpers
      void MoveCursor(Coordinates pos);

      // Called when tap received 
      void GestureTapPoint(Coordinates pos);

      // Puts mark with number there
      void PutMark(Coordinates pos, int value);
      void RemoveMark(Coordinates pos);
   };

}

#endif // EDITOR_LAYER_H__



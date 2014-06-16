//===-- EditorTool.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/18
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Tools are buttons with some action
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EDITORTOOL_H_
#define EDITORTOOL_H_

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   typedef unordered_map<Coordinates, int> ToolMap;

   // Forward dependencies
   class EditorLayer;
   class Pixel;
   class PixelView;
   enum class PixelColor;
   class GoalView;
   class Goal;
   class View;

   // EditorTool interface
   class EditorTool
   {
   public:

      // Editor tool base, it has only name, represented by label
      EditorTool(const char* name);
      virtual ~EditorTool(){}

      // If tool has children, they are given in map here
      virtual ToolMap* Childs();

      // Called by toolbox on chosen
      virtual void Chosen();

      // Called by toolbox on hide
      virtual void Hide();

      // Called by toolbox on show
      virtual void Show(Coordinates where);

      // Called by toolbox once on creation, toolbox passed as target actually
      virtual void BindContents(Node * target);

      // Called on user click on tool, returns true if need to close toolbox
      virtual bool onSelected(EditorLayer* target) = 0;

      // Returns name
      const char* GetName();

   protected:

      string name;
      Label* name_l;

   };

   // Folder tool has name and children, also an icon possible
   class EditorFolderTool : public EditorTool {
   public:

      EditorFolderTool(const char* name, const char* icon = nullptr);

      // Add an child id to end
      void Insert(int id);

      // Remove a child from end
      int RemoveLast();

      // Overrides to return map
      ToolMap* Childs() override;

      bool onSelected(EditorLayer* target) override;

      virtual void Hide();

      virtual void Show( Coordinates where );

      virtual void BindContents( Node * target );

      size_t Count() const;

      ~EditorFolderTool();

   protected:
      
      unordered_map<Coordinates, int> map;
      unordered_map<int, Coordinates> map_inv;
      Coordinates cur;
      void AddCur();
      void SubCur();
      const char* icon_frame;
      Node* icon;
      shared_ptr<View> icon_view;
   };

   // EditorToolPixel has pixel in it and can place it to field
   class EditorToolPixel : public EditorTool
   {
   public:

      EditorToolPixel(const char* name, shared_ptr<Pixel> pixel);

      void BindContents(Node * target) override;
      bool onSelected(EditorLayer* target) override;

      virtual void Hide();

      virtual void Show( Coordinates where );

   protected:

      // Store pixel and view for it
      shared_ptr<Pixel> px;
      shared_ptr<PixelView> view;

   };

   // EditorToolGoal holds goal view, but actually adds tasks
   class EditorGoalTool : public EditorTool
   {
   public:

      // Name, color and index of goal
      EditorGoalTool(const char* name, PixelColor color, int goaln);

      // On selected it adds task at cursor for specified goal
      bool onSelected(EditorLayer* target) override;

      virtual void Hide();

      virtual void Show( Coordinates where );

      virtual void BindContents( Node * target );

   private:
      // Store visual representation
      shared_ptr<Goal> g;
      shared_ptr<GoalView> gv;
      PixelColor color;
      int goalindex;
   };

   // EditorToolWall has pixel in it and can place it to walls
   class EditorToolWall : public EditorToolPixel
   {
   public:

      EditorToolWall(const char* name, shared_ptr<Pixel> pixel) : EditorToolPixel(name, pixel) {}
      virtual void Show( Coordinates where );
      bool onSelected(EditorLayer* target) override;

   };


   // EditorToolEraser
   class EditorToolEraser : public EditorTool
   {
   public:

      enum class Type {
         ALL,
         TOP,
         GOAL,
         PIXEL,
         WALL,
         Last,
         First = ALL
      };

      // Eraser removes something from cursor point
      EditorToolEraser(Type t);

      bool onSelected(EditorLayer* target) override;

      virtual void Hide();

      virtual void Show( Coordinates where );

      virtual void BindContents( Node * target );

      ~EditorToolEraser();

   private:
      Sprite* icon;
      Type t;

   };


}


#endif // EDITORTOOL_H_

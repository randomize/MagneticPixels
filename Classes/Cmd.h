//===-- Command.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Command is universal argument and command name holder
//
//===---------------------------------------------------------===//

#pragma once
#ifndef COMMAND_H_
#define COMMAND_H_

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // Commands used everywhere in project
   class Command
   {
   public:
      virtual ~Command() {}
      virtual ErrorCode Execute() {
         ErrorCode rets = ErrorCode::RET_FAIL;
         for (auto f : toExec) {
            rets = f();
         }
         return rets;
      }
   protected:
      // This stores ready bounded functions
      vector<function<ErrorCode(void)>> toExec;
   };

//////////////////////////////////////////////////////////////////////////
// UI Commands
//////////////////////////////////////////////////////////////////////////

   // Request update of a pixel view with giveng ID
   struct CmdUIUpdatePixelView : public Command {
      enum class Reason{ 
         MOVING,   // mover pixel - when view must be moved
         CREATED,  // any pixel - created
         CHANGED,  // any pixel - changed
         WAKE,     // ams pixel - when added to asm
         ASLEEP,   // ams pixel - when removed from asm (undo)
         RESET,    // pixel - when reset happens, move home and asleep after
         DIED,     // pixel - when lost from assembly
         ACCEPT,   // pixel - when lost from assembly
         RESURRECT,//pixel - when undo lost pixel
         CANFALL,  // asm pixel - can fall
         CANTFALL, // asm pixel - can't fall
         TAPPED,   // any puxel - tap on pixel
         SMILED,   // colorful pixel - when on goal
         UNSMILED, // colorful pixel - when leaves goal
         KILLED,   // killer pixel - when kills
         BLOCKING  // mover pixel - when move cannot be performed
      };
      CmdUIUpdatePixelView(int pieceID, Reason whyUpdate) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, pieceID, whyUpdate));
         }
      }
      static unordered_map<string,function<ErrorCode(int,CmdUIUpdatePixelView::Reason)>> listners;
   };

   // Request update of a goal view with given ID
   struct CmdUIUpdateGoalView : public Command {
      enum class Reason{ 
         CREATED,        // goal view created
         CHANGED,        // goal view changed
         ACCEPTED,       // goal task hides with ani
         UNACCEPTED,     // goal task returns back
         HIGHLIGHT,      // starts jumpin
         UNHIGHLIGHT     // stops jumpin
      };
      CmdUIUpdateGoalView (int goalId, MPix::Coordinates task, Reason whyUpdate) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, goalId, task, whyUpdate));
         }
      }
      static unordered_map<string,function<ErrorCode(int,MPix::Coordinates,CmdUIUpdateGoalView::Reason)>> listners;
   };

   // Create all views, called on level load once
   struct CmdUICreateViews : public Command {
      CmdUICreateViews () {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };

   struct CmdUIAddAssembly : public Command {
      CmdUIAddAssembly () {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };

   struct CmdUIRemoveAsembly : public Command {
      enum DestroyType{ TYPE_HIDE, TYPE_VANISH, TYPE_RESETSHAKE, TYPE_ACCEPTEDSOLUTION};
      CmdUIRemoveAsembly (DestroyType type) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, type));
         }
      }
      static unordered_map<string,function<ErrorCode(CmdUIRemoveAsembly::DestroyType)>> listners;
   };

   struct CmdUIUpdateViewport : public Command {
      CmdUIUpdateViewport () {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };

   struct CmdUIAddPixel : public Command {
      CmdUIAddPixel(int pieceID) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, pieceID));
         }
      }
      static unordered_map<string,function<ErrorCode(int)>> listners;
   };

   struct CmdUIRemovePixel : public Command {
      CmdUIRemovePixel(int pieceID) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, pieceID));
         }
      }
      static unordered_map<string,function<ErrorCode(int)>> listners;
   };

   struct CmdUIMovePixel : public Command {
      CmdUIMovePixel(int pieceID, Coordinates toWhere) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, pieceID, toWhere));
         }
      }
      static unordered_map<string,function<ErrorCode(int,Coordinates)>> listners;
   };

   struct CmdUIHighlightPixel : public Command {
      CmdUIHighlightPixel(int pieceID, Direction whatDirection) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, pieceID, whatDirection));
         }
      }
      static unordered_map<string,function<ErrorCode(int,Direction)>> listners;
   };

   struct CmdUIContentTransform : public Command {
      CmdUIContentTransform(float scale, Point position) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, scale, position));
         }
      }
      static unordered_map<string,function<ErrorCode(float,Point)>> listners;
   };

   struct CmdUIPutMark : public Command {
      CmdUIPutMark (Point position) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, position));
         }
      }
      static unordered_map<string,function<ErrorCode(Point)>> listners;
   };

   struct CmdUIGameFinished : public Command {
      CmdUIGameFinished () {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second));
         }
      }
      static unordered_map<string,function<ErrorCode()>> listners;
   };

   // =========== Sound manager commands =========================

   struct CmdPlaySound : public Command {
      CmdPlaySound(const string& file) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, file ));
         }
      }
      static unordered_map<string,function<ErrorCode(const string&)>> listners;
   };

   // =========== General commands FG/BG =========================

   struct CmdDidEnterBG : public Command {
      CmdDidEnterBG(void) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second ));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };

   struct CmdWillEnterFG : public Command {
      CmdWillEnterFG(void) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second ));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };

   // ============= Gameplay manager commands ====================

   // Click
   struct CmdGameplayClick : public Command {
      CmdGameplayClick(Coordinates position) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, position ));
         }
      }
      static unordered_map<string,function<ErrorCode(Coordinates)>> listners;
   };

   // Move
   struct CmdGameplayMove : public Command {
      CmdGameplayMove (Direction whereMove) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, whereMove ));
         }
      }
      static unordered_map<string,function<ErrorCode(Direction)>> listners;
   };

   // After move
   struct CmdGameplayAfterMove : public Command {
      CmdGameplayAfterMove (Direction whereMove) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, whereMove ));
         }
      }
      static unordered_map<string,function<ErrorCode(Direction)>> listners;
   };

   // MoveFast
   struct CmdGameplayMoveFast : public Command {
      CmdGameplayMoveFast (Direction whereMove) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, whereMove ));
         }
      }
      static unordered_map<string,function<ErrorCode(Direction)>> listners;
   }; 
   
   // Grow
   struct CmdGameplayGrowAsm : public Command {
      CmdGameplayGrowAsm () {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };

   // UndoMove
   struct CmdGameplayUndoMove: public Command {
      CmdGameplayUndoMove(void) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second ));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };

   // RedoMove
   struct CmdGameplayRedoMove : public Command {
      CmdGameplayRedoMove (void) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second ));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };

   // RestartAssembly
   struct CmdGameplayRestartAssembly : public Command {
      CmdGameplayRestartAssembly (void) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second ));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };
/*
   struct CmdGameplayClick : public Command {
      CmdGameplayClick(void) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second ));
         }
      }
      static unordered_map<string,function<ErrorCode(void)>> listners;
   };
   */

   // ============= Editor commands ====================

   struct CmdEditorAction : public Command {
      enum class EditorAction { ED_SHOW_TOOLS, ED_HIDE_TOOLS };
      CmdEditorAction (EditorAction act) {
         for (auto f : listners) {
            toExec.push_back(std::bind( f.second, act));
         }
      }
      static unordered_map<string,function<ErrorCode(CmdEditorAction::EditorAction)>> listners;
   };
}


#endif // COMMAND_H_

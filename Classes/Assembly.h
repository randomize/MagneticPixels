//===-- Assembly.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/04
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Assembly represents group of pixels moving together
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELASSEMBLY_H_
#define PIXELASSEMBLY_H_

#include "EMBase.h"
#include "MPix.h"
#include "Historical.h"


namespace MPix {

   // Forward dependencies
   class IAssembled;
   class IColorful;
   class Context;

   // Shorthand
   typedef std::forward_list<shared_ptr<IAssembled>> PixelMap;

   // Assembly
   class Assembly
   {

   public: // ==============  Interface for GameplayManager

      // Created always with a start point 
      Assembly();

      ErrorCode CheckMove(const Context& context, Direction d);
      // Perform a move if can, return ErrorCode::RET_YES if occurred
      ErrorCode Move(const Context& context, Direction d);

      // Perform extension(growing) by adding more pixels, one wave only, returns:
      //   ErrorCode::RET_YES - if occurred
      //   ErrorCode::RET_NO  - if can't grow
      ErrorCode Grow(const Context& context);
      
      // Called before removing assembly no notify that solution accepted
      ErrorCode AcceptedSolution( const Context& context );

      // Called after move and before grow to assure that all pixels are sill alive, returns:
      //   ErrorCode::RET_YES - if lost
      //   ErrorCode::RET_NO  - if no lose
      ErrorCode CheckForLost( const Context& context);

      // Called by GameplayManager on Undo, restores contents and history of moves
      ErrorCode InitSnapshots( const Context& context );
      ErrorCode PopSnapshots(const Context& context, int count = 1, bool shake = false);
      ErrorCode PushSnapshot();

      // Pixels call this when performing grow wave
      ErrorCode AddPixel(const Context& context, shared_ptr<IAssembled> pixel);

      // Returns current move in assembly
      int GetMoveNumber() { return move_number; }
      void SetMoveNumber(int n) { move_number = n; }

      //  Check if no contents in assembly
      bool IsEmpty();

      // Check if all smiling
      bool AreAllSmiling();

      // Return rectangle occupied by assembly pixels currently
      MPix::Rectangle GetContentsRect() const;

      // Return contents pos and ids
      std::forward_list<std::pair<int, Coordinates>> & GetAssemblyPositions() const;

      // Get center of assembly
      Point GetCenter();

   private: // ================= Storage =================================

      // Contents store currently added pixels
      Historical<PixelMap> contents;

      // Last dir
      Historical<Direction> last_dir;

      // In assembly move number
      Historical<int> move_number;

      // Center
      Point center;
      void UpdateCenter();
      mutable std::forward_list<shared_ptr<IColorful>> lst_colors;
      mutable std::forward_list<std::pair<int,Coordinates>> lst_coords;

   };

}


#endif // PIXELASSEMBLY_H_

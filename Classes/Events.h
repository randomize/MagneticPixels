//===-- PixelEvents.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/03
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Events for pixels, if someone wants to send event, include this file
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELEVENTS_H_
#define PIXELEVENTS_H_

#include "EMBase.h"

namespace MPix {

   enum class PixelEvent {
      CREATED,                    // Pixel activated(on paly or when unhidden)
      SMILED,                     // Pixel smiled
      UNSMILED,                   // Pixel ended smile
      LOST_FROM_ASSEMBLY,         // Pixel lost from assembly for some reason(currently death only)
      UNDO_LOST_FROM_ASSEMBLY,    // Lost pixel returned to assembly
      LOST_FROM_WORLD,            // Pixel removed from world
      UNDO_LOST_FROM_WORLD,       // Pixel returned to world
      BLOCKED_MOVE,               // Pixel can't move
      ADDED_TO_ASSEMBLY,          // Pixel included to assembly(waked)
      UNDO_ADDED_TO_ASSEMBLY,     // Pixel excluded from assembly(caused by undo)
      ACCEPTED_GOAL,              // Pixel accepted by goal
      UNDO_ACCEPTED_GOAL,         // Pixel acceptance by goal undone
      MOVED,                      // Pixel moved
      UNDO_MOVED,                 // Pixel undo move
      DIED,                       // Any Pixel - died
      UNDO_DIED,                  // Pixel resurrected with undo
      RESET,                      // Reset sent when pixel undoes multiple steps
      CHANGED_DATA,               // Any data change, requires feedback, universal event
      CAN_FALL,                   // Pixel in assembly, send when can fall
      CANT_FALL,                  // Pixel in assembly, sent when can't fall
      TAPPED,                     // Any pixel - clicked
      KILLED,                     // Killer pixel - killed
      Last,
      First = CREATED
   };

   enum class GoalEvent {
      CREATED,
      HIGHTLIGHT,
      UNDO_HIGHLIGHT,
      UNHIGHLIGHT,
      UNDO_UNHIGHLIGHT,
      ACCEPTED,
      UNDO_ACCEPTED,
      Last,
      First = CREATED
   };

}


#endif // PIXELEVENTS_H_
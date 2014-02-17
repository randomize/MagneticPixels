//===-- Script.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/02/01
//  Author:      Mihailenco E. at Emboss Games, 2014
//  Description: Base class for game script
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SCRIPT_H_
#define SCRIPT_H_

#include "EMBase.h"
#include "MPix.h"

namespace MPix {

   // Forward dependencies
   class Context;

   // Script

   class Script
   {
   public:

      Script(const string& name = "Default");
      virtual ~Script();

      // Called when level first loaded
      void OnFirstMove( const Context& context );

      // Called when player clicked
      bool OnPlayerClicked( const Context& context, Coordinates where );

      // Called when player tries to move assembly
      bool OnPlayerMove( const Context& context, Direction where );

      // Called after successfully last grow
      void OnLastGrow(  const Context& context );

   public:

      const string& GetName() const;

   protected:

      string m_name;

      std::function<void(const Context&)>              on_first_move;
      std::function<bool(const Context&, Coordinates)> on_clicked;
      std::function<bool(const Context&, Direction)>   on_moved;
      std::function<void(const Context&)>              on_last_grow;

   private:

   };

}


#endif // SCRIPT_H_


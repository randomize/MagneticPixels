//===-- IWallPixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/22
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Wall pixels must implement this interface
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IWALLPIXEL_H_
#define IWALLPIXEL_H_

#include "EMBase.h"
#include "Pixel.h"
#include "IMoveBlocker.h"
#include "IProtector.h"

namespace MPix {

   // Forward dependencies
   class Context;


   // IWallPixel

   class IWallPixel :
      public IMoveBlocker, // Blocks moves
      public IProtector    // Protects pixels
   {
   public:

      // Called by other pixels to check if magnetism is possible
      //  from - magnetizing pixel
      //  to - magnetized pixel
      virtual bool blocksMagnetizm(const Context& context, shared_ptr<Pixel> from, shared_ptr<Pixel> to) = 0;


      // ------ Implement Undo/Redo  as voids -------------------------------------

      void InitSnapshots( const Context& context )override{} //Nothing
      void PushSnapshot( const Context& context )override{} //Nothing
      void ClearSnapshots( const Context& context )override{} //Nothing
      void PopSnapshots( const Context& context, int n )override{} //Nothing

   protected:

   private:

   };

}


#endif // IWALLPIXEL_H_
//===-- IMagneticPixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/22
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Magnetic pixels must implement this
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IMAGNETICPIXEL_H_
#define IMAGNETICPIXEL_H_

#include "EMBase.h"
#include "MPix.h"
#include "Pixel.h"

namespace MPix {

   // Forward dependencies
   class Context;
   class IAssembled;

   // IMagneticPixel
   class IMagnetic : public virtual Pixel
   {
   public:

      // Called by grower to define if can be sticked
      virtual shared_ptr<IAssembled> tryMagnetizeToMe(const Context& context, shared_ptr<Pixel> pix) = 0;

      // Override specific history
      void InitSnapshots( const Context& context ) override {} // Stores no history
      void PushSnapshot( const Context& context ) override {} // Stores no history
      void ClearSnapshots( const Context& context ) override {} // Stores no history
      void PopSnapshots( const Context& context, int n ) override {} // Stores no history

   private:

   };

}


#endif // IMAGNETICPIXEL_H_
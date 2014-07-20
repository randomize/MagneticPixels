//===-- AssembledMagneticBase.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/03
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: This mixture is base for assembled magnetic pixels
//
//===---------------------------------------------------------===//

#pragma once
#ifndef ASSEMBLEDMAGNETICBASE_H_
#define ASSEMBLEDMAGNETICBASE_H_

#include "ECBase.h"

#include "IMagnetic.h"
#include "IAssembled.h"
#include "IMoveBlocker.h"

namespace MPix {

   // Mixtures are combinations of base interfaces
   // They realize history order
   // They realize some very base methods
   // They DONT setup strategies
   // They must be inherited, rather then instanced
   // Final pixel model is some mixture with stetted up strategies and some overriding

   class AssembledMagneticBase :
      public IMagnetic,   // Can be affected by magnetism
      public IAssembled,  // Can be added to assembly
      public IMoveBlocker // Blocks moves
   {
   public:

      AssembledMagneticBase(PixelColor color);

      // Pixel base
      PixelType GetType() const override { return PixelType::ASSEMBLY_PIX; }

      // History methods
      void InitSnapshots( const Context& context ) override;
      void PushSnapshot( const Context& context ) override;
      void ClearSnapshots( const Context& context ) override;
      void PopSnapshots( const Context& context, int n ) override;

      // IAssembly
      bool canStartAssembly( const Context& context ) override;

      // IMagnetic
      shared_ptr<IAssembled> tryMagnetizeToMe( const Context& context, shared_ptr<Pixel> pix ) override;

   protected:

   private:

   };

}


#endif // ASSEMBLEDMAGNETICBASE_H_
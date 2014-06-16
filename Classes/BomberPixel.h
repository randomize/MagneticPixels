//===-- BomberPixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2014/02/19
//  Author:      Mihailenco E. at TheEndlessCat Games, 2014
//  Description: Bomb explodes after some time
//
//===---------------------------------------------------------===//

#pragma once
#ifndef BOMBERPIXEL_H_
#define BOMBERPIXEL_H_

#include "EMBase.h"
#include "EMNode.h"
#include "MPix.h"

#include "AssembledMagneticBase.h"
#include "IDynamic.h"
#include "IKilling.h"

namespace MPix {

   // MagneticPixel

   class BomberPixel :
      public  AssembledMagneticBase,
      public  IDynamic
   {
   public:

      // Register this pixel type to pixel factory
      EM_NODE_CHILD(BomberPixel);
      PixelType GetType() const override { return PixelType::ASSEMBLY_PIX; }

      // History methods
      void InitSnapshots( const Context& context ) override;
      void PushSnapshot( const Context& context ) override;
      void ClearSnapshots( const Context& context ) override;
      void PopSnapshots( const Context& context, int n ) override;

      bool canStartAssembly(const Context& context) override;
      void updateFinalize(const Context& context) override;

      int GetTime() { return time.GetValue();  }

      //IAssembled
      void AddToAssembly(const Context& context) override;

   public:

      Historical<int> time;

      BomberPixel(PixelColor color = PixelColor::RED, int time = 5);
      ~BomberPixel();

   };

}


#endif // BOMBERPIXEL_H_


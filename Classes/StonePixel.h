//===-- StonePixel.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/16
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Stone can kill cactuses, fall to pits
//
//===---------------------------------------------------------===//

#pragma once
#ifndef STONEPIXEL_H_
#define STONEPIXEL_H_

#include "EMBase.h"
#include "EMNode.h"

#include "SokobanBase.h"
#include "IKilling.h"

namespace MPix {

   // Forward dependencies


   // StonePixel

   class StonePixel :
      public SokobanBase,
      public IKilling
   {
   public:

      StonePixel();
      ~StonePixel();

      EM_NODE_CHILD(StonePixel);

      PixelType GetType() const override { return PixelType::STONE_PIX; }

      // History methods
      void InitSnapshots( const Context& context ) override;
      void PushSnapshot( const Context& context ) override;
      void ClearSnapshots( const Context& context ) override;
      void PopSnapshots( const Context& context, int n ) override;


   protected:

   private:

   };

}


#endif // STONEPIXEL_H_
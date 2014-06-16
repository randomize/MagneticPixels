//===-- Pitfall.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Pixel that kills when standing on it
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PITFALL_H_
#define PITFALL_H_

#include "ECBase.h"

#include "IKilling.h"

#include "ECNode.h"

namespace MPix {

   // Forward dependencies


   // Pitfall

   class Pitfall : public IKilling
   {
   public:

      // Register this pixel type to pixel factory
      ECNODE_CHILD(Pitfall);

      Pitfall();
      ~Pitfall();

      PixelType GetType() const override { return PixelType::PITFALL_PIX; }
      void InitSnapshots( const Context& context ) override;
      void PushSnapshot( const Context& context ) override;
      void ClearSnapshots( const Context& context ) override;
      void PopSnapshots( const Context& context, int n ) override;

   protected:

   private:

   };

}


#endif // PITFALL_H_
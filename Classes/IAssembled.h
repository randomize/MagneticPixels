//===-- IAssembled.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/22
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Assembled means pixels that can be assembled
//
//===---------------------------------------------------------===//

#pragma once
#ifndef IASSEMBLED_H_
#define IASSEMBLED_H_

#include "EMBase.h"
#include "Historical.h"

#include "IColorful.h"
#include "IMovable.h"
#include "IAlive.h"

#include "AnyBehavior.h"

namespace MPix {

   // Forward dependencies
   class IAssembled;

   // Strategy
   class GrowBehavior : public AnyBehavior {
   public:
      virtual bool growThis(shared_ptr<IAssembled> subject, const Context& context) = 0;
   };


   // IAssembled

   class IAssembled : 
      public IColorful,        // Must have color to be assemble able
      public IAlive,           // TODO: Maybe can have strong pixel with kill resistance ?
      public IMovableStepper   // Moves by steps
   {
   public:

      IAssembled(PixelColor color = PixelColor::RED); 

      // Must call when pixel is tapped to start assembly, allows or blocks
      virtual bool canStartAssembly(const Context& context) = 0;

      // Called when assembly made a move and tries to grow
      virtual bool growThis(const Context& context);


      // Assembly presence
      virtual void AddToAssembly( const Context& context );
      virtual void RemoveFromAssembly( const Context& context );
      bool IsInAssembly();

      // Falling possibility
      void HintCanFall();
      bool CheckCanFall( const Context& context );

      // Movable -- Override move to update smiles
      void Move(const Context& context, Direction dir) override;

      // Override specific history
      void InitSnapshots( const Context& context ) override;
      void PushSnapshot( const Context& context ) override;
      void ClearSnapshots( const Context& context ) override;
      void PopSnapshots( const Context& context, int n ) override;

      // Behavior setting
      void SetGrowBehavior(shared_ptr<GrowBehavior> new_behavior);
      void SetGrowBehavior(const string& name);

   protected:

      Historical<bool> in_assembly;

      void UpdateSmile( const Context& context );
      
      Historical<bool> canFall;

      bool hintCanFall;

      shared_ptr<GrowBehavior> grower;

   };

}


#endif // IASSEMBLED_H_
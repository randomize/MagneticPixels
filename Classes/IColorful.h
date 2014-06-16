//===-- IColorful.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/22
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: For pixels that have color and can satisfy goal
//
//===---------------------------------------------------------===//

#pragma once
#ifndef ICOLORFUL_H_
#define ICOLORFUL_H_

#include "EMBase.h"
#include "MPix.h"
#include "Pixel.h"
#include "Historical.h"

namespace MPix {

   // IColorful is used for goals to satisfy

   class IColorful : public virtual Pixel
   {
   public:
      IColorful(PixelColor color = PixelColor::RED);

      // Get information what color has pixel
      virtual PixelColor GetColor() const;
      virtual string GetColorAsString() const;

      // Updating smile
      virtual void SetSmile(const Context & context, bool is_smiling);
      virtual bool IsSmiling() const;
      virtual void Accept( const Context& context );

      // Override specific history
      void InitSnapshots( const Context& context ) override; 
      void PushSnapshot( const Context& context ) override; 
      void ClearSnapshots( const Context& context ) override; 
      void PopSnapshots( const Context& context, int n ) override; 

   private:

      // Pixel color
      PixelColor color;

      // Pixel smile
      Historical<bool> smile;
      Historical<bool> accepted_by_goal;

      // Generates events diff
      void DiffSmile( const Context& context, bool from, bool to);

   };

}


#endif // ICOLORFUL_H_
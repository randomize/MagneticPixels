//===-- RepeatedSprite.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/12/03
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Repeated sprite is batch node that draws
//  the same texture multiple times. It replaces GL_REPEAT for
//  non power of two textures, when number of tiling stays in
//  reasonable limitation.
//
//===---------------------------------------------------------===//

#pragma once
#ifndef REPEATEDSPRITE_H_
#define REPEATEDSPRITE_H_

#include "ECBase.h"

namespace MPix {

   // RepeatedSprite

   class RepeatedSprite : public SpriteBatchNode
   {
   public:
      static RepeatedSprite* create();


   protected:

   private:

   };

}


#endif // REPEATEDSPRITE_H_



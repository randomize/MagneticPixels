//===-- HSVArmature.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/28
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Armature with HSV shader
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HSVARMATURE_H_
#define HSVARMATURE_H_

#include "EMBase.h"
#include "MPix.h"
#include "HSVProtocol.h"

namespace MPix {

   // Forward dependencies


   // HSVArmature

   class HSVArmature : public Armature, public HSVProtocol
   {
   public:

      static HSVArmature* create(const char *name);

      void draw() override;

   protected:

   private:

   };

}


#endif // HSVARMATURE_H_
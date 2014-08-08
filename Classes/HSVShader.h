//===-- HSVShader.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/29
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Shader for colorful variety
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HSVSHADER_H_
#define HSVSHADER_H_

#include "ECBase.h"

namespace MPix {


   // Any shader program in game should have this interface
   // TODO: if more shaders -- place this to standalone file
   class ECShader : public GLProgram {
   public:

      // Called on android when enters foreground, reinitializes shader
      virtual void Reload() = 0;

   };

   // HSVShader program
   class HSVShader : public ECShader
   {
   public:

      static HSVShader* create();

      void Reload() override;
      std::string getDescription() const override;

      static void GenHSVMatrix(float * mat4x4, float hue, float saturation, float value);
      void SetHSVMatrix(float* mat4x4);

   private:

      bool init();

      GLuint xformLocation;

   };

}


#endif // HSVSHADER_H_

//===-- HSVShader.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/29
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Shader for colorful variety
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HSVSHADER_H_
#define HSVSHADER_H_

#include "EMBase.h"

namespace MPix {


   // Any shader program in game should have this interface
   // TODO: if more shaders -- place this to standalone file
   class EMShader : public GLProgram {
   public:

      // Called on android when enters foreground, reinitializes shader
      virtual void Reload() = 0;

   };

   // HSVShader program
   class HSVShader : public EMShader
   {
   public:

      static HSVShader* create();

      void Reload() override;

      static void GenHSVMatrix(float * mat4x4, float hue, float saturation, float value);
      void SetHSVMatrix(float* mat4x4);

   private:

      bool init();
      static kmMat3 GetRGB2YIQ();
      static kmMat3 GetYIQ2RGB();

      GLuint xformLocation;

   };

}


#endif // HSVSHADER_H_
//
//#ifdef GL_ES
//precision lowp float;
//#endif
//
//varying vec4 v_fragmentColor;
//varying vec2 v_texCoord;
//uniform sampler2D CC_Texture0;
//const mat3 rgb2yiq = mat3(0.299, 0.595716, 0.211456, 0.587, -0.274453, -0.522591, 0.114, -0.321263, 0.311135);
//const mat3 yiq2rgb = mat3(1.0, 1.0, 1.0, 0.9563, -0.2721, -1.1070, 0.6210, -0.6474, 1.7046);
//
//void main()	
//{
//   vec3 v_hsv = v_fragmentColor.xyz;
//   vec4 textureColor = texture2D(CC_Texture0, v_texCoord);
//   vec3 yColor = rgb2yiq * textureColor.rgb;
//   float ang = ( 1.0 - v_hsv.x ) * 2.0 * 3.1415926;
//   float cf = v_hsv.y * cos(ang);
//   float sf = v_hsv.y * sin(ang);
//   mat3 xform = mat3(v_hsv.z, 0, 0, 0, cf, sf, 0, -sf, cf);
//   gl_FragColor  = vec4(yiq2rgb * xform * yColor, textureColor.w * v_fragmentColor.w);
//}

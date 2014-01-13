//===-- HSVSpriteBatchNode.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/26
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Sprite batch node for HSVSprites
//
//===---------------------------------------------------------===//

#pragma once
#ifndef HSVSPRITEBATCHNODE_H_
#define HSVSPRITEBATCHNODE_H_

#include "EMBase.h"
#include "HSVProtocol.h"

namespace MPix {

   // Forward dependencies


   // HSVSpriteBatchNode

   class HSVSpriteBatchNode : public SpriteBatchNode, public HSVProtocol
   {
   public:

      static HSVSpriteBatchNode* create(const char* fileImage, int capacity /* = DEFAULT_CAPACITY */);
      ~HSVSpriteBatchNode();

      //bool initWithTexture(Texture2D* texture, int capacity) override;
      void draw() override;

      void initProgram();
      void listenBackToForeground(Object *obj);
      void SetBlur(float f);

   protected:

      void buildUniforms();
      void setUniforms();

      const char* GetShaderFilename();

      Point blur_;
      GLfloat    sub_[4];

      GLuint    blurLocation;
      GLuint    subLocation;

   };

}


#endif // HSVSPRITEBATCHNODE_H_
#include "HSVSprite.h"
#include "HSVShader.h"
#include "ContentManager.h"

using namespace MPix;

//====---------------------------------------------======//

HSVSprite* MPix::HSVSprite::create( const char *filename )
{
   auto fab = new HSVSprite();
   if ( fab->initWithFile(filename) == false ) {
      delete fab;
      return nullptr;
   }
   fab->autorelease();
   return fab;
}

HSVSprite* MPix::HSVSprite::createWithSpriteFrameName( const char *spriteFrameName )
{
   auto fab = new HSVSprite();
   if ( fab->initWithSpriteFrameName(spriteFrameName) == false ) {
      delete fab;
      return nullptr;
   }
   fab->autorelease();
   return fab;
}

bool MPix::HSVSprite::initWithTexture( Texture2D* texture, const Rect& rect )
{
   if( Sprite::initWithTexture(texture, rect) )
   {
      auto pProgram = ContentManager::getInstance().GetShader("hsvshader");
      setShaderProgram(pProgram);
      return true;
   }
   return false;
}

void MPix::HSVSprite::draw()
{
   GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );
   BlendFunc blend = getBlendFunc();
   GL::blendFunc(blend.src, blend.dst);

   auto sp = static_cast<HSVShader*> (getShaderProgram());
   sp->use();
   sp->setUniformsForBuiltins();
   sp->SetHSVMatrix(GetHSVMatrix());

   GL::bindTexture2D( getTexture()->getName());

#define kQuadSize sizeof(_quad.bl)
   long offset = (long)&_quad;

   // vertex
   int diff = offsetof( V3F_C4B_T2F, vertices);
   glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

   // texCoods
   diff = offsetof( V3F_C4B_T2F, texCoords);
   glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

   // color
   diff = offsetof( V3F_C4B_T2F, colors);
   glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   CC_INCREMENT_GL_DRAWS(1);
}



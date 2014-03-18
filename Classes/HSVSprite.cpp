#include "HSVSprite.h"
#include "HSVShader.h"
#include "ContentManager.h"

using namespace MPix;

//====---------------------------------------------======//

HSVSprite* HSVSprite::create( const string& file_name )
{
   auto fab = new HSVSprite();
   if ( fab->initWithFile(file_name) == false ) {
      delete fab;
      return nullptr;
   }
   fab->autorelease();
   return fab;
}

HSVSprite* HSVSprite::createWithSpriteFrameName(const string& frame_name)
{
   auto fab = new HSVSprite();
   if ( fab->initWithSpriteFrameName(frame_name) == false ) {
      delete fab;
      return nullptr;
   }
   fab->autorelease();
   return fab;
}


bool HSVSprite::initWithTexture( Texture2D* texture, const Rect& rect )
{
   if( Sprite::initWithTexture(texture, rect) )
   {
      auto pProgram = ContentManager::getInstance().GetShader("hsvshader");
      setShaderProgram(pProgram);
      return true;
   }
   return false;
}

void HSVSprite::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
   _renderCommand.init(_globalZOrder);
   _renderCommand.func = CC_CALLBACK_0(HSVSprite::onDraw, this, transform, transformUpdated);
   renderer->addCommand(&_renderCommand);
}

void HSVSprite::onDraw(const kmMat4 &transform, bool transformUpdated)
{

   auto shader = getShaderProgram();
   shader->use();
   shader->setUniformsForBuiltins(transform);

   static_cast<HSVShader*> (getShaderProgram())->SetHSVMatrix(GetHSVMatrix());

   GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );
   GL::blendFunc(_blendFunc.src, _blendFunc.dst);
   GL::bindTexture2D( getTexture()->getName());
   
   //
   // Attributes
   //
   #define kQuadSize sizeof(_quad.bl)
   size_t offset = (size_t)&_quad;
   
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
   CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);

}




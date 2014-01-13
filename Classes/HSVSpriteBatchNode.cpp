#include "HSVSpriteBatchNode.h"

using namespace MPix;

//====---------------------------------------------======//

HSVSpriteBatchNode* MPix::HSVSpriteBatchNode::create( const char* fileImage, int capacity /* = DEFAULT_CAPACITY */ )
{
   auto fab = new HSVSpriteBatchNode();
   if (fab->initWithFile(fileImage, capacity)) {
      fab->autorelease();

      NotificationCenter::getInstance()->addObserver(fab,
         callfuncO_selector(HSVSpriteBatchNode::listenBackToForeground),
         EVNET_COME_TO_FOREGROUND,
         NULL);

      fab->initProgram();
      return fab;
   }
   delete fab;
   return nullptr;
}

void MPix::HSVSpriteBatchNode::listenBackToForeground( Object *obj )
{
   setShaderProgram(NULL);
   initProgram();
}

void MPix::HSVSpriteBatchNode::draw()
{

    // Optimization: Fast Dispatch
    if( _textureAtlas->getTotalQuads() == 0 )
    {
        return;
    }

   getShaderProgram()->use();
   getShaderProgram()->setUniformsForBuiltins();
   setUniforms();

   arrayMakeObjectsPerformSelector(_children, updateTransform, Sprite*);

   GL::blendFunc( _blendFunc.src, _blendFunc.dst );

   _textureAtlas->drawQuads();

}

void MPix::HSVSpriteBatchNode::initProgram()
{
   GLchar * fragSource = (GLchar*) String::createWithContentsOfFile(
      FileUtils::getInstance()->fullPathForFilename(GetShaderFilename()).c_str())->getCString();
   auto pProgram = new GLProgram();
   pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
   setShaderProgram(pProgram);
   pProgram->release();

   CHECK_GL_ERROR_DEBUG();

   getShaderProgram()->addAttribute(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
   getShaderProgram()->addAttribute(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
   getShaderProgram()->addAttribute(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);

   CHECK_GL_ERROR_DEBUG();

   getShaderProgram()->link();

   CHECK_GL_ERROR_DEBUG();

   getShaderProgram()->updateUniforms();

   CHECK_GL_ERROR_DEBUG();

   buildUniforms();

   CHECK_GL_ERROR_DEBUG();
}


void MPix::HSVSpriteBatchNode::buildUniforms()
{
   auto s = getTexture()->getContentSizeInPixels();

   blur_ = Point(1/s.width, 1/s.height);
   sub_[0] = sub_[1] = sub_[2] = sub_[3] = 0;

   subLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "substract");
   blurLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "blurSize");
}

void MPix::HSVSpriteBatchNode::setUniforms()
{
   getShaderProgram()->setUniformLocationWith2f(blurLocation, blur_.x, blur_.y);
   getShaderProgram()->setUniformLocationWith4fv(subLocation, sub_, 1);
}

const char* MPix::HSVSpriteBatchNode::GetShaderFilename()
{
   return "example_Blur.c";

}

void MPix::HSVSpriteBatchNode::SetBlur( float f )
{
    auto s = getTexture()->getContentSizeInPixels();
    blur_ = Point(1/s.width, 1/s.height);
    blur_ = blur_ * f;
}

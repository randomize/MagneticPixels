#include "HSVShader.h"

using namespace MPix;

//====---------------------------------------------======//

const char* shaderData = 
"\n\
#ifdef GL_ES\n\
precision lowp float;\n\
#endif\n\
\n\
varying vec4 v_fragmentColor;\n\
varying vec2 v_texCoord;\n\
uniform sampler2D CC_Texture0;\n\
uniform mat4 xform;\n\
\n\
void main() \n\
{\n\
   //gl_FragColor = xform * v_fragmentColor * texture2D(CC_Texture0, v_texCoord);\n\
   gl_FragColor = xform * texture2D(CC_Texture0, v_texCoord) * v_fragmentColor;\n\
   //gl_FragColor = vec4(1,0,0,1);\n\
}\n\
";

HSVShader* MPix::HSVShader::create()
{
   auto fab = new HSVShader();
   if ( fab->init() ) {
      fab->autorelease();
      return fab;
   }
   delete fab;
   return nullptr;
}

bool MPix::HSVShader::init()
{
   initWithVertexShaderByteArray(ccPositionTextureColor_vert, shaderData);

   CHECK_GL_ERROR_DEBUG();

   addAttribute(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
   addAttribute(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
   addAttribute(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);

   CHECK_GL_ERROR_DEBUG();

   link();

   CHECK_GL_ERROR_DEBUG();

   updateUniforms();

   CHECK_GL_ERROR_DEBUG();

   xformLocation = glGetUniformLocation( getProgram(), "xform");
   CHECK_GL_ERROR_DEBUG();

   return true;

}

void MPix::HSVShader::Reload()
{
   reset();
   init();
}

void MPix::HSVShader::GenHSVMatrix( float * xform, float hue, float saturation, float value )
{
   kmMat3 result = GetYIQ2RGB();
   kmMat3 inv = GetRGB2YIQ();
   kmMat3 rot;

   kmScalar cf = saturation * cosf(hue);
   kmScalar sf = saturation * sinf(hue);

   rot.mat[0] = value; rot.mat[1] = 0;    rot.mat[2] = 0;
   rot.mat[3] = 0;     rot.mat[4] = cf;   rot.mat[5] = -sf;
   rot.mat[6] = 0;     rot.mat[7] = sf;   rot.mat[8] = cf;

   // For some reason(maybe Chinese reason) kmMat3Multiply multiplies in wrong order
   // Actually out = M2 x M1 while expected out = M1 x M2
   // I spend 2 hours finding why multiplication is wrong... fuck kazmath library 
   kmMat3Multiply(&result, &rot, &result);
   kmMat3Multiply(&result, &inv, &result);

   xform[0]  = result.mat[0];
   xform[1]  = result.mat[3];
   xform[2]  = result.mat[6];
   xform[3]  = 0;
   xform[4]  = result.mat[1];
   xform[5]  = result.mat[4];
   xform[6]  = result.mat[7];
   xform[7]  = 0;
   xform[8]  = result.mat[2];
   xform[9]  = result.mat[5];
   xform[10] = result.mat[8];
   xform[11] = 0;
   xform[12] = 0;
   xform[13] = 0;
   xform[14] = 0;
   xform[15] = 1;
}

kmMat3 MPix::HSVShader::GetRGB2YIQ()
{
   kmMat3 m;
   float d[] = { 0.299f, 0.587f, 0.114f, 0.595716f, -0.274453f, -0.321263f, 0.211456f, -0.522591f, 0.31113f };
   kmMat3Fill(&m, d);
   return m;
}

kmMat3 MPix::HSVShader::GetYIQ2RGB()
{
   kmMat3 m;
   float d[] = { 1, 0.9563f, 0.6212f, 1, -0.2721f, -0.6474f, 1, -1.1070f, 1.7046f };
   kmMat3Fill(&m, d);
   return m;
}

void MPix::HSVShader::SetHSVMatrix( float* mat4x4 )
{
   // TODO: cache
   setUniformLocationWithMatrix4fv(xformLocation, mat4x4, 1);
}

std::string MPix::HSVShader::getDescription() const
{
   return "Shader implements hue shift filtering";
}

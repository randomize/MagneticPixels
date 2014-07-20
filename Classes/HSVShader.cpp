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
   initWithByteArrays(ccPositionTextureColor_vert, shaderData);

   CHECK_GL_ERROR_DEBUG();

   bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
   bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
   bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);

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

// Kazmath was deprecated, using own copypaste of multiplication here
// kmMat3* kmMat3Multiply(kmMat3* pOut, const kmMat3* pM1, const kmMat3* pM2);
// Note: pOut = M2 x M1 i.e reversed (kazmath order)
inline void Multiply3x3Matrix(float* pOut, float* m1, float* m2)
{
   pOut[0] = m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2];
   pOut[1] = m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2];
   pOut[2] = m1[2] * m2[0] + m1[5] * m2[1] + m1[8] * m2[2];
   pOut[3] = m1[0] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5];
   pOut[4] = m1[1] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5];
   pOut[5] = m1[2] * m2[3] + m1[5] * m2[4] + m1[8] * m2[5];
   pOut[6] = m1[0] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8];
   pOut[7] = m1[1] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8];
   pOut[8] = m1[2] * m2[6] + m1[5] * m2[7] + m1[8] * m2[8];
}

void MPix::HSVShader::GenHSVMatrix( float * xform, float hue, float saturation, float value )
{
   float result[9] = { 1, 0.9563f, 0.6212f, 1, -0.2721f, -0.6474f, 1, -1.1070f, 1.7046f };
   float inv[9] = { 0.299f, 0.587f, 0.114f, 0.595716f, -0.274453f, -0.321263f, 0.211456f, -0.522591f, 0.31113f };

   float cf = saturation * cosf(hue);
   float sf = saturation * sinf(hue);

   float rot[9] = { value, 0, 0, 0, cf, -sf, 0, sf, cf };

   // For some reason(maybe Chinese reason) kmMat3Multiply multiplies in wrong order
   // Actually out = M2 x M1 while expected out = M1 x M2
   // I spend 2 hours finding why multiplication is wrong... fuck kazmath library
   Multiply3x3Matrix(result, rot, result);
   Multiply3x3Matrix(result, inv, result);

   xform[0]  = result[0];
   xform[1]  = result[3];
   xform[2]  = result[6];
   xform[3]  = 0;
   xform[4]  = result[1];
   xform[5]  = result[4];
   xform[6]  = result[7];
   xform[7]  = 0;
   xform[8]  = result[2];
   xform[9]  = result[5];
   xform[10] = result[8];
   xform[11] = 0;
   xform[12] = 0;
   xform[13] = 0;
   xform[14] = 0;
   xform[15] = 1;
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

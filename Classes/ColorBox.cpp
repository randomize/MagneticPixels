#include "ColorBox.h"
#include "MPix.h"

using namespace MPix;

void ColorBox::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(ColorBox::onDraw, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
}

ColorBox* ColorBox::create()
{
   ColorBox *pRet = new ColorBox();
   pRet->autorelease();
   pRet->col = Color4F(1.0f, 0.0f, 1.0f, 1.0f);
   pRet->borders_col = Color4F(0.0f, 1.0f, 1.0f, 1.0f);
   pRet->borders = 0;
   pRet->width = 5.0f;
   return pRet;
}

void ColorBox::SetColor( Color4F col )
{
   this->col = col;
}

void ColorBox::SetBorders( unsigned borders, Color4F col )
{
   this->borders = borders;
   borders_col = col;
}

void ColorBox::onDraw(const kmMat4 &transform, bool transformUpdated)
{

   // Setup matrices
   kmGLPushMatrix();
   kmGLLoadMatrix(&transform);


   // Draw
   auto col = this->col;
   col.a *= (this->_displayedOpacity / 255.0f);

   auto ap = getAnchorPoint();
   Point p00 = ap * MPIX_CELL_SIZE * -1;
   Point p11 = p00 + MPIX_CELL_SIZE_P;
   Point p10 = p00; p10.x += MPIX_CELL_SIZE;
   Point p01 = p00; p01.y += MPIX_CELL_SIZE;

   DrawPrimitives::drawSolidRect(p00, p11, col);
   auto p = Point(width,width);
   if (borders) {
      if ( borders & BORDER_LEFT ) 
         DrawPrimitives::drawSolidRect(p00-p, p01+p, borders_col);
      if ( borders & BORDER_RIGHT )
          DrawPrimitives::drawSolidRect(p10-p, p11+p, borders_col);
      if ( borders & BORDER_UP )
          DrawPrimitives::drawSolidRect(p01-p, p11+p, borders_col);
      if ( borders & BORDER_DOWN )
          DrawPrimitives::drawSolidRect(p00-p, p10+p, borders_col);
   }

   // Restore matrix
   kmGLPopMatrix();
}

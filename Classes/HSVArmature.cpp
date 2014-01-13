#include "HSVArmature.h"
#include "HSVShader.h"
#include "ContentManager.h"

using namespace MPix;

//====---------------------------------------------======//

void MPix::HSVArmature::draw()
{

   if (_parentBone == nullptr && _batchNode == nullptr)
   {
      CC_NODE_DRAW_SETUP();
      static_cast<HSVShader*> (getShaderProgram())->SetHSVMatrix(GetHSVMatrix());
   }

   for (auto& object : _children)
   {
      if (Bone *bone = dynamic_cast<Bone *>(object))
      {
         Node *node = bone->getDisplayRenderNode();

         if (nullptr == node)
            continue;

         switch (bone->getDisplayRenderNodeType())
         {
            case CS_DISPLAY_SPRITE:
            {
               Skin *skin = static_cast<Skin *>(node);
               skin->updateTransform();

               bool blendDirty = bone->isBlendDirty();

               if (blendDirty)
               {
                  skin->setBlendFunc(bone->getBlendFunc());
               }
               skin->draw();
            }
            break;

            case CS_DISPLAY_ARMATURE:
            {
               node->draw();
            }
            break;

            default:
            {
               node->visit();
               CC_NODE_DRAW_SETUP();
               static_cast<HSVShader*> (getShaderProgram())->SetHSVMatrix(GetHSVMatrix());
            }
            break;
         }
      }
      else if (Node *node = dynamic_cast<Node *>(object))
      {
         node->visit();
         CC_NODE_DRAW_SETUP();
         static_cast<HSVShader*> (getShaderProgram())->SetHSVMatrix(GetHSVMatrix());
      }
   }

    /*{ // Replace each occurance of CC_NODE_DRAW_SETUP in original draw with this
        CC_NODE_DRAW_SETUP();
        static_cast<HSVShader*> (getShaderProgram())->SetHSVMatrix(GetHSVMatrix());
        //GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    }*/
}

HSVArmature* MPix::HSVArmature::create( const char *name )
{
    HSVArmature *armature = new HSVArmature();
    if (armature && armature->init(name))
    {
        armature->autorelease();
        auto pProgram = ContentManager::getInstance().GetShader("hsvshader");
        armature->setShaderProgram(pProgram);
        return armature;
    }
    delete armature;
    return nullptr;
}

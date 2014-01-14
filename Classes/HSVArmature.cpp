#include "HSVArmature.h"
#include "HSVShader.h"
#include "ContentManager.h"

using namespace MPix;

//====---------------------------------------------======//


HSVArmature* MPix::HSVArmature::create( const char *name )
{
    HSVArmature *armature = new HSVArmature();

    if (armature && armature->init(name))
    {
        armature->autorelease();
        auto pProgram = ContentManager::getInstance().GetShader("hsvshader");

        // Set my program(actually useless, not used later)
        armature->setShaderProgram(pProgram);

        return armature;
    }
    delete armature;
    return nullptr;
}

/*void MPix::HSVArmature::draw()
{

   auto pProgram = ContentManager::getInstance().GetShader("hsvshader");
   // Make sure all children use shader
        for (auto c : getChildren()) {
           if (Bone* bone = dynamic_cast<Bone*> (c)) {
              Node *node = bone->getDisplayRenderNode();
              Skin *skin = dynamic_cast<Skin *>(node);
              if (skin) {
                 skin->setShaderProgram(pProgram);
              }
           }
        }

   _groupCommand.init(0, _vertexZ);

   Renderer *renderer = Director::getInstance()->getRenderer();
   renderer->addCommand(&_groupCommand);
   renderer->pushGroup(_groupCommand.getRenderQueueID());

   _beginCommand.init(0, _vertexZ);
   _beginCommand.func = CC_CALLBACK_0(HSVArmature::OnBegin, this);
   renderer->addCommand(&_beginCommand);

   Armature::draw();

   _endCommand.init(0, _vertexZ);
   _endCommand.func = CC_CALLBACK_0(HSVArmature::OnEnd, this);
   renderer->addCommand(&_endCommand);

   renderer->popGroup();
   
}

void MPix::HSVArmature::OnBegin()
{
   getShaderProgram()->use();
   // Set color
   dynamic_cast<HSVShader*> (getShaderProgram())->SetHSVMatrix(GetHSVMatrix());
}

void MPix::HSVArmature::OnEnd()
{

}*/

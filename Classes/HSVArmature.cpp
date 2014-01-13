#include "HSVArmature.h"
#include "HSVShader.h"
#include "ContentManager.h"

using namespace MPix;

//====---------------------------------------------======//

void MPix::HSVArmature::draw()
{

    if (_parentBone == NULL)
    {
        CC_NODE_DRAW_SETUP();
        static_cast<HSVShader*> (getShaderProgram())->SetHSVMatrix(GetHSVMatrix());
        GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    }

    Object *object = NULL;
    CCARRAY_FOREACH(_children, object)
    {
        if (Bone *bone = dynamic_cast<Bone *>(object))
        {
            DisplayManager *displayManager = bone->getDisplayManager();
            Node *node = displayManager->getDisplayRenderNode();

            if (NULL == node)
                continue;

            switch (displayManager->getCurrentDecorativeDisplay()->getDisplayData()->displayType)
            {
            case CS_DISPLAY_SPRITE:
            {
                Skin *skin = static_cast<Skin *>(node);

                TextureAtlas *textureAtlas = skin->getTextureAtlas();
                BlendType blendType = bone->getBlendType();
                if(_atlas != textureAtlas || blendType != BLEND_NORMAL)
                {
                    if (_atlas)
                    {
                        _atlas->drawQuads();
                        _atlas->removeAllQuads();
                    }
                }

                _atlas = textureAtlas;
                if (_atlas->getCapacity() == _atlas->getTotalQuads() && !_atlas->resizeCapacity(_atlas->getCapacity() * 2))
                    return;

                skin->updateTransform();

                if (blendType != BLEND_NORMAL)
                {
                    updateBlendType(blendType);
                    _atlas->drawQuads();
                    _atlas->removeAllQuads();
                    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
                }
            }
            break;
            case CS_DISPLAY_ARMATURE:
            {
                Armature *armature = static_cast<Armature *>(node);

                TextureAtlas *textureAtlas = armature->getTextureAtlas();
                if(_atlas != textureAtlas)
                {
                    if (_atlas)
                    {
                        _atlas->drawQuads();
                        _atlas->removeAllQuads();
                    }
                }
                armature->draw();
            }
            break;
            default:
            {
                if (_atlas)
                {
                    _atlas->drawQuads();
                    _atlas->removeAllQuads();
                }
                node->visit();

                CC_NODE_DRAW_SETUP();
                static_cast<HSVShader*> (getShaderProgram())->SetHSVMatrix(GetHSVMatrix());
                GL::blendFunc(_blendFunc.src, _blendFunc.dst);
            }
            break;
            }
        }
        else if(Node *node = dynamic_cast<Node *>(object))
        {
            if (_atlas)
            {
                _atlas->drawQuads();
                _atlas->removeAllQuads();
            }
            node->visit();

            CC_NODE_DRAW_SETUP();
            static_cast<HSVShader*> (getShaderProgram())->SetHSVMatrix(GetHSVMatrix());
            GL::blendFunc(_blendFunc.src, _blendFunc.dst);
        }
    }

    if(_atlas && !_batchNode && _parentBone == NULL)
    {
        _atlas->drawQuads();
        _atlas->removeAllQuads();
    }
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

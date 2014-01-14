#include "ContentManager.h"

// To have access to Z orders
#include "GameMain.h"

#include "HSVShader.h"
#include "HSVSprite.h"

using namespace MPix;

// Stupid cocos listeners dont work with non-Object instances
// I don't want to have ContentManager subclassing Object, so 
// this little helper handles the situation
class ListenerDelegate : public Object {
public:

   void listenEnterForeground(Object* ) {
      callback();
   }

   std::function<void(void)> callback;

} listener;

MPix::ContentManager::ContentManager()
{
   EM_LOG_INFO("Content manager created");

   listener.callback = std::bind(&ContentManager::ReloadShaders, this);

   NotificationCenter::getInstance()->addObserver(
      &listener,
      callfuncO_selector(ListenerDelegate::listenEnterForeground),
      EVENT_COME_TO_FOREGROUND,
      nullptr
   );

}

MPix::ContentManager::~ContentManager()
{
   NotificationCenter::getInstance()->removeObserver(
      &listener,
      EVENT_COME_TO_FOREGROUND
   );

}

void MPix::ContentManager::LoadResources()
{
   CreateShaders();
   CreateAnimations();
   CreateSprites();
}

void MPix::ContentManager::CreateAnimations()
{

   // load armatures
   ArmatureDataManager::getInstance()->addArmatureFileInfo("magnetic.png", "magnetic.plist", "magnetic.xml");
   auto texture = Director::getInstance()->getTextureCache()->addImage("magnetic.png");
   texture->setAliasTexParameters();

}

void MPix::ContentManager::CreateSprites()
{
   SpriteFrameCache* cache = SpriteFrameCache::getInstance();

   // Scene objects // TODO: merge to one sprite sheet
   //cache->addSpriteFramesWithFile("scene.plist");
   resources.emplace("wall_pixel", "wall.png");
   resources.emplace("goal_bg", "pixel_goal.png");
   //resources.emplace()

   // UI Objects
   //cache->addSpriteFramesWithFile("ui.plist");
   //resources.emplace("goal_bg", "pixel_goal.png");
   //..

   // Editor
   cache->addSpriteFramesWithFile("ed.plist");
}

void MPix::ContentManager::CreateShaders()
{
   auto shader = HSVShader::create();
   shader->retain();
   shaders.emplace("hsvshader", shader);
}

GLProgram* MPix::ContentManager::GetShader( const string& name )
{
   auto pt = shaders.find(name);
   if (pt != shaders.end()) {
      return pt->second;
   }
   return nullptr;
}


void MPix::ContentManager::ReloadShaders()
{
   // Restore shaders
   for (auto sh : shaders) {
      sh.second->Reload();
   }
}

Sprite* MPix::ContentManager::GetSimpleSprite(const string& name)
{

   EM_LOG_DEBUG("Loading simple sprite resource: " + name);

   Sprite* fab = nullptr;

   auto it = resources.find(name);
   if (it == resources.end())
   {
       fab = Sprite::create("dummy.png");
       EM_LOG_WARNING("Not found resource " + name);
   }
   else
   {
      fab = Sprite::create(it->second);
   }

   assert(fab);
   return fab;

}

HSVSprite* MPix::ContentManager::GetHSVSprite(const string& name)
{
   EM_LOG_DEBUG("Loading HSV sprite resource: " + name);

   HSVSprite* fab = nullptr;

   auto it = resources.find(name);
   if (it == resources.end())
   {
       fab = HSVSprite::create("dummy.png");
       EM_LOG_WARNING("Not found resource " + name);
   }
   else
   {
      fab = HSVSprite::create(it->second);
   }

   assert(fab);
   return fab;
}




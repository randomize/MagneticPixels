#include "ContentManager.h"

// To have access to Z orders
#include "GameMain.h"

#include "HSVShader.h"
#include "HSVSprite.h"
#include "EMAnimation.h"

using namespace MPix;

// Stupid cocos listeners don't work with non-Object instances
// I don't want to have ContentManager deriving Object class, so 
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
   auto t_cache = Director::getInstance()->getTextureCache();
   auto arm_man = ArmatureDataManager::getInstance();

   // Load mimics for magnetic
   arm_man->addArmatureFileInfo("magnetic_mimics.png", "magnetic_mimics.plist", "magnetic_mimics.xml");
   auto texture = t_cache->addImage("magnetic_mimics.png");
   texture->setAliasTexParameters();
   resources.emplace("magnetic_mimics", "magnetic_0");
   resources.emplace("socoban_0", "magnetic_0");

}

void MPix::ContentManager::CreateSprites()
{
   SpriteFrameCache* cache = SpriteFrameCache::getInstance();

   // Scene objects // TODO: merge to one sprite sheet
   //cache->addSpriteFramesWithFile("scene.plist");
   resources.emplace("wall_element", "wall.png");
   resources.emplace("goal_bg", "pixel_goal.png");
   resources.emplace("magnetic_bg_smash", "magnetic_bg_smash.png");
   resources.emplace("magnetic_bg_norm", "magnetic_bg_norm.png");
   resources.emplace("pitfall_bg", "pitfall_pixel.png");
   resources.emplace("cactus_bg", "needle_small.png");

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
      assert(fab);
   }
   else
   {
      fab = HSVSprite::create(it->second);
      if (fab == nullptr) {
         fab = HSVSprite::create("dummy.png");
         EM_LOG_WARNING("Not found resource " + name);
         assert(fab);
      }
   }

   return fab;
}

EMAnimation* MPix::ContentManager::GetAnimation(const string& name)
{
   EM_LOG_DEBUG("Loading HSV sprite resource: " + name);

   EMAnimation* fab = nullptr;

   auto it = resources.find(name);
   if (it == resources.end())
   {
       EM_LOG_WARNING("Not found resource " + name);
       assert(false);
   }
   else
   {
      fab = EMAnimation::create(it->second);
   }

   assert(fab);
   return fab;
}

Node* MPix::ContentManager::GetNode(const string& node_name)
{
   auto it = nodes.find(node_name);

   if (it != nodes.end()) {
      return it->second;
   }

   return nullptr;
}

void MPix::ContentManager::AddNode(Node* new_node, const string& node_name)
{
   assert(GetNode(node_name) == nullptr);
   nodes.emplace(node_name, new_node);
   new_node->retain();
}

void MPix::ContentManager::RemoveNode(const string& node_name)
{
   auto n = GetNode(node_name);
   assert(n);
   n->release();
   nodes.erase(node_name);
}

string MPix::ContentManager::GetBaseFont()
{
   return "fonts/Exo2-Medium.ttf";
}

string MPix::ContentManager::GetBaseFontLight()
{
   return "fonts/Exo2-Light.ttf";
}

string MPix::ContentManager::GetLogoFont()
{
   return "fonts/Exo2-Thin.ttf";
}












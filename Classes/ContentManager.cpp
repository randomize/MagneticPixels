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
class ListenerDelegate : public Ref {
public:

   void listenEnterForeground(Ref* ) {
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
   animations.emplace("magnetic_mimics", "magnetic_0");

}

void MPix::ContentManager::CreateSprites()
{
   SpriteFrameCache* cache = SpriteFrameCache::getInstance();

   // Scene objects
   // TODO: merge object to one sprite sheet atlas
   //cache->addSpriteFramesWithFile("pixels.plist");
   resources.emplace("wall_element", "wall.png");
   resources.emplace("goal_bg", "pixel_goal.png");
   resources.emplace("magnetic_bg_smash", "magnetic_bg_smash.png");
   resources.emplace("magnetic_bg_norm", "magnetic_bg_norm.png");
   resources.emplace("pitfall_bg", "pitfall_pixel.png");
   resources.emplace("cactus_bg", "needle_small.png");
   resources.emplace("bomber_bg", "bomber_bg.png");
   resources.emplace("bomber_eyes", "bomber_eyes.png");
   resources.emplace("stone_bg", "stone_pixel.png");
   resources.emplace("stone_eye", "stone_pixel_eye.png");
   resources.emplace("mutant_dots", "mutant_color.png");

   // UI Objects
   // TODO: merge UI sprites to one sprite sheet atlas
   //cache->addSpriteFramesWithFile("ui.plist");
   //resources.emplace("goal_bg", "pixel_goal.png");
   resources.emplace("level_border", "level_border.png");
   resources.emplace("right_arrow", "right_arrow.png");
   resources.emplace("level_lock", "small_lock.png");
   resources.emplace("scroll_indicator", "small_dot.png");

   // Editor
   resources.emplace("trash_can", "trash_can.png");

   // BG
   resources.emplace("bg_1", "bg/01.jpg");
   resources.emplace("bg_2", "bg/02.jpg");
   resources.emplace("bg_3", "bg/03.jpg");
   resources.emplace("bg_4", "bg/04.jpg");
   resources.emplace("bg_5", "bg/05.jpg");

   // Preload all
   auto t_cache = Director::getInstance()->getTextureCache();
   for (auto& s : resources) {
      auto texture = t_cache->addImage(s.second);
      if (texture) {
         auto sz = texture->getContentSizeInPixels();
         EM_LOG_DEBUG("Cached: " + s.second + " [ " + sz.width + ", " + sz.height + "]");
      }
      else {
         EM_LOG_WARNING("Not found texture: " + s.second);
      }
   }

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
   EM_LOG_DEBUG("Loading animation: " + name);

   EMAnimation* fab = nullptr;

   auto it = animations.find(name);
   if (it == animations.end())
   {
       EM_LOG_WARNING("Not found animation " + name);
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

Sprite* MPix::ContentManager::GetScrollingBG(int index, bool scrolling)
{
   const float SCROLLING_SPEED = 4.0f;
   assert(index >= 1 && index <= 5);
   Size fullSize = Director::getInstance()->getWinSize();
   Size halfSize =  fullSize / 2.0f;
   Size visibleSize = Director::getInstance()->getVisibleSize();
   auto center = Point(halfSize.width, halfSize.height);
   
   // Create
   string name("bg_" + ToString(index) );
   auto bg1 = GetSimpleSprite(name.c_str());
   assert(bg1);

   // Setup
   float scale = visibleSize.height / bg1->getContentSize().height;
   float swing = bg1->getContentSize().width / 2 * scale - visibleSize.width/2;
   bg1->setScale(scale);
   bg1->setPosition(center);

   // Run scrolling actions
   if (scrolling) {
      bg1->runAction(
         RepeatForever::create(
            Sequence::create(
               MoveTo::create(SCROLLING_SPEED, center + Point(swing, 0)),
               MoveTo::create(SCROLLING_SPEED, center - Point(swing, 0)),
               nullptr
            )
         )
      );
   }
   return bg1;
}













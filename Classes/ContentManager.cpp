#include "ContentManager.h"

// To have access to Z orders
#include "GameMain.h"

#include "HSVShader.h"

using namespace MPix;

// Stupid cocos listeners dont work with non-Object instances
// I dont wanna have ContentManager subclassing Object, so 
// this little helper handles situation
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

void MPix::ContentManager::CreateBatchNodes()
{
   SpriteBatchNode* spritebatch;
   SpriteFrameCache* cache = SpriteFrameCache::getInstance();


   // Wall pixel
   spritebatch = SpriteBatchNode::create("wallpixel.png", 64);
   cache->addSpriteFramesWithFile("wallpixel.plist");
   MapBatchNode("wall_pixel", spritebatch, GameMain::Z_WALLS);

   // wake mimic
   spritebatch = SpriteBatchNode::create("1.png");
   cache->addSpriteFramesWithFile("1.plist");
   MapBatchNode("pixel_mimics", spritebatch, GameMain::Z_PIXEL_MIMICS);

   // editor UI elements
   spritebatch = SpriteBatchNode::create("ed.png");
   cache->addSpriteFramesWithFile("ed.plist");
   MapBatchNode("ui_editor", spritebatch, 1);

}

void MPix::ContentManager::LoadResources()
{
   CreateShaders();
   CreateBatchNodes();
   CreateAnimations();
}

SpriteBatchNode* MPix::ContentManager::GetBatchNode( Node* target, const string& name )
{
   auto p = bnode.find(name);
   assert (p != bnode.end());
   auto & lst = p->second;
   for ( auto n : lst ) {
      if ( n.node->getParent() == target ) {
         return n.node;
      }
   }
   return nullptr;
}

MPix::ContentManager::BatchNodeInfo MPix::ContentManager::GetBatchNodeInfo( Node* target, const string& name )
{
   auto p = bnode.find(name);
   assert (p != bnode.end());
   auto & lst = p->second;
   for ( auto n : lst ) {
      if ( n.node->getParent() == target ) {
         return n;
      }
   }
   assert(false);
   return BatchNodeInfo(0,nullptr);
}



void MPix::ContentManager::CreateAnimations()
{

   // load armatures
   ArmatureDataManager::getInstance()->addArmatureFileInfo("magnetic.png", "magnetic.plist", "magnetic.xml");
   auto texture = Director::getInstance()->getTextureCache()->addImage("magnetic.png");
   texture->setAliasTexParameters();

}


void MPix::ContentManager::CleanBatchNode(Node* target, const string& name )
{
   auto p = bnode.find(name);
   assert (p != bnode.end());
   auto & lst = p->second;
   for ( auto n : lst ) {
      if ( n.node->getParent() == target ) {
         n.node->removeAllChildren();
         return;
      } 
   }
   assert(false); // Was no found -- bad
}

void MPix::ContentManager::PutBatchNode( Node* target, const string& name )
{
   auto p = bnode.find(name);
   assert (p != bnode.end());
   auto & lst = p->second;
   for ( auto n : lst ) {
      if ( n.node->getParent() == nullptr ) { // Find that one null-parent

         // Put it to target as child
         assert(n.node->getChildrenCount() == 0); // should be clean
         target->addChild(n.node, n.zOrder);

         // Gen new replacement node, now it will be null-parent // TODO: reserve same space
         auto nn = SpriteBatchNode::createWithTexture(n.node->getTexture() );
         nn->retain();
         lst.push_back( BatchNodeInfo(n.zOrder, nn));
         return;
      } 
   }
   assert(false);
}

void MPix::ContentManager::RemoveBatchNode( Node* target, const string& name )
{
   auto p = bnode.find(name);
   assert (p != bnode.end());
   auto & lst = p->second;
   for ( auto n = lst.begin(); n != lst.end(); ++n ) {
      auto node = n->node;
      if ( node->getParent() == target ) { // Found node childed by tager

         // By default views should take care about removing everything from BatchNode 
        auto s = node->getChildrenCount();
         if (s != 0) {
            // Clean up it
            node->removeAllChildren();
            // Warn
            EM_LOG_WARNING("Batch node "+ name + " had " + ToString(s) + " children left unclean");
         }

         // Remove it from target
         target->removeChild(node);

         // Ensures null-parent
         assert(node->getParent() == nullptr);

         // And if it is not last node - kill it
         if (lst.size() >= 1) {
            node->release();
            lst.erase(n);
         } 
         return;
      } 
   }
   assert(false);
}

void MPix::ContentManager::SetupPixelNodes( Node* target )
{
   PutBatchNode(target,"wall_pixel");
   PutBatchNode(target,"pixel_mimics");
}

void MPix::ContentManager::UnsetupPixelNodes( Node* target )
{
   // Clean batches contents // Remove does this
   //CleanBatchNode(target, "wall_pixel");
   //CleanBatchNode(target, "pixel_mimics");

   // And remove them
   RemoveBatchNode(target, "wall_pixel");
   RemoveBatchNode(target, "pixel_mimics");

}

void MPix::ContentManager::MapBatchNode( const char* name, SpriteBatchNode* n, int z )
{
   list<BatchNodeInfo> lst;
   n->retain();
   lst.push_back(BatchNodeInfo(z, n));
   bnode.emplace(name, lst );
}

void MPix::ContentManager::CreateShaders()
{
   auto shader = HSVShader::create();
   shader->autorelease();
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



#include "BehaviorManager.h"
#include "AnyBehavior.h"
#include "ECNode.h"

using namespace MPix;

//====---------------------------------------------======//

shared_ptr<AnyBehavior> MPix::BehaviorManager::GetPixelBehavior( const string& name )
{
   auto p = dynamic_cast<AnyBehavior*>(AnyBehavior::Factory()->NewNodeByName(name));
   assert(p);
   return shared_ptr<AnyBehavior> (p);
}

MPix::BehaviorManager::BehaviorManager()
{
   ECLOG_INFO("===== Pixel Behavior init ========");
   shared.reserve(16);
}

shared_ptr<AnyBehavior> MPix::BehaviorManager::GetSharedBehaviour( const string& name )
{
   auto it = shared.find(name);
   if (it == shared.end()) { // If not exist
      ECLOG_INFO("Created behavior "+name);
      auto b = GetPixelBehavior(name);
      shared.emplace(name, b);
      return b; // New one
   }
   return it->second; // Existing one
}


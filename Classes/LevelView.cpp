
#include "LevelView.h"
#include "Level.h"

#include "LevelManager.h"
#include "ContentManager.h"

using namespace MPix;

//====---------------------------------------------======//


const float FNT_LAYER_NAME = 24.0f;

LevelView* MPix::LevelView::create(int lvl_id)
{
   auto fab = new LevelView;
   if (fab->initWithId(lvl_id)) {
      fab->autorelease();
      return fab;
   }
   delete fab;
   return nullptr;
}

bool MPix::LevelView::initWithId(int lvl_id)
{
   if (!init()) {
      return false;
   }

   auto lvl = LevelManager::getInstance().GetLevelNameByID(lvl_id);

   auto name = LabelTTF::create(lvl, ContentManager::getInstance().GetBaseFont(), FNT_LAYER_NAME);
   name->setPosition(0, -60.0f);

   auto sp = ContentManager::getInstance().GetSimpleSprite("level_border");

   addChild(name, 2);
   addChild(sp, 1);

   return true;
}





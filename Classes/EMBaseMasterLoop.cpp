#include "EMBaseMasterLoop.h"
#include "HistoryManager.h"
#include "SoundManager.h"
#include "GameplayManager.h"
#include "GameStateManager.h"
#include "LevelManager.h"

using namespace MPix;

//====---------------------------------------------======//


void MPix::EMBaseMasterLoop::InitGame()
{
   EM_LOG_INFO("==== Magnetic pixels master loop init =====");
   HistoryManager::getInstance();
}

void MPix::EMBaseMasterLoop::RunGame()
{
   EM_LOG_INFO("==== Magnetic pixels master loop run =====");

   // Setup update
   Director::getInstance()->getScheduler()->scheduleUpdateForTarget(this, 0, false);

   // Init sound
   SoundManager::getInstance();

   // Init gamepleyman
   GameplayManager::getInstance();

   // LoadData levels and worlds // TODO: delay load
   auto ret = LevelManager::getInstance().LoadData();

   if (ret != ErrorCode::RET_OK ) {
      return;
   }

   GameStateManager::getInstance().SwitchToStart();
}

MPix::EMBaseMasterLoop::EMBaseMasterLoop()
{
   EM_LOG_INFO("==== Magnetic pixels master loop constructed =====");
}

void MPix::EMBaseMasterLoop::MasterTick(float t)
{
   //EM_LOG_INFO("==== Magnetic pixels master tick =====");

   GameStateManager::getInstance().Tick(t);
   SoundManager::getInstance().UpdateSoundSystem(t);
}

void MPix::EMBaseMasterLoop::update( float t )
{
   MasterTick(t);
}

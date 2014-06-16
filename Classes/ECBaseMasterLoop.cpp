#include "ECBaseMasterLoop.h"
#include "HistoryManager.h"
#include "SoundManager.h"
#include "GameplayManager.h"
#include "GameStateManager.h"
#include "LevelManager.h"
#include "SettingsManager.h"

using namespace MPix;

double MPix::ECBaseMasterLoop::time_runs = 0.0;
//====---------------------------------------------======//

MPix::ECBaseMasterLoop::ECBaseMasterLoop()
{
   ECLOG_INFO("==== Magnetic pixels master loop constructed =====");
}

void MPix::ECBaseMasterLoop::InitGame()
{
   ECLOG_INFO("==== Magnetic pixels master loop init =====");
   HistoryManager::getInstance();
}

void MPix::ECBaseMasterLoop::RunGame()
{
   ECLOG_INFO("==== Magnetic pixels master loop run =====");

   // Setup update
   Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);

   // Init settings
   auto & st = SettingsManager::getInstance();
   st.LoadSettings();

   // Increase number of runs of the game
   auto n = st.GetKey(SettingsManager::Key::N_OF_RUNS);
   st.SetKey(SettingsManager::Key::N_OF_RUNS, n + 1);
   st.SaveSettings();

   // Init sound
   SoundManager::getInstance().InitSoundSystem();

   // Init game manager
   GameplayManager::getInstance();

   // LoadData levels and worlds
   auto ret = LevelManager::getInstance().LoadData();

   if (ret != ErrorCode::RET_OK ) {
      ECLOG_ERROR("Levels not loaded, game will exit now");
      return;
   }

   GameStateManager::getInstance().SwitchToStart();
}


void MPix::ECBaseMasterLoop::MasterTick(float t)
{
   //ECLOG_INFO("==== Magnetic pixels master tick =====");
   time_runs += t;

   GameStateManager::getInstance().Tick(t);

   SoundManager::getInstance().UpdateSoundSystem(t);
}

void MPix::ECBaseMasterLoop::update( float t )
{
   MasterTick(t);
}

void MPix::ECBaseMasterLoop::PrintTime()
{
   ECLOG_ERROR("Time is: " + time_runs);
}

double MPix::ECBaseMasterLoop::GetTime()
{
   return time_runs;
}

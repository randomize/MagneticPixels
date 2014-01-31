#include "GameStateManager.h"

#include "MPix.h"

// Concrete states
#include "GameMain.h"
#include "EditorMain.h"
#include "MenuMain.h"
#include "GameTest.h"
#include "LevelSelector.h"
#include "ResultsState.h"
#include "EditorMenu.h"
#include "SplashState.h"


// Other managers
#include "SoundManager.h"
#include "GameplayManager.h"
#include "LevelManager.h"
#include "ContentManager.h"
#include "CreditsState.h"

using namespace MPix;


GameStateManager::GameStateManager() :  
   currentState(nullptr) // State undefined
{
   EM_LOG_INFO("[ GameStateManager initialized ]");
}


void GameStateManager::SwitchToExit( void )
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameStateManager::SwitchToStart( void )
{

   GameState* newState = SplashState::create();

   assert(newState);
   currentState = newState;

   Director::getInstance()->runWithScene( newState );

}

void GameStateManager::SwitchToGame( void )
{
   SwitchScene (GameMain::create());
}

void MPix::GameStateManager::SwitchToEditor( void )
{
   SwitchScene(EditorMain::create());
}

void MPix::GameStateManager::SwitchToEditorMenu( void )
{
   SwitchScene(EditorMenu::create());
}

void MPix::GameStateManager::SwitchToTestGame( void )
{
   SwitchScene(GameTest::create());
}

void GameStateManager::SwitchToSelector( void )
{
   assert(currentState); 

   // Animating transition
   auto newState = LevelSelector::create();
   //Director::getInstance()->replaceScene( TransitionSlideInR::create(0.25f, newState ));
   Director::getInstance()->replaceScene( TransitionCrossFade::create(0.25f, newState ));

   currentState = newState;
}

void GameStateManager::SwitchToMenu( void )
{
   if (currentState->GetName() == string("LevelSelector")) // Animating transition
   {
      auto newState = MenuMain::create();
      Director::getInstance()->replaceScene(TransitionCrossFade::create(0.25f, newState));
      currentState = newState;
   }
   else  // Plain switch
   {
      SwitchScene(MenuMain::create());
   }
}

void MPix::GameStateManager::SwitchToResults( void )
{
   SwitchScene(ResultsState::create());
}

void MPix::GameStateManager::SwitchToCredits()
{
   SwitchScene(CreditsState::create());
}



//////////////////////////////////////////////////////////////////////////

inline bool GameStateManager::SwitchScene( GameState* newState )
{
   assert(currentState); // First time call
   Director::getInstance()->replaceScene( newState );
   currentState = newState;
   return true;
}

void MPix::GameStateManager::Busy( bool state )
{
   assert(currentState);
   if (state) {
      currentState->Lock();
   } else {
      currentState->Unlock();
   }
}

void MPix::GameStateManager::Tick( float t )
{
   timer += t;
   assert(currentState);
   auto ret = currentState->Tick(t);
   assert(ret != ErrorCode::RET_FAIL );
}

double MPix::GameStateManager::GetElapsedTime()
{
   return timer;
}


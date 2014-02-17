#include "GameplayManager.h"

#include "MPix.h"

#include "GameStateManager.h"
#include "Cmd.h"

#include "Pixel.h"
#include "IMagnetic.h"
#include "IAssembled.h"

#include "Goal.h"
#include "Assembly.h"
#include "Field.h"
#include "EventProcessor.h"
#include "Level.h"
#include "ScriptManager.h"

using namespace MPix;

GameplayManager::GameplayManager()
{
   // Register commands 

           CmdGameplayClick::listners["GameplayManager"] = std::bind(&GameplayManager::ClickAtPoint,     this, std::placeholders::_1);
            CmdGameplayMove::listners["GameplayManager"] = std::bind(&GameplayManager::MoveAssembly,     this, std::placeholders::_1);
       CmdGameplayAfterMove::listners["GameplayManager"] = std::bind(&GameplayManager::AfterMoveAssembly, this, std::placeholders::_1);
        CmdGameplayUndoMove::listners["GameplayManager"] = std::bind(&GameplayManager::UndoMove,         this);
 CmdGameplayRestartAssembly::listners["GameplayManager"] = std::bind(&GameplayManager::CancelAssembly,  this);
         CmdGameplayGrowAsm::listners["GameplayManager"] = std::bind(&GameplayManager::GrowAssembly,     this);
       CmdGameplayFirstMove::listners["GameplayManager"] = std::bind(&GameplayManager::FirstMove,     this);

   EM_LOG_INFO("[ GameplayManager initialized ]");

   st = State::IDLE;

}

//////////////////////////////////////////////////////////////////////////
// Getters

GameplayManager::State MPix::GameplayManager::GetState( void ) const
{
   return st;
}

const Context & MPix::GameplayManager::GetContext() const
{
   return context;
}

//////////////////////////////////////////////////////////////////////////
// State management

const char* MPix::GameplayManager::StateString( State s )
{
   switch (s)
   {
   case State::IDLE:
      return "IDLE";
   case State::READY:
      return "READY";
   case State::PLAYING:
      return "PLAYING";
   case State::PAUSED:
      return "PAUSE";
   case State::FINISHED:
      return "FINISHED";
   }
   return "WFT";
}

ErrorCode GameplayManager::SwitchState( State to )
{

   // Better check state graph
   assert( 
      (st==State::IDLE && to==State::READY) ||
      (st==State::READY && to==State::PLAYING) ||

      (st==State::PLAYING && to==State::PAUSED) ||
      (st==State::PAUSED && to==State::PLAYING) ||

      (st==State::PLAYING && to==State::FINISHED) ||

      (st==State::FINISHED && to==State::IDLE) ||  // Reset
      (st==State::PLAYING && to==State::IDLE)      // Reset
      );

   EM_LOG_INFO("<<<< GPM: change " + StateString(st) + " => " + StateString(to) + ">>>>" );
   st = to;
   
   return ErrorCode::RET_OK;

}


//////////////////////////////////////////////////////////////////////////
// Commands management

ErrorCode GameplayManager::ProcessOneCommand()
{
   // Return RET_FAIL if no more commands ans RET_OK when have more ti process

   // Break on wrong state
   if (st != State::PLAYING) 
      return ErrorCode::RET_FAIL;

   // Break on empty
   if (commands.empty())
      return ErrorCode::RET_FAIL;

   // Take one and exec
   auto cmd = commands.front();
   commands.pop_front();
   cmd->Execute();
   delete cmd;

   //EM_LOG_DEBUG(" GameplayManager : Executed CMD=[not implemented], return " + static_cast<int>(ret)); 

   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::PostCommand( Command* cmd )
{
   commands.push_back(cmd);
   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::PostPriorityCommand( Command* cmd )
{
   commands.push_front(cmd);
   return ErrorCode::RET_OK;
}


//////////////////////////////////////////////////////////////////////////
// Controls 

ErrorCode GameplayManager::LoadLevel( shared_ptr<Level> level )
{

   if (st != State::IDLE) {
      EM_LOG_WARNING("GameplayManager is not IDLE, level cant be loaded, need reset");
      GameplayManager::Reset();
   }

   // Asserting nonempty level with field in it
   assert(level);
   assert( ! level->GetField()->IsEmpty() );
   assert(level->GetState() == Level::State::IS_PLAYABLE);

   EM_LOG_INFO("LOADING LEVEL #" + level->GetID());

   this->level = level;

   ScriptManager::getInstance().LoadScript(level->GetID());

   SwitchState(State::READY);

   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::Play()
{
   EM_LOG_INFO("PLAY");

   assert(level);
   SwitchState(State::PLAYING);

   level->SetState(Level::State::IS_PLAYING);

   // Create event system
   context.pixel_events = make_shared<EventProcessor>();

   // Create context
   context.field = level->GetField();
   context.goals = level->GetGoals();
   context.assembly = make_shared<Assembly>();
   context.moveNumber = 0;
   context.solutionPhases = 0;

   // Saving first snapshot index = 0;
   context.field->InitSnapshots(context);
   context.assembly->InitSnapshots(context);
   context.goals->InitSnapshots(context);
   context.PushContextSnapshots();

   // Send Gui command to create pixel views and goal views
   GameStateManager::getInstance().CurrentState()->Execute(new CmdUICreateViews());

   context.field->SendCreateEvents(context);
   context.goals->SendCreateEvents(context);

   PostPriorityCommand(new CmdGameplayFirstMove());

   UpdateUI();

   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::FirstMove()
{
   context.field->WorldCheckForLost(context);

   ScriptManager::getInstance().OnFirstMove(context);

   CheckForSolution();

   UpdateUI();

   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::Reset()
{
   EM_LOG_INFO("RESET");

   // Break reference to level
   level.reset();

   context.pixel_events   = nullptr;

   // Clear context
   context.goals    = nullptr;
   context.field    = nullptr;
   context.assembly = nullptr;
   context.moveNumber = 0;

   // Remove commands
   commands.clear();

   SwitchState(State::IDLE);

   return ErrorCode::RET_OK;
}

//////////////////////////////////////////////////////////////////////////
// Internal 

ErrorCode GameplayManager::ClickAtPoint( Coordinates position )
{
   EM_LOG_DEBUG("ClickAtPoint");

   assert(st == State::PLAYING);

   // Break on script disallow clicking
   if (ScriptManager::getInstance().OnPlayerClicked(context, position) == false) {
      return ErrorCode::RET_FAIL;
   }

   // TODO: click pixels, for fun, cactus can react too, if assembling sleeppy can react with(vorchanie)
   auto px = context.field->GetTopAssembleAt(position);

   // not started assembly or lost all pixels -- starting
   if ( context.assembly->IsEmpty() )
   {
      // if clicked on asm pixel - wake it and start
      if (px) {
         StartAssembling(px);
         return ErrorCode::RET_OK;
      }
   }
   else // asm has pixels
   {
      // Check if click is on assembly
      if (px && px->IsInAssembly()) {

         // Possible will end assembling
         if (EndAssembling() == ErrorCode::RET_FAIL)
         {
            // Send tap anyway
            context.PostEvent(PixelEvent::TAPPED, px->GetID());
         }

         UpdateUI();

         return ErrorCode::RET_OK;
      }
   }

   auto p = context.field->GetActivePixelAt(position);
   if (p) {
      context.PostEvent(PixelEvent::TAPPED, p->GetID());
   }

   UpdateUI();

   return ErrorCode::RET_OK;
}

ErrorCode MPix::GameplayManager::StartAssembling( shared_ptr<IAssembled> pixel )
{
   assert(st == State::PLAYING);
   assert(context.assembly->IsEmpty());
   assert(pixel);

   EM_LOG_INFO("Starting assembly with " + pixel->GetTypeName() + " (id=" + pixel->GetID() + ") at " + pixel->GetPos() );

   // Save move number when started
   context.assembly->SetMoveNumber(context.moveNumber);

   // Take snapshot
   context.PushContextSnapshots();
   context.moveNumber++;

   // Put pixel to assembly
   context.assembly->AddPixel(context, pixel);

   UpdateUI();

   // Start growing sequence
   PostPriorityCommand(new CmdGameplayGrowAsm());

   return ErrorCode::RET_OK;;
}

ErrorCode GameplayManager::MoveAssembly(Direction d)
{
   assert(st == GameplayManager::State::PLAYING );

   EM_LOG_INFO("GameplayManager->MoveAssembly()");

   // Break on script disallow clicking
   if (ScriptManager::getInstance().OnPlayerMove(context, d) == false) {
      return ErrorCode::RET_FAIL;
   }

   if (context.assembly->IsEmpty()) {
      SendUINeedWakePixelFirst();
      return ErrorCode::RET_FAIL;
   }

   // Check if can move
   auto ret = context.assembly->CheckMove(context, d);

   if (ret == ErrorCode::RET_YES) 
   {

      // Moving
      EM_LOG_INFO("Moving Assembly to " + d);

      // Saving what was
      context.PushContextSnapshots();
      context.moveNumber++;

      // Performing move in field
      context.assembly->Move(context, d);

      // Performing worlds reaction
      context.field->WorldAfterPlayersMove(context, false);

      // Schedule more logic after move
      PostPriorityCommand(new CmdGameplayAfterMove(d));

   } 
   else
   {
      // Performing worlds reaction for block
      context.field->WorldAfterPlayersMove(context, true);
   }

   CheckForSolution();

   UpdateUI();

   return ErrorCode::RET_OK;
}

ErrorCode MPix::GameplayManager::AfterMoveAssembly( Direction d ) {

   assert(st == State::PLAYING);

   EM_LOG_INFO("GameplayManager->AfterMoveAssembly()");

   context.field->WorldCheckForLost(context);

   // 1 - check life of moved assembly
   // If lost
   if ( context.assembly->CheckForLost(context) == ErrorCode::RET_YES )
   {
      PostPriorityCommand(new CmdGameplayAfterMove(d));
      CheckForSolution();
      UpdateUI();
      return ErrorCode::RET_FAIL;
   } 

   // Moved assembly is stable now

   // 2 - World performs its move
   context.field->WorldMove(context);

   // 3 - World check lost
   context.field->WorldCheckForLost(context);

   // Schedule grow next time
   PostPriorityCommand(new CmdGameplayGrowAsm());

   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::GrowAssembly()
{
   assert(st == State::PLAYING);

   EM_LOG_INFO("GameplayManager->GrowAssembly()");

   // Searching for dead pixels killed by cactus or pits
   auto ret = context.assembly->CheckForLost(context); 

   // If all removed - do not try to grow
   if (!context.assembly->IsEmpty())
   {
      // Trying to grow assembly
      ret = context.assembly->Grow(context);

      if ( ret == ErrorCode::RET_YES ) {

         EM_LOG_DEBUG("Assembly grows");

         // Play sound
         GameStateManager::getInstance().CurrentState()->Execute(new CmdPlaySound("stick"));

         // Schedule more grow
         PostPriorityCommand(new CmdGameplayGrowAsm());

      } else {
         EM_LOG_DEBUG("Assembly last grow");
         ScriptManager::getInstance().OnLastGrow(context);
      }

   } 

   CheckForSolution();

   UpdateUI();

   return ErrorCode::RET_FAIL;
}

ErrorCode GameplayManager::UndoMove()
{
   if (st != State::PLAYING) return ErrorCode::RET_FAIL;

   if (context.moveNumber == 0) { 
      // TODO: Send UI : this is first move!! (Better gray out/remove undo)
      return ErrorCode::RET_FAIL;
   }

   // Undo one move and restore to snapshot
   context.moveNumber--;
   context.PopContextSnapshots();

   UpdateUI();

   return ErrorCode::RET_OK;

}

ErrorCode GameplayManager::CancelAssembly()
{
   if (st != State::PLAYING) return ErrorCode::RET_FAIL;

   if (context.moveNumber == 0) { 
      // TODO: Send UI : this is first move!!
      return ErrorCode::RET_FAIL;
   }

   if (context.assembly->IsEmpty()) {
      return ErrorCode::RET_FAIL;
   }

   int moves_passed = context.moveNumber - context.assembly->GetMoveNumber();
   if (moves_passed == 0) {
      assert(false);
      return ErrorCode::RET_FAIL;
   }

   // Get number of moves from start of assembly and restore to snapshot when assembly started
   context.moveNumber = context.assembly->GetMoveNumber();
   context.field->PopSnapshots(context, moves_passed);
   context.assembly->PopSnapshots(context, moves_passed, true);
   context.goals->PopSnapshots(context, moves_passed);

   UpdateUI();

   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::EndAssembling()
{
   assert(st == State::PLAYING);
   EM_LOG_INFO("Requested EndAssembly");

   if (context.goals->ExistsSolution() == false)  {
      return ErrorCode::RET_FAIL;
   }

   context.PushContextSnapshots();
   context.moveNumber++;

   // Accept by all goals
   context.goals->AcceptSolution(context);
   context.assembly->AcceptedSolution(context);

   EM_LOG_INFO("Accepted solution");

   // Win check
   if ( context.goals->HasUnsatisfiedGoals() == false )
   {
      SwitchState(State::FINISHED);
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIGameFinished());
      return ErrorCode::RET_OK;
   } 


   return ErrorCode::RET_OK;

}

//////////////////////////////////////////////////////////////////////////
// Helpers

ErrorCode MPix::GameplayManager::CheckLoseStatus()
{

   // TODO: assure that pixels count and type is enough to complete level

   return ErrorCode::RET_OK;

}

ErrorCode MPix::GameplayManager::UpdateUI()
{

   // Update pixel UI
   context.pixel_events->ProcessEvents();

   // If there is solution - ui notifies
   if ( context.goals->ExistsSolution()) {
      if ( context.goals->ExistsFullSolution()) {
         SendUIFullSolution();
      } else {
         SendUIPartSolution();
      }
   }

   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::CheckForSolution()
{

   // Update solution
   context.goals->UpdateSolution(context);

   return ErrorCode::RET_OK;
}


//////////////////////////////////////////////////////////////////////////
// UI messages from GameplayManager

ErrorCode MPix::GameplayManager::SendUINeedWakePixelFirst()
{
   // TODO: post Gui command to user with message "wake a pixel first"
   // Chose some random pixel and send coordinates, UI will draw a hand
   return ErrorCode::RET_OK;
}


ErrorCode MPix::GameplayManager::SendUIPleaseUndoOrRestart()
{
   // TODO: post Gui command to user with message "undo some moves or restart, you can't win now"
   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::GameplayManager::SendUIFullSolution()
{
   // TODO: sound
   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::GameplayManager::SendUIPartSolution()
{
   // TODO: sound
   return ErrorCode::RET_OK;
}

EmbossLib::ErrorCode MPix::GameplayManager::SendUIAcceptSolution()
{
   // TODO: play happy sound, and effect
   return ErrorCode::RET_OK;
}







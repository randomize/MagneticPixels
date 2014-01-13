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

using namespace MPix;

GameplayManager::GameplayManager()
{
   // Register commands 

           CmdGameplayClick::listners["GameplayManager"] = std::bind(&GameplayManager::ClickAtPoint,     this, std::placeholders::_1);
            CmdGameplayMove::listners["GameplayManager"] = std::bind(&GameplayManager::MoveAssembly,     this, std::placeholders::_1);
        CmdGameplayMoveFast::listners["GameplayManager"] = std::bind(&GameplayManager::MoveAssemblyFast, this, std::placeholders::_1);
       CmdGameplayAfterMove::listners["GameplayManager"] = std::bind(&GameplayManager::AfterMoveAssembly, this, std::placeholders::_1);
        CmdGameplayUndoMove::listners["GameplayManager"] = std::bind(&GameplayManager::UndoMove,         this);
 CmdGameplayRestartAssembly::listners["GameplayManager"] = std::bind(&GameplayManager::CancelAssembly,  this);
         CmdGameplayGrowAsm::listners["GameplayManager"] = std::bind(&GameplayManager::GrowAssembly,     this);

   EM_LOG_INFO("[ GameplayManager initialized ]");

   st = State::IDLE;

}

//////////////////////////////////////////////////////////////////////////
// Getters

const GameplayManager::State MPix::GameplayManager::GetState( void ) const
{
   return st;
}

const Context & MPix::GameplayManager::GetContext()
{
   return context;
}

const MPix::Rectangle & MPix::GameplayManager::GetViewport() const
{
   assert( st == State::READY);
   return level->GetViewport();
}

const MPix::Rectangle & MPix::GameplayManager::GetCurrentViewport() const
{
   assert( st == State::PLAYING || st == State::PLAYING_FAST);
   return context.viewport;
}

//////////////////////////////////////////////////////////////////////////
// State management

const char * MPix::GameplayManager::StateString( State s )
{
   switch (s)
   {
   case State::IDLE:
      return "IDLE";
   case State::READY:
      return "READY";
   case State::PLAYING:
      return "PLAYING";
   case State::PLAYING_FAST:
      return "PLAYING_FAST";
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

      (st==State::PLAYING && to==State::PLAYING_FAST) ||
      (st==State::PLAYING_FAST && to==State::PLAYING) ||

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
   if (st != State::PLAYING && st != State::PLAYING_FAST) return ErrorCode::RET_FAIL;

   // Break on empty
   if (commands.empty()) return ErrorCode::RET_FAIL;

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
   EM_LOG_INFO("GameplayManager->LoadLevel()");

   if (st != State::IDLE) {
      EM_LOG_WARNING("GameplayManager is not IDLE, level cant be loaded, need reset");
      GameplayManager::Reset();
   }

   // Asserting nonempty level with field in it
   assert(level);
   assert( ! level->GetField()->IsEmpty() );

   this->level = level;
   SwitchState(State::READY);

   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::Play()
{
   EM_LOG_INFO("GameplayManager->Play()");

   assert(level);
   SwitchState(State::PLAYING);

   level->SetState(Level::State::IS_PLAYING);

   // Create event system
   context.pixel_events = make_shared<EventProcessor>();

   // Create context
   context.field = level->GetField();
   context.goals = level->GetGoals();
   context.viewport = level->GetViewport();
   context.assembly = make_shared<Assembly>();
   context.moveNumber = 0;

   // Saving first snapshot index = 0;
   context.field->InitSnapshots(context);
   context.assembly->InitSnapshots(context);
   context.goals->InitSnapshots(context);

   // Push first snapshot
   PushContextSnapshots();

   // Send Gui command to create pixel views and goal views
   GameStateManager::getInstance().CurrentState()->Execute(new CmdUICreateViews());

   // Check lost 
   context.field->WorldCheckForLost(context);

   context.field->SendCreateEvents(context);
   //context.goals->SendCreateEvents(context);

   // Update goals
   CheckForSolution();

   UpdateUI();

   return ErrorCode::RET_OK;
}

ErrorCode GameplayManager::Reset()
{
   EM_LOG_INFO("GameplayManager->Reset()");

   // Called when other level should be loaded
   // Cleanups level and sets state to Idle
   // Reset always have success

   // Break reference to level
   level.reset();

   context.pixel_events   = nullptr;

   // Clear context
   context.goals    = nullptr;
   context.field    = nullptr;
   context.assembly = nullptr;
   context.viewport = MPix::Rectangle();
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
         EndAssembling();
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

   // Take snapshot (Call it STASM);
   PushContextSnapshots();
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
   EM_LOG_INFO("GameplayManager->MoveAssembly()");
   assert(st == GameplayManager::State::PLAYING || st == GameplayManager::State::PLAYING_FAST );

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
      PushContextSnapshots();
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

   EM_LOG_INFO("GameplayManager->AfterMoveAssembly()");
   assert(st == State::PLAYING || st == State::PLAYING_FAST);

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
   EM_LOG_INFO("GameplayManager->GrowAssembly()");
   assert(st == State::PLAYING || st == State::PLAYING_FAST );

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
      }

   } 

   CheckForSolution();

   UpdateUI();

   return ErrorCode::RET_FAIL;
}

ErrorCode GameplayManager::MoveAssemblyFast( Direction whereMove )
{
   static int cnt = 0;

   assert (st == State::PLAYING || st == State::PLAYING_FAST) ;

   if ( st == State::PLAYING )
   { 

      if ( context.assembly->IsEmpty() )
      {
         SendUINeedWakePixelFirst();
         return ErrorCode::RET_FAIL;
      }

      SwitchState(State::PLAYING_FAST);
      cnt = 0;
      PostPriorityCommand(new CmdGameplayMoveFast(whereMove));
      MoveAssembly(whereMove);
   } 
   else 
   {
      cnt++;
      if (context.pixel_events->WasBadEvent() == true || cnt > 4) { // No more then 4 moves!
         SwitchState(State::PLAYING);
         context.pixel_events->ProcessEvents();
         FitViewport();
         return ErrorCode::RET_OK;
      } else {
         PostPriorityCommand(new CmdGameplayMoveFast(whereMove));
         MoveAssembly(whereMove);
      }
   }
   
   return ErrorCode::RET_OK;

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
   PopContextSnapshots();

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

   PushContextSnapshots();
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

   UpdateUI();

   return ErrorCode::RET_OK;

}

//////////////////////////////////////////////////////////////////////////
// Helpers

ErrorCode MPix::GameplayManager::PushContextSnapshots()
{
   context.field->PushSnapshot(context);
   context.assembly->PushSnapshot();
   context.goals->PushSnapshot(context);
   return ErrorCode::RET_OK;
}

ErrorCode MPix::GameplayManager::PopContextSnapshots( int count /*= 1*/ )
{
   context.field->PopSnapshots(context, count);
   context.assembly->PopSnapshots(context, count);
   context.goals->PopSnapshots(context, count);
   return ErrorCode::RET_OK;
}

ErrorCode MPix::GameplayManager::CheckLoseStatus()
{

   // TODO: assure that pixels count and type is enough to complete level

   return ErrorCode::RET_OK;

}

ErrorCode MPix::GameplayManager::UpdateUI()
{

   // Update viewports
   FitViewport();

   // Update pixel UI
   if ( st != State::PLAYING_FAST )
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


ErrorCode MPix::GameplayManager::FitViewport()
{

   if (level->AutoPan() == false) return ErrorCode::RET_FAIL;

   if (context.assembly->IsEmpty()) {
      context.viewport = level->GetViewport();
      GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateViewport);
      return ErrorCode::RET_OK;
   }

   auto rect = context.assembly->GetContentsRect();
   const int basis = 2;

   // Borders
   auto bl = -( rect.BL.x - context.viewport.BL.x - basis );
   auto br = -( context.viewport.TR.x - rect.TR.x - basis );
   auto bd = -( rect.BL.y - context.viewport.BL.y - basis );
   auto bu = -( context.viewport.TR.y - rect.TR.y - basis );

   if (bl > 0) {
      context.viewport.MoveTo(Direction::DIR_LEFT, bl);
   } else if (br > 0) {
      context.viewport.MoveTo(Direction::DIR_RIGHT, br);
   }

   if (bd > 0) {
      context.viewport.MoveTo(Direction::DIR_DOWN, bd);
   } else if (bu > 0) {
      context.viewport.MoveTo(Direction::DIR_UP, bu);
   }

   GameStateManager::getInstance().CurrentState()->Execute(new CmdUIUpdateViewport);

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








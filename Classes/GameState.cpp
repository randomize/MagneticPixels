#include "GameState.h"
#include "Cmd.h"

using namespace MPix;

MPix::GameState::GameState()
{
   semaphore = 0;
}

MPix::GameState::~GameState()
{

}

ErrorCode GameState::Execute( Command* cmd ) const
{
   assert(cmd);
   ErrorCode ret = cmd->Execute();
   //ECLOG_DEBUG("Game state " + GetName() + " executed command" );
   delete cmd;
   return ret;
}

void MPix::GameState::Unlock()
{
   if (semaphore > 0) semaphore--;
}

void MPix::GameState::Lock()
{
   semaphore++;
   assert(semaphore>0); // JIC overflow
}

bool MPix::GameState::IsBusy()
{
   return semaphore != 0;
}

void MPix::GameState::ResetLocks()
{
   semaphore = 0;

}

EndlessCatLib::ErrorCode MPix::GameState::ExecuteAsync( Command* cmd )
{
   scheduled_commands.push_back(cmd);
   return ErrorCode::RET_OK;
}

EndlessCatLib::ErrorCode MPix::GameState::Tick( float deltha )
{
   // Check if async commands available
   if (!scheduled_commands.empty()) {
      for ( auto c : scheduled_commands )
         c->Execute();
   }
   return ErrorCode::RET_OK;
}


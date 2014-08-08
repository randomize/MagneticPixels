#include "Goal.h"

#include "IColorful.h"

#include "Context.h"

using namespace MPix;

//##########################################################################
//  Task class
//  Vector2 that need to be covered with matching IColorful.
//##########################################################################

GoalTask::GoalTask( Coordinates pos, PixelColor color ):
   pos(pos),
   color(color),
   st(State::UNACCEPTABLE)
{
}

//////////////////////////////////////////////////////////////////////////
// Manipulation and setters

void MPix::GoalTask::Reset( const Context& context, int goalID )
{
   assert( st != State::ACCEPTED && "Cannot reset satisfied task");

   // If was highlighted, send event
   if ( st == State::ACCEPTABLE )
   {
      context.PostEvent(GoalEvent::UNHIGHLIGHT, goalID, pos);
   }

   st = State::UNACCEPTABLE;
}

void MPix::GoalTask::Accept( const Context& context, int goalID )
{
   assert( st != State::ACCEPTED && "Cannot accept already satisfied task");
   st = State::ACCEPTED;
   context.PostEvent(GoalEvent::ACCEPTED, goalID, pos);
}

void MPix::GoalTask::Update( const Context& context, int goalID )
{

   assert(st != State::ACCEPTED && "Do not update satisfied goal");

   // Trying to find match - assembly or socoba
   auto cp = context.GetColorfulAt(pos);

   if ( cp && cp->GetColor() == color )  // Covered with my color IColorful pixel
   {
      ECLOG_DEBUG(" Matched color goal :" + color + " at " + pos + " with " + cp->GetID());

      if (st == State::UNACCEPTABLE) // Whoora! become acceptable
      {
         context.PostEvent(GoalEvent::HIGHTLIGHT, goalID, pos);
      }
      st = State::ACCEPTABLE;
   }
   else // if not covered by pixel or covered with wrong one
   {
      if (st == State::ACCEPTABLE) // Noooo. it was acceptable
      {
         context.PostEvent(GoalEvent::UNHIGHLIGHT, goalID, pos);
      }

      st = State::UNACCEPTABLE;
   }
}


//////////////////////////////////////////////////////////////////////////
// Getters and informers

bool GoalTask::MatchesColor( PixelColor c ) const
{
   return color == c;
}

bool MPix::GoalTask::WasAccepted() const
{
   return st == State::ACCEPTED;
}

MPix::PixelColor MPix::GoalTask::GetColor() const
{
   return color;
}

MPix::Coordinates MPix::GoalTask::GetPos() const
{
   return pos;
}

bool GoalTask::Acceptable() const
{
   return st == State::ACCEPTABLE;
}

//////////////////////////////////////////////////////////////////////////
// History support

ErrorCode GoalTask::InitSnapshots( const Context& context )
{
   st.InitSnapshots();
   return ErrorCode::RET_OK;
}

ErrorCode GoalTask::PopSnapshots( const Context& context, int goalID, int count /*= 1*/ )
{
   State was = st;
   st.PopSnapshots(count);
   if (was != st.GetValue()) {
      // Change detected

      if (was == State::UNACCEPTABLE && st == State::ACCEPTABLE) {
         context.PostEvent(GoalEvent::UNDO_UNHIGHLIGHT, goalID, pos);
         return ErrorCode::RET_OK;
      }

      if (was == State::ACCEPTABLE && st == State::UNACCEPTABLE) {
         context.PostEvent(GoalEvent::UNDO_HIGHLIGHT, goalID, pos);
         return ErrorCode::RET_OK;
      }

      if (was == State::ACCEPTED && st != State::ACCEPTED) {
         context.PostEvent(GoalEvent::UNDO_ACCEPTED, goalID, pos);
         return ErrorCode::RET_OK;
      }

   }
   return ErrorCode::RET_OK;
}

ErrorCode GoalTask::PushSnapshot( const Context& context )
{
   st.PushSnapshot();
   return ErrorCode::RET_OK;
}

//##########################################################################
// Goal class
// Groups some amount of tasks into one goal
//##########################################################################

Goal::Goal():
   st(State::ACCEPTED), // Empty is satisfied by default
   id(0)                 // No ID assigned
{
}

shared_ptr<Goal> Goal::Dublicate()
{
   // Only simple types here, we can just copy
   auto fab = new Goal(*this);
   return shared_ptr<Goal>(fab);
}

//////////////////////////////////////////////////////////////////////////
// Helpers

const GoalTask* Goal::TaskAt( Coordinates pos ) const
{
   auto it = tasks.find(pos);
   if (it != tasks.end()) {
      return &(it->second);
   }
   return nullptr;
}

bool Goal::FindMatch( Coordinates p, PixelColor c ) const
{
   assert(st != State::ACCEPTED && "Shouldn't call this if goal marked as satisfied");
   assert(!tasks.empty() && "Ensure that tasks exists");

   // Checking point in map
   auto task = TaskAt(p);
   if (task && task->WasAccepted() == false && task->MatchesColor(c) ) {
      return true;
   }

   return false;
}

//////////////////////////////////////////////////////////////////////////
// History support

ErrorCode Goal::InitSnapshots( const Context& context )
{
   st.InitSnapshots();
   for (auto &t : tasks) {
      t.second.InitSnapshots(context);
   }
   return ErrorCode::RET_OK;
}

ErrorCode Goal::PushSnapshot( const Context& context )
{
   st.PushSnapshot();
   for (auto &t : tasks) {
      t.second.PushSnapshot(context);
   }
   return ErrorCode::RET_OK;
}

ErrorCode Goal::PopSnapshots( const Context& context, int count /*= 1*/ )
{
   st.PopSnapshots(count);
   for (auto &t : tasks) {
      t.second.PopSnapshots(context, id, count);
   }
   return ErrorCode::RET_OK;
}

//////////////////////////////////////////////////////////////////////////
// Manipulation

void Goal::AddTask( Coordinates pos, PixelColor c ) {
   assert (TaskAt(pos) == nullptr && "Task already exists at pos");
   tasks.emplace(pos, GoalTask(pos,c));
   st = State::CANT_BE_SATISFIED;
}

void Goal::RemoveTask( Coordinates pos )
{
   tasks.erase(pos);
}

void MPix::Goal::Reset( const Context& context )
{
   assert(st != State::ACCEPTED && "Do not reset satisfied goal");
   st = State::CANT_BE_SATISFIED;
   for (auto &t : tasks) {

      auto & task = t.second;
      if (task.WasAccepted()) continue;

      task.Reset(context, id);

   }
}

ErrorCode Goal::AcceptSolution( const Context& context )
{
   bool all = true;
   for (auto & t : tasks) {

      auto & task = t.second;
      if (task.WasAccepted()) continue;

      if ( task.Acceptable() ) {
         task.Accept(context, id);
      } else {
         all = false; // Found one that cannot
      }
   }
   st = Goal::State::CANT_BE_SATISFIED;
   if (all) {
      st = Goal::State::ACCEPTED;
   }
   return ErrorCode::RET_OK;
}


void Goal::UpdateSolution( const Context& context )
{
   bool full = true;
   for (auto & t : tasks) {

      auto &task = t.second;
      if (task.WasAccepted()) continue;

      task.Update(context, id);

      if (task.Acceptable()) {
         st = Goal::State::CAN_BE_SATISFIED_PARTIALLY;
      } else {
         full = false; // Found one unsatisfied
      }
   }
   if (full) {
      st = Goal::State::CAN_BE_SATISFIED_FULLY;
   }
}

void Goal::SetID( int id )
{
   this->id = id;
}

//////////////////////////////////////////////////////////////////////////
// Informers


bool Goal::ContainsPoint( Coordinates pos ) const
{
   return TaskAt(pos) != nullptr;
}

//##########################################################################
//  Goals class, represents group of goals
//##########################################################################

Goals::Goals()
{
}

shared_ptr<Goals> Goals::Dublicate()
{
   auto fab = new Goals();

   for (auto g : this->goals) {
      fab->goals.push_back( g->Dublicate() );
   }

   fab->has_unsatisfied_goals = has_unsatisfied_goals;
   fab->has_solution = has_solution;
   fab->has_full_solution = has_full_solution;

   return shared_ptr<Goals>(fab);
}

//////////////////////////////////////////////////////////////////////////
// History support

ErrorCode Goals::InitSnapshots( const Context& context )
{
   for (auto g : this->goals) {
      g->InitSnapshots(context);
   }
   return ErrorCode::RET_OK;
}

ErrorCode Goals::PopSnapshots( const Context& context, int count /*= 1*/ )
{
   for (auto g : this->goals) {
      g->PopSnapshots(context, count);
   }
   return ErrorCode::RET_OK;
}

ErrorCode Goals::PushSnapshot( const Context& context )
{
   for (auto g : this->goals) {
      g->PushSnapshot(context);
   }
   return ErrorCode::RET_OK;
}

//////////////////////////////////////////////////////////////////////////
// Goal manipulation

void Goals::InsertGoal( shared_ptr<Goal> goal )
{
   goal = goal->Dublicate();
   assert(goal->GetID() == 0);
   goals.push_back(goal);
   goal->SetID(goals.size()); // Counting ids
   has_unsatisfied_goals = true;
}

ErrorCode Goals::UpdateSolution( const Context& context )
{
   ECLOG_DEBUG("Updating solution");
   has_full_solution = false;
   has_solution = false;

   // Check that assembly is fully smiling
   if ( context.IsAssemblyEmpty() || context.IsAssemblySmiling() == false ) {

      ECLOG_DEBUG("Assembly is not smiling, no solution");

      // No solution
      for (auto g: goals) {
         g->Reset(context);
      }

      return ErrorCode::RET_FAIL;
   }

   has_full_solution = true; // Assume
   for (auto g : goals)
   {
      // Only unsatisfied playing now
      if ( g->WasAccepted() )  continue;

      g->UpdateSolution(context);

      if (g->GetState() != Goal::State::CAN_BE_SATISFIED_FULLY) {
         has_full_solution = false;
      }

      if (g->GetState() != Goal::State::CANT_BE_SATISFIED ) {
         has_solution = true;
      }
   }
   if (!has_solution) {  // JIC. remove assumptions if no solution
      has_full_solution = false;
   }

   return ErrorCode::RET_NO;
}

ErrorCode Goals::AcceptSolution( const Context& context )
{
   assert(has_solution && "Cannot accept, because no solution");

   has_unsatisfied_goals = false;
   for (auto g : goals) {

      if ( g->WasAccepted() ) continue;

      // Send accept to goal
      g->AcceptSolution(context);

      if ( g->WasAccepted() == false) {
         has_unsatisfied_goals = true;
      }

   }

   return ErrorCode::RET_OK;
}

//////////////////////////////////////////////////////////////////////////
// Getters

shared_ptr<Goal> Goals::GetGoalByID( int id )
{
   assert(id != 0); return goals.at(id-1);
}

shared_ptr<Goal> Goals::GoalAtPoint( Coordinates pos )
{
   for (auto g : goals) {
      if ( g->ContainsPoint(pos) ) {
         return g;
      }
   }
   return nullptr;
}

int MPix::Goals::GoalsCount() const
{
   return (int)goals.size();
}

void MPix::Goals::ResetSolution( const Context& context )
{
   for (auto g : goals)
   {
      // Only unsatisfied playing now
      if ( g->WasAccepted() )  continue;
      // Reset
      g->Reset(context);
   }
}

EndlessCatLib::ErrorCode MPix::Goals::SendCreateEvents(const Context& context)
{
   for (auto g : goals) {
      context.PostEvent(GoalEvent::CREATED, g->GetID(), Coordinates(0, 0));
   }
   return ErrorCode::RET_OK;
}


int Goals::FindMatchingGoal( Coordinates pos, PixelColor color ) const
{
   for (auto g : goals) {
      if ( g->WasAccepted() ) continue;
      if ( g->FindMatch(pos, color) ) {
         return g->GetID();
      }
   }
   return 0;
}

bool Goals::HasUnsatisfiedGoals() const
{
   return has_unsatisfied_goals;
}

bool Goals::ExistsSolution() const
{
   return has_solution;
}

bool Goals::ExistsFullSolution() const
{
   return has_full_solution;
}

bool Goals::ExistsMatchingGoal( Coordinates pos, PixelColor color )
{
   return FindMatchingGoal(pos, color) != 0;
}


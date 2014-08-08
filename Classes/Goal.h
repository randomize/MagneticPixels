//===-- Goal.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/03
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Goal is map of colors on the floor
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELSHAPE_H_
#define PIXELSHAPE_H_

#include "ECBase.h"
#include "MPix.h"
#include "Historical.h"

namespace MPix {

   // Forward dependencies
   class IColorful;
   class Goals;
   class Context;

   //##########################################################################
   // Goal task is a point with color
   //##########################################################################

   struct GoalTask {

      enum class State {
         UNACCEPTABLE, // CANT_BE_ACCEPTED
         ACCEPTABLE, // CAN_BE_ACCEPTED
         ACCEPTED // ACCEPTED
      };

      //////////////////////////////////////////////////////////////////////////
      // Base

      GoalTask(Coordinates pos, PixelColor color);

      //////////////////////////////////////////////////////////////////////////
      // History support

      ErrorCode InitSnapshots( const Context& context );
      ErrorCode PopSnapshots(const Context& context, int goalID, int count = 1);
      ErrorCode PushSnapshot( const Context& context );

      //////////////////////////////////////////////////////////////////////////
      // Manipulators

      void Accept(  const Context& context, int goalID );
      void Update(  const Context& context, int goalID );
      void Reset(  const Context& context, int goalID );

      //////////////////////////////////////////////////////////////////////////
      // Information

      bool MatchesColor(PixelColor c) const;
      bool IsUnsatisfied() const;
      bool WasAccepted() const;
      bool Acceptable() const;
      PixelColor GetColor() const;
      Coordinates GetPos() const;

   private:

      Coordinates pos;
      PixelColor color;
      Historical<State> st;

   };

   //##########################################################################
   // Goal is groups of tasks forming shape
   //##########################################################################

   class Goal
   {
   public:

      enum class State {
         CANT_BE_SATISFIED,
         CAN_BE_SATISFIED_FULLY,
         CAN_BE_SATISFIED_PARTIALLY,
         ACCEPTED
      };

      //////////////////////////////////////////////////////////////////////////
      // Base

      Goal();
      shared_ptr<Goal> Dublicate();

      //////////////////////////////////////////////////////////////////////////
      // History support

      ErrorCode InitSnapshots( const Context& context );
      ErrorCode PopSnapshots(const Context& context, int count = 1);
      ErrorCode PushSnapshot( const Context& context );

      //////////////////////////////////////////////////////////////////////////
      // Task manipulation

      void AddTask(Coordinates pos, PixelColor color);
      void RemoveTask(Coordinates pos);
      void SetID( int id );

      // Reset solution
      void Reset( const Context& context );

      // Accept solution
      ErrorCode AcceptSolution( const Context& context );

      // Update tasks to set
      void UpdateSolution( const Context& context );

      //////////////////////////////////////////////////////////////////////////
      // Informations

      unordered_map<Coordinates, GoalTask> & GetTasks() { return tasks; }

      bool FindMatch(Coordinates pos, PixelColor color) const;

      bool ContainsPoint(Coordinates pos) const;

      bool WasAccepted() const { return st == State::ACCEPTED; }

      int GetID() const { return id; }

      State GetState() { return st; }

      size_t TaskCount() { return tasks.size(); }

   private:

      //////////////////////////////////////////////////////////////////////////
      // Helpers

      // Return nullptr if not found
      const GoalTask* TaskAt(Coordinates pos) const;

      //////////////////////////////////////////////////////////////////////////
      // Storage

      // Tasks, mapped to their positions
      unordered_map<Coordinates, GoalTask> tasks;

      // State of this goal
      Historical<State> st;

      // ID of this goal, id > 0
      int id;

   };

   //##########################################################################
   // Goals is group of goals for level
   //##########################################################################

   class Goals {
   public:

      Goals();
      shared_ptr<Goals> Dublicate();

      static const int FIRST_GOAL = 1;

      ErrorCode SendCreateEvents(const Context& context);

      //////////////////////////////////////////////////////////////////////////
      // History support

      ErrorCode InitSnapshots( const Context& context );
      ErrorCode PopSnapshots(const Context& context, int count = 1);
      ErrorCode PushSnapshot( const Context& context );

      //////////////////////////////////////////////////////////////////////////
      // Goal manipulation

      // Create goal with smart pointer
      void InsertGoal(shared_ptr<Goal> goal);

      // UpdateSolution return ErrorCode::RET_YES if a zone can be satisfied
      ErrorCode UpdateSolution( const Context& context );

      // Called to accept current solution
      ErrorCode AcceptSolution( const Context& context );

      //////////////////////////////////////////////////////////////////////////
      // Informations

      // Goal getters
      shared_ptr<Goal> GetGoalByID(int id);
      shared_ptr<Goal> GoalAtPoint(Coordinates pos);
      vector<shared_ptr<Goal>> & GetGoals() { return goals; }

      bool ExistsMatchingGoal(Coordinates pos, PixelColor color);

      int GoalsCount() const;
      bool HasUnsatisfiedGoals() const;
      bool ExistsSolution() const;
      bool ExistsFullSolution() const;

   private:

      //////////////////////////////////////////////////////////////////////////
      // Helpers

      int FindMatchingGoal(Coordinates pos, PixelColor color) const;

      void ResetSolution( const Context& context );

      //////////////////////////////////////////////////////////////////////////
      // Storage

      // Goal list
      vector<shared_ptr<Goal>> goals;

      // Solution flag
      bool has_solution;

      // Full solution flag
      bool has_full_solution;

      // True when there is more goals to achieve
      bool has_unsatisfied_goals;

   };

}


#endif // PIXELSHAPE_H_

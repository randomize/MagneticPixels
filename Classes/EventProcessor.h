//===-- EventProcessor.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/03
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Events processor realizes events storing and processing
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PIXELEVENTPROCESSOR_H_
#define PIXELEVENTPROCESSOR_H_

#include "ECBase.h"
#include "MPix.h"

#include "Events.h"

// Defining hasher for Event,id pairs
namespace std
{
   template <> struct hash<MPix::PixelEvent> {
      size_t operator()(const MPix::PixelEvent& v) const
      {
         return static_cast<unsigned>(v) ;
      }
   };
   template <> struct hash<MPix::GoalEvent> {
      size_t operator()(const MPix::GoalEvent& v) const
      {
         return static_cast<unsigned>(v) ;
      }
   };
}

namespace MPix {

   // Forwards
   class EventProcessor;
   class EventPacket;
   typedef list<shared_ptr<EventPacket>> EventList;

   // Packet is container for event, it has priority
   class EventPacket {
   public:

      EventPacket(int pri);
      virtual ~EventPacket();
      bool operator<(const EventPacket& rhs) const;
      void ProcessEvent();
      void Deactivate();

      // Packet interface
      virtual bool operator==(const EventPacket& rhs) const = 0;
      virtual size_t hash() const = 0;
      virtual void RuleCheck(EventList & events) = 0;

   protected:
      virtual void Dispatch() = 0;
      int priority;
      bool isActive;
   };

}

namespace std
{
   template <> struct hash<MPix::EventPacket> {
      size_t operator()(const MPix::EventPacket& v) const
      {
         return v.hash();
      }
   };
}

namespace MPix {

   typedef unordered_map<PixelEvent, std::forward_list<PixelEvent>> PixelRules;

   // Packet with pixel events
   class PixelEventPacket : public EventPacket {
   public:

      PixelEventPacket(PixelEvent e, int id, int pri);

      // Impl. interface
      bool operator==(const EventPacket& rhs_base) const override;
      size_t hash() const override;
      void Dispatch() override;
      void RuleCheck(EventList & events) override;

   private:

      // Rule set for overriding
      static PixelRules rules;

      // Events, that cause fast move stop
      static unordered_set<PixelEvent> good_events;

      PixelEvent event;
      int pixelID;
   };

   typedef unordered_map<GoalEvent, std::forward_list<GoalEvent>> GoalRules;

   // Packet with goal events
   class GoalEventPacket : public EventPacket {
   public:

      GoalEventPacket(GoalEvent e, int id, Coordinates task, int pri);

      // Impl. interface
      bool operator==(const EventPacket& rhs_base) const override;
      size_t hash() const override;
      void Dispatch() override;
      void RuleCheck(EventList & events) override;

   private:

      // Rule set for overriding
      static GoalRules rules;

      GoalEvent event;
      int goalID;
      Coordinates task;
   };

}


namespace MPix {


   // EventProcessor
   class EventProcessor
   {
   public:

      EventProcessor();
      ~EventProcessor();

      // Events
      void PostEvent(shared_ptr<EventPacket> e);
      void ClearEvents();

      ErrorCode ProcessEvents();

   private:

      // Prepares events
      ErrorCode PreprocessEvents();

      // Holds events(changes) happened due move(undo) pairs(event,id)
      EventList events;

   };

}


#endif // PIXELEVENTPROCESSOR_H_

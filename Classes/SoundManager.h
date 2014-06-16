//===-- SoundManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Sound manager facades SimpleAudioEngine
//
//===---------------------------------------------------------===//

#pragma once
#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include "EMBase.h"

namespace MPix {

   struct SoundTask {
      string name;
   };

   // TODO: have here random sound chosen form list,
   // // Sound manager plays not immediatly
   // remove double tasks

   class SoundManager
   {
   public:

      // Must be called on startup
      void InitSoundSystem();

      ErrorCode PlayInBackground( const string& name);
      ErrorCode PlaySoundSync( const string& name);
      ErrorCode PlaySoundDelayed( const string& name, float delay); // after delays::seconds
      ErrorCode StartLoopedSound( const string& name );

      // Called on each tick
      ErrorCode UpdateSoundSystem(float deltha);

   protected:
   	
   private:

   ////// Singleton ///////////////////////////////////////////////////////////////////

   public: 

      static SoundManager& getInstance() {
         static SoundManager theSingleInstance;
         return theSingleInstance;
      }

   private:

      SoundManager();
      SoundManager(SoundManager& root){}
      SoundManager& operator=(SoundManager&){return *this;}

      ErrorCode PauseEngine();
      ErrorCode ResumeEngine();

   };


}


#endif // SOUNDMANAGER_H_

//===-- PlatformManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/10/06
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Platform-dependent features encapsulated in one class
//
//===---------------------------------------------------------===//

#pragma once
#ifndef PLATFORMMANAGER_H_
#define PLATFORMMANAGER_H_

#include "EMBase.h"

namespace EMCore {


   class PlatformManager
   {
   public:

   ////// Common ///////////////////////////////////////////////////////////////////

      ~PlatformManager();

      // Returns path to save file
      // Windows: Desktop
      // Android: SD card
      string GetSavingPath();


      // Shows simple message box
      void ShowMessage(const char* title, const char* text);

   ////// Platforms ///////////////////////////////////////////////////////////////////

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)

      // Android specific methods
      string GetSDCardPath();
      void AndroidShowBannerActivity();
      string GetDeviceModel();

#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)

      // IOS specific methods

#elif (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)

      // Windows specific methods
      string GetDesktopPath();

#endif

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 
   
      static PlatformManager& getInstance() {
         static PlatformManager theSingleInstance;
         return theSingleInstance;
      }
   
   private:
   
      PlatformManager();
      PlatformManager(PlatformManager& root){}
      PlatformManager& operator=(PlatformManager&){return *this;}
   

   };

}


#endif // PLATFORMMANAGER_H_

//===-- LocaleManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/19
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Localization support
//
//===---------------------------------------------------------===//

#pragma once
#ifndef LOCALEMANAGER_H_
#define LOCALEMANAGER_H_

#include "EMBase.h"

namespace EMCore {

   // Forward dependencies
   class Dictionary;

   // LocaleManager

   class LocaleManager
   {
   public:

      void SetLocale(string locale);
      string GetLocale() const;

      const string& GetLocalisedString(const string& key);
      const char* GetLocalisedChar(const char* key);

   private:

      Dictionary* current;
      unordered_map<string, Dictionary*> languages;

      string locale;

      // FIXME: For now cocos has no EnumerateFilesInFolder function in FileUtils... hardcoding langs
      static const vector<string> implemented_langs;

      const char* GetCurrentLanguage();

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 
   
      static LocaleManager& getInstance() {
         static LocaleManager theSingleInstance;
         return theSingleInstance;
      }
   
   private:
   
      LocaleManager();
      ~LocaleManager();
      LocaleManager(LocaleManager& root){}
      LocaleManager& operator=(LocaleManager&){return *this;}
   

   };

}


#endif // LOCALEMANAGER_H_

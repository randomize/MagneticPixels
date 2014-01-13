//===-- ContentManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/10
//  Author:      Mihailenco E. at Emboss Games, 2013
//  Description: Content manager holds textures, sprites etc
//
//===---------------------------------------------------------===//

#pragma once
#ifndef CONTENTMANAGER_H_
#define CONTENTMANAGER_H_

#include "EMBase.h"

namespace MPix {

   // forwards
   class EMShader;

   // ContentManager

   class ContentManager
   {
   public:

      // =========== General interface ==========================================

      // Called once on startup
      void LoadResources();

      // =========== Shader management ==========================================
      GLProgram* GetShader(const string& name);

      // =========== Sprite management ==========================================

      Sprite* GetSimpleSprite(const string& name);

      // =========== Animation management ==========================================

      /// DEPRECATED, armatures used

      // =========== GFX helpers ==========================================
      // If some state wants to pass some (data)node to another they use 
      // these methods, ContentManager acts as middle-man 

      void PutNode(Node*, const string& name);
      Node* GetNode(const string& name);

   private:

      // Initialization 
      void CreateAnimations();
      void CreateShaders();

      // Helpers
      void ReloadShaders();

      unordered_map<string, EMShader*> shaders;
      unordered_map<string, string> resources;

   ////// Singleton ///////////////////////////////////////////////////////////////////
   
   public: 
   
      static ContentManager& getInstance() {
         static ContentManager theSingleInstance;
         return theSingleInstance;
      }
   
   private:
   
      ContentManager();
      ~ContentManager();
      ContentManager(ContentManager& root){}
      ContentManager& operator=(ContentManager&){return *this;}
   };

}


#endif // CONTENTMANAGER_H_

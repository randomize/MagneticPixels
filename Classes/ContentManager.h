//===-- ContentManager.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/10
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: Content manager holds textures, sprites etc
//
//===---------------------------------------------------------===//

#pragma once
#ifndef CONTENTMANAGER_H_
#define CONTENTMANAGER_H_

#include "ECBase.h"

namespace MPix {

   // forwards
   class ECShader;
   class HSVSprite;
   class ECAnimation;

   // ContentManager
   // It is actually a resource manager ;)

   class ContentManager
   {
   public:

      class ResourceItem {

         enum class Type {
            SINGLE_SPRITE,
            SPRITE_FRAME,
            HSV_SPRITE,
            ANIMATION,
            SOUND
         };
      };

      // =========== General interface ==========================================

      // Called once on startup
      // TODO: make async with notifier delegate
      void LoadResources();

      // =========== Shader management ==========================================
      GLProgram* GetShader(const string& name);

      // =========== Sprite management ==========================================

      Sprite* GetSimpleSprite(const string& name);
      HSVSprite* GetHSVSprite(const string& name);

      Sprite* GetScrollingBG(int index, bool isScrolling = true);

      // =========== Animation management ==========================================

      ECAnimation* GetAnimation(const string& name);

      // =========== Font management =====================================

      string GetBaseFont(); 
      string GetBaseFontLight();
      string GetLogoFont();

      // =========== GFX helpers ==========================================
      // If some state wants to pass some (data)node to another they use 
      // these methods, ContentManager acts as middle-man 

      void AddNode(Node* new_node, const string& node_name);
      void RemoveNode(const string& node_name);
      Node* GetNode(const string& node_name);

   private:

      // Initialization 
      void CreateAnimations();
      void CreateShaders();
      void CreateSprites();

      // Helpers
      void ReloadShaders();

      // List of shaders
      unordered_map<string, ECShader*> shaders;

      // List of resources
      unordered_map<string, string> resources;

      // List of animations
      unordered_map<string, string> animations;

      // List of saved nodes
      unordered_map<string, Node*> nodes;

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

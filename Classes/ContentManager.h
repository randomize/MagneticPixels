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

      // =========== Batch nodes management ==========================================
      // This singleton loads all plist files on LoadResources() and creates companion batch nodes
      // - Nodes are identified by string name
      // - At startup only one node exists per plist
      // - When someone wants to display something plisted, it is called target node
      // - Target node asks by string name, ContentManager returns a dupe(clone pattern)
      // - Target node parents a Batchnode to display something
      // - `something` knows its target and knows its Batchnode name, it childes to corresponding node to be displayed

      // Structure used to store node and some info(Z order)
      struct BatchNodeInfo;

      // Returns batch node named `name` and childed by target
      SpriteBatchNode* GetBatchNode(Node* target, const string& name);

      // Returns batch node nfo for node named `name` and childed by target
      BatchNodeInfo GetBatchNodeInfo(Node* target, const string& name);

      // Removes all children form map named `name` and childed by target
      void CleanBatchNode(Node* target, const string& name);

      // Puts new batch node named `name` to target as child
      void PutBatchNode(Node* target, const string& name);

      // Removes batch node named `name` from target's children
      void RemoveBatchNode(Node* target, const string& name);

      // Setups base pixel nodes, as targets children, called target that want to display pixels
      void SetupPixelNodes(Node* target);

      // Cleans up base pixel nodes, removes them from targets children, called target that no longer needs them
      void UnsetupPixelNodes(Node* target);

      // =========== Animation management ==========================================

      /// DEPRECATED, armatures used

      // =========== GFX helpers ==========================================
      // If some state wants to pass some (data)node to another they use 
      // these methods, ContentManager acts as middle-man 

      void PutNode(Node*, const string& name);
      Node* GetNode(const string& name);

   private:

      // Initialization 
      void CreateBatchNodes();
      void CreateAnimations();
      void CreateShaders();
      void ReloadShaders();

      // Helpers
      void MapBatchNode( const char* name, SpriteBatchNode* n, int z = 0 );

      unordered_map<string, list<BatchNodeInfo>> bnode;
      unordered_map<string, Node*> nodes;
      unordered_map<string, EMShader*> shaders;

   public: // Helper structures

      struct BatchNodeInfo {
         BatchNodeInfo(int zOrder, SpriteBatchNode* node) : node(node), zOrder(zOrder) {}
         int zOrder;
         SpriteBatchNode* node;
      };


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

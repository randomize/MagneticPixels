//===-- EMNode.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: C++ auto factory implementation 
//  Usage:
//   Base class should have EM_NODE_BASE(ClassName) in declaration
//   Derived classes should have EM_NODE_CHILD(ClassName) in declaration
//   Derived classes should have EM_NODE_CHILD_CPP(ClassName) in definition 
//   Now can use:
//      GetTypeName() for "classname" string
//      BaseName::Factory().NewNodeByName ByID etc
//
//===---------------------------------------------------------===//

#pragma once
#ifndef EMNODE_H_
#define EMNODE_H_

#include "EMBase.h"

namespace EndlessCatLib {

   // Base class for anything that is factored 
   class EMNode
   {
   public:
      virtual EMNode* Clone()            = 0; 
      virtual string GetTypeName() const = 0; 
      virtual int GetTypeID()      const = 0;
      virtual ~EMNode() {}
   };

   // Factories store generators, not objects
   class EMNodeGenerator {
   public:
      virtual EMNode* Gen() = 0; 
   };

//Add this macro to each base class having factory
#define EM_NODE_BASE(NodeType)                             \
public:                                                    \
   static NodeFactory* Factory() {                         \
      static NodeFactory factory(#NodeType);               \
      return &factory;                                     \
   }                                                       \
// EM_NODE_BASE

// This macro registers factorized child with type NodeType but named NodeTypeString 
#define EM_NODE_CHILD_CUSTOM_NAME(NodeType, NodeTypeString)                \
public:                                                                    \
                                                                           \
   EMNode* Clone() override { return new NodeType(*this); }                \
                                                                           \
   /* Get this instance ID*/                                                 \
   int GetTypeID() const override { return NodeType##_type_gen.id ; }      \
   /* Get whole set ID */                                                     \
   static int GetClassTypeID() { return NodeType##_type_gen.id ; }      \
                                                                           \
   string GetTypeName() const override {                                   \
      static string name = EndlessCatLib::StringToLowcase( #NodeTypeString );  \
      return name;                                                         \
   }                                                                       \
                                                                           \
private:                                                                   \
                                                                           \
   struct NodeType##Gen : public EMNodeGenerator {                         \
      NodeType##Gen() {                                                    \
         id = Factory()->RegistrateGen( this, #NodeTypeString );            \
      }                                                                    \
      EMNode* Gen() { return new NodeType(); }                             \
      int id;                                                              \
   };                                                                      \
                                                                           \
   static NodeType##Gen NodeType##_type_gen;                               \
                                                                           \
public:                                                                    \
// EM_NODE_CHILD_CUSTOM_NAME

#define EM_NODE_CHILD(NodeType)                 \
   EM_NODE_CHILD_CUSTOM_NAME(NodeType,NodeType) \
// EM_NODE_CHILD_NODE

#define EM_NODE_CHILD_CPP(NodeType)                         \
   NodeType::NodeType##Gen NodeType::NodeType##_type_gen ;  \
// EM_NODE_CHILD_CPP

class NodeFactory
{
public:
   NodeFactory(const char* name);

   EMNode* NewNodeByName(const string& name);
   EMNode* NewNodeByID(int typeID);

   int GetTypeID(const string& name);

   string name;

public:
   // Called automatically, do not call this = private
   int RegistrateGen( EMNodeGenerator* node, const string& name);
protected:
   int counter;
private:
   unordered_map <string, int>             nameToID;
   unordered_map <int, EMNodeGenerator* >  idToNode;
};


} // namespace EndlessCatLib

#endif //NODE_FACTORY_H

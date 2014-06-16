//===-- ECNode.h -------------------------------*- C++ -*-===//
//
//  Created:     2013/09/02
//  Author:      Mihailenco E. at TheEndlessCat Games, 2013
//  Description: C++ auto factory implementation 
//  Usage:
//   Base class should have ECNODE_BASE(ClassName) in declaration
//   Derived classes should have ECNODE_CHILD(ClassName) in declaration
//   Derived classes should have ECNODE_CHILD_CPP(ClassName) in definition 
//   Now can use:
//      GetTypeName() for "classname" string
//      BaseName::Factory().NewNodeByName ByID etc
//
//===---------------------------------------------------------===//

#pragma once
#ifndef ECNODE_H_
#define ECNODE_H_

#include "ECBase.h"

namespace EndlessCatLib {

   // Base class for anything that is factored 
   class ECNode
   {
   public:
      virtual ECNode* Clone()            = 0; 
      virtual string GetTypeName() const = 0; 
      virtual int GetTypeID()      const = 0;
      virtual ~ECNode() {}
   };

   // Factories store generators, not objects
   class ECNodeGenerator {
   public:
      virtual ECNode* Gen() = 0; 
   };

//Add this macro to each base class having factory
#define ECNODE_BASE(NodeType)                             \
public:                                                    \
   static NodeFactory* Factory() {                         \
      static NodeFactory factory(#NodeType);               \
      return &factory;                                     \
   }                                                       \
// ECNODE_BASE

// This macro registers factorized child with type NodeType but named NodeTypeString 
#define ECNODE_CHILD_CUSTOM_NAME(NodeType, NodeTypeString)                \
public:                                                                    \
                                                                           \
   ECNode* Clone() override { return new NodeType(*this); }                \
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
   struct NodeType##Gen : public ECNodeGenerator {                         \
      NodeType##Gen() {                                                    \
         id = Factory()->RegistrateGen( this, #NodeTypeString );            \
      }                                                                    \
      ECNode* Gen() { return new NodeType(); }                             \
      int id;                                                              \
   };                                                                      \
                                                                           \
   static NodeType##Gen NodeType##_type_gen;                               \
                                                                           \
public:                                                                    \
// ECNODE_CHILD_CUSTOM_NAME

#define ECNODE_CHILD(NodeType)                 \
   ECNODE_CHILD_CUSTOM_NAME(NodeType,NodeType) \
// ECNODE_CHILD_NODE

#define ECNODE_CHILD_CPP(NodeType)                         \
   NodeType::NodeType##Gen NodeType::NodeType##_type_gen ;  \
// ECNODE_CHILD_CPP

class NodeFactory
{
public:
   NodeFactory(const char* name);

   ECNode* NewNodeByName(const string& name);
   ECNode* NewNodeByID(int typeID);

   int GetTypeID(const string& name);

   string name;

public:
   // Called automatically, do not call this = private
   int RegistrateGen( ECNodeGenerator* node, const string& name);
protected:
   int counter;
private:
   unordered_map <string, int>             nameToID;
   unordered_map <int, ECNodeGenerator* >  idToNode;
};


} // namespace EndlessCatLib

#endif //NODE_FACTORY_H

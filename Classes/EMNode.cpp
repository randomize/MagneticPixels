#include "EMNode.h"

using namespace EndlessCatLib;

NodeFactory::NodeFactory(const char* name):
   name(name),
   counter(0)
{
   EM_LOG_DEBUG("-= Created " + name + " factory =-");
}

int NodeFactory::RegistrateGen( EMNodeGenerator* node, const string& s )
{
    int newID = ++counter; // form 1
    nameToID[EndlessCatLib::StringToLowcase(s)] = newID;
    EM_LOG_INFO("Registered " + s + " as " + newID + " at " + name + " factory" );
    idToNode[newID] = node;
    
    //nameDictionary[newID] = node.getTypeName();
    return newID;
}


EMNode* NodeFactory::NewNodeByName(const string& name )
{
    
    auto idIter = nameToID.find(EndlessCatLib::StringToLowcase(name));
    if ( idIter != nameToID.end() )
    {
        return NewNodeByID( idIter->second );
    } else {
        EM_LOG_ERROR( "No '"+ name + "' in " + name + " factory!" );
        return nullptr;
    }
    
}

EMNode* NodeFactory::NewNodeByID( int typeID )
{
    auto nodeIter = idToNode.find( typeID );
    if ( nodeIter != idToNode.end() )
    {
        return nodeIter->second->Gen();
    } else {
        EM_LOG_ERROR( "No ID="+ typeID + " in " + name + " factory!" );
        return nullptr;
    }
}

int NodeFactory::GetTypeID(const string& name )
{
    auto idIter = nameToID.find( name );
    if ( idIter != nameToID.end() )
    {
        return idIter->second;
    } else {
        EM_LOG_ERROR( "No `"+ name + "` in " + name + " factory!" );
        return 0;
    }
}


#include "ECNode.h"

using namespace EndlessCatLib;

NodeFactory::NodeFactory(const char* name):
   name(name),
   counter(0)
{
   ECLOG_DEBUG("-= Created " + name + " factory =-");
}

int NodeFactory::RegistrateGen( ECNodeGenerator* node, const string& s )
{
    int newID = ++counter; // form 1
    nameToID[EndlessCatLib::StringToLowcase(s)] = newID;
    ECLOG_INFO("Registered " + s + " as " + newID + " at " + name + " factory" );
    idToNode[newID] = node;
    
    //nameDictionary[newID] = node.getTypeName();
    return newID;
}


ECNode* NodeFactory::NewNodeByName(const string& name )
{
    
    auto idIter = nameToID.find(EndlessCatLib::StringToLowcase(name));
    if ( idIter != nameToID.end() )
    {
        return NewNodeByID( idIter->second );
    } else {
        ECLOG_ERROR( "No '"+ name + "' in " + name + " factory!" );
        return nullptr;
    }
    
}

ECNode* NodeFactory::NewNodeByID( int typeID )
{
    auto nodeIter = idToNode.find( typeID );
    if ( nodeIter != idToNode.end() )
    {
        return nodeIter->second->Gen();
    } else {
        ECLOG_ERROR( "No ID="+ typeID + " in " + name + " factory!" );
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
        ECLOG_ERROR( "No `"+ name + "` in " + name + " factory!" );
        return 0;
    }
}


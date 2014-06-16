
#include "ScriptManager.h"
#include "Script.h"
#include "Scripts.h"

using namespace MPix;

//====---------------------------------------------======//

ScriptManager::ScriptManager()
{
   ECLOG_INFO("Script manager init");

   m_scripts = CppScripts::GetScripts();
   //m_scripts = LuaScripts::GetScripts(); // TODO: swap this when lua ready

   ECLOG_INFO("Loaded " + m_scripts.size() + " scripts");
}

MPix::ScriptManager::~ScriptManager()
{

}

void MPix::ScriptManager::LoadScript(unsigned id)
{
   auto it = m_scripts.find(id);
   if (it != m_scripts.end()) {
      m_current = it->second;
   }
   else {
      m_current = make_shared<Script>();
   }

   ECLOG_INFO("Loaded script : " + m_current->GetName());
}

void MPix::ScriptManager::OnFirstMove(const Context& context)
{
   assert(m_current);
   m_current->OnFirstMove(context);
}

bool MPix::ScriptManager::OnPlayerClicked(const Context& context, Coordinates where)
{
   assert(m_current);
   return m_current->OnPlayerClicked(context, where);
}

bool MPix::ScriptManager::OnPlayerMove(const Context& context, Direction where)
{
   assert(m_current);
   return m_current->OnPlayerMove(context, where);
}

void MPix::ScriptManager::OnLastGrow(const Context& context)
{
   assert(m_current);
   m_current->OnLastGrow(context);
}










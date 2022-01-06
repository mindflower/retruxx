#include <stdexcept>
#include <script/scriptserver.h>

namespace m3d
{
    Class* ScriptServer::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    Object* ScriptServer::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    int ScriptServer::_getScriptObject(Object*)
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::reloadScript(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::callScriptFunc(char const*, sArgStack&, int)
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::done()
    {
        throw std::logic_error("Not implemented");
    }

    char const* ScriptServer::getNameOfLastScript() const
    {
        throw std::logic_error("Not implemented");
    }

    std::map<CStr, ScriptServer::auxFuncDesc> const& ScriptServer::getRegisteredFunctionsDesc() const
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::addScript(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::execute(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::executeBuffer(void*, unsigned, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    ScriptServer::~ScriptServer()
    {
        throw std::logic_error("Not implemented");
    }

    lua_State* ScriptServer::getGlobalEnvironment()
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::reloadAllScripts()
    {
        throw std::logic_error("Not implemented");
    }

    CStr ScriptServer::getFormatedScriptErrorDesc(eScriptError) const
    {
        throw std::logic_error("Not implemented");
    }

    auxScriptErrorDesc const& ScriptServer::getLastErrorDesc() const
    {
        throw std::logic_error("Not implemented");
    }

    Class* ScriptServer::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    char const* ScriptServer::getErrorDescString(eScriptError) const
    {
        throw std::logic_error("Not implemented");
    }

    void ScriptServer::dumpStack()
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::executeScriptFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::init()
    {
        throw std::logic_error("Not implemented");
    }

    Object* ScriptServer::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::registerGlobalFunction(int(*)(sArgStack&), char const*, char const*, char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    ScriptServer::ScriptServer(ScriptServer const&)
    {
        throw std::logic_error("Not implemented");
    }

    ScriptServer::ScriptServer()
    {
        throw std::logic_error("Not implemented");
    }
}

#include <script/scriptserver.h>
#include <script/luaquaternion.h>
#include <script/luavector.h>
extern "C"
{
#include <lualib.h>
#include <lauxlib.h>
}
#include <stdexcept>

namespace
{
    int _getGlobalObject(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    int _logMethod(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    int _execLuaScript(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    int _errorMethod(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    int _callClassMethod(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    int _callClassNativeMethod(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    int _toString(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    int _callNativeGlobalFunction(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    lua_CFunction oldToString = nullptr;
    m3d::ScriptServer* g_scriptServer = nullptr;
}

namespace m3d
{
    Class ScriptServer::m_classScriptServer {"ScriptServer", sizeof(ScriptServer), CreateObject, GetBaseClass};

    Class* ScriptServer::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    Object* ScriptServer::CreateObject()
    {
        return new ScriptServer;
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
        L = lua_open();
        if (!L)
        {
            return OTHER_ERROR;
        }
        luaopen_base(L);
        luaopen_string(L);
        luaopen_math(L);
        luaopen_io(L);
        luaopen_table(L);
        lua_pushstring(L, "GET_GLOBAL_OBJECT");
        lua_pushcclosure(L, _getGlobalObject, 0);
        lua_settable(L, -10001);
        lua_pushstring(L, "LOG");
        lua_pushcclosure(L, _logMethod, 0);
        lua_settable(L, -10001);
        lua_pushstring(L, "EXECUTE_SCRIPT");
        lua_pushcclosure(L, _execLuaScript, 0);
        lua_settable(L, -10001);
        lua_pushstring(L, "_ALERT");
        lua_pushcclosure(L, _errorMethod, 0);
        lua_settable(L, -10001);
        ext_initVector(L);
        ext_initQuaternion(L);
        lua_newtable(L);
        lua_pushstring(L, "__call");
        lua_pushcclosure(L, _callClassMethod, 0);
        lua_settable(L, -3);
        m_metatable_ClassMethod = luaL_ref(L, -10000);
        lua_newtable(L);
        lua_pushstring(L, "__call");
        lua_pushcclosure(L, _callClassNativeMethod, 0);
        lua_settable(L, -3);
        m_metatable_ClassNativeMethod = luaL_ref(L, -10000);
        lua_pushstring(L, "tostring");
        lua_gettable(L, -10001);
        oldToString = lua_tocfunction(L, -1);
        lua_settop(L, -2);
        lua_pushcclosure(L, _toString, 0);
        lua_pushstring(L, "tostring");
        lua_insert(L, -2);
        lua_settable(L, -10001);
        Scriptlet::g_scriptServer = this;
        g_scriptServer = this;
        m_bInitialized = true;
        return SUCCESS;
    }

    Object* ScriptServer::Clone()
    {
        throw std::logic_error("Not implemented");   
    }

    eScriptError ScriptServer::registerGlobalFunction(int(*NativeGlobalFunc)(sArgStack&), char const* name, char const* returnValue, char const* params, char const* shortDesc)
    {
        if (!m_bInitialized)
        {
            return NOT_INITIALIZED;
        }
        if (!NativeGlobalFunc)
        {
            return OTHER_ERROR;
        }
        auto const it = m_funcDescs.find(name);
        if (it != m_funcDescs.cend())
        { 
            return ALREADY_REGISTERED;
        }

        auto data = (int(**)(sArgStack&))lua_newuserdata(L, sizeof(NativeGlobalFunc));
        *data = NativeGlobalFunc;
        lua_newtable(L);
        lua_pushstring(L, "__call");
        lua_pushcclosure(L, _callNativeGlobalFunction, 0);
        lua_settable(L, -3);
        lua_setmetatable(L, -2);
        lua_pushstring(L, name);
        lua_insert(L, -2);
        lua_settable(L, -10001);

        auxFuncDesc desc;
        desc.returnValue = CStr(returnValue);
        desc.params = params;
        desc.shortDesc = shortDesc;
        m_funcDescs.emplace(name, desc);

        return SUCCESS;
    }
}

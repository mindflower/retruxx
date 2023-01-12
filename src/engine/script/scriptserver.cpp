#include <script/scriptserver.h>
#include <script/luaquaternion.h>
#include <script/luavector.h>

#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "script/scriptcontext.h"

extern "C"
{
#include <lualib.h>
#include <lauxlib.h>
}
#include <stdexcept>

namespace
{
    int _getGlobalObject(lua_State * L)
    {
        auto glob = lua_tostring(L, 1);
        auto obj = m3d::g_Kernel->FindGlobal(glob);
        if (obj)
        {
            auto scriptObj = m3d::ScriptServer::_getScriptObject(obj);
            lua_rawgeti(L, -10000, scriptObj);
            return 1;
        }
        M3D_LOG_WARN("Script side: cound not find global " + CStr(glob));
        lua_pushnil(L);
        return 1;
    }

    int _logMethod(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    int _execLuaScript(lua_State * L)
    {
        luaL_checktype(L, 1, 4);
        auto file = lua_tostring(L, 1);
        return m3d::Scriptlet::g_scriptServer->executeScriptFile(file);
    }

    int _errorMethod(lua_State *)
    {
        throw std::logic_error("Not implemented");
    }

    int _callClassMethod(lua_State *L)
    {
        m3d::LuaContext ctx;
        ctx.L = L;
        ctx.m_stackStart = 2;
        ctx.m_numInputs = lua_gettop(L) - 1;
        ctx.m_numOutputs = 0;
        auto func = reinterpret_cast<int(**)(m3d::Context*)>(lua_touserdata(L, 1));
        (*func)(&ctx);
        return ctx.m_numOutputs;
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

    void _addExports(m3d::Class* pClass)
    {
        //TODO: check this and refactor!!!
        lua_State* v1; // esi
        m3d::Class* v3; // eax
        m3d::ExportInfo* v4; // edi
        m3d::ExportInfo* v5; // ebx
        m3d::eExportType v6; // eax
        int v7; // edx

        v1 = m3d::ScriptServer::L;
        if (pClass)
        {
            v3 = pClass->m_fnGetBaseClass();
            _addExports(v3);
        	v4 = pClass->m_lExports;
            if (v4)
            {
                if (v4->name)
                {
                    v5 = v4;
                    while (1)
                    {
                        lua_pushstring(v1, v4->name);
                        v6 = v4->type;
                        if (v6 == m3d::METHOD)
                            break;
                        if (v6 == m3d::NATIVE_METHOD)
                        {
                            auto newData = reinterpret_cast<void**>(lua_newuserdata(v1, 4u));
                            *newData = v4->addr1;
                            v7 = m3d::ScriptServer::m_metatable_ClassNativeMethod;
                        LABEL_9:
                            lua_rawgeti(v1, -10000, v7);
                            lua_setmetatable(v1, -2);
                        }
                        lua_settable(v1, -3);
                        v4 = ++v5;
                        if (!v5->name)
                            return;
                    }
                    auto data = reinterpret_cast<void**>(lua_newuserdata(v1, 4u));
                    *data = v4->addr1;
                    v7 = m3d::ScriptServer::m_metatable_ClassMethod;
                    goto LABEL_9;
                }
            }
        }
    }

    void _buildExportMap(m3d::Class* pClass)
    {
        int n; // eax

        lua_newtable(m3d::ScriptServer::L);
        n = luaL_ref(m3d::ScriptServer::L, -10000);
        pClass->m_scriptHandle = reinterpret_cast<void*>(n);
        lua_rawgeti(m3d::ScriptServer::L, -10000, n);
        lua_pushstring(m3d::ScriptServer::L, "internalTag");
        lua_pushnumber(m3d::ScriptServer::L, 1002.0);
        lua_settable(m3d::ScriptServer::L, -3);
        _addExports(pClass);
    	lua_settop(m3d::ScriptServer::L, -2);
    }

    int _indexObject(lua_State* L)
    {
        lua_rawgeti(L, 1, 1);
        lua_insert(L, -2);
        lua_gettable(L, -2);
        return 1;
    }

    lua_CFunction oldToString = nullptr;
    m3d::ScriptServer* g_scriptServer = nullptr;
    m3d::auxScriptErrorDesc errDesc;
}

namespace m3d
{
    Class ScriptServer::m_classScriptServer {"ScriptServer", sizeof(ScriptServer), CreateObject, GetBaseClass};

    eScriptError Scriptlet::compile()
    {
	    throw std::logic_error("Not implemented");
    }

    Scriptlet::~Scriptlet()
    {
        delete[] m_data;
    }

    eScriptError Scriptlet::loadFromFile(char const* fileName)
    {
        delete[] m_data;
        m_data = nullptr;
        m_bLoaded = false;
        scoped_ptr stream = g_Kernel->GetFileServer().CreateFileStream();
        if (stream->Open(fileName, fs::IStream::OPEN_READ))
        {
            m_dataLen = stream->GetSize();
            m_data = new char[m_dataLen];
            if (stream->ReadBytes(m_data, m_dataLen))
            {
                stream->Close();
                m_bLoaded = true;
                return SUCCESS;
            }
            else
            {
                M3D_LOG_INFO("Could not read script file " + CStr(fileName));
                return FILE_NOT_FOUND;
            }
        }
        else
        {
            M3D_LOG_INFO("Could not open script file " + CStr(fileName));
            return FILE_NOT_FOUND;
        }
    }

    eScriptError Scriptlet::execute(char const* nameAs, bool bGlobalEnv)
    {
        if (!m_bLoaded)
        {
            return OTHER_ERROR;
        }
        if (m_bCompiled)
        {
            return g_scriptServer->executeBuffer(m_compiledData, m_compiledDataLen, nameAs);
        }
        g_scriptServer->executeBuffer(m_data, m_dataLen, nameAs);
    }

    Scriptlet::Scriptlet()
    {
    }

    Class* ScriptServer::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    Object* ScriptServer::CreateObject()
    {
        return new ScriptServer;
    }

    int ScriptServer::_getScriptObject(Object* pObj)
    {
        if (!pObj->m_scriptHandle)
        {
            lua_newtable(L);
            lua_pushlightuserdata(L, pObj);
            lua_rawseti(L, -2, 0);
            auto cls  = pObj->GetClass();
            if (!cls->m_scriptHandle)
            {
                _buildExportMap(cls);
            }
        	lua_rawgeti(L, -10000, reinterpret_cast<int>(cls->m_scriptHandle));
            lua_type(L, -1);
            lua_type(L, -2);
            lua_rawseti(L, -2, 1);
            lua_newtable(L);
            lua_pushstring(L, "__index");
            lua_pushcclosure(L, _indexObject, 0);
        	lua_settable(L, -3);
            lua_setmetatable(L, -2);
            pObj->m_scriptHandle = reinterpret_cast<void*>(luaL_ref(L, -10000));
        }
        return reinterpret_cast<int>(pObj->m_scriptHandle);
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
        for (auto& script : m_scripts)
        {
            delete script.second;
        }
        m_scripts.clear();
        if (L)
        {
            lua_close(L);
            L = nullptr;
        }
        m_bInitialized = false;
        return SUCCESS;
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

    eScriptError ScriptServer::executeBuffer(void* buf, unsigned bufSize, char const* bufName)
    {
        if (!m_bInitialized)
        {
            return NOT_INITIALIZED;
        }
        if (bufName)
        {
            m_lastScriptExecuted = bufName;
            UnifyFileName(m_lastScriptExecuted);
            errDesc.sourceString = m_lastScriptExecuted;
        }
        switch(lua_dobuffer(L, static_cast<const char*>(buf), bufSize, m_lastScriptExecuted.c_str()))
        {
        case 0:
            return  SUCCESS;
        case 1:
            return  RUNTIME_ERROR;
        case 3:
            return  SYNTAX_ERROR;
        case 4:
            return  MEMORY_ERROR;
        default:
            return OTHER_ERROR;
        }
    }

    ScriptServer::~ScriptServer()
    {
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
        return RT_CLASS_LOCAL(ScriptServer);
    }

    char const* ScriptServer::getErrorDescString(eScriptError) const
    {
        throw std::logic_error("Not implemented");
    }

    void ScriptServer::dumpStack()
    {
        throw std::logic_error("Not implemented");
    }

    eScriptError ScriptServer::executeScriptFile(char const* fileName)
    {
        if (!m_bInitialized)
        {
            return NOT_INITIALIZED;
        }
        CStr unifiedFileName = fileName;
        UnifyFileName(unifiedFileName);
        m_lastScriptExecuted = unifiedFileName;
        errDesc.sourceString = m_lastScriptExecuted;
        auto it = m_scripts.find(unifiedFileName);
        if (it == m_scripts.end())
        {
            Scriptlet scriptlet;
            auto res = scriptlet.loadFromFile(unifiedFileName.c_str());
            if (res == SUCCESS)
            {
                res = scriptlet.execute(unifiedFileName.c_str(), true);
	            if (res)
	            {
                    M3D_LOG_ERR(getFormatedScriptErrorDesc(res));
	            }
            }
            return res;
        }
        else
        {
            return it->second->execute(unifiedFileName.c_str(), true);
        }
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

ext_InternalTags ext_getTag(lua_State* L, int pos)
{
    //TODO: check this and refactor
    int v4; // eax
    int v5; // eax
    int v7; // edx
    ext_InternalTags v8; // edi

    v4 = lua_type(L, pos) - 2;
    if (v4)
    {
        v5 = v4 - 3;
        if (v5)
        {
            if (v5 != 2)
                return tag_Unknown;
        }
    }
    lua_pushstring(L, "internalTag");
    v7 = pos - 1;
    if (pos >= 0)
        v7 = pos;
    lua_gettable(L, v7);
    v8 = tag_Unknown;
    if (lua_isnumber(L, -1))
        v8 = static_cast<ext_InternalTags>(lua_tonumber(L, -1));
    lua_settop(L, -2);
    return v8;
}

bool ext_checkTag(lua_State* L, int pos, ext_InternalTags tag)
{
    return ext_getTag(L, pos) == tag;
}

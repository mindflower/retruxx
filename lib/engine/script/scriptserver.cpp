#include <script/scriptserver.h>
#include <script/luaquaternion.h>
#include <script/luavector.h>

#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "math/vector.h"
#include "script/scriptcontext.h"

extern "C"
{
#include <lualib.h>
#include <lauxlib.h>
}
#include <stdexcept>

const char* ScriptErrorDesc[9] = {
    "success",
    "ScriptServer::init() was not called or that call had been failed",
    "file not found or could not be read",
    "run-time script error",
    "syntax error during pre-compilation",
    "memory allocation error",
    "unknown error",
    "no such function",
    "function is already registered",
};

namespace
{
    m3d::auxScriptErrorDesc errDesc;
    lua_CFunction oldToString = nullptr;
    m3d::ScriptServer* g_scriptServer = nullptr;

    int _getGlobalObject(lua_State* L)
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

    int _getLastErrorInfo(lua_State* L, lua_Debug* dbgInfo)
    {
        int v4 = 1;
        int level = 1;
        int v5 = 0;
        while (1)
        {
            v5 = lua_getstack(L, v4, dbgInfo);
            if (v5)
                break;
            v4 = ++level;
            if (!v5)
                return 0;
        }
        lua_getinfo(L, "Sln", dbgInfo);
        if (strcmp(dbgInfo->what, "Lua") && strcmp(dbgInfo->what, "main"))
        {
            v4 = ++level;
            if (!v5)
                return 0;
        }
        return 1;
    }

    int _logMethod(lua_State* L)
    {
        auto v2 = lua_gettop(L);
        lua_pushstring(L, "tostring");
        lua_gettable(L, -10001);

        CStr buf;
        for (int i = 1; i <= v2; ++i)
        {
            lua_pushvalue(L, -1);
            lua_pushvalue(L, i);
            lua_call(L, 1, 1);
            auto v4 = lua_tostring(L, -1);
            if (!v4)
            {
                lua_pushstring(L, "ScriptSystem::_logMethod(): `tostring' must return a string to `print'");
                lua_error(L);
            }
            if (i > 1)
            {
                buf += "\t";
            }
            buf += v4;
            lua_settop(L, -2);
        }

        lua_Debug ar;
        if (_getLastErrorInfo(L, &ar))
        {
            auto v6 = NameFromFileName(g_scriptServer->getNameOfLastScript());
            m3d::g_Kernel->m_Log->sourceLine() = ar.currentline;
            m3d::g_Kernel->m_Log->setSourceFile(v6.c_str());
            m3d::g_Kernel->m_Log->logTex(buf, m3d::LOG_INFO);
        }
        return 0;
    }

    int _execLuaScript(lua_State* L)
    {
        luaL_checktype(L, 1, 4);
        auto file = lua_tostring(L, 1);
        return m3d::Scriptlet::g_scriptServer->executeScriptFile(file);
    }

    int _errorMethod(lua_State* L)
    {
        luaL_checktype(L, 1, 4);
        errDesc.descriptionString = lua_tostring(L, 1);

        lua_Debug ar;
        if (_getLastErrorInfo(L, &ar))
        {
            errDesc.lineNumber = ar.currentline;
            errDesc.defLineNumber = ar.linedefined;
            errDesc.whatString = ar.what;
            errDesc.nameString = ar.name;
            errDesc.nameWhatString = ar.namewhat;
        }
        else
        {
            errDesc.lineNumber = 0;
            errDesc.defLineNumber = 0;
            errDesc.whatString = {};
            errDesc.nameString = {};
            errDesc.nameWhatString = {};
        }
        return 1;
    }

    int _callClassMethod(lua_State* L)
    {
        m3d::LuaContext ctx;
        ctx.L = L;
        ctx.m_stackStart = 2;
        ctx.m_numInputs = lua_gettop(L) - 1;
        ctx.m_numOutputs = 0;
        auto func = reinterpret_cast<int (**)(m3d::Context*)>(lua_touserdata(L, 1));
        (*func)(&ctx);
        return ctx.m_numOutputs;
    }

    int _callClassNativeMethod(lua_State* L)
    {
        // RVA 0x620A80 - __call of a NATIVE_METHOD export: (func userdata, self, args...)
        m3d::LuaContext ctx;
        ctx.L = L;
        ctx.m_stackStart = 2;
        ctx.m_numInputs = lua_gettop(L) - 1;
        ctx.m_numOutputs = 0;
        // NOTE: the userdata block itself is called, while _addExports stores the function
        // address *inside* it, so invoking any native method jumps into data. No shipped class
        // seems to export one.
        auto const func = reinterpret_cast<int(__fastcall*)(m3d::Object*, m3d::sArgStack*)>(lua_touserdata(L, 1));
        m3d::Object* const self = ctx.asObject(0, "Object");

        // NOTE: the arguments are popped from the top, so they reach the function in reverse
        // order. Booleans and full userdata (script vectors and quaternions) are dropped, and a
        // tagged instance table yields a null object since lua_touserdata is used on a table.
        m3d::sArgStack stack;
        while (lua_gettop(L) != 2)
        {
            switch (lua_type(L, -1))
            {
            case LUA_TNIL:
                stack.newIn()->SetB(false);
                break;
            case LUA_TNUMBER:
                stack.newIn()->SetF(static_cast<float>(lua_tonumber(L, -1)));
                break;
            case LUA_TSTRING:
                stack.newIn()->SetS(lua_tostring(L, -1));
                break;
            case LUA_TTABLE:
                switch (ext_getTag(L, -1))
                {
                case tag_luaVector:
                    stack.newIn()->SetV(*static_cast<CVector*>(lua_touserdata(L, -1)));
                    break;
                case tag_instance:
                    stack.newIn()->SetO(static_cast<m3d::Object*>(lua_touserdata(L, -1)));
                    break;
                case tag_luaQuaternion:
                    stack.newIn()->SetQ(*static_cast<Quaternion*>(lua_touserdata(L, -1)));
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            lua_settop(L, -2);
        }

        if (!func(self, &stack))
        {
            // NOTE: -1 is handed back to Lua as the number of results.
            return -1;
        }

        for (unsigned i = 0; i < stack.getNumOutArgs(); ++i)
        {
            m3d::sArg* const out = stack.popOut();
            switch (out->GetType())
            {
            case m3d::sArg::ARGTYPE_INT:
                lua_pushnumber(L, out->GetI());
                break;
            case m3d::sArg::ARGTYPE_FLOAT:
                lua_pushnumber(L, out->GetF());
                break;
            case m3d::sArg::ARGTYPE_BOOL:
                // NOTE: true goes out as the number 1 and false as nil.
                if (out->GetB())
                {
                    lua_pushnumber(L, 1.0);
                }
                else
                {
                    lua_pushnil(L);
                }
                break;
            case m3d::sArg::ARGTYPE_STRING:
                lua_pushstring(L, out->GetS());
                break;
            case m3d::sArg::ARGTYPE_OBJECT:
                if (auto* const obj = out->GetO())
                {
                    lua_rawgeti(L, LUA_REGISTRYINDEX, m3d::ScriptServer::_getScriptObject(obj));
                }
                else
                {
                    lua_pushnil(L);
                }
                break;
            default:
                // NOTE: vectors and quaternions push nothing but are still counted as results.
                break;
            }
        }
        return stack.getNumOutArgs();
    }

    char buf_0[50] = {0};
    char buf[50] = {0};

    int _toString(lua_State* L)
    {
        auto v2 = ext_getTag(L, -1) - 1001;
        if (v2)
        {
            if (v2 == 2)
            {
                auto v4 = (float*)lua_touserdata(L, -1);
                sprintf(buf_0, "(%.3f, %.3f, %.3f, %.3f)", *v4, v4[1], v4[2], v4[3]);
                lua_pushstring(L, buf_0);
                return 1;
            }
            else
            {
                return oldToString(L);
            }
        }
        else
        {
            auto v5 = (float*)lua_touserdata(L, -1);
            sprintf(buf, "(%.3f, %.3f, %.3f)", *v5, v5[1], v5[2]);
            lua_pushstring(L, buf);
            return 1;
        }
    }

    int _callNativeGlobalFunction(lua_State* L)
    {
        // RVA 0x6205A0 - __call of a native global function: (func userdata, args...)
        auto const func = *static_cast<int(__thiscall**)(m3d::sArgStack*)>(lua_touserdata(L, 1));

        m3d::sArgStack stack;
        for (int i = 2; i <= lua_gettop(L); ++i)
        {
            switch (lua_type(L, i))
            {
            case LUA_TNIL:
                stack.newIn()->SetB(false);
                break;
            case LUA_TBOOLEAN:
                stack.newIn()->SetB(lua_toboolean(L, i) != 0);
                break;
            case LUA_TLIGHTUSERDATA:
            case LUA_TUSERDATA:
                if (ext_getTag(L, i) == tag_luaVector)
                {
                    stack.newIn()->SetV(*static_cast<CVector*>(lua_touserdata(L, i)));
                }
                else if (ext_getTag(L, i) == tag_luaQuaternion)
                {
                    stack.newIn()->SetQ(*static_cast<Quaternion*>(lua_touserdata(L, i)));
                }
                break;
            case LUA_TNUMBER:
                stack.newIn()->SetF(static_cast<float>(lua_tonumber(L, i)));
                break;
            case LUA_TSTRING:
                stack.newIn()->SetS(lua_tostring(L, i));
                break;
            case LUA_TTABLE:
                if (ext_getTag(L, i) == tag_instance)
                {
                    lua_rawgeti(L, i, 0);
                    auto* const obj = static_cast<m3d::Object*>(lua_touserdata(L, -1));
                    lua_settop(L, -2);
                    stack.newIn()->SetO(obj);
                }
                break;
            default:
                break;
            }
        }

        if (!func(&stack))
        {
            return 0;
        }

        for (unsigned i = 0; i < stack.getNumOutArgs(); ++i)
        {
            m3d::sArg* const out = stack.popOut();
            switch (out->GetType())
            {
            case m3d::sArg::ARGTYPE_INT:
                lua_pushnumber(L, out->GetI());
                break;
            case m3d::sArg::ARGTYPE_FLOAT:
                lua_pushnumber(L, out->GetF());
                break;
            case m3d::sArg::ARGTYPE_BOOL:
                // Script booleans are 1 or nil.
                if (out->GetB())
                {
                    lua_pushnumber(L, 1.0);
                }
                else
                {
                    lua_pushnil(L);
                }
                break;
            case m3d::sArg::ARGTYPE_STRING:
                lua_pushstring(L, out->GetS());
                break;
            case m3d::sArg::ARGTYPE_VECTOR:
                *ext_createVector(L) = out->GetV();
                break;
            case m3d::sArg::ARGTYPE_QUATERNION:
                *ext_createQuaternion(L) = out->GetQ();
                break;
            case m3d::sArg::ARGTYPE_OBJECT:
                if (m3d::Object* const obj = out->GetO())
                {
                    lua_rawgeti(L, LUA_REGISTRYINDEX, m3d::ScriptServer::_getScriptObject(obj));
                }
                else
                {
                    lua_pushnil(L);
                }
                break;
            default:
                // NOTE: an output of any other type pushes nothing, although it is still counted in the result.
                break;
            }
        }
        return stack.getNumOutArgs();
    }

    void _addExports(m3d::Class* pClass)
    {
        // RVA 0x61F970 - adds the exports of pClass and its bases to the export table on top of the stack, base
        // classes first so that derived classes override them.
        lua_State* const L = m3d::ScriptServer::L;
        if (!pClass)
        {
            return;
        }
        _addExports(pClass->m_fnGetBaseClass());

        if (!pClass->m_lExports)
        {
            return;
        }
        for (m3d::ExportInfo const* exp = pClass->m_lExports; exp->name; ++exp)
        {
            lua_pushstring(L, exp->name);

            // Each method is a userdata holding its address, called through the metatable's __call.
            // NOTE: an export of any other type pushes no value, so lua_settable below would take the name as
            // the value. Only METHOD and NATIVE_METHOD exist.
            if (exp->type == m3d::METHOD || exp->type == m3d::NATIVE_METHOD)
            {
                *static_cast<void**>(lua_newuserdata(L, sizeof(void*))) = exp->addr1;
                lua_rawgeti(
                    L,
                    LUA_REGISTRYINDEX,
                    exp->type == m3d::METHOD ? m3d::ScriptServer::m_metatable_ClassMethod
                                             : m3d::ScriptServer::m_metatable_ClassNativeMethod);
                lua_setmetatable(L, -2);
            }
            lua_settable(L, -3);
        }
    }

    void _buildExportMap(m3d::Class* pClass)
    {
        int n;  // eax

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
}  // namespace

void _dumpStack(lua_State* L)
{
    // RVA 0x621F00
    for (int i = 1; i <= lua_gettop(L); ++i)
    {
        int const type = lua_type(L, i);
        char buffer[64];
        sprintf(buffer, "%2d: %s ", i, lua_typename(L, type));
        CStr const tmp(buffer);
        switch (type)
        {
        case LUA_TLIGHTUSERDATA:
            sprintf(buffer, "(light user data)");
            break;
        case LUA_TNUMBER:
            sprintf(buffer, "(%f)", static_cast<double>(lua_tonumber(L, i)));
            break;
        case LUA_TSTRING:
            // NOTE: unbounded; a long string overruns the 64-byte buffer.
            sprintf(buffer, "(\"%s\")", lua_tostring(L, i));
            break;
        case LUA_TTABLE:
            sprintf(buffer, "(table)");
            break;
        case LUA_TUSERDATA:
            sprintf(buffer, "(user data)");
            break;
        default:
            buffer[0] = 0;
            break;
        }
        M3D_KERNEL->m_Log->sourceLine() = __LINE__;
        M3D_KERNEL->m_Log->setSourceFile(__FILE__);
        M3D_KERNEL->m_Log->logRaw((tmp + CStr(buffer) + CStr("\n")).c_str());
    }
}

namespace m3d
{
    Class ScriptServer::m_classScriptServer{"ScriptServer", sizeof(ScriptServer), CreateObject, GetBaseClass};

    eScriptError Scriptlet::compile()
    {
        // RVA 0x895B20 - there is no compiler; this only reports whether the source is loaded.
        return m_bLoaded ? SUCCESS : OTHER_ERROR;
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
        // RVA 0x895B30
        if (!m_bLoaded)
        {
            return OTHER_ERROR;
        }
        if (m_bCompiled)
        {
            return g_scriptServer->executeBuffer(m_compiledData, m_compiledDataLen, nameAs);
        }
        return g_scriptServer->executeBuffer(m_data, m_dataLen, nameAs);
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
            auto cls = pObj->GetClass();
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

    eScriptError ScriptServer::reloadScript(char const* fileName)
    {
        // RVA 0x622A40
        if (!m_bInitialized)
        {
            return NOT_INITIALIZED;
        }
        // NOTE: unlike addScript and executeScriptFile, the name is looked up without
        // UnifyFileName, so it must already be in unified form.
        auto const it = m_scripts.find(CStr(fileName));
        if (it == m_scripts.end())
        {
            return OTHER_ERROR;
        }
        return it->second->loadFromFile(fileName);
    }

    eScriptError ScriptServer::callScriptFunc(char const* funcName, sArgStack& stack, int nresults)
    {
        // RVA 0x6216F0 - calls the global Lua function funcName with the input arguments of `stack` and appends up
        // to nresults of its results (all of them, up to 100, for -1) to the stack's outputs.
        if (!m_bInitialized)
        {
            return NOT_INITIALIZED;
        }

        errDesc.sourceString = CStr(funcName);

        lua_pushstring(L, funcName);
        lua_gettable(L, LUA_GLOBALSINDEX);
        if (lua_type(L, -1) == LUA_TNIL)
        {
            lua_settop(L, -2);
            return NO_SUCH_FUNCTION;
        }

        for (unsigned i = 0; i < stack.getNumInArgs(); ++i)
        {
            sArg* const arg = stack.popIn();
            switch (arg->GetType())
            {
            case sArg::ARGTYPE_INT:
                lua_pushnumber(L, arg->GetI());
                break;
            case sArg::ARGTYPE_FLOAT:
                lua_pushnumber(L, arg->GetF());
                break;
            case sArg::ARGTYPE_BOOL:
                // Script booleans are 1 or nil.
                if (arg->GetB())
                {
                    lua_pushnumber(L, 1.0);
                }
                else
                {
                    lua_pushnil(L);
                }
                break;
            case sArg::ARGTYPE_STRING:
                lua_pushstring(L, arg->GetS());
                break;
            case sArg::ARGTYPE_VECTOR:
                *ext_createVector(L) = arg->GetV();
                break;
            case sArg::ARGTYPE_OBJECT:
                if (Object* const obj = arg->GetO())
                {
                    lua_rawgeti(L, LUA_REGISTRYINDEX, _getScriptObject(obj));
                }
                else
                {
                    lua_pushnil(L);
                }
                break;
            case sArg::ARGTYPE_QUATERNION:
                *ext_createQuaternion(L) = arg->GetQ();
                break;
            default:
                // NOTE: the function and the arguments pushed so far are left on the Lua stack.
                return OTHER_ERROR;
            }
        }

        switch (lua_pcall(L, stack.getNumInArgs(), nresults, 0))
        {
        case 0:
            break;
        case LUA_ERRRUN:
            return RUNTIME_ERROR;
        case LUA_ERRSYNTAX:
            return SYNTAX_ERROR;
        case LUA_ERRMEM:
            return MEMORY_ERROR;
        default:
            return OTHER_ERROR;
        }

        if (nresults == -1)
        {
            nresults = 100;
        }

        // The results are taken from the top of the stack down, i.e. the last result becomes the first output.
        for (int j = 0; j < nresults && lua_gettop(L); ++j)
        {
            switch (lua_type(L, -1))
            {
            case LUA_TNIL:
                stack.newOut()->SetB(false);
                break;
            case LUA_TNUMBER:
                stack.newOut()->SetF(static_cast<float>(lua_tonumber(L, -1)));
                break;
            case LUA_TSTRING:
                stack.newOut()->SetS(lua_tostring(L, -1));
                break;
            case LUA_TTABLE:
            {
                // A script object: its native Object is stored at index 0.
                lua_rawgeti(L, -1, 0);
                auto* const obj = static_cast<Object*>(lua_touserdata(L, -1));
                stack.newOut()->SetO(obj);
                lua_settop(L, -2);
                break;
            }
            case LUA_TUSERDATA:
                // NOTE: any full userdata is read as a vector, without checking its tag.
                stack.newOut()->SetV(*static_cast<CVector*>(lua_touserdata(L, -1)));
                break;
            default:
                break;
            }
            lua_settop(L, -2);
        }
        return SUCCESS;
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
        return this->m_lastScriptExecuted.c_str();
    }

    retruxx::map<CStr, ScriptServer::auxFuncDesc> const& ScriptServer::getRegisteredFunctionsDesc() const
    {
        // RVA 0x8BB6E0
        return m_funcDescs;
    }

    eScriptError ScriptServer::addScript(char const* fileName)
    {
        // RVA 0x6241C0
        if (!m_bInitialized)
        {
            return NOT_INITIALIZED;
        }
        CStr fName(fileName);
        UnifyFileName(fName);
        if (m_scripts.find(fName) != m_scripts.end())
        {
            return SUCCESS;
        }
        auto* const scriptlet = new Scriptlet;
        // NOTE: loads from the name as given, not the unified one it is stored under.
        eScriptError const res = scriptlet->loadFromFile(fileName);
        if (res != SUCCESS)
        {
            delete scriptlet;
            return res;
        }
        m_scripts[fName] = scriptlet;
        return SUCCESS;
    }

    eScriptError ScriptServer::execute(char const* str, char const* bufName)
    {
        if (this->m_bInitialized)
        {
            return executeBuffer((void*)str, strlen(str), bufName);
        }
        return NOT_INITIALIZED;
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
        switch (lua_dobuffer(L, static_cast<char const*>(buf), bufSize, m_lastScriptExecuted.c_str()))
        {
        case 0:
            return SUCCESS;
        case 1:
            return RUNTIME_ERROR;
        case 3:
            return SYNTAX_ERROR;
        case 4:
            return MEMORY_ERROR;
        default:
            return OTHER_ERROR;
        }
    }

    ScriptServer::~ScriptServer()
    {
    }

    lua_State* ScriptServer::getGlobalEnvironment()
    {
        // RVA 0x895AE0
        return L;
    }

    eScriptError ScriptServer::reloadAllScripts()
    {
        // RVA 0x622AC0 - reloads every source, returning the last error met.
        if (!m_bInitialized)
        {
            return NOT_INITIALIZED;
        }
        eScriptError res = SUCCESS;
        for (auto& [name, scriptlet] : m_scripts)
        {
            if (eScriptError const err = scriptlet->loadFromFile(name.c_str()))
            {
                res = err;
            }
        }
        return res;
    }

    CStr ScriptServer::getFormatedScriptErrorDesc(eScriptError err) const
    {
        // RVA 0x621500 - "(source/name @ line): error\ndescription"
        CStr desc;
        if (err)
        {
            desc = CStr("(");
            desc += errDesc.sourceString;
            desc += CStr("/");
            desc += errDesc.nameString;
            desc += CStr(" @ ");
            desc += CStr(errDesc.lineNumber);
            desc += CStr("): ");
            desc += CStr(ScriptErrorDesc[err]);
            desc += CStr("\n");
            desc += errDesc.descriptionString;
        }
        return desc;
    }

    auxScriptErrorDesc const& ScriptServer::getLastErrorDesc() const
    {
        // RVA 0x61F930
        return errDesc;
    }

    Class* ScriptServer::GetClass() const
    {
        return RT_CLASS_LOCAL(ScriptServer);
    }

    char const* ScriptServer::getErrorDescString(eScriptError err) const
    {
        // RVA 0x61F920 - no range check
        return ScriptErrorDesc[err];
    }

    void ScriptServer::dumpStack()
    {
        // RVA 0x6225A0
        _dumpStack(L);
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
        // RVA 0x6249A0
        // NOTE: a member-wise copy, so both servers own the same Scriptlet pointers.
        return new ScriptServer(*this);
    }

    eScriptError ScriptServer::registerGlobalFunction(
        int (*NativeGlobalFunc)(sArgStack&),
        char const* name,
        char const* returnValue,
        char const* params,
        char const* shortDesc)
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

        auto data = (int (**)(sArgStack&))lua_newuserdata(L, sizeof(NativeGlobalFunc));
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
}  // namespace m3d

ext_InternalTags ext_getTag(lua_State* L, int pos)
{
    // RVA 0x61F870 - the "internalTag" field of a table or (light) userdata; tag_Unknown for anything else.
    int const type = lua_type(L, pos);
    if (type != LUA_TLIGHTUSERDATA && type != LUA_TTABLE && type != LUA_TUSERDATA)
    {
        return tag_Unknown;
    }

    lua_pushstring(L, "internalTag");
    // A relative index moves down by one once the key is pushed.
    lua_gettable(L, pos >= 0 ? pos : pos - 1);
    ext_InternalTags tag = tag_Unknown;
    if (lua_isnumber(L, -1))
    {
        tag = static_cast<ext_InternalTags>(static_cast<int>(lua_tonumber(L, -1)));
    }
    lua_settop(L, -2);
    return tag;
}

bool ext_checkTag(lua_State* L, int pos, ext_InternalTags tag)
{
    return ext_getTag(L, pos) == tag;
}

#include <cassert>
#include <stdexcept>
#include <script/scriptcontext.h>

#include "core/kernel.h"
#include "core/log.h"
#include "math/vector.h"
#include "script/luaaiparam.h"
#include "script/luavector.h"
#include "script/luaquaternion.h"
#include "script/scriptserver.h"

extern "C"{
#include "lauxlib.h"
}

namespace m3d
{
	void LuaContext::pushObject(Object* x)
	{
        if (x)
        {
            auto scriptObject = ScriptServer::_getScriptObject(x);
            lua_rawgeti(this->L, -10000, scriptObject);

        }
        else
        {
            lua_pushnil(this->L);
        }
        ++this->m_numOutputs;
	}

	void LuaContext::pushAIParam(AIParam const& x)
	{
		auto param = ext_createAIParam(this->L);
		*param = x;
		++this->m_numOutputs;
	}

	int LuaContext::asInt(int i)
	{
        // RVA 0x899C00 - accepts a number or a numeric string; anything else raises a Lua type error.
        if (i < 0)
        {
            lua_pushstring(L, "not enough arguments");
            lua_error(L);
        }
        int const pos = i + m_stackStart;
        switch (lua_type(L, pos))
        {
        case LUA_TNUMBER:
            return static_cast<int>(lua_tonumber(L, pos));
        case LUA_TSTRING:
            return atoi(lua_tostring(L, pos));
        default:
            luaL_checktype(L, pos, LUA_TNUMBER);
            return 0;
        }
	}

	bool LuaContext::asBool(int i)
	{
		if (i < 0)
		{
			lua_pushstring(this->L, "not enough arguments");
			lua_error(this->L);
		}
		auto pos = i + this->m_stackStart;
		auto type = lua_type(this->L, pos);
		if (!type)
			return 0;
		auto v5 = type - 1;
		if (!v5)
			return lua_toboolean(this->L, pos) != 0;
		if (v5 == 2)
			return lua_tonumber(this->L, pos) != 0;
		return 1;
	}

	Object* LuaContext::asObject(int i, char const* className)
	{
		if (i < 0)
		{
			lua_pushstring(L, "not enough arguments");
			lua_error(L);
		}
		auto pos = i + m_stackStart;
		if (!lua_type(L, pos))
		{
			return 0;
		}
		bool res1 = ext_checkTag(L, pos, tag_instance);
		lua_rawgeti(L, pos, 0);
		auto obj = reinterpret_cast<Object*>(lua_touserdata(L, -1));
		auto cls = m3d::g_Kernel->FindClass(className);
		bool res2 = obj->IsKindOf(cls);
		if (!res1 || !res2)
		{
			SYS_ERROR("!\"Very bad!!! Most likely you've typed '.' instead of ':'. Or some function param is ID but an object type is expected. Ignore this assertion and look to the log file for details...\"");
			M3D_LOG_INFO("Invalid argument, " + CStr(className) + " expected");
			if (obj)
			{
				M3D_LOG_INFO("Got " + CStr(obj->GetClassNameA()));
			}
			if (!i)
			{
				M3D_LOG_INFO("Did you use '.' instead of ':'?");
			}
			lua_error(L);
		}
		lua_settop(L, -2);
		return obj;
	}

	int LuaContext::countArgs()
	{
		// RVA 0x8999C0
		return m_numInputs;
	}

	float LuaContext::asFloat(int i)
	{
		if (i < 0)
		{
			lua_pushstring(this->L, "not enough arguments");
			lua_error(this->L);
		}
		auto v3 = i + this->m_stackStart;
		auto v4 = lua_type(this->L, v3) - 3;
		L = this->L;
		if (!v4)
			return lua_tonumber(L, v3);
		if (v4 == 1)
		{
			auto v7 = lua_tostring(L, v3);
			return atof(v7);
		}
		else
		{
			luaL_checktype(L, v3, 3);
			return 0.0;
		}
	}

	AIParam& LuaContext::asAIParam(int i)
	{
        // RVA 0x89A270 - a number, string or vector argument is converted into a new AIParam userdata pushed on
        // the Lua stack. It stays alive while it is on the stack, i.e. until the calling C function returns.
        if (i < 0)
        {
            lua_pushstring(L, "not enough arguments");
            lua_error(L);
        }

        int const pos = i + m_stackStart;
        switch (lua_type(L, pos))
        {
        case LUA_TNUMBER:
        {
            AIParam* const result = ext_createAIParam(L);
            *result = static_cast<float>(lua_tonumber(L, pos));
            return *result;
        }
        case LUA_TSTRING:
        {
            AIParam* const result = ext_createAIParam(L);
            *result = CStr(lua_tostring(L, pos));
            return *result;
        }
        case LUA_TUSERDATA:
            if (ext_checkTag(L, pos, tag_luaAIParam))
            {
                return *static_cast<AIParam*>(lua_touserdata(L, pos));
            }
            if (ext_checkTag(L, pos, tag_luaVector))
            {
                AIParam* const result = ext_createAIParam(L);
                *result = *static_cast<CVector*>(lua_touserdata(L, pos));
                return *result;
            }
            [[fallthrough]];
        default:
            M3D_LOG_INFO("Invalid argument type.");
            lua_pushstring(L, "Invalid argument type.");
            lua_error(L);
            break;
        }
        // Not reached: lua_error does not return.
        return *ext_createAIParam(L);
	}

	void LuaContext::pushBool(bool x)
	{
		if (x)
			lua_pushnumber(L, 1.0);
		else
			lua_pushnil(L);
		++this->m_numOutputs;
	}

	char const* LuaContext::asString(int i)
	{
		int v3; // esi

		if (i < 0)
		{
			lua_pushstring(this->L, "not enough arguments");
			lua_error(this->L);
		}
		v3 = i + this->m_stackStart;
		luaL_checktype(this->L, v3, 4);
		return lua_tostring(this->L, v3);
	}

	void LuaContext::pushQuaternion(Quaternion const& x)
	{
		// RVA 0x899B00
		*ext_createQuaternion(L) = x;
		++m_numOutputs;
	}

	void LuaContext::pushInt(int x)
	{
		lua_pushnumber(L, x);
		++m_numOutputs;
	}

	void LuaContext::pushFloat(float x)
	{
		lua_pushnumber(this->L, x);
		++this->m_numOutputs;
	}

	Quaternion& LuaContext::asQuaternion(int i)
	{
		if (i < 0)
		{
			lua_pushstring(this->L, "not enough arguments");
			lua_error(this->L);
		}
		auto pos = i + this->m_stackStart;
		assert(ext_checkTag(L, pos, tag_luaQuaternion));
		return *(Quaternion*)lua_touserdata(this->L, pos);
	}

	void LuaContext::pushString(char const* x)
	{
		lua_pushstring(this->L, x);
		++this->m_numOutputs;
	}

	CVector& LuaContext::asVector(int i)
	{
		if (i < 0)
		{
			lua_pushstring(this->L, "not enough arguments");
			lua_error(this->L);
		}
		auto pos = i + this->m_stackStart;
		assert(ext_checkTag(L, pos, tag_luaVector));
		return *(CVector*)lua_touserdata(this->L, pos);
	}

	void LuaContext::pushVector(CVector const& x)
	{
		auto* vec = ext_createVector(L);
		*vec = x;
        ++m_numOutputs;
	}

	int LuaContext::_validateArg(int i)
	{
		// RVA 0x899B30
		if (i < 0)
		{
			lua_pushstring(this->L, "not enough arguments");
			lua_error(this->L);
		}
		return i + this->m_stackStart;
	}
}

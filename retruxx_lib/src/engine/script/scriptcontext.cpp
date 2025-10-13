#include <cassert>
#include <stdexcept>
#include <script/scriptcontext.h>

#include "core/kernel.h"
#include "core/log.h"
#include "math/vector.h"
#include "script/luaaiparam.h"
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
            auto ScriptObject = ScriptServer::_getScriptObject(x);
            lua_rawgeti(this->L, -10000, ScriptObject);
        }
        else
        {
            lua_pushnil(this->L);
        }
        ++this->m_numOutputs;
	}

	void LuaContext::pushAIParam(AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	int LuaContext::asInt(int i)
	{
        // TODO: check this
        if (i < 0)
        {
            lua_pushstring(this->L, "not enough arguments");
            lua_error(this->L);
        }
        auto v3 = i + this->m_stackStart;
        auto v4 = lua_type(this->L, v3) - 3;
        L = this->L;
        if (!v4)
            return (int)lua_tonumber(L, v3);
        if (v4 == 1)
        {
            auto v7 = lua_tostring(L, v3);
            return atoi(v7);
        }
        else
        {
            luaL_checktype(L, v3, 3);
            return 0;
        }
	}

	bool LuaContext::asBool(int)
	{
		throw std::logic_error("Not implemented");
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
		throw std::logic_error("Not implemented");
	}

	float LuaContext::asFloat(int)
	{
		throw std::logic_error("Not implemented");
	}

	AIParam& LuaContext::asAIParam(int i)
	{
        if (i < 0)
        {
            lua_pushstring(this->L, "not enough arguments");
            lua_error(this->L);
        }

        auto idx = i + m_stackStart;
        switch (lua_type(L, idx))
        {
        case 3:
        {
            auto result = ext_createAIParam(this->L);
            auto ia = lua_tonumber(this->L, idx);
            *result = (float)ia;
            //TODO: dangling pointer?
            return *result;
        }
        case 4:
        {
            auto result = ext_createAIParam(this->L);
            CStr str = lua_tostring(this->L, idx);
            *result = str;
            return *result;
        }
        case 7:
        {
            if (ext_checkTag(L, idx, tag_luaAIParam))
            {
                return *(m3d::AIParam*)lua_touserdata(L, idx);
            }
            if (ext_checkTag(L, idx, tag_luaVector))
            {
                auto* vec = (CVector*)lua_touserdata(L, idx);
                auto result = ext_createAIParam(this->L);
                *result = *vec;
                return *result;
            }
        }
        default:
        {
            M3D_LOG_ERR("Invalid argument type.");
            lua_pushstring(this->L, "Invalid argument type.");
            lua_error(this->L);
            break;
        }
        }
        // TODO: check this
        return *ext_createAIParam(this->L);
	}

	void LuaContext::pushBool(bool)
	{
		throw std::logic_error("Not implemented");
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

	void LuaContext::pushQuaternion(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	void LuaContext::pushInt(int x)
	{
		lua_pushnumber(L, x);
		++m_numOutputs;
	}

	void LuaContext::pushFloat(float)
	{
		throw std::logic_error("Not implemented");
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

	void LuaContext::pushString(char const*)
	{
		throw std::logic_error("Not implemented");
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

	void LuaContext::pushVector(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	int LuaContext::_validateArg(int)
	{
		throw std::logic_error("Not implemented");
	}
}

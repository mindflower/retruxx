#include <stdexcept>
#include <script/scriptcontext.h>

#include "core/kernel.h"
#include "core/log.h"
#include "script/scriptserver.h"

namespace m3d
{
	void LuaContext::pushObject(Object*)
	{
		throw std::logic_error("Not implemented");
	}

	void LuaContext::pushAIParam(AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	int LuaContext::asInt(int)
	{
		throw std::logic_error("Not implemented");
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

	AIParam& LuaContext::asAIParam(int)
	{
		throw std::logic_error("Not implemented");
	}

	void LuaContext::pushBool(bool)
	{
		throw std::logic_error("Not implemented");
	}

	char const* LuaContext::asString(int)
	{
		throw std::logic_error("Not implemented");
	}

	void LuaContext::pushQuaternion(Quaternion const&)
	{
		throw std::logic_error("Not implemented");
	}

	void LuaContext::pushInt(int)
	{
		throw std::logic_error("Not implemented");
	}

	void LuaContext::pushFloat(float)
	{
		throw std::logic_error("Not implemented");
	}

	Quaternion& LuaContext::asQuaternion(int)
	{
		throw std::logic_error("Not implemented");
	}

	void LuaContext::pushString(char const*)
	{
		throw std::logic_error("Not implemented");
	}

	CVector& LuaContext::asVector(int)
	{
		throw std::logic_error("Not implemented");
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

#include <stdexcept>
#include <script/luavector.h>

#include "script/scriptserver.h"

#include <assert.h>

extern "C"
{
#include "lauxlib.h"
}

int ext_vecConstructor(lua_State* L)
{
	// TODO: check this
	int result = 0;
	auto v2 = lua_gettop(L);
	auto Vector = ext_createVector(L);
	if (v2 == 1)
	{
		if (lua_type(L, 1) == 7)
		{
			if (ext_checkTag(L, 1, tag_luaVector))
			{
				auto v4 = (float*)lua_touserdata(L, 1);
				Vector->x = *v4;
				Vector->y = v4[1];
				Vector->z = v4[2];
			}
			else
			{
				lua_settop(L, -2);
			}
			return 1;
		}
	}
	else if (v2 < 1)
	{
		auto v6 = 0.0;
		Vector->x = v6;
		auto v7 = 0.0;
		if (v2 >= 2 && lua_type(L, 2))
			v7 = luaL_checknumber(L, 2);

		Vector->y = v7;
		if (v2 >= 3 && lua_type(L, 3))
		{
			Vector->z = luaL_checknumber(L, 3);
			return 1;
		}
		else
		{
			result = 1;
			Vector->z = 0.0;
		}
		return result;
	}
	if (lua_type(L, 1))
	{
		auto v7 = 0.0;
		auto v6 = luaL_checknumber(L, 1);
		Vector->x = v6;
		if (v2 >= 2 && lua_type(L, 2))
			v7 = luaL_checknumber(L, 2);
		else
			v7 = 0.0;
		Vector->y = v7;
		if (v2 >= 3 && lua_type(L, 3))
		{
			Vector->z = luaL_checknumber(L, 3);
			return 1;
		}
		else
		{
			result = 1;
			Vector->z = 0.0;
		}
		return result;
	}
	auto v6 = 0.0;
	auto v7 = 0.0;
	Vector->x = v6;
	if (v2 >= 2 && lua_type(L, 2))
		v7 = luaL_checknumber(L, 2);
	else
		v7 = 0.0;
	Vector->y = v7;
	if (v2 >= 3 && lua_type(L, 3))
	{
		Vector->z = luaL_checknumber(L, 3);
		return 1;
	}
	else
	{
		result = 1;
		Vector->z = 0.0;
	}
	return result;
}

void ext_initVector(lua_State* L)
{
	lua_pushcclosure(L, ext_vecConstructor, 0);
	lua_pushstring(L, "CVector");
	lua_insert(L, -2);
	lua_settable(L, -10001);
}

int ext_vecDestructor(lua_State* L)
{
	return 1;
}

int ext_vecNormalize(lua_State* L)
{
	assert(!ext_checkTag(L, 1, tag_luaVector));

	float* v2 = (float*)lua_touserdata(L, 1); // esi

	CVector* vec = ext_createVector(L); // eax

	float x = v2[0];
	float y = v2[1];
	float z = v2[2];

	float len = 1.0 / sqrt(x * x + y * y + z * z + 0.00000011920929); // [esp+8h] [ebp-10h]

	vec->x = x * len;
	vec->y = y * len;
	vec->z = z * len;

	return 1;
}

int ext_vecLength(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaVector));

	float* v2 = (float*)lua_touserdata(L, 1);

	lua_pushnumber(L, sqrt(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]));

	return 1;
}

int ext_vecLengthSq(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaVector));

	float* v2 = (float*)lua_touserdata(L, 1);

	lua_pushnumber(L, (v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]));

	return 1;
}

int ext_vecZero(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaVector));

	float* v2 = (float*)lua_touserdata(L, 1);

	v2[0] = 0.0f;
	v2[1] = 0.0f;
	v2[2] = 0.0f;

	return 1;
}

int ext_vecOne(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaVector));

	float* v2 = (float*)lua_touserdata(L, 1);

	v2[0] = 1.0f;
	v2[1] = 1.0f;
	v2[2] = 1.0f;

	return 1;
}

int ext_vecDotProduct(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaVector) && ext_checkTag(L, 2, tag_luaVector));

	float* v1 = (float*)lua_touserdata(L, 1);
	float* v2 = (float*)lua_touserdata(L, 2);

	lua_pushnumber(L, v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
	return 1;

}

int ext_vecCrossProduct(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaVector) && ext_checkTag(L, 2, tag_luaVector));

	float* v2 = (float*)lua_touserdata(L, 1);
	float* v3 = (float*)lua_touserdata(L, 2);
	CVector* vec = ext_createVector(L);

	vec->x = (float)(v2[1] * v3[2]) - (float)(v2[2] * v3[1]);
	vec->y = (float)(v2[2] * v3[0]) - (float)(v3[2] * v2[0]);
	vec->z = v3[1] * v2[0] - v2[1] * v3[0];

	return 1;
}

int ext_vecGet(lua_State* L)
{
	// TODO: generated code
	// Get the CVector userdata from Lua stack
	CVector* vec = static_cast<CVector*>(lua_touserdata(L, 1));

	// Get the property name being accessed
	const char* propertyName = luaL_checklstring(L, 2, nullptr);

	// Handle special internal tag
	if (strcmp(propertyName, "internalTag") == 0)
	{
		lua_pushnumber(L, 1001.0);
		return 1;
	}

	// Handle single-character properties (x, y, z)
	if (strlen(propertyName) == 1)
	{
		switch (propertyName[0]) {
		case 'x':
			lua_pushnumber(L, vec->x);
			return 1;
		case 'y':
			lua_pushnumber(L, vec->y);
			return 1;
		case 'z':
			lua_pushnumber(L, vec->z);
			return 1;
		}
	}

	// Handle method calls (return closure functions)
	if (strcmp(propertyName, "normalize") == 0)
	{
		lua_pushcclosure(L, ext_vecNormalize, 0);
		return 1;
	}
	else if (strcmp(propertyName, "length") == 0)
	{
		lua_pushcclosure(L, ext_vecLength, 0);
		return 1;
	}
	else if (strcmp(propertyName, "lengthSq") == 0)
	{
		lua_pushcclosure(L, ext_vecLengthSq, 0);
		return 1;
	}
	else if (strcmp(propertyName, "zero") == 0)
	{
		lua_pushcclosure(L, ext_vecZero, 0);
		return 1;
	}
	else if (strcmp(propertyName, "one") == 0)
	{
		lua_pushcclosure(L, ext_vecOne, 0);
		return 1;
	}
	else if (strcmp(propertyName, "dot") == 0)
	{
		lua_pushcclosure(L, ext_vecDotProduct, 0);
		return 1;
	}
	else if (strcmp(propertyName, "cross") == 0)
	{
		lua_pushcclosure(L, ext_vecCrossProduct, 0);
		return 1;
	}

	lua_pushnumber(L, 0.0);
	return 1;
}

int ext_vecSet(lua_State* L)
{
	float* v2 = (float*)lua_touserdata(L, 1);
	const char* v3 = luaL_checklstring(L, 2, 0);
	float a = luaL_checknumber(L, 3);

	if (v3[1])
	{
		lua_pushnumber(L, 0.0);
		return 1;
	}

	switch (*v3)
	{
	case 'w':
		v2[3] = a;
		break;
	case 'x':
		v2[0] = a;
		break;
	case 'y':
		v2[1] = a;
		break;
	case 'z':
		v2[2] = a;
		break;
	default:
		return 1;
	}

	return 1;
}

int ext_vecAdd(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaVector) && ext_checkTag(L, 2, tag_luaVector));

	float* v1 = (float*)lua_touserdata(L, 1);
	float* v2 = (float*)lua_touserdata(L, 2);
	CVector* vec = ext_createVector(L);

	vec->x = v1[0] + v2[0];
	vec->y = v1[1] + v2[1];
	vec->z = v1[2] + v2[2];

	return 1;
}

int ext_vecSub(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaVector) && ext_checkTag(L, 2, tag_luaVector));

	float* v1 = (float*)lua_touserdata(L, 1);
	float* v2 = (float*)lua_touserdata(L, 2);
	CVector* vec = ext_createVector(L);

	vec->x = v1[0] - v2[0];
	vec->y = v1[1] - v2[1];
	vec->z = v1[2] - v2[2];

	return 1;
}

int ext_vecNeg(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaVector));

	float* v = (float*)lua_touserdata(L, 1);
	CVector* vec = ext_createVector(L);
	vec->x = -v[0];
	vec->y = -v[1];
	vec->z = -v[2];

	return 1;
}

int ext_vecMul(lua_State* L)
{
	int v2 = lua_type(L, 1);
	int v3 = lua_type(L, 2);

	char v9[12]; // vector;

	ext_InternalTags Tag = ext_getTag(L, 1);

	ext_getTag(L, 2);

	if (v2 != 3 && v3 != 3)
	{
		lua_pushstring(L, "CVector: bad multiplier");
		lua_error(L);
	}

	float* v5 = (float*)lua_touserdata(L, (Tag != tag_luaVector) + 1);

	float a = lua_tonumber(L, (unsigned int)(v2 != 3) + 1);

	CVector* Vector = ext_createVector(L);

	*(float*)v9 = a * v5[0];

	*(float*)&v9[4] = a * v5[1];

	*Vector = *(CVector*)v9;
	return 1;

}

int ext_vecCall(lua_State* L)
{
	lua_pushnumber(L, 0.0);
	return 1;
}

CVector* ext_createVector(lua_State* L)
{
	auto buff = lua_newuserdata(L, sizeof(CVector));
	lua_newtable(L);
	lua_pushstring(L, "__gc");
	lua_pushcclosure(L, ext_vecDestructor, 0);
	lua_pushstring(L, "__index");
	lua_pushcclosure(L, ext_vecGet, 0);
	lua_pushstring(L, "__newindex");
	lua_pushcclosure(L, ext_vecSet, 0);
	lua_pushstring(L, "__add");
	lua_pushcclosure(L, ext_vecAdd, 0);
	lua_pushstring(L, "__sub");
	lua_pushcclosure(L, ext_vecSub, 0);
	lua_pushstring(L, "__unm");
	lua_pushcclosure(L, ext_vecNeg, 0);
	lua_pushstring(L, "__mul");
	lua_pushcclosure(L, ext_vecMul, 0);
	lua_pushstring(L, "__call");
	lua_pushcclosure(L, ext_vecCall, 0);
	lua_settable(L, -17);
	lua_settable(L, -15);
	lua_settable(L, -13);
	lua_settable(L, -11);
	lua_settable(L, -9);
	lua_settable(L, -7);
	lua_settable(L, -5);
	lua_settable(L, -3);
	lua_setmetatable(L, -2);

	auto* vec = new (buff) CVector();
	return vec;
}

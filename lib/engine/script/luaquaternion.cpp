#include <stdexcept>
#include <script/luaquaternion.h>

#include "script/scriptserver.h"

#include <cassert>

extern "C"
{
#include "lauxlib.h"
}

int ext_quatConstructor(lua_State* L)
{
	// TODO: check this
	int result = 0;
	auto v2 = lua_gettop(L);
	auto quaternion = ext_createQuaternion(L);
	if (v2 == 1)
	{
		if (lua_type(L, 1) == 7)
		{
			if (ext_checkTag(L, 1, tag_luaQuaternion))
			{
				auto v4 = (float*)lua_touserdata(L, 1);
				quaternion->x = *v4;
				quaternion->y = v4[1];
				quaternion->z = v4[2];
				quaternion->w = v4[3];
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
		auto v7 = 0.0;
		auto v8 = 0.0;
		quaternion->x = v6;
		if (v2 >= 2 && lua_type(L, 2))
			v7 = luaL_checknumber(L, 2);
		else
			v7 = 0.0;
		quaternion->y = v7;
		if (v2 >= 3 && lua_type(L, 3))
			v8 = luaL_checknumber(L, 3);
		else
			v8 = 0.0;
		quaternion->z = v8;
		if (v2 >= 4 && lua_type(L, 4))
		{
			quaternion->w = luaL_checknumber(L, 4);
			return 1;
		}
		else
		{
			result = 1;
			quaternion->w = 0.0;
		}
		return result;
	}
	if (lua_type(L, 1))
	{
		auto v6 = luaL_checknumber(L, 1);
		auto v7 = 0.0;
		auto v8 = 0.0;
		quaternion->x = v6;
		if (v2 >= 2 && lua_type(L, 2))
			v7 = luaL_checknumber(L, 2);
		else
			v7 = 0.0;
		quaternion->y = v7;
		if (v2 >= 3 && lua_type(L, 3))
			v8 = luaL_checknumber(L, 3);
		else
			v8 = 0.0;
		quaternion->z = v8;
		if (v2 >= 4 && lua_type(L, 4))
		{
			quaternion->w = luaL_checknumber(L, 4);
			return 1;
		}
		else
		{
			result = 1;
			quaternion->w = 0.0;
		}
		return result;
	}
}

void ext_initQuaternion(lua_State* L)
{
	lua_pushcclosure(L, ext_quatConstructor, 0);
	lua_pushstring(L, "Quaternion");
	lua_insert(L, -2);
	lua_settable(L, -10001);
}

int ext_quatIdentity(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaQuaternion));

	float* quat = static_cast<float*>(lua_touserdata(L, 1));

	quat[0] = 0;
	quat[1] = 0;
	quat[2] = 0;
	quat[3] = 1.0f;

	return 1;
}

int ext_quatZero(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaQuaternion));

	float* quat = static_cast<float*>(lua_touserdata(L, 1));

	quat[0] = 0.0f;
	quat[1] = 0.0f;
	quat[2] = 0.0f;
	quat[3] = 0.0f;

	return 1;
}

int ext_quatNorm(lua_State* L)
{
	assert(ext_checkTag(L, 1, tag_luaQuaternion));

	float* quat = static_cast<float*>(lua_touserdata(L, 1));
    lua_pushnumber(L, sqrt(quat[0] * quat[0] + quat[1] * quat[1] + quat[2] * quat[2] + quat[3] * quat[3]));
	return 1;
}

int ext_quatNormalize(lua_State* L)
{
    float* quat = 0;         // eax
	long double invLen = 0.0; // st7
	float quatLen = 0.0f; // [esp+4h] [ebp-4h]

	assert(ext_checkTag(L, 1, tag_luaQuaternion));

	quat = static_cast<float*>(lua_touserdata(L, 1));
    quatLen = (quat[0] * quat[0]) + (quat[1] * quat[1]) + (quat[2] * quat[2]) + (quat[3] * quat[3]);

	if (quatLen <= 0.0)
	{
        quat[0] = 0.0f;
		quat[1] = 0.0f;
		quat[2] = 0.0f;
		quat[3] = 1.0f;
	}
	else
	{
        invLen = 1.0 / sqrt(quatLen);
		quat[0] = invLen * quat[0];
		quat[1] = invLen * quat[1];
		quat[2] = invLen * quat[2];
		quat[3] = invLen * quat[3];
	}
	return 1;
}

int ext_quatRotX(lua_State* L)
{
	ext_InternalTags Tag; // edi
	int v3; // eax
	float* v4; // edi
	long double v5; // st7
	int result; // eax

	assert(ext_getTag(L, 1) == tag_luaQuaternion && lua_type(L, 2) == LUA_TNUMBER);

	v4 = (float*)lua_touserdata(L, 1);
	v5 = lua_tonumber(L, 2) * 0.5;
	v4[1] = 0.0f;
	v4[2] = 0.0f;
	result = 1;
	*v4 = sin(v5);
	v4[3] = cos(v5);
	return result;
}

int ext_quatRotY(lua_State* L)
{
	ext_InternalTags Tag; // edi
	int v3; // eax
	float* v4; // edi
	long double v5; // st7
	int result; // eax

	assert(ext_getTag(L, 1) == tag_luaQuaternion && lua_type(L, 2) == LUA_TNUMBER);

	v4 = static_cast<float*>(lua_touserdata(L, 1));
	v5 = lua_tonumber(L, 2) * 0.5;
	*v4 = 0.0f;
	v4[2] = 0.0f;
	result = 1;
	v4[1] = sin(v5);
	v4[3] = cos(v5);
	return result;
}

int ext_quatRotZ(lua_State* L)
{
	ext_InternalTags Tag; // edi
	int v3; // eax
	float* v4; // edi
	long double v5; // st7
	int result; // eax

	assert(ext_getTag(L, 1) == tag_luaQuaternion && lua_type(L, 2) == LUA_TNUMBER);

	v4 = static_cast<float*>(lua_touserdata(L, 1));
	v5 = lua_tonumber(L, 2) * 0.5;
	*v4 = 0.0f;
	v4[1] = 0.0f;
	result = 1;
	v4[2] = sin(v5);
	v4[3] = cos(v5);
	return result;
}

int ext_quatFromAxisAngle(lua_State* L)
{
	float* v5; // edi
	float* v6; // ebx
	double v7; // st7
	int result; // eax
	double v9; // st6

	assert(ext_getTag(L, 1) == tag_luaQuaternion && ext_getTag(L, 2) == tag_luaVector && lua_type(L, 3) == LUA_TNUMBER);

	v5 = static_cast<float*>(lua_touserdata(L, 1));
    v6 = static_cast<float*>(lua_touserdata(L, 1));
	v7 = lua_tonumber(L, 3) * 0.5;
	result = 1;
	v9 = sinf(v7);
	v5[0] = v9 * v6[0];
	v5[1] = v9 * v6[1];
	v5[2] = v9 * v6[2];
	v5[3] = cosf(v7);
	return result;
}

int ext_quatGetConjugated(lua_State* L)
{
	float* v2; // esi
	Quaternion* q; // eax
	float v4; // xmm1_4
	float v5; // xmm0_4
	float v7; // [esp+10h] [ebp-8h]

	assert(ext_checkTag(L, 1, tag_luaQuaternion));

	v2 = static_cast<float*>(lua_touserdata(L, 1));
	q = ext_createQuaternion(L);
	v4 = 0.0 - v2[1];
	v7 = 0.0 - v2[2];
	v5 = v2[3];
	q->x = 0.0 - *v2;
	q->y = v4;
	q->z = v7;
	q->w = v5;
	return 1;
}

int ext_quatGetInversed(lua_State* L)
{
	Quaternion* srcQuat; // edi
	Quaternion* resQuat; // esi

	assert(ext_checkTag(L, 1, tag_luaQuaternion));

	srcQuat = static_cast<Quaternion*>(lua_touserdata(L, 1));
    resQuat = ext_createQuaternion(L);
    *resQuat = srcQuat->getInversed();

	return 1;
}

int ext_quatFromYPR(lua_State* L)
{
	Quaternion* quat; // edi
	float roll; // [esp+8h] [ebp-Ch]
	float pitch; // [esp+Ch] [ebp-8h]
	float yaw; // [esp+10h] [ebp-4h]

	assert(ext_checkTag(L, 1, tag_luaQuaternion));
	assert(lua_type(L, 2) != 3 && lua_type(L, 3) != 3 && lua_type(L, 4) != 3);

	quat = static_cast<Quaternion*>(lua_touserdata(L, 1));
	yaw = lua_tonumber(L, 2);
	pitch = lua_tonumber(L, 3);
	roll = lua_tonumber(L, 4);

	quat->fromYPR(yaw, pitch, roll);

	return 1;
}

int ext_quatGet(lua_State* L)
{
	// TODO: generated code
	Quaternion* quat = static_cast<Quaternion*>(lua_touserdata(L, 1));

	// Get the property name being accessed
	const char* propertyName = luaL_checklstring(L, 2, nullptr);

	// Handle special internal tag
	if (strcmp(propertyName, "internalTag") == 0)
	{
		lua_pushnumber(L, 1003.0);
		return 1;
	}

	// Handle single-character properties (x, y, z, w)
	if (strlen(propertyName) == 1)
	{
		switch (propertyName[0]) {
		case 'x':
			lua_pushnumber(L, quat->x);
			return 1;
		case 'y':
			lua_pushnumber(L, quat->y);
			return 1;
		case 'z':
			lua_pushnumber(L, quat->z);
			return 1;
		case 'w':
			lua_pushnumber(L, quat->w);
			return 1;
		}
	}

	// Handle method calls (return closure functions)
	if (strcmp(propertyName, "Identity") == 0)
	{
		lua_pushcclosure(L, ext_quatIdentity, 0);
		return 1;
	}
	else if (strcmp(propertyName, "Zero") == 0)
	{
		lua_pushcclosure(L, ext_quatZero, 0);
		return 1;
	}
	else if (strcmp(propertyName, "Norm") == 0)
	{
		lua_pushcclosure(L, ext_quatNorm, 0);
		return 1;
	}
	else if (strcmp(propertyName, "Normalize") == 0)
	{
		lua_pushcclosure(L, ext_quatNormalize, 0);
		return 1;
	}
	else if (strcmp(propertyName, "RotX") == 0)
	{
		lua_pushcclosure(L, ext_quatRotX, 0);
		return 1;
	}
	else if (strcmp(propertyName, "RotY") == 0)
	{
		lua_pushcclosure(L, ext_quatRotY, 0);
		return 1;
	}
	else if (strcmp(propertyName, "RotZ") == 0)
	{
		lua_pushcclosure(L, ext_quatRotZ, 0);
		return 1;
	}
	else if (strcmp(propertyName, "FromAxisAngle") == 0)
	{
		lua_pushcclosure(L, ext_quatFromAxisAngle, 0);
		return 1;
	}
	else if (strcmp(propertyName, "getConjugated") == 0)
	{
		lua_pushcclosure(L, ext_quatGetConjugated, 0);
		return 1;
	}
	else if (strcmp(propertyName, "getInversed") == 0)
	{
		lua_pushcclosure(L, ext_quatGetInversed, 0);
		return 1;
	}
	else if (strcmp(propertyName, "fromYPR") == 0)
	{
		lua_pushcclosure(L, ext_quatFromYPR, 0);
		return 1;
	}

	lua_pushnumber(L, 0.0);
	return 1;
}

int ext_quatSet(lua_State* L)
{
    float* q = static_cast<float*>(lua_touserdata(L, 1));
	const char* field = luaL_checklstring(L, 2, 0);
	float val = luaL_checknumber(L, 3);

	if (field[1])
	{
		lua_pushnumber(L, 0.0);
		return 1;
	}

	switch (*field)
	{
	case 'w':
		q[3] = val;
		break;
	case 'x':
		q[0] = val;
		break;
	case 'y':
		q[1] = val;
		break;
	case 'z':
		q[2] = val;
		break;
	default:
		return 1;
	}

	return 1;
}

int ext_quatMul(lua_State* L)
{
    int argType1, argType2;
    ext_InternalTags tag1, tag2;
    Quaternion* q;
    float x, y, z, w;

	argType1 = lua_type(L, 1);
    argType2 = lua_type(L, 2);
    tag1 = ext_getTag(L, 1);
    tag2 = ext_getTag(L, 2);

    if (argType1 == LUA_TNUMBER || argType2 == LUA_TNUMBER)
	{
        float* srcQuat = static_cast<float*>(lua_touserdata(L, (tag1 != tag_luaQuaternion) + 1));
        float mulCoeff = lua_tonumber(L, (unsigned int)(argType1 != LUA_TNUMBER) + 1);
		q = ext_createQuaternion(L);
        x = srcQuat[0] * mulCoeff;
        y = srcQuat[1] * mulCoeff;
        z = srcQuat[2] * mulCoeff;
        w = srcQuat[3] * mulCoeff;
	}
	else
	{
        if (tag1 != tag_luaQuaternion || tag2 != tag_luaQuaternion)
		{
			lua_pushstring(L, "Quaternion: bad multiplier");
			lua_error(L);
		}

        float* qa = static_cast<float*>(lua_touserdata(L, 1));
        float* qb = static_cast<float*>(lua_touserdata(L, 2));
		q = ext_createQuaternion(L);
		x = qa[1] * qb[2] + qa[3] * qb[0] + qb[3] * qa[0] - qb[1] * qa[2];
		y = qa[1] * qb[3] + qb[0] * qa[2] + qa[3] * qb[1] - qa[0] * qb[2];
		z = qb[1] * qa[0] + qa[3] * qb[2] + qb[3] * qa[2] - qa[1] * qb[0];
		w = qa[3] * qb[3] - qb[0] * qa[0] - qa[1] * qb[1] - qb[2] * qa[2];
	}

	q->x = x;
	q->y = y;
	q->z = z;
	q->w = w;

	return 1;
}

int ext_quatCall(lua_State* L)
{
	lua_pushnumber(L, 0.0);
	return 1;
}

Quaternion* ext_createQuaternion(lua_State* L)
{
	void* buff = lua_newuserdata(L, sizeof(Quaternion));
	lua_newtable(L);
	lua_pushstring(L, "__index");
	lua_pushcclosure(L, ext_quatGet, 0);
	lua_pushstring(L, "__newindex");
	lua_pushcclosure(L, ext_quatSet, 0);
	lua_pushstring(L, "__mul");
	lua_pushcclosure(L, ext_quatMul, 0);
	lua_pushstring(L, "__call");
	lua_pushcclosure(L, ext_quatCall, 0);
	lua_settable(L, -9);
	lua_settable(L, -7);
	lua_settable(L, -5);
	lua_settable(L, -3);
	lua_setmetatable(L, -2);

	Quaternion* res = new (buff) Quaternion;
	return res;
}

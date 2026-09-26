#include <stdexcept>
#include <cassert>
#include <cmath>
#include <math/quaternion.h>
#include <math/vector.h>
#include <script/luaquaternion.h>

#include "script/scriptserver.h"

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
    // RVA 0x898290
    assert(ext_checkTag(L, 1, tag_luaQuaternion));
    auto* const q = static_cast<Quaternion*>(lua_touserdata(L, 1));
    q->x = 0.0f;
    q->y = 0.0f;
    q->z = 0.0f;
    q->w = 1.0f;
    return 1;
}

int ext_quatZero(lua_State* L)
{
    // RVA 0x8982F0
    assert(ext_checkTag(L, 1, tag_luaQuaternion));
    auto* const q = static_cast<Quaternion*>(lua_touserdata(L, 1));
    q->x = 0.0f;
    q->y = 0.0f;
    q->z = 0.0f;
    q->w = 0.0f;
    return 1;
}

int ext_quatNorm(lua_State* L)
{
    // RVA 0x898350
    assert(ext_checkTag(L, 1, tag_luaQuaternion));
    auto const* const q = static_cast<Quaternion const*>(lua_touserdata(L, 1));
    lua_pushnumber(L, sqrt(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w));
    return 1;
}

int ext_quatNormalize(lua_State* L)
{
    // RVA 0x8983D0 - a zero quaternion becomes the identity.
    assert(ext_checkTag(L, 1, tag_luaQuaternion));
    auto* const q = static_cast<Quaternion*>(lua_touserdata(L, 1));
    float const norm2 = q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w;
    if (norm2 <= 0.0f)
    {
        q->x = 0.0f;
        q->y = 0.0f;
        q->z = 0.0f;
        q->w = 1.0f;
        return 1;
    }
    float const inv = static_cast<float>(1.0 / sqrt(norm2));
    q->x = inv * q->x;
    q->y = inv * q->y;
    q->z = inv * q->z;
    q->w = inv * q->w;
    return 1;
}

int ext_quatRotX(lua_State* L)
{
    // RVA 0x8984B0
    assert(ext_getTag(L, 1) == tag_luaQuaternion && lua_type(L, 2) == LUA_TNUMBER);
    auto* const q = static_cast<Quaternion*>(lua_touserdata(L, 1));
    double const half = lua_tonumber(L, 2) * 0.5;
    float const s = static_cast<float>(sin(half));
    q->x = s;
    q->y = 0.0f;
    q->z = 0.0f;
    q->w = static_cast<float>(cos(half));
    return 1;
}

int ext_quatRotY(lua_State* L)
{
    // RVA 0x898540
    assert(ext_getTag(L, 1) == tag_luaQuaternion && lua_type(L, 2) == LUA_TNUMBER);
    auto* const q = static_cast<Quaternion*>(lua_touserdata(L, 1));
    double const half = lua_tonumber(L, 2) * 0.5;
    float const s = static_cast<float>(sin(half));
    q->x = 0.0f;
    q->y = s;
    q->z = 0.0f;
    q->w = static_cast<float>(cos(half));
    return 1;
}

int ext_quatRotZ(lua_State* L)
{
    // RVA 0x8985D0
    assert(ext_getTag(L, 1) == tag_luaQuaternion && lua_type(L, 2) == LUA_TNUMBER);
    auto* const q = static_cast<Quaternion*>(lua_touserdata(L, 1));
    double const half = lua_tonumber(L, 2) * 0.5;
    float const s = static_cast<float>(sin(half));
    q->x = 0.0f;
    q->y = 0.0f;
    q->z = s;
    q->w = static_cast<float>(cos(half));
    return 1;
}

int ext_quatFromAxisAngle(lua_State* L)
{
    // RVA 0x898660 - the axis is used as given, not normalized.
    assert(ext_getTag(L, 1) == tag_luaQuaternion && ext_getTag(L, 2) == tag_luaVector && lua_type(L, 3) == LUA_TNUMBER);
    auto* const q = static_cast<Quaternion*>(lua_touserdata(L, 1));
    auto const* const axis = static_cast<CVector const*>(lua_touserdata(L, 2));
    double const half = lua_tonumber(L, 3) * 0.5;
    float const s = static_cast<float>(sin(half));
    q->x = s * axis->x;
    q->y = s * axis->y;
    q->z = s * axis->z;
    q->w = static_cast<float>(cos(half));
    return 1;
}

int ext_quatGetConjugated(lua_State* L)
{
    // RVA 0x898710
    assert(ext_checkTag(L, 1, tag_luaQuaternion));
    auto const* const q = static_cast<Quaternion const*>(lua_touserdata(L, 1));
    Quaternion* const res = ext_createQuaternion(L);
    res->x = 0.0f - q->x;
    res->y = 0.0f - q->y;
    res->z = 0.0f - q->z;
    res->w = q->w;
    return 1;
}

int ext_quatGetInversed(lua_State* L)
{
    // RVA 0x898C90
    assert(ext_checkTag(L, 1, tag_luaQuaternion));
    auto const* const q = static_cast<Quaternion const*>(lua_touserdata(L, 1));
    Quaternion* const res = ext_createQuaternion(L);
    *res = q->getInversed();
    return 1;
}

int ext_quatFromYPR(lua_State* L)
{
    // RVA 0x897EF0
    assert(ext_checkTag(L, 1, tag_luaQuaternion));
    assert(lua_type(L, 2) == LUA_TNUMBER && lua_type(L, 3) == LUA_TNUMBER && lua_type(L, 4) == LUA_TNUMBER);
    auto* const q = static_cast<Quaternion*>(lua_touserdata(L, 1));
    float const yaw = static_cast<float>(lua_tonumber(L, 2));
    float const pitch = static_cast<float>(lua_tonumber(L, 3));
    float const roll = static_cast<float>(lua_tonumber(L, 4));
    q->fromYPR(yaw, pitch, roll);
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
    // RVA 0x897E00 - only x, y, z and w can be assigned; anything else pushes a 0.
    auto* const q = static_cast<Quaternion*>(lua_touserdata(L, 1));
    char const* const name = luaL_checklstring(L, 2, nullptr);
    float const value = static_cast<float>(luaL_checknumber(L, 3));
    if (name[1])
    {
        lua_pushnumber(L, 0.0);
        return 1;
    }
    switch (name[0])
    {
    case 'w':
        q->w = value;
        break;
    case 'x':
        q->x = value;
        break;
    case 'y':
        q->y = value;
        break;
    case 'z':
        q->z = value;
        break;
    default:
        break;
    }
    return 1;
}

int ext_quatMul(lua_State* L)
{
    // RVA 0x898060 - quaternion times number (either way round) or quaternion times quaternion.
    int const type1 = lua_type(L, 1);
    int const type2 = lua_type(L, 2);
    ext_InternalTags const tag1 = ext_getTag(L, 1);
    ext_InternalTags const tag2 = ext_getTag(L, 2);
    Quaternion* res;
    float x;
    float y;
    float z;
    float w;
    if (type1 == LUA_TNUMBER || type2 == LUA_TNUMBER)
    {
        auto const* const q = static_cast<float const*>(lua_touserdata(L, (tag1 != tag_luaQuaternion) + 1));
        float const a = static_cast<float>(lua_tonumber(L, (type1 != LUA_TNUMBER) + 1));
        res = ext_createQuaternion(L);
        x = q[0] * a;
        y = q[1] * a;
        z = q[2] * a;
        w = q[3] * a;
    }
    else
    {
        if (tag1 != tag_luaQuaternion || tag2 != tag_luaQuaternion)
        {
            lua_pushstring(L, "Quaternion: bad multiplier");
            lua_error(L);
        }
        auto const* const a = static_cast<float const*>(lua_touserdata(L, 1));
        auto const* const b = static_cast<float const*>(lua_touserdata(L, 2));
        res = ext_createQuaternion(L);
        x = a[1] * b[2] + a[3] * b[0] + b[3] * a[0] - b[1] * a[2];
        y = a[1] * b[3] + b[0] * a[2] + a[3] * b[1] - a[0] * b[2];
        z = b[1] * a[0] + a[3] * b[2] + b[3] * a[2] - a[1] * b[0];
        w = a[3] * b[3] - b[0] * a[0] - a[1] * b[1] - b[2] * a[2];
    }
    res->x = x;
    res->y = y;
    res->z = z;
    res->w = w;
    return 1;
}

int ext_quatCall(lua_State* L)
{
    // RVA 0x897ED0
    lua_pushnumber(L, 0.0);
    return 1;
}

Quaternion* ext_createQuaternion(lua_State* L)
{
    auto buff = lua_newuserdata(L, sizeof(Quaternion));
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

    auto* res = new (buff) Quaternion;
    return res;
}

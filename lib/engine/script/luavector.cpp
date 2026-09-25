#include <cassert>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <script/luavector.h>

#include "script/scriptserver.h"

extern "C"
{
#include "lauxlib.h"
}

int ext_vecConstructor(lua_State* L)
{
    // RVA 0x899790 - CVector(), CVector(x, y, z) with missing or nil components as 0, or
    // CVector(v) to copy another vector.
    int const top = lua_gettop(L);
    CVector* vec = ext_createVector(L);
    if (top == 1 && lua_type(L, 1) == LUA_TUSERDATA)
    {
        if (ext_checkTag(L, 1, tag_luaVector))
        {
            CVector const* src = static_cast<CVector const*>(lua_touserdata(L, 1));
            vec->x = src->x;
            vec->y = src->y;
            vec->z = src->z;
        }
        else
        {
            // NOTE: any other userdata pops the new vector, so the argument itself is returned.
            lua_settop(L, -2);
        }
        return 1;
    }
    vec->x = (top >= 1 && lua_type(L, 1) != LUA_TNIL) ? static_cast<float>(luaL_checknumber(L, 1)) : 0.0f;
    vec->y = (top >= 2 && lua_type(L, 2) != LUA_TNIL) ? static_cast<float>(luaL_checknumber(L, 2)) : 0.0f;
    vec->z = (top >= 3 && lua_type(L, 3) != LUA_TNIL) ? static_cast<float>(luaL_checknumber(L, 3)) : 0.0f;
    return 1;
}

void ext_initVector(lua_State* L)
{
    // RVA 0x899980
    lua_pushcclosure(L, ext_vecConstructor, 0);
    lua_pushstring(L, "CVector");
    lua_insert(L, -2);
    lua_settable(L, -10001);
}

int ext_vecDestructor(lua_State* L)
{
    // RVA 0x898E20 - the userdata owns nothing.
    return 1;
}

int ext_vecNormalize(lua_State* L)
{
    // RVA 0x899040 - returns a new, normalized vector.
    assert(ext_checkTag(L, 1, tag_luaVector));
    CVector const* v = static_cast<CVector const*>(lua_touserdata(L, 1));
    CVector* result = ext_createVector(L);
    // NOTE: FLT_EPSILON is added under the root, so a zero vector comes out as zero and no
    // vector comes out at exactly unit length.
    float const inv = static_cast<float>(
        1.0 / sqrt(double(v->x) * v->x + double(v->y) * v->y + double(v->z) * v->z + 0.00000011920929));
    result->x = v->x * inv;
    result->y = v->y * inv;
    result->z = v->z * inv;
    return 1;
}

int ext_vecLength(lua_State* L)
{
    // RVA 0x899110
    assert(ext_checkTag(L, 1, tag_luaVector));
    CVector const* v = static_cast<CVector const*>(lua_touserdata(L, 1));
    lua_pushnumber(L, sqrt(double(v->x) * v->x + double(v->y) * v->y + double(v->z) * v->z));
    return 1;
}

int ext_vecLengthSq(lua_State* L)
{
    // RVA 0x899180
    assert(ext_checkTag(L, 1, tag_luaVector));
    CVector const* v = static_cast<CVector const*>(lua_touserdata(L, 1));
    lua_pushnumber(L, double(v->x) * v->x + double(v->y) * v->y + double(v->z) * v->z);
    return 1;
}

int ext_vecZero(lua_State* L)
{
    // RVA 0x8991F0 - zeroes the vector in place; nothing is pushed,
    // so the call returns the top of the stack.
    assert(ext_checkTag(L, 1, tag_luaVector));
    CVector* v = static_cast<CVector*>(lua_touserdata(L, 1));
    v->z = 0.0f;
    v->y = 0.0f;
    v->x = 0.0f;
    return 1;
}

int ext_vecOne(lua_State* L)
{
    // RVA 0x899250 - sets every component to 1 in place; nothing is pushed.
    assert(ext_checkTag(L, 1, tag_luaVector));
    CVector* v = static_cast<CVector*>(lua_touserdata(L, 1));
    v->z = 1.0f;
    v->y = 1.0f;
    v->x = 1.0f;
    return 1;
}

int ext_vecDotProduct(lua_State* L)
{
    // RVA 0x899390
    assert(ext_checkTag(L, 1, tag_luaVector) && ext_checkTag(L, 2, tag_luaVector));
    CVector const* a = static_cast<CVector const*>(lua_touserdata(L, 1));
    CVector const* b = static_cast<CVector const*>(lua_touserdata(L, 2));
    lua_pushnumber(L, double(b->z) * a->z + double(b->y) * a->y + double(b->x) * a->x);
    return 1;
}

int ext_vecCrossProduct(lua_State* L)
{
    // RVA 0x899890
    assert(ext_checkTag(L, 1, tag_luaVector) && ext_checkTag(L, 2, tag_luaVector));
    CVector const* a = static_cast<CVector const*>(lua_touserdata(L, 1));
    CVector const* b = static_cast<CVector const*>(lua_touserdata(L, 2));
    CVector* result = ext_createVector(L);
    result->x = a->y * b->z - a->z * b->y;
    result->y = a->z * b->x - b->z * a->x;
    result->z = b->y * a->x - a->y * b->x;
    return 1;
}

int ext_vecGet(lua_State* L)
{
    // RVA 0x899420 - __index: the components, the internal tag, and the methods.
    CVector const* vec = static_cast<CVector const*>(lua_touserdata(L, 1));
    char const* name = luaL_checklstring(L, 2, nullptr);
    double n;
    if (!strcmp(name, "internalTag"))
    {
        n = 1001.0;
    }
    // NOTE: a one-character name is detected by name[1] alone, which reads past the end of an
    // empty string.
    else if (!name[1] && name[0] == 'x')
    {
        n = vec->x;
    }
    else if (!name[1] && name[0] == 'y')
    {
        n = vec->y;
    }
    else if (!name[1] && name[0] == 'z')
    {
        n = vec->z;
    }
    else
    {
        if (!strcmp(name, "normalize"))
        {
            lua_pushcclosure(L, ext_vecNormalize, 0);
            return 1;
        }
        if (!strcmp(name, "length"))
        {
            lua_pushcclosure(L, ext_vecLength, 0);
            return 1;
        }
        if (!strcmp(name, "lengthSq"))
        {
            lua_pushcclosure(L, ext_vecLengthSq, 0);
            return 1;
        }
        if (!strcmp(name, "zero"))
        {
            lua_pushcclosure(L, ext_vecZero, 0);
            return 1;
        }
        if (!strcmp(name, "one"))
        {
            lua_pushcclosure(L, ext_vecOne, 0);
            return 1;
        }
        if (!strcmp(name, "dot"))
        {
            lua_pushcclosure(L, ext_vecDotProduct, 0);
            return 1;
        }
        if (!strcmp(name, "cross"))
        {
            lua_pushcclosure(L, ext_vecCrossProduct, 0);
            return 1;
        }
        n = 0.0;
    }
    lua_pushnumber(L, n);
    return 1;
}

int ext_vecSet(lua_State* L)
{
    // RVA 0x898D70 - __newindex: only x, y and z can be set.
    CVector* vec = static_cast<CVector*>(lua_touserdata(L, 1));
    char const* name = luaL_checklstring(L, 2, nullptr);
    float const value = static_cast<float>(luaL_checknumber(L, 3));
    // NOTE: as in ext_vecGet, name[1] is read even for an empty name. A longer name pushes 0,
    // while a successful set (or an unknown one-character name) pushes nothing.
    if (name[1])
    {
        lua_pushnumber(L, 0.0);
        return 1;
    }
    switch (name[0])
    {
    case 'x':
        vec->x = value;
        break;
    case 'y':
        vec->y = value;
        break;
    case 'z':
        vec->z = value;
        break;
    default:
        break;
    }
    return 1;
}

int ext_vecAdd(lua_State* L)
{
    // RVA 0x898E30
    assert(ext_checkTag(L, 1, tag_luaVector) && ext_checkTag(L, 2, tag_luaVector));
    CVector const* a = static_cast<CVector const*>(lua_touserdata(L, 1));
    CVector const* b = static_cast<CVector const*>(lua_touserdata(L, 2));
    CVector* result = ext_createVector(L);
    result->x = b->x + a->x;
    result->y = b->y + a->y;
    result->z = b->z + a->z;
    return 1;
}

int ext_vecSub(lua_State* L)
{
    // RVA 0x898EF0
    assert(ext_checkTag(L, 1, tag_luaVector) && ext_checkTag(L, 2, tag_luaVector));
    CVector const* a = static_cast<CVector const*>(lua_touserdata(L, 1));
    CVector const* b = static_cast<CVector const*>(lua_touserdata(L, 2));
    CVector* result = ext_createVector(L);
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
    return 1;
}

int ext_vecNeg(lua_State* L)
{
    // RVA 0x898FB0
    assert(ext_checkTag(L, 1, tag_luaVector));
    CVector const* v = static_cast<CVector const*>(lua_touserdata(L, 1));
    CVector* result = ext_createVector(L);
    result->x = 0.0f - v->x;
    result->y = 0.0f - v->y;
    result->z = 0.0f - v->z;
    return 1;
}

int ext_vecMul(lua_State* L)
{
    // RVA 0x8992B0 - vector * number or number * vector.
    int const type1 = lua_type(L, 1);
    int const type2 = lua_type(L, 2);
    ext_InternalTags const tag1 = ext_getTag(L, 1);
    ext_getTag(L, 2);
    if (type1 != LUA_TNUMBER && type2 != LUA_TNUMBER)
    {
        lua_pushstring(L, "CVector: bad multiplier");
        lua_error(L);
        return 0;
    }
    // NOTE: the operand that is not the number is taken to be a vector without a check.
    CVector const* v = static_cast<CVector const*>(lua_touserdata(L, (tag1 != tag_luaVector) + 1));
    float const a = static_cast<float>(lua_tonumber(L, (type1 != LUA_TNUMBER) + 1));
    CVector* result = ext_createVector(L);
    result->x = a * v->x;
    result->y = a * v->y;
    result->z = a * v->z;
    return 1;
}

int ext_vecCall(lua_State* L)
{
    // RVA 0x898D50 - calling a vector does nothing useful.
    lua_pushnumber(L, 0.0);
    return 1;
}

CVector* ext_createVector(lua_State* L)
{
    // RVA 0x899630
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

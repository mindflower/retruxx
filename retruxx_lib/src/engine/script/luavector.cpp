#include <stdexcept>
#include <script/luavector.h>

#include "script/scriptserver.h"

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
    throw std::logic_error("Not implemented");
}

int ext_vecGet(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_vecSet(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_vecAdd(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_vecSub(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_vecNeg(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_vecMul(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_vecCall(lua_State* L)
{
    throw std::logic_error("Not implemented");
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

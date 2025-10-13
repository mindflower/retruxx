#include <stdexcept>
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
    throw std::logic_error("Not implemented");
}

int ext_quatZero(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatNorm(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatNormalize(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatRotX(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatRotY(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatRotZ(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatFromAxisAngle(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatGetConjugated(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatGetInversed(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatFromYPR(lua_State* L)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
}

int ext_quatMul(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_quatCall(lua_State* L)
{
    throw std::logic_error("Not implemented");
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

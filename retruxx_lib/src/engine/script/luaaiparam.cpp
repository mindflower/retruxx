#include "script/luaaiparam.h"

#include <cassert>
#include <stdexcept>

#include "script/scriptserver.h"

extern "C"
{
#include "lauxlib.h"
}

void SetParams(m3d::AIParam* p, lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_AIParamDestructor(lua_State* L)
{
    auto* aiParam = (m3d::AIParam*)lua_touserdata(L, 1);
    aiParam->~AIParam();
    return 1;
}

int ext_AIParamGet(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

int ext_AIParamSet(lua_State* L)
{
    auto* param = (m3d::AIParam*)lua_touserdata(L, 1);
    auto str = luaL_checklstring(L, 2, 0);
    if (!strcmp(str, "AsFloat")
        || !strcmp(str, "AsID")
        || !strcmp(str, "AsInt")
        || !strcmp(str, "AsString")
        || !strcmp(str, "AsVector")
        || !strcmp(str, "AsRange")
        || !strcmp(str, "AsNumList"))
    {
        SetParams(param, L);
        return 1;
    }
    else
    {
        lua_pushnumber(L, 0.0);
        return 1;
    }
}

int ext_AIParamEq(lua_State* L)
{
    assert(ext_checkTag(L, 1, tag_luaAIParam) && ext_checkTag(L, 2, tag_luaAIParam));
    
    auto* lhs = (m3d::AIParam*)lua_touserdata(L, 1);
    auto* rhs = (m3d::AIParam*)lua_touserdata(L, 2);
    auto res = *lhs == *rhs;
    lua_pushboolean(L, res);
    return 1;
}

int ext_AIParamLt(lua_State* L)
{
    assert(ext_checkTag(L, 1, tag_luaAIParam) && ext_checkTag(L, 2, tag_luaAIParam));

    auto* lhs = (m3d::AIParam*)lua_touserdata(L, 1);
    auto* rhs = (m3d::AIParam*)lua_touserdata(L, 2);
    auto res = *lhs < *rhs;
    lua_pushboolean(L, res);
    return 1;
}

int ext_AIParamLe(lua_State* L)
{
    assert(ext_checkTag(L, 1, tag_luaAIParam) && ext_checkTag(L, 2, tag_luaAIParam));

    auto* lhs = (m3d::AIParam*)lua_touserdata(L, 1);
    auto* rhs = (m3d::AIParam*)lua_touserdata(L, 2);
    auto res = *lhs > *rhs;
    lua_pushboolean(L, res);
    return 1;
}

m3d::AIParam* ext_createAIParam(lua_State* L)
{
    auto* buff = (char*)lua_newuserdata(L, sizeof(m3d::AIParam));
    lua_newtable(L);
    lua_pushstring(L, "__gc");
    lua_pushcclosure(L, ext_AIParamDestructor, 0);
    lua_pushstring(L, "__index");
    lua_pushcclosure(L, ext_AIParamGet, 0);
    lua_pushstring(L, "__newindex");
    lua_pushcclosure(L, ext_AIParamSet, 0);
    lua_pushstring(L, "__eq");
    lua_pushcclosure(L, ext_AIParamEq, 0);
    lua_pushstring(L, "__lt");
    lua_pushcclosure(L, ext_AIParamLt, 0);
    lua_pushstring(L, "__le");
    lua_pushcclosure(L, ext_AIParamLe, 0);
    lua_settable(L, -13);
    lua_settable(L, -11);
    lua_settable(L, -9);
    lua_settable(L, -7);
    lua_settable(L, -5);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);

    // TODO: check this
    auto* res = new (buff) m3d::AIParam;
    return res;
}

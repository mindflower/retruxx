#include "script/luaaiparam.h"

#include <cassert>
#include <stdexcept>

#include "math/vector.h"
#include "script/luavector.h"
#include "script/scriptserver.h"

extern "C"
{
#include "lauxlib.h"
}

void SetParams(m3d::AIParam* p, lua_State* L)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ext_AIParamDestructor(lua_State* L)
{
    auto* aiParam = (m3d::AIParam*)lua_touserdata(L, 1);
    aiParam->~AIParam();
    return 1;
}

int ext_AIParamCmp(lua_State* L)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ext_AIParamGet(lua_State* L)
{
    auto aiParam  = (m3d::AIParam*)lua_touserdata(L, 1);
    auto param = luaL_checklstring(L, 2, 0);
    if (!strcmp(param, "internalTag"))
    {
        lua_pushnumber(L, 1000.0);
        return 1;
    }
    if (!strcmp(param, "Cmp"))
    {
        lua_pushcclosure(L, ext_AIParamCmp, 0);
        return 1;
    }
    if (!strcmp(param, "AsFloat"))
    {
        auto num = aiParam->GetAsFloat();
        lua_pushnumber(L, num);
        return 1;
    }
    if (!strcmp(param, "AsID") || !strcmp(param, "AsInt"))
    {
        auto num = aiParam->GetAsID();
        lua_pushnumber(L, num);
        return 1;
    }
    if (!strcmp(param, "AsString"))
    {
        auto str = aiParam->GetAsStr();
        lua_pushstring(L, str.c_str());
        return 1;
    }
    if (!strcmp(param, "AsVector"))
    {
        auto vec = aiParam->GetAsVector();
        auto res = ext_createVector(L);
        *res = vec;
        return 1;
    }
    if (!strcmp(param, "AsRange"))
    {
        auto vec = aiParam->GetAsRange();
        auto res = ext_createVector(L);
        res->x = vec.x;
        res->y = vec.y;
        lua_pushlightuserdata(L, res);
        return 1;
    }
    if (!strcmp(param, "AsNumList"))
    {
        // TODO: check this
        lua_gettop(L);
        lua_newtable(L);
        auto top = lua_gettop(L);
        auto idList = aiParam->GetAsIdList();
        for (int i = 0; i < idList.size(); ++i)
        {
            lua_pushnumber(L, i);
            lua_pushnumber(L, idList[i]);
            lua_rawset(L, top);
        }
        return 1;
    }
    lua_pushnumber(L, 0.0);
    return 1;
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

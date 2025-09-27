#include "script/luaaiparam.h"

#include <stdexcept>

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

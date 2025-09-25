#include <stdexcept>
#include <script/luavector.h>

int ext_vecConstructor(lua_State* L)
{
    throw std::logic_error("Not implemented");
}

void ext_initVector(lua_State* L)
{
    lua_pushcclosure(L, ext_vecConstructor, 0);
    lua_pushstring(L, "CVector");
    lua_insert(L, -2);
    lua_settable(L, -10001);
}

CVector* ext_createVector(lua_State*)
{
    throw std::logic_error("Not implemented");
}

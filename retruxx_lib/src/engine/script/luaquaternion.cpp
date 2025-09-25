#include <stdexcept>
#include <script/luaquaternion.h>

int ext_quatConstructor(lua_State*)
{
    throw std::logic_error("Not implemented");
}

void ext_initQuaternion(lua_State* L)
{
    lua_pushcclosure(L, ext_quatConstructor, 0);
    lua_pushstring(L, "Quaternion");
    lua_insert(L, -2);
    lua_settable(L, -10001);
}

Quaternion* ext_createQuaternion(lua_State*)
{
    throw std::logic_error("Not implemented");
}

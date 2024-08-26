#pragma once
extern "C"
{
#include "lua/lua.h"
}

int ext_quatConstructor(lua_State*);
void ext_initQuaternion(lua_State*);

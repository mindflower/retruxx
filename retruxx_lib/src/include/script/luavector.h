#pragma once
extern "C"
{
#include "lua/lua.h"
}

int ext_vecConstructor(lua_State*);
void ext_initVector(lua_State*);

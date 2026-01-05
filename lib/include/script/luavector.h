#pragma once
#include "math/vector.h"

extern "C"
{
#include "lua/lua.h"
}

int ext_vecConstructor(lua_State*);
void ext_initVector(lua_State*);
CVector* ext_createVector(lua_State*);

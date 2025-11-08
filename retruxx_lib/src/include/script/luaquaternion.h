#pragma once
#include "math/quaternion.h"

extern "C"
{
#include "lua/lua.h"
}

int ext_quatConstructor(lua_State*);
void ext_initQuaternion(lua_State*);
Quaternion* ext_createQuaternion(lua_State*);

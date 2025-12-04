#pragma once
#include "core/aiparam.h"

extern "C"
{
#include "lua/lua.h"
}

m3d::AIParam* ext_createAIParam(lua_State*);
